

/**
 * @file sumnode.h
 * @brief Header for the ClassName class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef SUM_NODE_H
#define SUM_NODE_H

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
 * @brief Class used to store a rate as the sum of two nodes' rates.
 *
 * SumNode inherits RateNode. SumNode can be used to cumulate rates within a
 * binary tree, the leaves typically representing atomic rates. The sum is only
 * computed when prompted by user.
 * @sa RateTree.
 */
class SumNode : public RateNode
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor.
   */
  SumNode (RateNode& left_child, RateNode& right_child)
    : _left (left_child)
    , _right (right_child)
  {
    update();
  }

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor.
  //  */
  // SumNode (const SumNode& other_class_name);

  /**
   * @brief Destructor.
   */
  ~SumNode (void) {}

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  int find (double value)
    { 
      if (_left.rate() >= value) return _left.find (value);
      else return _right.find (value - _left.rate());
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
  RateNode& _left;
  RateNode& _right;

  // =================
  //  Private Methods
  // =================
  //
  void perform_update (void)
  {
    _set_rate (_left.rate() + _right.rate());
  }

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
