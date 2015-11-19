

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
#include "cobserverclient.h"

/**
 * @brief Class handling updates of reaction rates by using dependencies between reactions and reactants.
 *
 * DependencyRateManager inherits and implements the interface provided by RateManager.
 * It handles its updating task by a dependency approach: it stores a map that links a reactant to all
 * reactions that depend on it, enabling to update only those reactions which rate might have been changed.
 * It acts as an observer: it attaches itself to all reactants it knows and receives updates when concentrations
 * change. All rates that may have changed are stored in a set until user asks to update rates.
 * @sa RateManager
 */
class DependencyRateManager : public RateManager, public CObserverClient
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor.
   * @param reactions Vector of reactions whose rates need to be stored and updated.
   */
  DependencyRateManager (const std::vector <Reaction*>& reactions);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor.
  //  */
  // DependencyRateManager ( const DependencyRateManager& other_dependency_rate_manager );

  /**
   * @brief Destructor.
   */
  virtual ~DependencyRateManager (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Update rates according to current chemical levels.
   *
   * This manager uses a dependency approach. Using an observer architecture, it receives
   * updates about concentration changes and stores reactions whose rates may have changed.
   * When prompted to update, it recomputes these rates.
   */
  virtual void update_rates (void);

  /**
   * @brief [Re]set the vector of reactions handled by the class.
   * @param reactions Vector of reactions whose rates need to be stored and updated.   
   */
  virtual void manage (const std::vector <Reaction*>& reactions);

  /**
   * @brief Notify a change in concentration levels and update list of rates to recompute.
   * @param reactions_to_update Indices of the reactions that need to be updated. This function
   *  is intended to be called by a ConcentrationObserver.
   */
  void update (int reaction_to_update);

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
  // DependencyRateManager& operator= ( const DependencyRateManager& other_dependency_rate_manager );


private:

  // ============
  //  Attributes
  // ============
  //
  /**
   * @brief Vector indicating reactions whose rates need to be recomputed at next update.
   */
  std::vector <bool> _reactions_to_update;

  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Create dependency map from the list of reactions and subscribe for notfication by reactants.
   */
  void create_dependencies (void);

  // ======================
  //  Forbidden Operations
  // ======================
  //

};

// ======================
//  Inline declarations
// ======================
//
inline void DependencyRateManager::update (int reaction_to_update)
{
  _reactions_to_update [reaction_to_update] = true;
}

#endif // DEPENDENCY_RATE_MANAGER_H
