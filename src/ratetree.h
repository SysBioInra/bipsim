

/**
 * @file ratetree.h
 * @brief Header for the RateTree class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef RATE_TREE_H
#define RATE_TREE_H

// ==================
//  General Includes
// ==================
//
#include <vector> // std::vector
#include <list> // std::list

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"
#include "ratecontainer.h"
#include "vectorqueue.h"

/**
 * @brief Class storing rates using a binary tree to cumulate rates.
 *
 * RateTree inherits from RateContainer. It cumulates reaction rates (and thus
 * total rate) as a binary tree. This allows fast updating from the leaves, as
 * only the parents of the leaves need to be updated. Drawing of random reaction
 * is efficient as searching for a cumulated value is done by going down from
 * the root, as in binary search. Both operations are O(logN) where N is the 
 * number of stored rates.
 * @sa RateVector.
 * @sa RateNode, SumNode, ReactionNode, DummyNode.
 */
class RateTree : public RateContainer
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor.
   */
  RateTree (int number_rates);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor.
  //  */
  // RateTree (const RateTree& other_class_name);

  /**
   * @brief Destructor.
   */
  ~RateTree (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  virtual int find (double value) const;

  virtual void update_cumulates (void);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  virtual double total_rate (void) const;

  // ==========================
  //  Public Methods - Setters
  // ==========================
  //
  virtual void set_rate (int index, double value);


  // =======================================
  //  Public Methods - Operator overloading
  // =======================================
  //
  // Not needed for this class (use of default overloading) !
  // /*
  //  * @brief Assignment operator.
  //  */
  // RateTree& operator= (const RateTree& other_class_name);

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
  /** @brief Root of tree (holding total rate). */
  RateNode* _root;

  /** @brief Leaves of tree (holding reaction rates). */
  std::vector <ReactionNode*> _leaves;

  /** @brief Queue of nodes to update. */
  VectorQueue <RateNode*> _update_queue;

  /** @brief Nodes of tree. */
  std::list <RateNode*> _nodes;

  // =================
  //  Private Methods
  // =================
  //
  
  // ======================
  //  Forbidden Operations
  // ======================
  //

};

// ======================
//  Inline declarations
// ======================
//
#include "ratenode.h"
#include "reactionnode.h"
#include "macros.h"

inline int RateTree::find (double value) const
{
  /** @pre value must be smaller than total tree rate. */
  REQUIRE (value <= total_rate());
  int index = _root->find (value);
  ENSURE (_leaves [index]->rate() > 0);
  return index;
}

inline double RateTree::total_rate (void) const
{
  /** @post Total rate should be positive. */
  ENSURE (_root->rate() >= 0);
  return _root->rate();
}

inline void RateTree::set_rate (int index, double value)
{
  /** @pre index must be consistent with number of rates. */
  REQUIRE ((index >= 0) && (index < _leaves.size()));
  /** @pre Rate value must be nonnegative. */
  REQUIRE (value >= 0);

  RateNode* next = _leaves [index]->set_rate (value);
  if (next != 0) _update_queue.push (next);
}


#endif // RATE_TREE_H
