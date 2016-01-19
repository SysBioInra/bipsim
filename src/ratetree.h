

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

 private:
  // Forbidden
  /** @brief Copy constructor. */
  RateTree (const RateTree& other_rate_tree);
  /** @brief Assignment operator. */
  RateTree& operator= (const RateTree& other_rate_tree);

 public:
  /**
   * @brief Destructor.
   */
  ~RateTree (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // Redefined from RateContainer
  int random_index (void) const;
  void update_cumulates (void);
  void set_rate (int index, double value);

  /**
   * @brief Find reaction index corresponding to cumulated rate value.
   * @param value Cumulated rate value.
   * @return Reaction index such that the cumulated rate < index
   *  is smaller than cumulated_value while the cumulated rate <= index
   *  is greater or equal to cumulated_value. This index is computed from
   *  the cumulated rates as of last update, not current rates.
   */
  int find (double value) const;

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  // Redefined from RateContainer
  double total_rate (void) const;

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
  // Redefined from RateContainer
  std::ostream& _print (std::ostream& output) const;

  /**
   * @brief Compute sum of rates contained in leaves directly.
   * @return Sum of rates contained in leaves.
   *
   * This function is for verification purposes.
   */
  double _sum_of_leaf_rates (void) const;  
};

// ======================
//  Inline declarations
// ======================
//
#include "ratenode.h"
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

inline int RateTree::random_index (void) const
{
  /** Total rate must be strictly positive. */
  ENSURE (total_rate() > 0);
  return find (RandomHandler::instance().draw_uniform
	       (1e-16*total_rate(), total_rate()));
}

inline double RateTree::_sum_of_leaf_rates (void) const
{
  double total = 0;
  for (std::vector <ReactionNode*>::const_iterator l_it = _leaves.begin();
       l_it != _leaves.end(); ++l_it)
    {
      total += (*l_it)->rate();
    }
  return total;
}

inline std::ostream& RateTree::_print (std::ostream& output) const
{
  for (std::vector <ReactionNode*>::const_iterator l_it = _leaves.begin();
       l_it != _leaves.end(); ++l_it)
    {
      output << (*l_it)->rate() << " ";
    }
  output << total_rate();
  return output;
}

#endif // RATE_TREE_H
