

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
  , _sense_factory (sense, antisense, _left_ends, _right_ends)
  , _antisense_factory (antisense, sense, _left_ends, _right_ends)
{
  sense.set_appariated_strand (antisense, _sense_factory);
  antisense.set_appariated_strand (sense, _antisense_factory);
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
