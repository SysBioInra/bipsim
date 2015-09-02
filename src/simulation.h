

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

/**
 * @brief Class that creates and handles the whole simulation.
 *
 * Simulation is the main object in the simulator. It reads configuration parameters
 * and then creates every object needed.
 */
class Simulation
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor.
   * @param filename File containing reactions to read.
   */
  Simulation (const char* filename);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor.
  //  */
  // Simulation ( const Simulation& other_simulation );

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
   * @param time_interval Time for which simulation must be run.
   */
  void run (double time_interval);

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
  // /*
  //  * @brief Assignment operator.
  //  */
  // Simulation& operator= ( const Simulation& other_simulation );

  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @brief Check class invariant.
   * @return True if class invariant is preserved.
   */
  bool check_invariant (void) const;


private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Simulation time. */
  double _t;

  /** @brief Global cell state. */
  CellState* _cell_state;

  /** @brief Solver used to integrate system. */
  Solver* _solver;

  /** @brief Logger used to write to file. */
  ChemicalLogger* _logger;

  // =================
  //  Private Methods
  // =================
  //

  // ======================
  //  Forbidden Operations
  // ======================
  //

};

// ======================
//  Inline declarations
// ======================
//

#endif // SIMULATION_H
