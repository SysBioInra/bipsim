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
 * @file switch.h
 * @brief Header for the Switch class.
 * @authors Marc Dinh, Stephan Fischer
 */

// Multiple include protection
//
#ifndef SWITCH_H
#define SWITCH_H

// ==================
//  General Includes
// ==================
//

// ==================
//  Project Includes
// ==================
//
#include "simulatorinput.h"

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"

/**
 * @brief Class describing switching rules for BoundChemical reaching a switch
 *  site.
 */
class Switch : public SimulatorInput
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /** 
   * @brief Constructor. 
   * @param A Input BoundChemical of the switch.
   * @param B Output BoundChemical of the switch.
   */
  Switch (BoundChemical& A, BoundChemical& B)
    : _id (next_id())
    , _input (A)
    , _output (B)
  {
  }

  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // Switch (const Switch& other);
  // /* @brief Assignment operator. */
  // Switch& operator= (const Switch& other);
  // /* @brief Destructor. */
  // ~Switch (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /** 
   * @brief Accessor to switch identifier.
   * @return Switch identifier.
   */
  int id (void) const { return _id; }
  /** 
   * @brief Accessor to input of switch rule.
   * @return BoundChemical being transformed by the switch.
   */
  const BoundChemical& input (void) const { return _input; }
  /** 
   * @brief Accessor to output of switch rule.
   * @return BoundChemical resulting from the switch.
   */
  BoundChemical& output (void) const { return _output; }

private:
  // =================
  //  Private Methods
  // =================
  //
  /** @brief Function returning next id. */
  static int next_id (void) { static int next_id = 0; return next_id++; }

  // ============
  //  Attributes
  // ============
  //  
  /** @brief Identifier. */
  int _id;
  /** @brief Input and output of the rule. */
  BoundChemical& _input;
  BoundChemical& _output;
};

// ======================
//  Inline declarations
// ======================
//

#endif // SWITCH_H
