

/**
 * @file graphratemanager.h
 * @brief Header for the GraphRateManager class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef GRAPH_RATE_MANAGER_H
#define GRAPH_RATE_MANAGER_H

// ==================
//  General Includes
// ==================
//
#include <vector> // std::vector
#include <map> // std::map
#include <list> // std::list
#include <set> // std::set

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "ratemanager.h"

/**
 * @brief Class handling updates of reaction rates by using dependencies 
 *  between reactions and reactants.
 *
 * GraphRateManager inherits and implements the interface provided by
 * RateManager. It handles its updating task by a graph approach: 
 * it stores a map that links a reactant to all reactions that depend on it, 
 * enabling to update only those reactions which rate might have been changed.
 * It acts as an observer: it attaches itself to all reactants it knows and 
 * receives updates when concentrations change. All rates that may have 
 * changed are stored in a set until user asks to update rates.
 * @sa RateManager
 */
class GraphRateManager : public RateManager
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor.
   * @param params Simulation parameters.
   * @param reactions Vector of reactions whose rates need to be stored and updated.
   * @param dependency_graph Global dependencies between reactions.
   */
  GraphRateManager (const SimulationParams& params,
		    const std::vector <Reaction*>& reactions,
		    const DependencyGraph& dependency_graph);

 private:
  // Forbidden
  /** @brief Copy constructor. */
  GraphRateManager (const GraphRateManager& other_manager);
  /** @brief Assignment operator.*/
  GraphRateManager& operator= (const GraphRateManager& other_manager);
 public:

  /**
   * @brief Destructor.
   */
  ~GraphRateManager (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // redefined from RateManager
  /**
   * @brief Update rates according to current chemical levels.
   *
   * This manager uses a graph approach. Using an observer architecture, it 
   * receives updates about reaction occuring and stores reactions whose rates 
   * may have changed. When prompted to update, it recomputes these rates.
   */
  void update_rates (void);

  /**
   * @brief Notify a change in concentration levels and update list of rates to
   *  recompute.
   * @param reactions_to_update Indices of the reactions that need to be
   *  updated. This function is intended to be called by a ReactionObserver.
   */
  void update (const std::list<int>& reactions_to_update);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
private:

  // ============
  //  Attributes
  // ============
  //
  /**
   * @brief Vector indicating reactions whose rates need to be recomputed at next update.
   */
  std::vector <bool> _reactions_to_update;

  /**
   * @brief List of observers used to monitor reaction occurring.
   */
  std::list <ReactionObserver*> _reaction_observers;

  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Create reaction observers to receive notifications about reaction 
   *  occuring.
   * @param dependency_graph Global dependencies between reactions.
   */
  void create_observers (const DependencyGraph& dependency_graph);

  /**
   * @brief Clear all reaction observers.
   */
  void clear_observers (void);

  /**
   * @brief Convert list of reaction pointers to list of integers.
   * @return List of integers corresponding to reactions found in the map. 
   *  If reactions are not found in the map, they are ignored, so that the 
   *  list returned can be shorter than the initial list of reaction pointers.
   * @param reaction_to_index Map enabling to convert reaction pointers to 
   *  integers.
   * @param reactions Set of reactions to convert to integers.
   */
  std::list<int> convert_to_indices (const std::map <Reaction*, int>& reaction_to_index, 
				     const std::set <Reaction*>& reactions);
};

// ======================
//  Inline declarations
// ======================
//

#endif // GRAPH_RATE_MANAGER_H
