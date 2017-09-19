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

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // SimulatorInput (const SimulatorInput& other_simulator_input);
  // /* @brief Assignment operator. */
  // SimulatorInput& operator= (const SimulatorInput& other_simulator_input);
  // Note that destructor is empty (0 part of the rule applies).
  // Making the destructor pure virtual ensures the class is abstract.
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

private:
  // ============
  //  Attributes
  // ============
  //
  

  // =================
  //  Private Methods
  // =================
  //
};

// ======================
//  Inline declarations
// ======================
//
inline SimulatorInput::~SimulatorInput (void) {}

#endif // SIMULATOR_INPUT_H
