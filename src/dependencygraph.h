

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
#include <list> // std::list

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
  DependencyGraph (const std::list<Reaction*>& reactions);

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
   * @brief Get reactions to update given last reaction that occurred.
   * @param reaction_occurring Pointer to the last reaction that occured.
   * @return Set of pointers of the reactions that need to be updated.
   */
  const std::set<Reaction*>& reactions_to_update (Reaction* reaction_occurring) const;

  /** 
   * @brief Accessor to the reactions used to create the dependency graph.
   * @return List of reactions used to create the dependency graph.
   */
  const std::list<Reaction*>& reactions (void) const;

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
   * @brief Check class invariant.
   * @return True if class invariant is preserved
   */
  virtual bool check_invariant (void) const;


private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Vector that stores dependencies between reactions. */
  std::map< Reaction*, std::set<Reaction*> > _dependencies;

  /** @brief Reactions used to create the dependency graph. */
  std::list <Reaction*> _reactions;  


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

inline const std::set<Reaction*>& DependencyGraph::reactions_to_update (Reaction* reaction_occurring) const
{
  /** @pre Reaction must be known. */
  REQUIRE (_dependencies.find (reaction_occurring) != _dependencies.end()); 
  return _dependencies.find(reaction_occurring)->second;
}

inline const std::list<Reaction*>& DependencyGraph::reactions (void) const
{
  return _reactions;
}

#endif // DEPENDENCY_GRAPH_H
