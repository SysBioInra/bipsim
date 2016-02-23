

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
#include "freeendfactory.h"
#include "bindingsite.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
const int Segment::END = std::numeric_limits<int>::max();

Segment::Segment (int first, int last, const FreeEndFactory& factory) 
  : _first (first)
  , _last (last)
  , _right_site (0)
  , _left_site (0)
{
  /** first must be smaller than last. */
  REQUIRE (first <= last);
  /** values must be positive. */
  REQUIRE ((first == START) || (first >= 0));
  if ((_first != START) && (_first != END))
    { _right_site = factory.create_right (first); }
  if ((_last != START) && (_last != END))
    { _left_site = factory.create_left (last); }
}

// Forbidden
// Segment::Segment (const Segment& other);
// Segment& Segment::operator= (const Segment& other);

Segment::~Segment (void)
{
  delete _right_site;
  delete _left_site;
}


// ===========================
//  Public Methods - Commands
// ===========================
//
void Segment::extend_right (void) 
{ 
  REQUIRE ((_first != START) && (_first != END));
  --_first;
  if (_first != START)
    { _right_site->move(1); } // site moves on OPPOSITE strand
  else { delete _right_site; }
}

void Segment::extend_left (void) 
{ 
  REQUIRE ((_last != START) && (_last != END));
  ++_last;
  _left_site->move(-1); // site moves on OPPOSITE strand
}

void Segment::absorb_left (Segment& other)
{
  delete _left_site; delete other._right_site;
  _last = other._last;
  _left_site = other._left_site;
  other._first = START; other._last = START;
  other._right_site = 0; other._left_site = 0;
}

void Segment::set_first (int position, const FreeEndFactory& factory)
{
  /** position must be smaller than last. */
  REQUIRE (position <= _last);
  /** position must be positive. */
  REQUIRE ((position == START) || (position >= 0));
  _first = position;
  delete _right_site;
  if ((_first != START) && (_first != END)) 
    { _right_site = factory.create_right (_first); }
  else { _right_site = 0; }
}

void Segment::set_last (int position, const FreeEndFactory& factory)
{
  /** position must be greater than first. */
  REQUIRE (position >= _first);
  /** position must be positive. */
  REQUIRE ((position == START) || (position >= 0));
  _last = position;
  delete _left_site;
  if ((_last != START) && (_last != END)) 
    { _left_site = factory.create_left (_last); }
  else { _left_site = 0; }
}


// ============================
//  Public Methods - Accessors
// ============================
//


// =================
//  Private Methods
// =================
//
