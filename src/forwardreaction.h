
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
    

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // ForwardReaction (ForwardReaction& other_base_loading);

  /**
   * @brief Destructor
   */
  virtual ~ForwardReaction (void) {}
  
  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  //

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Returns whether there are enough chemicals to perform reaction.
   * @return True if there are enough reactant, false otherwise.
   */
  virtual bool is_reaction_possible (void) const
  {
    return _reaction.is_forward_reaction_possible();
  }

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
  //  * @brief Assignment operator
  //  */
  // ForwardReaction& operator= (ForwardReaction& other_base_loading);


 protected:
  // ===================
  //  Protected Methods
  // ===================
  //  


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
  /**
   * @brief Update chemical quantities according to the reaction.
   */
  virtual void do_reaction (void) { _reaction.perform_forward(); }

  /**
   * @brief Compute current reaction rates.
   * @return Current reaction rate.
   */
  virtual double compute_rate (void) const { return _reaction.forward_rate(); }

  /**
   * @brief Print class content.
   * @param output Stream where output should be written.
   */
  virtual void print (std::ostream& output) const { output << _reaction; }
};

// =====================
//  Inline Declarations
// =====================
//

#endif // FORWARD_REACTION_H
