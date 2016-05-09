

/**
 * @file partialstrand.h
 * @brief Header for the PartialStrand class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef PARTIAL_STRAND_H
#define PARTIAL_STRAND_H

// ==================
//  General Includes
// ==================
//
#include <list> // std::list
#include <vector> // std::vector

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
 * @brief Class representing a strand composed of subsegments.
 */
class PartialStrand
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /** @brief Constructor. */
  PartialStrand (int length);


 private:
  // Forbidden
  /** @brief Copy constructor. */
  PartialStrand (const PartialStrand& other);
  /** @brief Assignment operator. */
  PartialStrand& operator= (const PartialStrand& other);
 public:
  
  // /* @brief Destructor. */
  // ~PartialStrand (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Occupy given position.
   * @param position Position that should be started.
   * @return True if occupation succeeded, false if position was occupied.
   */
  bool occupy (int position);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Check whether strand has been completed.
   * @return True if strand has been completed.
   */
  bool completed (void) const;

  /**
   * @brief Positon of segments composing partial strand.
   * @return Vector containing positions of segments. Numbers are to be read
   *  two by two, each pair representing a new segment. E.g. [1, 10, 15, 20]
   *  means that the strand is composed of segment [1 10] and [15 20].
   */
  std::vector <int> segments (void) const;

private:
  /** 
   * @brief struct representing segments. Important: extremities of segments 
   *  are considered unoccupied. 
   */
  struct Segment 
  { 
    Segment (int first, int last) : first (first), last (last) {}
    int first, last; 
  };

  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Join segment to segment at its right if there is a one base
   *  gap between them.
   * @return True if joining succeeded.
   */
  bool join_right (void);

  /**
   * @brief Find segment containing or immediately starting before position.
   * @param position Position used to select segment.
   * @return Segment containing or immediately starting before position.
   */
  std::list <Segment>::iterator find (int position);
  bool find_postcondition (std::list <Segment>::iterator result, int position);

  /** @brief Check whether partial strand is complete. */
  void check_completeness (void);

  // ============
  //  Attributes
  // ============
  //
  /** @brief Length of the strand. */
  int _length;

  /** @brief True if strand has been completed. */
  bool _complete;

  /** @brief List of sequence segments. */
  std::list <Segment> _segments;

  /** @brief Segment currently used. */
  std::list <Segment>::iterator _segment_it;
};

// ======================
//  Inline declarations
// ======================
//
#include "macros.h"

inline bool PartialStrand::completed (void) const
{
  return _complete;
}

inline std::list <PartialStrand::Segment>::iterator 
PartialStrand::find (int position)
{
  /** @pre position must be consistent with length provided at construction. */
  REQUIRE ((position >= 0) && (position < _length));

  std::list <Segment>::iterator result = _segment_it;
  if (result->first < position)
    { 
      if (result->last < position)
	{
	  ++result;
	  while (result->first <= position) { ++result; } 
	  --result;
	}
    }
  else
    { while (result->first > position) { --result; } }

  /**
   * @post The selected segment (a b) must meet two conditions. 
   *  (i) a must be smaller or equal to position.
   *  (iia) b is larger or equal to position
   *  OR (iib) a of the following segment is strictly larger than position.
   */
  ENSURE (find_postcondition (result, position));
  return result;
}

inline bool PartialStrand::find_postcondition 
(std::list <Segment>::iterator result, int position)
{
  std::list <Segment>::iterator next = result; ++next;
  return ((result->first <= position) 
	  && ((result->last >= position) || (next->first > position)));
}

inline bool PartialStrand::join_right (void)
{
  REQUIRE (_segment_it != _segments.end());
  std::list <Segment>::iterator next_it = _segment_it; ++next_it;
  REQUIRE (next_it != _segments.end());
  // junction rule: (a b) + (b c) = (a c)
  if (_segment_it->last == next_it->first)
    { 
      _segment_it->last = next_it->last; 
      _segments.erase (next_it);
      return true;
    }  
  else { return false; }
}

inline void PartialStrand::check_completeness (void)
{
  // strand is complete when our two dummy segments finally meet <3
  _complete = (_segment_it->first == -1) && (_segment_it->last == _length);
  ENSURE ((_complete == true) || (_segments.size() > 1));
}

#endif // PARTIAL_STRAND_H
