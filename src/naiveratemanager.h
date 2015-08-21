

/**
 * @file naiveratemanager.h
 * @brief Header for the NaiveRateManager class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef NAIVE_RATE_MANAGER_H
#define NAIVE_RATE_MANAGER_H

// ==================
//  General Includes
// ==================
//


// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "ratemanager.h"

/**
 * @brief Class handling updates of reaction rates in a naive way.
 *
 * NaiveRateManager inherits and implements the interface provided by RateManager.
 * It handles its updating task by a brute force approach: every time it is supposed
 * to update rates, it recomputes every single rate it knows of.
 * @sa RateManager
 */
class NaiveRateManager : public RateManager
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
  NaiveRateManager (const std::vector <Reaction*>& reactions);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor.
  //  */
  // NaiveRateManager ( const NaiveRateManager& other_naive_rate_manager );

  /**
   * @brief Destructor.
   */
  virtual ~NaiveRateManager (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Update rates according to current chemical levels.
   *
   * This manager uses a brute force approach: when prompted to update, it recomputes
   * every single rate under its control.
   */
  virtual void update_rates (void);


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
  // NaiveRateManager& operator= ( const NaiveRateManager& other_naive_rate_manager );

  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @brief Check class invariant.
   * @return True if class invariant is preserved.
   */
  virtual bool check_invariant (void) const;


private:

  // ============
  //  Attributes
  // ============
  //

  // ===================
  //  Protected Methods
  // ===================
  //
  

  // ======================
  //  Forbidden Operations
  // ======================
  //

};

// ======================
//  Inline declarations
// ======================
//

#endif // NAIVE_RATE_MANAGER_H
