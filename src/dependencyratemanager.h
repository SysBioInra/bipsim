

/**
 * @file dependencyratemanager.h
 * @brief Header for the DependencyRateManager class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef DEPENDENCY_RATE_MANAGER_H
#define DEPENDENCY_RATE_MANAGER_H

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
 * @brief Class handling updates of reaction rates by using dependencies between
 *  reactions and reactants.
 *
 * DependencyRateManager inherits and implements the interface provided by
 * RateManager. It handles its updating task by a dependency approach: it stores
 * a map that links a reactant to all reactions that depend on it, enabling to
 * update only those reactions which rate might have been changed. It acts as an
 * observer: it attaches itself to all reactants it knows and receives updates
 * when concentrations change. All rates that may have changed are stored in a
 * set until user asks to update rates.
 * @sa RateManager
 */
class DependencyRateManager : public RateManager
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
   */
  DependencyRateManager (const SimulationParams& params,
			 const std::vector <Reaction*>& reactions);

 private:
  // Forbidden
  /** @brief Copy constructor. */
  DependencyRateManager (const DependencyRateManager& other);
  /** @brief Assignment operator. */
  DependencyRateManager& operator= (const DependencyRateManager& other);
 public:
  
  // /* @brief Destructor. */
  // ~DependencyRateManager (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // redefined from RateManager
  /**
   * @brief Update rates according to current chemical levels.
   * @details This manager uses a dependency approach. Using an observer
   *  architecture, it receives updates about concentration changes and stores
   *  reactions whose rates may have changed. When prompted to update, it 
   *  recomputes these rates.
   */
  void update_rates (void);

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
inline void DependencyRateManager::update_rates (void)
{
  while (!_rate_validity.empty())
    {
      update_reaction (_rate_validity.front());
      _rate_validity.pop();
    }
  cumulate_rates();
}

#endif // DEPENDENCY_RATE_MANAGER_H
