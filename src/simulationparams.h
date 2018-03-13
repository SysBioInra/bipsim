// 
// Copyright 2017 INRA
// Authors: M. Dinh, S. Fischer
// Last modification: 2017-09-19
// 
// 
// Licensed under the GNU General Public License.
// You should have received a copy of the GNU General Public License
// along with BiPSim.  If not, see <http://www.gnu.org/licenses/>.
// 


/**
 * @file simulationparams.h
 * @brief Header for the SimulationParams class.
 * @authors Marc Dinh, Stephan Fischer
 */

// Multiple include protection
//
#ifndef SIMULATION_PARAMS_H
#define SIMULATION_PARAMS_H

// ==================
//  General Includes
// ==================
//
#include <string> // std::string
#include <vector> // std::vector
#include <sstream> // std::istringstream

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"

/**
 * @brief Class reading and storing simulation parameters.
 */
class SimulationParams
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor from file.
   * @param filename Path to the file to read parameters from.
   */
  SimulationParams (const std::string& filename);

 private:
  // Forbidden
  /** @brief Copy constructor. */
  SimulationParams (const SimulationParams& other_simulation_params);
  /** @brief Assignment operator. */
  SimulationParams& operator= (const SimulationParams& other_simulation_params);

 public:
  /**
   * @brief Destructor.
   */
  ~SimulationParams (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Accessor to random handler seed.
   * @return Random handler seed. 0 if none specified.
   */
  double seed (void) const { return _seed; }


  /**
   * @brief Accessor to initial simulation time.
   * @return Initial simulation time. 0 if none specified.
   */
  double initial_time (void) const { return _initial_time; }

  /**
   * @brief Accessor to final simulation time.
   * @return Final simulation time. 1000 if none specified.
   */
  double final_time (void) const { return _final_time; }

  /**
   * @brief Accessor to input files.
   * @return Vector of input file names. Empty if none specified.
   */
  const std::vector <std::string>& input_files (void) const 
  { return _input_files; }
  
  /**
   * @brief Accessor to output directory.
   * @return Path to output directory.
   */
  std::string output_dir (void) const
    { return _output_dir; }

  /**
   * @brief Accessor to name for concentration output file.
   * @return Output file name.
   */
  std::string concentration_file (void) const 
    { return _output_dir + "/" + _concentration_file; }

  /**
   * @brief Accessor to name for reaction output file.
   * @return Output file name.
   */
  std::string reaction_file (void) const
    { return _output_dir + "/" + _reaction_file; }

  /**
   * @brief Accessor to output step.
   * @return Output step. 1 if none specified
   */
  double output_step (void) const { return _output_step; }
  
  /**
   * @brief Accessor to entities to output.
   * @return Vector of entity names. Empty if none specified.
   */
  const std::vector <std::string>& output_entities (void) const
  { return _output_entities; }

  /**
   * @brief Accessor to default solver factory.
   * @return A factory creating the default type of solver to use.
   */
  const SolverFactory& solver_factory (void) const { return *_solver_factory; }

  /**
   * @brief Accessor to default rate manager factory.
   * @return A factory creating the default type of rate manager to use.
   */
  const RateManagerFactory& rate_manager_factory (void) const
  { return *_rate_manager_factory; }

  /**
   * @brief Accessor to default rate container factory.
   * @return A factory creating the default type of rate container to use.
   */
  const RateContainerFactory& rate_container_factory (void) const
  { return *_rate_container_factory; }

  /**
   * @brief Accessor to base rate to use for hybrid method.
   * @return Base rate to use for hybrid method (1 by default).
   */
  double hybrid_base_rate (void) const { return _hybrid_base_rate; }

  /**
   * @brief Accessor to base volume value.
   * @return Base value of volume (1 by default).
   */
  double base_volume (void) const { return _base_volume; }

  /**
   * @brief Accessor to volume update step.
   * @return Volume update step (1 by default).
   */
  double volume_step (void) const { return _volume_step; }

  /**
   * @brief Accessor to chemicals impacting volume.
   * @return Vector of chemical names impacting volume (empty by default).
   */
  const std::vector <std::string>& volume_modifiers (void) const 
  { return _volume_modifiers; }

  /**
   * @brief Accessor to weights of chemicals impacting volume.
   * @return Vector of weights of chemicals impacting volume (empty by default).
   */
  const std::vector <double>& volume_weights (void) const 
  { return _volume_weights; }

private:
  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Check whether line is empty or commented.
   * @return True if line is empty or commented.
   */
  bool is_line_empty (void) const;

  /**
   * @brief Interpret drawing algorithm.
   * @param tag String input for drawing algorithm.
   * @return True if drawing algorithm was successfully converted.
   */
  bool interpret_drawing_algorithm (const std::string& tag);

  /**
   * @brief Write relevant simulation parameters to a file for future reference.
   */
  void _write_params_out (void) const;

  // ============
  //  Attributes
  // ============
  //
  // various tags used to parse file
  static const std::string _seed_tag, _initial_time_tag, _final_time_tag, 
    _input_files_tag, _output_dir_tag, _output_step_tag, _output_entities_tag, 
    _drawing_algorithm_tag, _hybrid_base_rate_tag, _base_volume_tag, 
    _volume_modifier_tag, _volume_step_tag;

  // names for output files
  static const std::string _concentration_file, _param_file, _reaction_file;

  /** @brief Current line read. */
  std::string _line;

  /** @brief Random handler seed. */
  int _seed;

  /** @brief Initial simulation time.. */
  double _initial_time;
  /** @brief Final simulation time. */
  double _final_time;

  /** @brief Vector of input file names. */
  std::vector<std::string> _input_files;
  /** @brief Output dir name. */
  std::string _output_dir;
  /** @brief Output step. */
  double _output_step;
  /** @brief Vector of names of elements to output. */
  std::vector <std::string> _output_entities;

  /** @brief Factory for default solver. */
  SolverFactory* _solver_factory;
  /** @brief Factory for default rate manager. */
  RateManagerFactory* _rate_manager_factory;
  /** @brief Factory for default rate container. */
  RateContainerFactory* _rate_container_factory;
  /** @brief Base rate for the HybridRateContainer. */
  double _hybrid_base_rate;
  /** @brief Base volume value. */
  double _base_volume;

  /** @brief Volume modification step. */
  double _volume_step;
  /** @brief Chemicals acting on volume. */
  std::vector <std::string> _volume_modifiers;
  /** @brief Weights of chemicals acting on volume. */
  std::vector <double> _volume_weights;
};

// ======================
//  Inline declarations
// ======================
//

#endif // SIMULATION_PARAMS_H
