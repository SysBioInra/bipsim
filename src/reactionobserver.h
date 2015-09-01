

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
 * ReactionObserver uses an observer pattern to receive notifications from reactions
 * when they are performed and sends an update() command to its RateManager parent, passing along
 * a user-defined message, i.e. identifiers of the reactions to update in the rate manager.
 * @sa Reaction
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
   * @param raection_to_observe Reference to the reaction to observe.
   * @param message Identifiers used by the rate manager to determine what it needs to do.
   */
  ReactionObserver (GraphRateManager& parent, Reaction* reaction_to_observe, const std::list<int>& message);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor.
  //  */
  // ReactionObserver ( const ReactionObserver& other_reaction_observer );

  /**
   * @brief Destructor.
   */
  ~ReactionObserver (void);    

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Called to warn about reaction changed.
   *
   * The call is simply redirected to the parent along with a reference of the reaction that changed.
   */
  void update (void);  

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
  // /*
  //  * @brief Assignment operator.
  //  */
  // ReactionObserver& operator= (const ReactionObserver& other_reaction_observer);

  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @brief Check class invariant.
   * @return True if class invariant is preserved.
   */
  bool check_invariant (void) const;

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

  // ======================
  //  Forbidden Operations
  // ======================
  //

};

// =================
//  Inline Includes
// =================
//

// ======================
//  Inline declarations
// ======================
//

#endif // REACTION_OBSERVER_H
