

/**
 * @file segment.cpp
 * @brief Implementation of the ClassName class.
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
#include "segment.h"
#include "freeendhandler.h"
#include "bindingsite.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
const int Segment::END = std::numeric_limits<int>::max();

Segment::Segment (int first, int last, const FreeEndHandler& handler) 
  : _first (first)
  , _last (last)
  , _right_site (0)
  , _left_site (0)
  , _handler (handler)
{
  /** first must be smaller than last. */
  REQUIRE (first <= last);
  /** values must be positive. */
  REQUIRE ((first == START) || (first >= 0));

  if ((_first != START) && (_first != END))
    { _left_site = _handler.create_left (first); }
  if ((_last != START) && (_last != END))
    { _right_site = _handler.create_right (last); }
}

// Forbidden
// Segment::Segment (const Segment& other);
// Segment& Segment::operator= (const Segment& other);

Segment::~Segment (void)
{
  _handler.remove (_right_site);
  _handler.remove (_left_site);
}


// ===========================
//  Public Methods - Commands
// ===========================
//
void Segment::extend_left (void) 
{ 
  REQUIRE ((_first != START) && (_first != END));
  --_first;

  if (_left_site)
    {
      if (_first != START)
	{ _left_site->move(1); } // site moves on OPPOSITE strand
      else { _handler.remove (_left_site); }
    }
}

void Segment::extend_right (void) 
{ 
  REQUIRE ((_last != START) && (_last != END));
  ++_last;
  if (_right_site) { _right_site->move(-1); } // site moves on OPPOSITE strand
}

void Segment::absorb_right (Segment& other)
{
  REQUIRE (&_handler == &(other._handler));

  _last = other._last;
  other._first = START; other._last = START;
  
  if (_right_site || other._right_site)
    {
      _handler.remove (_right_site);
      _right_site = other._right_site;
      other._right_site = 0;
    }
  if (other._left_site)
    { _handler.remove (other._left_site); other._left_site = 0; }
}

void Segment::set_first (int position)
{
  /** position must be smaller than last. */
  REQUIRE (position <= _last);
  /** position must be positive. */
  REQUIRE ((position == START) || (position >= 0));

  _first = position;

  _handler.remove (_left_site);
  if ((_first != START) && (_first != END)) 
    { _left_site = _handler.create_left (_first); }
  else { _left_site = 0; }
}

void Segment::set_last (int position)
{
  /** position must be greater than first. */
  REQUIRE (position >= _first);
  /** position must be positive. */
  REQUIRE ((position == START) || (position >= 0));

  _last = position;

  _handler.remove (_right_site);
  if ((_last != START) && (_last != END)) 
    { _right_site = _handler.create_right (_last); }
  else { _right_site = 0; }
}


// ============================
//  Public Methods - Accessors
// ============================
//


// =================
//  Private Methods
// =================
//
