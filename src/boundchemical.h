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
 * @file boundchemical.h
 * @brief Header for the BoundChemical class.
 * @authors Marc Dinh, Stephan Fischer
 */

// Multiple include protection
//
#ifndef BOUNDCHEMICAL_H
#define BOUNDCHEMICAL_H

// ==================
//  General Includes
// ==================
//
#include <list>  // std::list
#include <map> // std::map

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "chemical.h"
#include "boundunitlist.h"


/**
 * @brief The BoundChemical class describes chemicals in a bound form.
 *
 * Bound chemicals may be linked with specific reactions (i.e. different from
 * their free counterparts) and collide with other bound elements.
 * Inherits class Chemical.
 */
class BoundChemical : public Chemical
{
public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  /** @brief Default constructor */
  BoundChemical (void);
  
 private:
  // Forbidden
  /** @brief Copy constructor */
  BoundChemical (BoundChemical& other_bound_chemical);
  /** @brief Assignment operator */
  BoundChemical& operator= (BoundChemical& other_chemical);
 public:

  // /* @brief Destructor. */
  // ~BoundChemical (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Add unit.
   * @param unit BoundUnit to add to species.
   */
  void add (BoundUnit& unit);
  
  /**
   * @brief Remove unit.
   * @param unit BoundUnit to remove from species.
   */
  void remove (BoundUnit& unit);

  /**
   * @brief Add filter to organize units.
   * @param filter BoundUnitFilter used to organize units.
   */
  void add_filter (BoundUnitFilter& filter);

  /**
   * @brief Remove filter.
   * @param filter BoundUnitFilter to remove.
   */
  void remove_filter (BoundUnitFilter& filter);

  /**
   * @brief Add switch.
   * @param switch_ Switch changing a bound chemical's nature.
   */
  void add_switch (Switch& switch_);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Accessor to random unit.
   * @return Random BoundUnit belonging to this species.
   */
  BoundUnit& random_unit (void) const;

 private:
  // =================
  //  Private Methods
  // =================
  //
  // redefined from Chemical
  void print (std::ostream& output) const;

  // ============
  //  Attributes
  // ============
  //  
  /** @brief Units belonging to species. */
  BoundUnitList _units;
  /** @brief Filters used to organize units. */
  std::list <BoundUnitFilter*> _filters;    
  /** @brief Switch that can be applied on units. */
  std::map <int, Switch*> _switches;
};

// ======================
//  Inline declarations
// ======================
//
inline void BoundChemical::add_filter (BoundUnitFilter& filter)
{
  _filters.push_back (&filter);
}

inline void BoundChemical::remove_filter (BoundUnitFilter& filter)
{
  _filters.remove (&filter);
}

inline BoundUnit& BoundChemical::random_unit (void) const
{
  /** @pre There must be at least one unit stored. */
  REQUIRE (_units.size() > 0);
  return _units.random_unit();
}

#endif // BOUNDCHEMICAL_H
