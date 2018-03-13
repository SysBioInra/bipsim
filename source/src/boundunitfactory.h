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
 * @file boundunitfactory.h
 * @brief Header for the BoundUnitFactory class.
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef BOUND_UNIT_FACTORY_H
#define BOUND_UNIT_FACTORY_H

// ==================
//  General Includes
// ==================
//
#include <list>

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
 * @brief Class used to create and clean BoundUnits.
 *
 * BoundUnitFactory creates and deletes BoundUnits. It uses the Singleton
 * pattern.
 */
class BoundUnitFactory
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //

 private:
  /** @brief Constructor. */
  BoundUnitFactory (void);
  /** @brief Copy constructor. */
  BoundUnitFactory (const BoundUnitFactory& other);
  /** @brief Assignment operator. */
  BoundUnitFactory& operator= (const BoundUnitFactory& other);
 public:

  /** @brief Destructor. */
  ~BoundUnitFactory (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Create new BoundUnit at given location.
   * @param site BindingSite the unit bound to.
   * @return BoundUnit instantiated at given site.
   */
  BoundUnit& create (const BindingSite& site);

  /**
   * @brief Create new BoundUnit at given location.
   * @param location ChemicalSequence to which unit bound.
   * @param first First position occupied by unit.
   * @param last Last position occupied by unit.
   * @param reading_frame Reading frame of new unit.
   * @return BoundUnit instantiated at given location.
   */
  BoundUnit& create (ChemicalSequence& location, int first,
		     int last, int reading_frame);

  /**
   * @brief Indicate that BoundUnit is not used anymore and can be deleted.
   * @param unit BoundUnit that is not used by the program anymore.
   */
  void free (BoundUnit& unit);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Accessor to only instance of class (singleton pattern).
   * @return Only instance of BoundUnitFactory.
   */
  static BoundUnitFactory& instance (void);

private:
  // =================
  //  Private Methods
  // =================
  //

  // ============
  //  Attributes
  // ============
  //  
  /** @brief List of all BoundUnits created. */
  std::list <BoundUnit*> _created;

  /** @brief List of BoundUnits that are unused and can be recycled. */
  std::list <BoundUnit*> _unused;

  /** @brief Only instance of class (singleton). */
  static BoundUnitFactory _instance;
};

// ======================
//  Inline declarations
// ======================
//
inline BoundUnitFactory& BoundUnitFactory::instance (void)
{
  return _instance;
}

#endif // BOUND_UNIT_FACTORY_H
