
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
  , _number_occupied (0)
  , _occupied (length, false)
{
}

// PartialStrand::PartialStrand (const PartialStrand& other)
// PartialStrand& PartialStrand::operator= (const PartialStrand& other)
// PartialStrand::~PartialStrand (void)

// ===========================
//  Public Methods - Commands
// ===========================
//

// ============================
//  Public Methods - Accessors
// ============================
//

std::vector <int> PartialStrand::segments (void) const
{
  std::vector <int> result;
  if (_occupied [0]) { result.push_back (0); }
  for (int i = 1; i < _length; ++i)
    {
      // detect beginning of segment
      if (_occupied [i] && !(_occupied [i-1])) { result.push_back (i); }
      // detect end of segment
      if (!(_occupied [i]) && _occupied [i-1]) { result.push_back (i-1); }
    }
  if (_occupied [_length-1]) { result.push_back (_length-1); }
  /** @post Length of result must be a multiple of 2. */
  ENSURE (result.size() % 2 == 0);
  return result;
}

// =================
//  Private Methods
// =================
//
