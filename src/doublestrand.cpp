

/**
 * @file doublestrand.cpp
 * @brief Implementation of the DoubleStrand class.
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
#include "doublestrand.h"
#include "chemicalsequence.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
DoubleStrand::DoubleStrand (ChemicalSequence& sense, 
			    ChemicalSequence& antisense)
  : _sense (sense)
  , _antisense (antisense)
{
  sense.set_appariated_sequence (antisense);
  antisense.set_appariated_sequence (sense);
}

// Not needed for this class (use of compiler-generated versions)
// DoubleStrand::DoubleStrand (const DoubleStrand& other);
// DoubleStrand& DoubleStrand::operator= (const DoubleStrand& other);
// DoubleStrand::~DoubleStrand (void);

// ===========================
//  Public Methods - Commands
// ===========================
//
void DoubleStrand::add (int number)
{
  Chemical::add (number);
  _sense.add (number);
  _antisense.add (number);
}

void DoubleStrand::remove (int number)
{
  Chemical::remove (number);
  _sense.remove (number);
  _antisense.remove (number);
}

// ============================
//  Public Methods - Accessors
// ============================
//


// =================
//  Private Methods
// =================
//
