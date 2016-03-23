

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

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "ratemanager.h"
#include "ratevalidity.h"

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

  // /* @brief Destructor. */
  // ~GraphRateManager (void);

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
  // =================
  //  Private Methods
  // =================
  //

  // ============
  //  Attributes
  // ============
  //
  /** @brief Container storing rate indices to update. */
  RateValidity _rate_validity;
};

// ======================
//  Inline declarations
// ======================
//
inline void GraphRateManager::update_rates (void)
{
  while (!_rate_validity.empty())
    {
      update_reaction (_rate_validity.front());
      _rate_validity.pop();
    }
  cumulate_rates();
}

#endif // GRAPH_RATE_MANAGER_H
