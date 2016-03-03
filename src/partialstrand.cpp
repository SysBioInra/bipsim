

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
PartialStrand::PartialStrand (int length, 
			      const FreeEndHandler& free_end_handler)
: _length (length)
, _complete (false)
, _free_end_handler (free_end_handler)
{
  // We place two dummy segments to mark beginning and end of the sequence.
  // We start with {[S S],[E E]} so nothing is occupied at beginning.
  // Dummy segments can be extended and ligated so when
  // the strand is completed we will be reduced to {[S E]}.
  // Using dummy segments simplifies algorithms used in the class
  // (because a lot of potential border effects are removed).
  _segments.push_back (new Segment (Segment::START, Segment::START,
				    _free_end_handler));
  _segments.push_back (new Segment (Segment::END, Segment::END,
				    _free_end_handler));
  _segment_it = _segments.begin();
}


// Forbidden
// PartialStrand::PartialStrand (const PartialStrand& other)
// PartialStrand& PartialStrand::operator= (const PartialStrand& other)

PartialStrand::~PartialStrand (void)
{
  // delete segments
  for (std::list <Segment*>::iterator segment_it = _segments.begin();
       segment_it != _segments.end(); ++segment_it)
    { delete *segment_it; }
}

// ===========================
//  Public Methods - Commands
// ===========================
//
bool PartialStrand::start_segment (int position)
{
  /** @pre position must be consistent with length provided at construction. */
  REQUIRE ((position >= 0) && (position < _length));

  std::list <Segment*>::iterator result = _find (position);
  // if previous segment spans position, no new segment can be created
  if ((*result)->last() > position) { return false; }
  _segment_it = result;

  // if the new segment starts right next to an existing segment, we elongate
  // the existing segment, else we create a new one
  if ((*_segment_it)->last() < position)
    {
      if (position != 0)
	{
	  ++_segment_it;
	  int last_position = (position+1 < _length)? position+1 : Segment::END;
	  _segment_it = _segments.
	    insert (_segment_it, 
		    new Segment (position-1, last_position, _free_end_handler));
	}
      else
	{
	  (*_segment_it)->set_last (1);
	  // put a free end on the last segment if necessary...
	  if (_segments.back()->first() == Segment::END)
	    { _segments.back()->set_first (_length-1); }
	}
    }
  else { extend_segment (position); }
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
