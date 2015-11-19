

/**
 * @file reactionobserver.cpp
 * @brief Implementation of the ReactionObserver class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream>

// ==================
//  Project Includes
// ==================
//
#include "reactionobserver.h"
#include "reaction.h"
#include "graphratemanager.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
ReactionObserver::ReactionObserver (GraphRateManager& parent, Reaction* reaction_to_observe, const std::list<int>& message)
  : _parent (parent)
  , _reaction (reaction_to_observe)
  , _message (message)
{
  _reaction->attach (*this);
}

// Not needed for this class (use of default copy constructor) !
// ReactionObserver::ReactionObserver ( const ReactionObserver& other_reaction_observer );

ReactionObserver::~ReactionObserver (void)
{
  if (_reaction != 0) _reaction->detach (*this);
}


// ===========================
//  Public Methods - Commands
// ===========================
//
void ReactionObserver::update (void)
{
  _parent.update (_message);
}


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
// ReactionObserver& ReactionObserver::operator= ( const ReactionObserver& other_reaction_observer );


// =================
//  Private Methods
// =================
//
