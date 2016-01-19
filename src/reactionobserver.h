

/**
 * @file reactionobserver.h
 * @brief Header for the ReactionObserver class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef REACTION_OBSERVER_H
#define REACTION_OBSERVER_H

// ==================
//  General Includes
// ==================
//
#include <list> // std::list

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"

/**
 * @brief Class observing reactions and noticing parents when a reaction occurs.
 *
 * ReactionObserver uses an observer pattern to receive notifications from 
 * reactions when they are performed and sends an update() command to its 
 * RateManager parent, passing along a user-defined message, i.e. identifiers
 * of the reactions to update in the rate manager.
 */
class ReactionObserver
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor.
   * @param parent Reference to the rate manager to warn when a change occurs.
   * @param reaction_to_observe Reference to the reaction to observe.
   * @param message Identifiers used by the rate manager to determine what it 
   *  needs to do.
   */
  ReactionObserver (GraphRateManager& parent, Reaction* reaction_to_observe, 
		    const std::list<int>& message);

 private:
  // Forbidden
  // /* @brief Copy constructor. */
  // ReactionObserver (const ReactionObserver& other_observer);
  // /* @brief Assignment operator. */
  // ReactionObserver& operator= (const ReactionObserver& other_observer);

 public:
  /**
   * @brief Destructor.
   */
  ~ReactionObserver (void);    

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Called to warn about reaction changes.
   */
  void update (void);  

  /**
   * @brief Called to signal that observed subject is destructed.
   */
  void deregister (void);
  

  // ============================
  //  Public Methods - Accessors
  // ============================
  //

private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Parent to warn when reaction changes. */
  GraphRateManager& _parent;

  /** @brief Reaction to observe. */
  Reaction* _reaction;
  
  /** @brief Message to pass along with reaction change notifications. */
  std::list<int> _message;

  // =================
  //  Private Methods
  // =================
  //
};

// ======================
//  Inline declarations
// ======================
//
inline void ReactionObserver::deregister (void)
{
  _reaction = 0;
}


#endif // REACTION_OBSERVER_H
