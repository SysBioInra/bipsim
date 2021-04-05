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
 * @file boundunitlist.h
 * @brief Header for the BoundUnitList class.
 * @authors Marc Dinh, Stephan Fischer
 */

// Multiple include protection
//
#ifndef BOUND_UNIT_LIST_H
#define BOUND_UNIT_LIST_H

// ==================
//  General Includes
// ==================
//
#include <iostream> // std::cerr
#include <vector> // std::vector

#include "macros.h" // REQUIRE
#include "randomhandler.h"

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"

/**
 * @brief Class storing bound units in a vector in arbitrary order.
 * @details BoundUnitList stores elements in a vector. It enables simple 
 *  operations like adding, optimized accessing and erasing of elements. 
 */
class BoundUnitList
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  // Not needed for this class (use of compiler-generated versions)
  // /* @brief Default constructor. */
  // BoundUnitList (void);
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // BoundUnitList (const BoundUnitList& other);
  // /* @brief Assignment operator. */
  // BoundUnitList& operator= (const BoundUnitList& other);
  // /* @brief Destructor. */
  // ~BoundUnitList (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Add unit to list.
   * @param unit BoundUnit to add to list.
   */
  void add (BoundUnit& unit);

  /**
   * @brief Erase element.
   * @param unit BoundUnit to remove from list.
   * @details Only the first instance of element found is removed. If no 
   * element to erase is found, the list remains unchanged.
   */
  void remove (BoundUnit& unit);
  
  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Accessor to list size.
   * @return Number of elements stored in list.
   */
  int size (void) const { return _v.size(); }

  /**
   * @brief Accessor to random unit from list.
   * @return Random unit stored in list.
   */
  BoundUnit& random_unit (void) const
    {
      /** @pre List must not be empty. */
      REQUIRE (_v.size() > 0);
      return *(_v [RandomHandler::instance().draw_uniform (0, _v.size()-1)]);
    }

  /**
   * @brief Accessor to all elements of list.
   * @return Vector of BoundUnit in arbitrary order.
   */
  const std::vector <BoundUnit*>& operator() (void) const { return _v; }

private:
  // =================
  //  Private Methods
  // =================
  //

  // ============
  //  Attributes
  // ============
  //
  /** @brief Vector holding the stored elements. */
  std::vector <BoundUnit*> _v;
};

// ======================
//  Inline declarations
// ======================
//
#include <algorithm> // std::find
#include "boundunit.h"

inline void BoundUnitList::add (BoundUnit& unit)
{
  // do not forget to store the position of unit within this list
  unit.list_indices(BoundUnitListKey()).push_back(_v.size());
  _v.push_back (&unit);
}

inline void BoundUnitList::remove (BoundUnit& unit)
{
  if (_v.size() == 0) { return; }

  // get the list of positions of unit within the various
  // BoundUnitLists in which it was stored.
  std::vector<int>& indices = unit.list_indices(BoundUnitListKey());
  // find the position corresponding to the current list
  for (std::vector<int>::iterator i = indices.begin(); i != indices.end(); ++i)
  {
    if ((*i < _v.size()) && (&unit == _v[*i]))
    {
      unsigned int index = *i;
      // swap unit with last element if it is not the last element
      if (index < _v.size()-1)
      {
	_v [index] = _v.back();
	// update indices stored in the unit that got swaped
	std::vector<int>& ind = _v[index]->list_indices(BoundUnitListKey());
	ind.erase(std::find(ind.begin(), ind.end(), _v.size()-1));
	ind.push_back (index);
      }
      // remove original unit (that is now at the end of the vector)
      // do not forget to remove the position of unit within this list
      indices.erase (i);
      _v.pop_back();
      return;
    }
  }
}

#endif // BOUND_UNIT_LIST_H
