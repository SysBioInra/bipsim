

/**
 * @file vectorstack.h
 * @brief Header for the VectorStack class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef VECTOR_STACK_H
#define VECTOR_STACK_H

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
 * @brief Generic class implementing a stack as a vector.
 *
 * VectorStack offers classical stack functionalities, except it uses a fixed
 * amount of memory, chosen at construction. Pushing operations are slightly
 * more efficient than STL based implementations of stacks using vectors.
 * @sa VectorQueue.
 */
template <typename T>
class VectorStack
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor.
   * @param max_size Maximal size for the stack.
   */
  VectorStack (int max_size)
    : _v (max_size)
    , _position (_v.end())
    {
    }

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor.
  //  */
  // VectorStack (const VectorStack& other_vector_stack);

  /**
   * @brief Destructor.
   */
  ~VectorStack (void) {}

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Push new element into the stack.
   * @param value Element to add to the stack.
   */
  void push (const T& value)
  {
    /** @pre Stack must not be full. */
    REQUIRE (_position != _v.begin());
    --_position;
    *_position = value;
  }

  /**
   * @brief Pop last element of the stack.
   */
  void pop (void)  { ++_position; }

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Accessor to current element.
   * @return Element currently pointed to in the stack.
   */
  const T& item (void) const
  { 
    /** @pre End of stack must not have been reached. */
    REQUIRE (!empty());
    return *_position;
  }

  /**
   * @brief Returns whether stack is empty.
   * @return True if stack is empty.
   */
  bool empty (void) const { return _position == _v.end(); }


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
  // VectorStack& operator= (const VectorStack& other_vector_stack);

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
  /** @brief Vector representing the stack. */
  std::vector <T> _v;
  
  /** @brief Current position of the head of stack (within the vector). */
  typename std::vector <T>::iterator _position;

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

#endif // VECTOR_STACK_H
