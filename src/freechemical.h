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
 * @file freechemical.h
 * @brief Header for the ClassName class.
 * @authors Marc Dinh, Stephan Fischer
 */

// Multiple include protection
//
#ifndef FREE_CHEMICAL_H
#define FREE_CHEMICAL_H

// ==================
//  General Includes
// ==================
//

// ==================
//  Project Includes
// ==================
//
#include "chemical.h"
#include "forwarddeclarations.h"

/**
 * @brief Class for chemicals diffusing freely in the cytosol.
 */
class FreeChemical : public Chemical
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  // Not needed for this class (use of compiler-generated versions)
  /** @brief Default constructor. */
  FreeChemical (void) : _constant (false), _ignored_variation (0) {}

  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // FreeChemical (const FreeChemical& other_chemical);
  // /* @brief Assignment operator. */
  // FreeChemical& operator= (const FreeChemical& other_chemical);
  // /* @brief Destructor. */
  // ~FreeChemical (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // redefined from Chemical
  void add (int quantity) {
    if (!_constant)
      { Chemical::add (quantity); }
    else
      { _ignored_variation += quantity; }
  }
  void remove (int quantity) {
    if (!_constant)
      { Chemical::remove (quantity); }
    else
      { _ignored_variation -= quantity; }
  }

  /**
   * @brief Specify whether chemical's concentration should change over time.
   * @param is_constant If set to true, all add and remove
   *  commands will be ignored.
   */
  void set_constant (bool is_constant) { _constant = is_constant; }

  /**
   * @brief Reset counter recording add/record events for constant chemicals.
   */
  void reset_ignored_variation (void) { _ignored_variation = 0; }

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Accessor to flag specifying if chemical has constant concentration.
   * @return True if chemical has constant concentartion (i.e. it ignores all
   *  add and remove commands).
   */
  bool is_constant (void) const { return _constant; }

  /**
   * @brief Return number of molecules added/removed while constant.
   * @return Integer reflecting add() and remove() commands that were
   *  ignored because the chemical was defined as constant.
   */
  int ignored_variation (void) const { return _ignored_variation; }

private:
  // =================
  //  Private Methods
  // =================
  //

  // ============
  //  Attributes
  // ============
  //
  /** @brief Whether chemical has constant concentration or not. */
  bool _constant;
  /** @brief Number of molecules added/removed while constant. */
  int _ignored_variation;
};

// ======================
//  Inline declarations
// ======================
//

#endif // FREE_CHEMICAL_H
