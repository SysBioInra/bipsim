

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
#include <list> //std::list

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

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // Not needed for this class (use of default copy constructor) !
  // /* @brief Copy constructor. */
  // DependencyRateManager (const DependencyRateManager& other_manager);
  // /* @brief Assignment operator. */
  // DependencyRateManager& operator= (const DependencyRateManager& other_manager);
  // /* @brief Destructor. */
  // virtual ~DependencyRateManager (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // redefined from RateManager
  /**
   * This manager uses a dependency approach. Using an observer architecture,
   * it receives updates about concentration changes and stores reactions
   * whose rates may have changed. When prompted to update, it recomputes 
   * these rates.
   */
  virtual void update_rates (void);

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
  RateValidity _rate_validity;

  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Create dependency map from the list of reactions and subscribe for notfication by reactants.
   */
  void create_dependencies (void);
};

// ======================
//  Inline declarations
// ======================
//

#endif // DEPENDENCY_RATE_MANAGER_H
