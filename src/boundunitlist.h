

/**
 * @file boundunitlist.h
 * @brief Header for the BoundUnitList class.
 * 
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
#include <vector> // std::vector

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"

/**
 * @brief Class storing a list of BoundUnit.
 *
 * BoundUnitList stores pointers to BoundUnit. It enables simple operations
 * like adding, optimized accessing and erasing of elements. What is more,
 * compared to STL lists (as of C++ 98), it yields constant time access to 
 * size (linear in C++ 98). For convenience, a [] operator is provided, but
 * there is NO WARRANTY on the order of elements in the list, the user MUST
 * assume that it is random.
 */
class BoundUnitList
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /** @brief Default constructor. */
  BoundUnitList (void);

  // Not needed for this class (use of compiler-generated versions)
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
   * @brief Add element to list (in unknown position).
   * @param unit Pointer to add to list.
   */
  void add (BoundUnit* unit);

  /**
   * @brief Erase element.
   * @param unit Pointer to remove from list.
   *
   * Only the first instance of element found is removed. If no element to
   * erase is found, a warning is displayed and the list remains unchanged.
   */
  void remove (const BoundUnit* unit);
  
  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Accessor to list size.
   * @return Number of elements stored in list.
   */
  int size (void) const;

  /**
   * @brief Accessor to random unit from list.
   * @return Random unit stored in list.
   */
  BoundUnit& random_unit (void) const;

  /**
   * @brief Accessor to total unbinding rate of stored elements.
   * @return Sum of k_offs of binding sites of the BoundUnit stored.
   */
  double unbinding_rate (void) const;

  /**
   * @brief Accessor to all elements of list.
   * @return Vector of BoundUnit in an arbitrary order.
   */
  const std::vector <BoundUnit*>& operator() (void) const;

private:
  // =================
  //  Private Methods
  // =================
  //
  /** 
   * @brief Remove element from vector.
   * @param index Index of element to remove.
   */
  void erase (int index);

  // ============
  //  Attributes
  // ============
  //
  /** @brief Vector holding the stored pointers. */
  std::vector <BoundUnit*> _v;

  /** @brief Index of last element accessed. */
  mutable int _current;
};

// ======================
//  Inline declarations
// ======================
//
#include "macros.h" // REQUIRE

inline void BoundUnitList::add (BoundUnit* unit)
{
  _v.push_back (unit);
  _current = _v.size()-1;
}

inline void BoundUnitList::remove (const BoundUnit* unit)
{
  /** @pre List must not be empty. */
  REQUIRE (_v.size() > 0);
  
  // often the user will want to erase the last element accessed
  if (unit == _v [_current]) { erase (_current); return; }
  
  // else we just parse the vector until the right element is found.
  for (int i = 0; i < _v.size(); ++ i)
    { if (unit == _v[i]) { erase (i); return; } }
  
  // element was not found...
  std::cerr << "WARNING: trying to erase non existent element "
	    << "from a BoundUnitList." << std::endl;
}

inline int BoundUnitList::size (void) const 
{ 
  return _v.size(); 
}

inline const std::vector <BoundUnit*>& BoundUnitList::operator() (void) const
{
  return _v;
}

inline void BoundUnitList::erase (int index)
{
  _v [index] = _v.back();
  _v.pop_back();
  _current = 0;
}


#endif // BOUND_UNIT_LIST_H
