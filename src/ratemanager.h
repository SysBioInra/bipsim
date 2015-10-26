

/**
 * @file ratemanager.h
 * @brief Header for the RateManager class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef RATE_MANAGER_H
#define RATE_MANAGER_H

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

/**
 * @brief Abstract class for handling updates of reaction rates.
 *
 * RateManager is an abstract class that stores reaction rates and
 * provides an interface for rate updating that has to be implemented
 * by inheriting classes. Objects are built from vectors of reactions
 * and are responsible for updating rates when prompted by user. Rates
 * are made available in a vector that of same size as reaction vector.
 */
class RateManager
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
  RateManager (const std::vector <Reaction*>& reactions);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor.
  //  */
  // RateManager ( const RateManager& other_rate_manager );

  /**
   * @brief Destructor.
   */
  virtual ~RateManager (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Update rates according to current chemical levels.
   */
  virtual void update_rates (void) = 0;

  /**
   * @brief [Re]set the vector of reactions handled by the class.
   * @param reactions Vector of reactions whose rates need to be stored and
   *  updated.   
   */
  virtual void manage (const std::vector <Reaction*>& reactions);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Accessor to the cumulated rate vector.
   * @return Vector containing cumulated reaction rates (as of last update).
   */
  const std::vector<double>& cumulated_rates (void) const;

  /**
   * @brief Accessor to total reaction rate.
   * @brief Total reaction rate (as of last update).
   */
  double total_rate (void) const;

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
  // RateManager& operator= ( const RateManager& other_rate_manager );


 protected:
  
  // ===================
  //  Protected Methods
  // ===================
  //
  /**
   * @brief Recompute rates for all reactions and refill whole _rates vector..
   */
  void compute_all_rates (void);  

  /**
   * @brief Update rates for reaction at a specific index.
   * @param reaction_index Index of the reaction in the reaction vector.
   */
  void update_reaction (int reaction_index);

  /**
   * @brief Compute cumulated rates and total rate from the _rates vector.
   */
  void cumulate_rates (void);

  /**
   * @brief Accessor to reaction vector.
   * @return Reactions handled by the rate manager.
   */
  const std::vector <Reaction*>& reactions (void) const;

 private:

  // ============
  //  Attributes
  // ============
  //
  /**
   * @brief Vector of reactions currently handled by the manager.
   */
  std::vector <Reaction*> _reactions;

  /**
   * @brief Vector of rates as of last update.
   */
  std::vector <double> _rates;

  /**
   * @brief Vector of cumulated rates as of last update.
   */
  std::vector <double> _cumulated_rates;

  /**
   * @brief Total reaction rate.
   */
  double _total_rate;

  // ===================
  //  Protected Methods
  // ===================
  //
  

  // ======================
  //  Forbidden Operations
  // ======================
  //

};

// ==================
//  Inline Includes
// ==================
//
#include <numeric> // std::partial_sum
#include "reaction.h"

// ======================
//  Inline declarations
// ======================
//
inline void RateManager::compute_all_rates (void)
{
  for (int i = 0; i < _reactions.size(); ++i) { update_reaction (i); }
}

inline void RateManager::cumulate_rates (void)
{
  std::partial_sum (_rates.begin(), _rates.end(), _cumulated_rates.begin());
  _total_rate = _cumulated_rates.back();
}

inline void RateManager::update_reaction (int reaction_index)
{
  Reaction* reaction_to_update = _reactions [reaction_index];
  reaction_to_update->update_rate();
  _rates [reaction_index] = reaction_to_update->rate();
}

inline const std::vector <double>& RateManager::cumulated_rates (void) const
{
  return _cumulated_rates;
}

inline const std::vector <Reaction*>& RateManager::reactions (void) const
{
  return _reactions;
}

inline double RateManager::total_rate (void) const
{
  return _total_rate;
}


#endif // RATE_MANAGER_H
