

/**
 * @file simulation.h
 * @brief Header for the Simulation class.
 * 
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
 *
 * Simulation is the main object in the simulator. It reads configuration
 * parameters and then creates every object needed.
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
private:

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

  /** @brief Logger used to write to file. */
  ChemicalLogger* _logger;

  /** @brief Logger used to write to file. */
  DoubleStrandLogger* _replication_logger;

  /** @brief Handler reading and performing user-defined events. */
  EventHandler _event_handler;

  // =================
  //  Private Methods
  // =================
  //
  /** @brief Write logs if necessary. */
  void write_logs (void);
};

// ======================
//  Inline declarations
// ======================
//

#endif // SIMULATION_H
