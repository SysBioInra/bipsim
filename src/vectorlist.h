
/**
 * @file vectorlist.h
 * @brief Header for the VectorList class.
 * @authors Marc Dinh, Stephan Fischer
 */

// Multiple include protection
//
#ifndef VECTOR_LIST_H
#define VECTOR_LIST_H

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
 * @brief Class storing a list of elements in a vector in arbitrary order.
 * @tparam Type of elements to store.
 * @details VectorList stores elements in a vector. It enables simple operations
 *  like adding, optimized accessing and erasing of elements. What is more,
 *  compared to STL lists (as of C++ 98), it yields constant time access to 
 *  size (linear in C++ 98). For convenience, a [] operator is provided, but
 *  there is NO WARRANTY on the order of elements in the list, the user MUST
 *  assume that it is random.
 */
template <typename T>
class VectorList
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /** @brief Default constructor. */
  VectorList (void) : _current (0) {}

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // VectorList (const VectorList& other);
  // /* @brief Assignment operator. */
  // VectorList& operator= (const VectorList& other);
  // /* @brief Destructor. */
  // ~VectorList (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Add element to list (in unknown position).
   * @param element Pointer to add to list.
   */
  void add (const T& element)
  {
    _v.push_back (element);
    _current = _v.size()-1;
  }

  /**
   * @brief Erase element.
   * @param element Pointer to remove from list.
   *
   * Only the first instance of element found is removed. If no element to
   * erase is found, a warning is displayed and the list remains unchanged.
   */
  void remove (const T& element)
  {
    if (_v.size() == 0) { return; }
    
    // often the user will want to erase the last element accessed
    if (element == _v [_current]) { erase (_current); return; }
    
    // else we just parse the vector until the right element is found.
    for (int i = 0; i < _v.size(); ++ i)
      { if (element == _v[i]) { erase (i); return; } }
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
   * @brief Accessor to random element from list.
   * @return Random element stored in list.
   */
  const T& random_element (void) const
    {
      /** @pre List must not be empty. */
      REQUIRE (_v.size() > 0);
      _current = RandomHandler::instance().draw_uniform (0, _v.size()-1);
      return _v [_current];
    }

  /**
   * @brief Accessor to all elements of list.
   * @return Vector of T in an arbitrary order.
   */
  const std::vector <T>& operator() (void) const { return _v; }

private:
  // =================
  //  Private Methods
  // =================
  //
  /** 
   * @brief Remove element from vector.
   * @param index Index of element to remove.
   */
  void erase (int index)
  {
    _v [index] = _v.back();
    _v.pop_back();
    _current = 0;
  }

  // ============
  //  Attributes
  // ============
  //
  /** @brief Vector holding the stored elements. */
  std::vector <T> _v;

  /** @brief Index of last element accessed. */
  mutable int _current;
};

// ======================
//  Inline declarations
// ======================
//

#endif // VECTOR_LIST_H
