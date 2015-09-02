

/**
 * @file simulation.cpp
 * @brief Implementation of the Simulation class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream>

// ==================
//  Project Includes
// ==================
//
#include "simulation.h"
#include "cellstate.h"
#include "chemical.h"
#include "naivesolver.h"
#include "manualdispatchsolver.h"
#include "reactionclassification.h"
#include "chemicallogger.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
Simulation::Simulation (const char* filename)
 : _t (0)
{
  // read cell state from file
  _cell_state = new CellState ("../data/test_input.txt");

  // create solver
#ifdef CLASSIFICATION
  ReactionClassification classification;
  int class_id = classification.create_new_class (0.0001);
  // int class_id = classification.create_new_class (ReactionClassification::ALWAYS_UPDATED);
  classification.add_reaction_list_to_class (class_id, _cell_state->reaction_list());
  _solver = new ManualDispatchSolver (_t, _cell_state, classification);
#else
  _solver = new NaiveSolver (_t, *_cell_state);
#endif

  // create logger
  std::list <int> chemical_ids = _cell_state->chemical_handler().existing_identifiers();
  std::list <const Chemical*> chemical_refs;
  std::list <std::string> chemical_names;
  for (std::list<int>::iterator id_it = chemical_ids.begin(); id_it != chemical_ids.end(); ++id_it)
    {
      chemical_refs.push_back (&(_cell_state->chemical_handler().reference (*id_it)));
      chemical_names.push_back (_cell_state->chemical_handler().name (*id_it));
    }
  _logger = new ChemicalLogger ("concentrations.txt", chemical_refs, chemical_names);
}

// Not needed for this class (use of default copy constructor) !
// Simulation::Simulation ( const Simulation& other_simulation );

Simulation::~Simulation (void)
{
  delete _logger;
  delete _solver;
  delete _cell_state;
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void Simulation::run (double time_interval)
{
  double final_time = _t + time_interval;
  std::cout << "Solving from t = " << _t << " to t = " << final_time << "..." << std::endl;
  while (_solver->time() < final_time)
    {
      if ((_solver->number_reactions_performed() % 10000) == 0) { _logger->log (_solver->time()); }
      
      _solver->go_to_next_reaction();
    }
  
  std::cout << _solver->number_reactions_performed() << " reactions occurred." << std::endl;
}

// ============================
//  Public Methods - Accessors
// ============================
//


// ==========================
//  Public Methods - Setters
// ==========================
//


// =======================================
//  Public Methods - Operator overloading
// =======================================
//
// Not needed for this class (use of default overloading) !
// Simulation& Simulation::operator= ( const Simulation& other_simulation );

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool Simulation::check_invariant (void) const
{
  bool result = true;
  return result;
}


// =================
//  Private Methods
// =================
//
