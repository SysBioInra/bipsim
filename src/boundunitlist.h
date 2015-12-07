

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
#include "bindingsite.h"
#include "boundunit.h"

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
  /**
   * @brief Default constructor.
   */
  BoundUnitList (void) { _current_element = _v.begin(); }

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor.
  //  */
  // BoundUnitList (const BoundUnitList& other_bound_unit_list);

  /**
   * @brief Destructor.
   */
  ~BoundUnitList (void) 
    {
    }

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Add element to list (in unknown position).
   * @param unit Pointer to add to list.
   */
  void insert (BoundUnit* unit)
  {
    _v.push_back (unit);
    _current_element = _v.end()-1;
  }

  /**
   * @brief Erase element.
   * @param unit Pointer to remove from list.
   *
   * Only the first instance of element found is removed. If no element to
   * erase is found, a warning is displayed and the list remains unchanged.
   */
  void erase (const BoundUnit* unit)
  {
    /** @pre List must not be empty. */
    REQUIRE (_v.size() > 0);

    // often the user will want to erase the last element accessed
    if (unit == *_current_element) { _erase (_current_element); return; }

    // else we just parse the vector until the right element is found.
    std::vector <BoundUnit*>::iterator unit_it = _v.begin();
    while (unit_it != _v.end())
      {
	if (unit == *unit_it) { _erase (unit_it); return; }
	++unit_it;
      }

    // element was not found...
    std::cerr << "WARNING: trying to erase non existent element "
	      << "from a BoundUnitList." << std::endl;
  }
  
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
   * @brief Accessor to element of list.
   * @param index Index of element to access.
   * @return Pointer to element at position corresponding to index.
   * 
   * Note that position CANNOT be directly deduced from insertion order. This
   * function is provided for convenience. It was prefered to a random_item()
   * method to avoid drawing a random number within the class. Basically, it
   * only make sense to use this operator with a random number, but letting
   * the caller draw the random number allows for optimizations on the caller's
   * side.
   */
  BoundUnit* operator[] (int index)
  {
    /** @pre index must be consistent with list size. */
    REQUIRE ((index >= 0) && (index < _v.size()));
    _current_element = _v.begin()+index;
    return *_current_element;
  }

  /**
   * @brief Accessor to element of list (const version).
   * @param index Index of element to access.
   * @return Const pointer to element at position corresponding to index.
   * @sa operator[]
   */
  const BoundUnit* operator[] (int index) const
  {
    /** @pre index must be consistent with list size. */
    _current_element = 
      const_cast <std::vector <BoundUnit*>&> (_v).begin()+index;
    return *_current_element;
  }

  /**
   * @brief Accessor to total unbinding rate of stored elements.
   * @return Sum of k_offs of binding sites of the BoundUnit stored.
   * @sa BoundUnit.
   */
  double unbinding_rate (void) const
  {
    // we loop through the list and sum the k_off
    double r_total = 0;
    for (std::vector <BoundUnit*>::const_iterator unit_it = _v.begin();
	 unit_it != _v.end(); ++unit_it)
      { r_total += (*unit_it)->binding_site().k_off(); }
    return r_total;
  }

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
  // BoundUnitList& operator= (const BoundUnitList& other_bound_unit_list);

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
  /** @brief Vector holding the stored pointers. */
  std::vector <BoundUnit*> _v;

  /** @brief Iterator to last element accessed. */
  mutable std::vector <BoundUnit*>::iterator _current_element;

  /** @brief Iterator to last valid element. */
  mutable std::vector <BoundUnit*>::iterator _last_element;

  /** @brief size of the list. */
  int _size;

  // =================
  //  Private Methods
  // =================
  //
  void _erase (std::vector <BoundUnit*>::iterator _element)
  {
    *_element = _v.back();
    _v.pop_back();
    _current_element = _v.begin();
  }

  // ======================
  //  Forbidden Operations
  // ======================
  //

};

// ======================
//  Inline declarations
// ======================
//

#endif // BOUND_UNIT_LIST_H
