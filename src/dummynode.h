

/**
 * @file dummynode.h
 * @brief Header for the ClassName class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef DUMMY_NODE_H
#define DUMMY_NODE_H

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
  DummyNode (RateNode& child)
    : _child (child)
    {
      update();
    }

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor.
  //  */
  // RateNode (const RateNode& other_class_name);

  /**
   * @brief Destructor.
   */
  ~DummyNode (void) {}

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  virtual int find (double value) { return _child.find (value); }


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
  // DummyNode& operator= (const DummyNode& other_class_name);

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
  /** @brief Unique child of the dummy node in the tree. */
  RateNode& _child;

  // =================
  //  Private Methods
  // =================
  //
  virtual void perform_update (void)
  {
    _set_rate (_child.rate());
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

#endif // RATE_NODE_H
