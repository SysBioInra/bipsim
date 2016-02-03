

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

  // (3-0 rule: either define all 3 following or none of them)
  /** @brief Copy constructor. */
  PartialStrand (const PartialStrand& other);
  /** @brief Assignment operator. */
  PartialStrand& operator= (const PartialStrand& other);
  // /* @brief Destructor. */ really not needed here.
  // ~PartialStrand (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Start new segment at given position.
   * @param position Position where segment should be started.
   * @return True if segment was created, false if position was occupied.
   */
  bool start_new_segment (int position);

  /**
   * @brief Extend existing segment at given position.
   * @param position Position where extension should take place.
   * @return True if extension took place, False if position was occupied or
   *  if there was no segment spanning to given position.
   */
  bool extend_segment (int position);


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
   * @brief Accessor to left free ends.
   * @return List of positions of free ends on the left side of segments.
   */
  std::list <int> left_ends (void) const;

  /**
   * @brief Accessor to right free ends.
   * @return List of positions of free ends on the right side of segments.
   */
  std::list <int> right_ends (void) const;

private:
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

  /** @brief Last segment used. */
  std::list <Segment>::iterator _segment_it;

  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Move _segment_it to segment immediately starting before position.
   * @param position Position used to select segment.
   */
  void _move_to_segment (int position);

  /**
   * @brief Check whether last segment manipulated should be ligated to 
   *  segment that directly follows.
   */
  void _check_ligation (void);
};

// ======================
//  Inline declarations
// ======================
//
#include "macros.h"
#include "segment.h"

inline bool PartialStrand::extend_segment (int position)
{
  /** @pre position must be consistent with length provided at construction. */
  REQUIRE ((position >= 0) && (position < _length));

  // A segment can be extended:
  //  (i) if the previous segment spans exactly until the given position
  //  (ii) if position = 0, the last segment must span until the last base 
  //       (note that condition (i) holds because of the [-1 0] dummy segment).
  // Condition (ii) is fullfilled iff a segment has been fused to the dummy
  // [L L] segment placed at construction. In this case, the last segment
  // is no longer [L L] but some [a L] where a ≠ L.
  _move_to_segment (position);
  if ((_segment_it->last != position)
      || ((position == 0) && (_segments.back().first == _length))) 
    { return false; }
  
  ++(_segment_it->last);
  _check_ligation();
  return true;
}

inline bool PartialStrand::completed (void) const
{
  return _complete;
}

inline bool move_postcondition (std::list <Segment>::iterator result, 
				int position)
{
  std::list <Segment>::iterator next = result; ++next;
  return ((result->first <= position) 
	  && ((result->last < position) || (next->first > position)));
}

inline void PartialStrand::_move_to_segment (int position)
{
  /** @pre position must be consistent with length provided at construction. */
  REQUIRE ((position >= 0) && (position < _length));

  // this algorithm works thanks to the dummy segments
  if (_segment_it->first <= position)
    { 
      if (_segment_it->last < position)
	{
	  ++_segment_it;
	  while (_segment_it->first <= position) { ++_segment_it; } 
	  --_segment_it;
	}
    }
  else
    { while (_segment_it->first > position) { --_segment_it; } }

  /**
   * @post The selected segment must meet two conditions. (i) starting position
   *  of the pointed segment must be smaller or equal to position. (ii) starting
   *  position of the following segment must be strictly greater than position.
   *  This is always true thanks to the dummy segments inserted at construction.
   */
  ENSURE (move_postcondition (_segment_it, position));
}

inline void PartialStrand::_check_ligation (void)
{
  std::list <Segment>::iterator next_it = _segment_it; ++next_it;

  // ligation occurs if end of last manipulated segments meets its follower
  if ((next_it != _segments.end())
      && (_segment_it->last == next_it->first))
    { 
      _segment_it->last = next_it->last; 
      _segments.erase (next_it); 

      // strand is complete when our two dummy segments finally meet <3
      if ((_segment_it->first == -1) && (_segment_it->last == _length)) 
	{ _complete = true; }
    }  

  ENSURE ((_complete == true) || (_segments.size() > 1));
}


#endif // PARTIAL_STRAND_H
