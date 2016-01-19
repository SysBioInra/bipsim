
/**
 * @file forwardreaction.h
 * @brief Header for the ForwardReaction class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef FORWARD_REACTION_H
#define FORWARD_REACTION_H

// ==================
//  General Includes
// ==================
//
#include <string>

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "bidirectionalreaction.h"
#include "reaction.h"

// ===================
//  Class Declaration
// ===================
//
/**
 * @brief Adpater class representing forward part of a BidirectionalReaction.
 *
 * ForwardReaction is an adapter class used to transform a BidirectionalReaction
 * into a simple Reaction. It represents the forward part of the reaction
 * @sa Reaction, BidirectionalReaction, BackwardReaction
 */
class ForwardReaction : public Reaction
{
public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor
   * @param bidirectional_reaction
   *  BidirectionalReaction whose forward part will be accessed by the adapter.
   */
  ForwardReaction (BidirectionalReaction& bidirectional_reaction)
    : _reaction (bidirectional_reaction)
  {
    _reactants = _reaction.forward_reactants();
    _products = _reaction.backward_reactants();
  }
    
  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // ForwardReaction (const ForwardReaction& other_reaction);
  // /* @brief Assignment operator. */
  // ForwardReaction& operator= (const ForwardReaction& other_reaction);
  // /* @brief Destructor. */
  // ~ForwardReaction (void);

  
  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  //

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  // Redefined from Reaction
  bool is_reaction_possible (void) const
  {
    return _reaction.is_forward_reaction_possible();
  }

private:
  // ============
  //  Attributes
  // ============
  //
  /** @brief Reaction accessed by the adapter. */
  BidirectionalReaction& _reaction;  
  
  // =================
  //  Private Methods
  // =================
  //
  // Redefined from Reaction
  void do_reaction (void) { _reaction.perform_forward(); }
  double compute_rate (void) const { return _reaction.forward_rate(); }
  void print (std::ostream& output) const { output << _reaction; }
};

// =====================
//  Inline Declarations
// =====================
//

#endif // FORWARD_REACTION_H
