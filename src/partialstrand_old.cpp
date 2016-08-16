
/**
 * @file partialstrand.cpp
 * @brief Implementation of the PartialStrand class.
 * @authors Marc Dinh, Stephan Fischer
 */

// ==================
//  General Includes
// ==================
//
#include <limits>

// ==================
//  Project Includes
// ==================
//
#include "partialstrand.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
PartialStrand::PartialStrand (int length)
  : _length (length)
  , _complete (false)
{
  // We place two dummy segments to mark beginning and end of the sequence.
  // We start with {(-1 0),(L-1 L)} so nothing is occupied at beginning,
  // because extremities of segment are considered UNOCCUPIED!
  // Dummy segments can be extended and ligated so when
  // the strand is completed we will be reduced to {(-1 L)}.
  // Using dummy segments simplifies algorithms used in the class
  // (because a lot of potential border effects are removed).
  _segments.push_back (Segment (-1, 0));
  _segments.push_back (Segment (length-1, length));
  _segment_it = _segments.begin();
}

// Forbidden
// PartialStrand::PartialStrand (const PartialStrand& other)
// PartialStrand& PartialStrand::operator= (const PartialStrand& other)
// PartialStrand::~PartialStrand (void)

// ===========================
//  Public Methods - Commands
// ===========================
//
bool PartialStrand::occupy (int position)
{
  /** @pre position must be consistent with length provided at construction. */
  REQUIRE ((position >= 0) && (position < _length));

  std::list <Segment>::iterator result = find (position);
  // if position is within an existing segment, no new segment can be created
  if ((result->first < position) && (result->last > position)) { return false; }
  _segment_it = result;

  // if the new segment starts right next to an existing segment, we extend
  // the existing segment, else we create a new one
  if (_segment_it->first == position) 
    {
      // extend to the left: first try to fuse with preceding segment
      --_segment_it;
      if (!join_right()) { ++_segment_it; --(_segment_it->first); }
      check_completeness();
    }
  else if (_segment_it->last == position) 
    { 
      // extend to the right: first try to fuse with following segment
      if (!join_right()) { ++(_segment_it->last); }
      check_completeness();
    }
  else 
    {
      // no joining = new segment
      ++_segment_it;
      _segment_it = _segments.insert 
	(_segment_it, Segment (position-1, position+1));
    }
  return true;
}

// ============================
//  Public Methods - Accessors
// ============================
//

std::vector <int> PartialStrand::segments (void) const
{
  std::vector <int> result;
  for (std::list <Segment>::const_iterator it = _segments.begin();
       it != _segments.end(); ++it)
    {
      int first = it->first; int last = it->last;
      // print if not a dummy segment
      if (last > first+1) 
	{ result.push_back (first+1); result.push_back (last-1); }
    }
  return result;
}

// =================
//  Private Methods
// =================
//
