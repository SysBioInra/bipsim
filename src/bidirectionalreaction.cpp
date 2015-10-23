

/**
 * @file reaction.cpp
 * @brief Implementation of the BidirectionalReaction class.
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
#include "bidirectionalreaction.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
BidirectionalReaction::BidirectionalReaction (void)
{
}

// Not needed for this class (use of default copy constructor) !
// BidirectionalReaction::BidirectionalReaction (BidirectionalReaction& other_bidireactional_reaction);

BidirectionalReaction::~BidirectionalReaction (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//


// ============================
//  Public Methods - Accessors
// ============================
//


// ==========================
//  Public Methods - Setters
// ==========================
//


// =======================================
//  Public Methods - Operator overloading
// =======================================
//
// Not needed for this class (use of default overloading) !
// BidirectionalReaction& BidirectionalReaction::operator= (BidirectionalReaction& other_bidireactional_reaction);

std::ostream& operator<< (std::ostream& output, const BidirectionalReaction& reaction)
{
  reaction.print (output);
  return output;
}


// =================
//  Private Methods
// =================
//
