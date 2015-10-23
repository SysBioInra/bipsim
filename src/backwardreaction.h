
/**
 * @file backwardreaction.h
 * @brief Header for the BackwardReaction class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef BACKWARD_REACTION_H
#define BACKWARD_REACTION_H

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
 * @brief Adpater class representing backward part of a BidirectionalReaction.
 *
 * BackwardReaction is an adapter class used to transform a
 * BidirectionalReaction into a simple Reaction. It represents the backward part
 * of the reaction.
 * @sa Reaction, BidirectionalReaction, ForwardReaction
 */
class BackwardReaction : public Reaction
{
public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor
   * @param bidirectional_reaction
   *  BidirectionalReaction whose backward part will be accessed by the adapter.
   */
  BackwardReaction (BidirectionalReaction& bidirectional_reaction)
    : _reaction (bidirectional_reaction)
  {
    _reactants = _reaction.backward_reactants();
    _products = _reaction.forward_reactants();
  }
    

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // BackwardReaction (BackwardReaction& other_base_loading);

  /**
   * @brief Destructor
   */
  virtual ~BackwardReaction (void) {}
  
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
    return _reaction.is_backward_reaction_possible();
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
  // BackwardReaction& operator= (BackwardReaction& other_base_loading);


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
  virtual void do_reaction (void) { _reaction.perform_backward(); }

  /**
   * @brief Compute current reaction rates.
   * @return Current reaction rate.
   */
  virtual double compute_rate (void) const { return _reaction.backward_rate(); }

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

#endif // BACKWARD_REACTION_H
