

/**
 * @file vectorqueue.h
 * @brief Header for the VectorQueue class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef VECTOR_QUEUE_H
#define VECTOR_QUEUE_H

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
 * @brief Generic class implementing a queue as a vector.
 *
 * VectorQueue offers classical queue functionalities, except it uses a fixed
 * amount of memory, chosen at construction. This avoids constant memory
 * [de]allocation. Operations are slightly different from classical queue
 * operations, as the pop() method is inefficient on a vector implementation. It
 * is replaced by a clear() method that resets the queue. Because of this
 * difference it is particularly useful when alternating phases where the
 * queue is building up with phases where it is completely read and cleared.
 */
template <typename T>
class VectorQueue
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor.
   * @param max_size Maximal size for the queue.
   */
  VectorQueue (int max_size)
    : _v (max_size)
    , _current_position (_v.begin())
    , _max_position (_v.begin())
    {}

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // VectorQueue (const VectorQueue& other_vector_queue);
  // /* @brief Assignment operator. */
  // VectorQueue& operator= (const VectorQueue& other_vector_queue);
  // /* @brief Destructor. */
  // ~VectorQueue (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Push new element into the queue.
   * @param value Element to add to the queue.
   */
  void push (const T& value)
  {
    /** @pre Queue must not be full. */
    REQUIRE (_max_position != _v.end());
    *_max_position = value;
    ++_max_position;
  }

  /**
   * @brief Go to next element in the queue.
   */
  void next (void)
  {
    /** @pre End of queue must not be reached already. */
    REQUIRE (_current_position != _max_position);
    ++_current_position;
  }

  /**
   * @brief Clear queue.
   */
  void clear (void)
  {
    _current_position = _max_position = _v.begin();
  }

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Accessor to current element.
   * @return Element currently pointed to in the queue.
   */
  const T& item (void) const
  { 
    /** @pre End of queue must not have been reached. */
    REQUIRE (!end());
    return *_current_position;
  }

  /**
   * @brief Returns whether end of queue is reached.
   * @return True if end of queue is reached.
   */
  bool end (void) const { return _current_position == _max_position; }

private:
  // ============
  //  Attributes
  // ============
  //
  /** @brief Vector representing the queue. */
  std::vector <T> _v;
  
  /** @brief Current position of the queue (within the vector). */
  typename std::vector <T>::iterator _current_position;

  /** @brief Current maximal position of the queue. */
  typename std::vector <T>::iterator _max_position;

  // =================
  //  Private Methods
  // =================
  //
};

// ======================
//  Inline declarations
// ======================
//

#endif // VECTOR_QUEUE_H
