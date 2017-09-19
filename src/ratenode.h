
/**
 * @file ratenode.h
 * @brief Header for the RateNode, DummyNode, ReactionNode, SumNode classes.
 * @authors Marc Dinh, Stephan Fischer
 */

// Multiple include protection
//
#ifndef RATE_NODE_H
#define RATE_NODE_H

// ==================
//  General Includes
// ==================
//

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"
#include "macros.h"

/**
 * @brief Abstract class used to store rates in a tree.
 *
 * RateNode stores rate in a tree structure that can be updated when prompted
 * by user. Inheriting classes should be able to store a rate. Apart from 
 * the classical node structure, they can be invalidated (because there rate
 * is not to date) and updated.
 * @sa RateTree
 */
class RateNode
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor.
   */
  RateNode (void) : _parent (0),  _rate (0), _invalidated (false)  {}

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // RateNode (const RateNode& other_class_name);
  // /* @brief Assignment operator. */
  // RateNode& operator= (const RateNode& other_class_name);
  /** @brief Destructor. */
  virtual ~RateNode (void) {}

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Update rate if necessary.
   * @return Parent node that will need an update due to update propagating
   *  to higher levels of the tree. Return 0 if parent does not need update
   *  or has already been invalidated (and thus should already be listed for
   *  update somewhere else).
   */
  RateNode* update (void)
  {
    perform_update();
    _invalidated = false;
    return invalidate_parent();
  }

  /**
   * @brief Find value in the tree.
   * @param value Cumulated rate value.
   * @return Integer corresponding to a rate index. Each node will typically
   *  propagate the request to the child encompassing the requested value until
   *  a leaf is reached which will return the rate index it holds.
   */
  virtual int find (double value) = 0;

  /**
   * @brief Mark node as needing an update.
   */
  void invalidate (void) { _invalidated = true; }

  /**
   * @brief Setter for parent.
   * @param parent Pointer to new parent for the node.
   */
  void set_parent (RateNode* parent) { _parent = parent; }

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Accessor to rate stored by the node.
   * @return Rate as of last update.
   */
  double rate (void) { return _rate; }

  /**
   * @brief Accessor to parent of the node.
   * @return Parent.
   */
  RateNode* parent (void) { return _parent; }

  /**
   * @brief Accessor to invalidation status.
   * @return True if node has been invalidated since last update.
   */
  bool is_invalidated (void) { return _invalidated; }

protected:
  // ===================
  //  Protected Methods
  // ===================
  //
  /**
   * @brief Setter for rate.
   * @param value New rate held by the node.
   */
  void _set_rate (double value) { _rate = value; }

  /**
   * @brief Invalidate parent if needed.
   * @return Pointer to parent if it has not already been invalidate, 0 if it
   *  already has.
   */
  RateNode* invalidate_parent (void)
  {
    if ((_parent == 0) || _parent->is_invalidated()) return 0;
    else { _parent->invalidate(); return _parent; }
  }

private:
  // ============
  //  Attributes
  // ============
  //
  /** @brief Parent of the node. */
  RateNode* _parent;

  /** @brief Rate stored in the node. */
  double _rate;

  /** @brief Flag indicating if the node needs to be updated. */
  bool _invalidated;

  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Update rate (without worrying about propagation to parent).
   */
  virtual void perform_update (void) = 0;
};

/**
 * @brief Class for nodes with unique children in RateTree.
 *
 * DummyNode is a class that is used to manage the depth of the branches of the
 * tree. Because it has a unique children, it breaks the binary tree pattern but
 * it can be used to ensure that all leaves are at the same depth. It passes
 * any order it receives over to its only child, hence its name.
 * DummyNode inherits RateNode.
 * @sa RateTree.
 */
class DummyNode : public RateNode
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor.
   */
  DummyNode (RateNode& child) : _child (child) { update(); }

    // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // DummyNode (const DummyNode& other_class_name);
  // /* @brief Assignment operator. */
  // DummyNode& operator= (const DummyNode& other_class_name);
  // /* @brief Destructor. */
  // ~DummyNode (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // redefined from RateNode
  int find (double value) 
  { 
    return _child.find (value); 
  }


  // ============================
  //  Public Methods - Accessors
  // ============================
  //
private:
  // ============
  //  Attributes
  // ============
  //
  /** @brief Unique child of the dummy node in the tree. */
  RateNode& _child;

  // =================
  //  Private Methods
  // =================
  //
  // redefined from RateNode
  void perform_update (void) { _set_rate (_child.rate()); }  
};

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
  ReactionNode (int rate_index) : _rate_index (rate_index) {}

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // ReactionNode (const ReactionNode& other_node);
  // /* @brief Assignment operator. */
  // ReactionNode& operator= (const ReactionNode& other_node);
  // /* @brief Destructor. */
  // ~ReactionNode (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // Redefined from RateNode
  int find (double value) 
  { 
    return _rate_index; 
  }

  /**
   * @brief Change rate associated to reaction represented by the node.
   */
  RateNode* set_rate (double rate)
  { 
    _set_rate (rate);
    return invalidate_parent();
  }

  // ============================
  //  Public Methods - Accessors
  // ============================
  //

private:
  // ============
  //  Attributes
  // ============
  //
  /** @brief Index of the reaction rate stored by node. */
  int _rate_index;

  // =================
  //  Private Methods
  // =================
  //
  // Redefined from RateNode
  void perform_update (void) {}
};

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

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // SumNode (const SumNode& other_node);
  // /* @brief Assignment operator. */
  // ReactionNode& operator= (const ReactionNode& other_node);
  // /* @brief Destructor. */
  // ~SumNode (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // Redefined from RateNode
  int find (double value)
  { 
    if (_left.rate() >= value) { return _left.find (value); }
    else { return _right.find (value - _left.rate()); }
  }
  
  // ============================
  //  Public Methods - Accessors
  // ============================
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
  // Redefined from RateNode
  void perform_update (void) { _set_rate (_left.rate() + _right.rate()); }
};

// ======================
//  Inline declarations
// ======================
//

#endif // RATE_NODE_H
