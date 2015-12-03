

/**
 * @file reactionnode.h
 * @brief Header for the ClassName class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef REACTION_NODE_H
#define REACTION_NODE_H

// ==================
//  General Includes
// ==================
//


// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"
#include "ratenode.h"

/**
 * @brief Class for leaf nodes in a tree storing reaction rates.
 *
 * ReactionNode stores a rate associated to a reaction. It is a leaf node: it
 * has no children. It can only be updated by changing the rate it stores.
 * ReactionNode inherits RateNode.
 * @sa RateTree.
 */
class ReactionNode : public RateNode
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor.
   */
  ReactionNode (int rate_index)
    : _rate_index (rate_index)
  {
  }

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor.
  //  */
  // ReactionNode (const ReactionNode& other_class_name);

  /**
   * @brief Destructor.
   */
  ~ReactionNode (void) {}

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  int find (double value) { return _rate_index; }

  // ============================
  //  Public Methods - Accessors
  // ============================
  //

  // ==========================
  //  Public Methods - Setters
  // ==========================
  //
  /**
   * @brief Change rate associated to reaction represented by the node.
   */
  RateNode* set_rate (double rate)
  { 
    _set_rate (rate);
    return invalidate_parent();
  }


  // =======================================
  //  Public Methods - Operator overloading
  // =======================================
  //
  // Not needed for this class (use of default overloading) !
  // /*
  //  * @brief Assignment operator.
  //  */
  // ReactionNode& operator= (const ReactionNode& other_class_name);

protected:
  // ======================
  //  Protected Attributes
  // ======================
  //

  // ===================
  //  Protected Methods
  // ===================
  //


private:
  // ============
  //  Attributes
  // ============
  //
  int _rate_index;

  // =================
  //  Private Methods
  // =================
  //
  void perform_update (void) {}

  // ======================
  //  Forbidden Operations
  // ======================
  //

};

// ======================
//  Inline declarations
// ======================
//

#endif // REACTION_NODE_H
