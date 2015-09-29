

/**
 * @file simulatorinput.h
 * @brief Header for the SimulatorInput class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef SIMULATOR_INPUT_H
#define SIMULATOR_INPUT_H

// ==================
//  General Includes
// ==================
//


// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"

/**
 * @brief Abstract class for input read from file.
 *
 * SimulatorInput provides an empty interface but enables
 * to define a common type to all input read from file.
 */
class SimulatorInput
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor.
   */
  SimulatorInput (void) {}

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor.
  //  */
  // SimulatorInput (const SimulatorInput& other_simulator_input);

  /**
   * @brief Destructor.
   */
  virtual ~SimulatorInput (void) = 0;

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //


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
  // SimulatorInput& operator= (const SimulatorInput& other_simulator_input);

protected:
  // ======================
  //  Protected Attributes
  // ======================
  //

  // ===================
  //  Protected Methods
  // ===================
  //


private:

  // ============
  //  Attributes
  // ============
  //
  

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
inline SimulatorInput::~SimulatorInput (void) {}

#endif // SIMULATOR_INPUT_H
