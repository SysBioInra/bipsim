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
 * @file simulation.h
 * @brief Header for the Simulation class.
 * @authors Marc Dinh, Stephan Fischer
 */

// Multiple include protection
//
#ifndef SIMULATION_H
#define SIMULATION_H

// ==================
//  General Includes
// ==================
//
#include <list> // std::list

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "cellstate.h"
#include "eventhandler.h"
#include "simulationparams.h"

/**
 * @brief Class that creates and handles the whole simulation.
 * @details Simulation is the main object in the simulator. It reads 
 * configuration parameters and then creates every object needed.
 */
class Simulation
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor from parameter file.
   * @param filename Path to parameter file.
   */
  Simulation (const std::string& filename);

 private:
  // Forbidden
  /** @brief Copy constructor. */
  Simulation (const Simulation& other_simulation);
  /** @brief Assignment operator. */
  Simulation& operator= ( const Simulation& other_simulation );

 public:
  /**
   * @brief Destructor.
   */
  ~Simulation (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Run simulator for given amount of time.
   */
  void run (void);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Accessor to CellState object.
   * @return Reference to CellState object holding most proprietes of the cell
   *  such as reactions and reactants.
   */
  const CellState& cell_state (void) const;

private:
  // =================
  //  Private Methods
  // =================
  //
  /** @brief Compute type and time of next event or reaction. */
  void compute_next_timing (void);

  /** @brief Write logs if necessary. */
  void write_logs (double time);

  /** @brief Create various loggers from simulation parameters. */
  void create_loggers (void);

  // ============
  //  Attributes
  // ============
  //
  /** @brief Simulation parameters. */
  SimulationParams _params;
  /** @brief Global cell state. */
  CellState _cell_state;
  /** @brief Solver used to integrate system. */
  Solver* _solver;
  /** @brief Handler reading and performing user-defined events. */
  EventHandler _event_handler;
  /** @brief List of loggers. */
  std::list <Logger*> _loggers;

  /** @brief Next log time. */
  double _next_log_time;
  /** @brief Next volume update time. */
  double _next_volume_time;
  /** @brief Next timing of reaction, event, volume change, etc. */
  double _next_timing;
  /** @brief Possible types for next occurrence (reaction, event, etc.). */
  enum NextType { REACTION, EVENT, VOLUME };
  /** @brief Type of next occurrence (reaction, event, etc.). */
  NextType _next;
};

// ======================
//  Inline declarations
// ======================
//
inline const CellState& Simulation::cell_state (void) const
{
  return _cell_state;
}

#endif // SIMULATION_H
