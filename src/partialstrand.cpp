

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

// Not needed for this class (use of compiler-generated versions)
// PartialStrand::PartialStrand (void);
// PartialStrand::PartialStrand (const PartialStrand& other_strand);
// PartialStrand& PartialStrand::operator= (const PartialStrand& other_strand);
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


// =================
//  Private Methods
// =================
//
