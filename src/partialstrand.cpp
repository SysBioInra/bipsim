

/**
 * @file partialstrand.cpp
 * @brief Implementation of the PartialStrand class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream> // std::cout

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
  // We place two dummy segments to mark beginning and end of the sequence
  // Last base of a segment is considered as unoccupied.
  // We start with {[-1 0],[L L]} so nothing is occupied at beginning.
  // Dummy segments can be extended and ligated so when
  // the strand is completed we will be reduced to {[-1 L]}.
  // Using dummy segments *greatly* simplifies algorithms used in the class
  // (because a lot of potential border effects are removed).
  _segments.push_back (Segment (-1, 0));
  _segments.push_back (Segment (_length, _length));
  _segment_it = _segments.begin();
}

PartialStrand::PartialStrand (const PartialStrand& other)
  : _length (other._length)
  , _complete (other._complete)
  , _segments (other._segments)
{
  _segment_it = _segments.begin();
}

PartialStrand& PartialStrand::operator= (const PartialStrand& other)
{
  _length = other._length; _complete = other._complete; 
  _segments = other._segments; _segment_it = _segments.begin();
}
// PartialStrand::~PartialStrand (void);

// ===========================
//  Public Methods - Commands
// ===========================
//
bool PartialStrand::start_new_segment (int position)
{
  /** @pre position must be consistent with length provided at construction. */
  REQUIRE ((position >= 0) && (position < _length));

  _move_to_segment (position);
  // if previous segment spans position, no new segment can be created
  if (_segment_it->last > position) { return false; }

  // if the new segment starts right next to an existing segment, we elongate
  // the existing segment, else we create a new one
  if (_segment_it->last < position)
    {
      ++_segment_it;
      _segment_it = _segments.insert (_segment_it, 
				      Segment (position, position+1));
    }
  else { ++(_segment_it->last); }
  _check_ligation();
  return true;
}


// ============================
//  Public Methods - Accessors
// ============================
//
std::list <int> PartialStrand::left_ends (void) const
{
  std::list <int> result;
  if (_complete) { return result; }

 // we ignore the first segment that starts at -1
  std::list <Segment>::const_iterator segment_it =_segments.begin(); 
  std::list <Segment>::const_iterator last_it = --_segments.end();
  ++segment_it;
  while (segment_it != last_it) 
    { result.push_back (segment_it->first); ++segment_it; }
  // keep last segment if it is not dummy
  if (segment_it->first != _length) { result.push_back (segment_it->first); }
  return result;
}

std::list <int> PartialStrand::right_ends (void) const
{
  std::list <int> result;
  if (_complete) { return result; }

  std::list <Segment>::const_iterator segment_it =_segments.begin();
  std::list <Segment>::const_iterator last_it = --_segments.end();
  // skip first segment if it's the dummy segment
  bool first_is_dummy = (segment_it->last == 0);
  if (first_is_dummy) { ++segment_it; }
  while (segment_it != last_it) 
    { result.push_back (segment_it->last - 1); ++segment_it; }
  // keep last segment if it is not dummy AND first is dummy (no circularity)
  if ((segment_it->first != _length) && (first_is_dummy))
    { result.push_back (_length - 1); }
  return result;
}


// =================
//  Private Methods
// =================
//
