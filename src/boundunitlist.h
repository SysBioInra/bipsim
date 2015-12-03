

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
#include <list> // std::list

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
 * size (linear in C++ 98).
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
  BoundUnitList (void)
    : _size (0)
    , _current_position (0)
    { _current_element = _list.begin(); }

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
   * @brief Add element at beginning of list.
   * @param unit Pointer to add to list.
   */
  void push_front (BoundUnit* unit)
  {
    ++_size;
    _list.push_front (unit);
    _current_position = 0;
    _current_element = _list.begin();
  }

  /**
   * @brief Add element at end of list.
   * @param unit Pointer to add to list.
   */
  void push_back (BoundUnit* unit)
  {
    ++_size;
    _list.push_back (unit);
    _current_position = _size-1;
    _current_element = _list.end();
    --_current_element;
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
    // often the user will want to erase the last element accessed
    if (unit == *_current_element) { _list.erase (_current_element); }
    else
      {
	// else we just parse the list until the right element is found.
	std::list <BoundUnit*>::iterator unit_it = _list.begin();
	while ((unit_it != _list.end()) && (unit != *unit_it)) { ++unit_it; }
	if (unit_it != _list.end()) { _list.erase (unit_it); }
	else
	  {
	    std::cerr << "WARNING: trying to erase non existent element "
		      << "from a BoundUnitList." << std::endl;
	    return;
	  }
      }
    --_size;
    _current_position = 0;
    _current_element = _list.begin();
  }
  
  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Accessor to list size.
   * @return Number of elements stored in list.
   */
  int size (void) const { return _size; }

  /**
   * @brief Accessor to element of list.
   * @param index Index of element to access.
   * @return Pointer to element at position corresponding to index.
   */
  BoundUnit* operator[] (int index)
  {
    /** @pre index must be consistent with list size. */
    REQUIRE ((index >= 0) && (index < _size));
    if (index < _current_position) 
      { 
	_current_position = 0;
	_current_element = _list.begin();
      }
    while (_current_position < index)
      { 
	++_current_position;
	++_current_element;
      }
    return *_current_element;
  }

  /**
   * @brief Accessor to element of list (const version).
   * @param index Index of element to access.
   * @return Const pointer to element at position corresponding to index.
   */
  const BoundUnit* operator[] (int index) const
  {
    /** @pre index must be consistent with list size. */
    REQUIRE ((index >= 0) && (index < _size));
    if (index < _current_position) 
      { 
	_current_position = 0;
	_current_element = 
	  const_cast <std::list <BoundUnit*>&> (_list).begin();
      }
    while (_current_position < index)
      { 
	++_current_position;
	++_current_element;
      }
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
    for (std::list <BoundUnit*>::const_iterator unit_it = _list.begin();
	 unit_it != _list.end(); ++unit_it)
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
  /** @brief List holding the stored pointers. */
  std::list <BoundUnit*> _list;

  /** @brief Iterator to last element accessed. */
  mutable std::list <BoundUnit*>::iterator _current_element;

  /** @brief Iterator to last position accessed. */
  mutable int _current_position;

  /** @brief size of the list. */
  int _size;

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

#endif // BOUND_UNIT_LIST_H
