

/**
 * @file dependencygraph.h
 * @brief Header for the DependencyGraph class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef DEPENDENCY_GRAPH_H
#define DEPENDENCY_GRAPH_H

// ==================
//  General Includes
// ==================
//
#include <vector> // std::vector
#include <set> // std::set

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "macros.h" // REQUIRE

/**
 * @brief A DependencyGraph stores dependencies between reactions.
 *
 * The aim of this class is to detect how a reaction can impact other reaction
 * rates. For example, given that reaction A occurred, which rates of other reactions
 * should I update ? Or, said differently, which rates can in afford to NOT update.
 */
class DependencyGraph
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor.
   * @param reactions Vector of reactions for which the dependencies are needed.
   *
   * Dependencies are built from the reaction vector once and for all during object
   * construction.
   */
  DependencyGraph (const std::vector<Reaction*> reactions);

  /**
   * @brief Destructor
   */
  ~DependencyGraph (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Get reaction indices to update given last reaction that occurred.
   * @param last_reaction Index of the last reaction that occured.
   * @return Set of indices of the reactions that need to be updated.
   */
  const std::set<int>& reactions_to_update (int last_reaction) const;

  // ==========================
  //  Public Methods - Setters
  // ==========================
  //


  // =======================================
  //  Public Methods - Operator overloading
  // =======================================
  //

  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @return True if class invariant is preserved
   */
  virtual bool check_invariant (void) const;


private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Vector that stores dependency between reactions. */
  std::vector< std::set<int> > _dependencies;


  // =================
  //  Private Methods
  // =================
  //


  // ======================
  //  Forbidden Operations
  // ======================
  /**
   * @brief Assignment operator forbidden.
   */
  DependencyGraph& operator= (const DependencyGraph& other_dependency_graph);

  /**
   * @brief Copy constructor forbidden.
   */
  DependencyGraph (const DependencyGraph& other_dependency_graph);


};

// ======================
//  Inline declarations
// ======================
//

inline const std::set<int>& DependencyGraph::reactions_to_update (int last_reaction_index) const
{
  /** @pre last_reaction_index must be nonnegative. */
  REQUIRE (last_reaction_index >= 0); 
  /** @pre last_reaction_index must be smaller than _dependencies size. */
  REQUIRE (last_reaction_index < _dependencies.size()); 
  return _dependencies[last_reaction_index];
}

#endif // DEPENDENCY_GRAPH_H
