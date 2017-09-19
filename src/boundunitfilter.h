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
 * @file boundunitfilter.h
 * @brief Header for the BoundUnitFilter class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef BOUND_UNIT_FILTER_H
#define BOUND_UNIT_FILTER_H

// ==================
//  General Includes
// ==================
//

// ==================
//  Project Includes
// ==================
//

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"

/**
 * @brief Abstract class defining interface for BoundUnit filters.
 *
 * BoundUnitFilter defines an interface for organizing bound units. A filter
 * can receive new units, remove them and draw a random unit according to
 * internal criteria.
 */
class BoundUnitFilter
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  // Not needed for this class (use of compiler-generated versions)
  // /* @brief Default constructor. */
  // BoundUnitFilter (void);
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // BoundUnitFilter (const BoundUnitFilter& other);
  // /* @brief Assignment operator. */
  // BoundUnitFilter& operator= (const BoundUnitFilter& other);
  /** @brief Destructor. */
  virtual ~BoundUnitFilter (void) {}

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Add unit to filter.
   * @param unit BoundUnit to add to filter if it matches the filter's criteria.
   */
  virtual void add (BoundUnit& unit) = 0;

  /**
   * @brief Remove previously added unit.
   * @param unit BoundUnit to remove from filter (ignored if unit was not 
   *  stored).
   */
  virtual void remove (BoundUnit& unit) = 0;

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Pick random unit according to filter criteria.
   * @return Random BoundUnit stored by the filter.
   */
  virtual BoundUnit& random_unit (void) const = 0;

private:
  // =================
  //  Private Methods
  // =================
  //

  // ============
  //  Attributes
  // ============
  //  
};

// ======================
//  Inline declarations
// ======================
//

#endif // BOUND_UNIT_FILTER_H
