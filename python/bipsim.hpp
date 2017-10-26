/**
 * @file bipsim.hpp
 * @brief Header for the BipSim class.
 * @authors Marc Dinh, Stephan Fischer
 */

// Multiple include protection
//
#ifndef BIPSIM_HPP
#define BIPSIM_HPP

// ==================
//  General Includes
// ==================
//
#include <string> // std::string
#include <list> // std::list
#include <vector> // std::vector
#include <boost/python.hpp>
#include <boost/python/stl_iterator.hpp>

// ==================
//  Project Includes
// ==================
//
#include "simulation.h"
#include "cellstate.h"

/**
 * @brief Facade class for python module.
 */

class BipSim {
public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor from filename.
   */
  BipSim(const std::string filename) : _simulation(filename) {}

  // Not needed for this class (use of default compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // BiasedWheel (const BiasedWheel& other_class_name);
  // /* @brief Assignment operator. */
  // BiasedWheel& operator= (const BiasedWheel& other_class_name);
  // /* @brief Destructor. */
  // ~BiasedWheel (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  void perform_next_reaction(void) {
    _simulation.perform_next_reaction();
  }

  void skip(double time_) {
    _simulation.skip(time_);
  }

  void set_chemicals(boost::python::list names)
  {
    _chemical_names.clear();
    _chemicals.clear();
    boost::python::ssize_t len = boost::python::len(names);
    for (int i = 0; i < len; i++) {
      std::string name = boost::python::extract<std::string>(names[i]);
      _chemicals.push_back(
        &_simulation.cell_state().fetch<FreeChemical>(name)
      );
      _chemical_names.push_back(name);
    }
  }

  void set_chemical_number(boost::python::list values) {
    _simulation.set_free_chemicals(
      _chemical_names,
      std::vector<int>(
        boost::python::stl_input_iterator<int>(values),
        boost::python::stl_input_iterator<int>()
      )
    );
  }

  /**
   * @brief Test.
   */
  void reset_ignored_variation(void) {
    for (std::vector<std::string>::iterator name = _chemical_names.begin();
    name != _chemical_names.end(); ++name) {
      _simulation.reset_ignored_variation(*name);
    }
  }

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  double simulation_time(void) {
    return _simulation.time();
  }

  double next_reaction_time(void) {
    return _simulation.next_reaction_time();
  }

  double volume(void) {
    return _simulation.cell_state().volume();
  }

  boost::python::list chemical_number(void) {
    boost::python::list result;
    for (unsigned int i = 0; i < _chemicals.size(); ++i) {
      result.append(_chemicals[i]->number());
    }
    return result;
  }

  boost::python::list chemical_ignored_variation(void) {
    boost::python::list result;
    for (unsigned int i = 0; i < _chemicals.size(); ++i) {
      result.append(_chemicals[i]->ignored_variation());
    }
    return result;
  }

private:
  // ============
  //  Attributes
  // ============
  //
  Simulation _simulation;
  std::vector<const FreeChemical*> _chemicals;
  std::vector<std::string> _chemical_names;

  // =================
  //  Private Methods
  // =================
  //
};

#endif // BIPSIM_HPP
