

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
#include <map> // std::map
#include <set> // std::set
#include <vector> // std::vector

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
 * rates. For example, given that reaction A occurred, which rates of other 
 * reactions should I update ? Or, said differently, which rates can in afford
 * to NOT update.
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
   */
  DependencyGraph (const std::vector<Reaction*>& reactions);

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // DependencyGraph (const DependencyGraph& other_dependency_graph);
  // /* @brief Assignment operator. */
  // DependencyGraph& operator= (const DependencyGraph& other_dependency_graph);
  // /* @brief Destructor */
  // ~DependencyGraph (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Get reactions to update given last reaction that occurred.
   * @param reaction_occurring Pointer to the last reaction that occured.
   * @return Set of pointers of the reactions that need to be updated.
   */
  const std::set<Reaction*>& reactions_to_update (Reaction* reaction_occurring) const;

  /** 
   * @brief Accessor to the reactions used to create the dependency graph.
   * @return Vector of reactions used to create the dependency graph.
   */
  const std::vector<Reaction*>& reactions (void) const;

  // ==========================
  //  Public Methods - Setters
  // ==========================
  //

private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Vector that stores dependencies between reactions. */
  std::map< Reaction*, std::set<Reaction*> > _dependencies;

  /** @brief Reactions used to create the dependency graph. */
  std::vector <Reaction*> _reactions;  


  // =================
  //  Private Methods
  // =================
  //
};

// ======================
//  Inline declarations
// ======================
//

inline const std::set<Reaction*>& DependencyGraph::reactions_to_update (Reaction* reaction_occurring) const
{
  /** @pre Reaction must be known. */
  REQUIRE (_dependencies.find (reaction_occurring) != _dependencies.end()); 
  return _dependencies.find(reaction_occurring)->second;
}

inline const std::vector<Reaction*>& DependencyGraph::reactions (void) const
{
  return _reactions;
}

#endif // DEPENDENCY_GRAPH_H
