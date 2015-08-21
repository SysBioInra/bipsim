

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
#include "reaction.h"

/**
 * @brief Abstract class for handling updates of reaction rates.
 *
 * RateManager is an abstract class that stores reaction rates and
 * provides an interface for rate updating that has to be implemented
 * by inheriting classes. Objects are built from vectors of reactions
 * and are responsible for updating rates when prompted by user. Rates
 * are made available in a vector that is twice the size of the reactions
 * an object manages: the rates of reaction i are stored at indices 
 * 2i (forward rate) and 2i+1 (backward rate) of the vectors returned
 * by rate();
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
   * @param reactions Vector of reactions whose rates need to be stored and updated.   
   */
  virtual void manage (const std::vector <Reaction*>& reactions);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Accessor to the rate vector.
   * @return Vector containing reaction rates (as of last update). Size of vector is twice the size of
   * reaction set handled. Rates of reaction of index i are stored at indices 2i
   * (forward) and 2i+1 (backward) of the rate vector.
   */
  const std::vector<double>& rates (void) const;

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

  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @brief Check class invariant.
   * @return True if class invariant is preserved.
   */
  virtual bool check_invariant (void) const;


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
   * @brief Compute total rate by summing all values currently contained in the _rates vector.
   */
  void compute_total_rate (void);

  /**
   * @brief Accessor to reaction vector.
   * @param Reactions handled by the rate manager.
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
   * @brief Vector of rates as of last update. Rate for reaction i are stored at 2i (forward) and 2i+1 (backward).
   */
  std::vector <double> _rates;

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

// ======================
//  Inline declarations
// ======================
//
inline const std::vector <double>& RateManager::rates (void) const
{
  return _rates;
}

inline const std::vector <Reaction*>& RateManager::reactions (void) const
{
  return _reactions;
}

inline double RateManager::total_rate (void) const
{
  return _total_rate;
}

inline void RateManager::update_reaction (int reaction_index)
{
  Reaction* reaction_to_update = _reactions [reaction_index];
  reaction_to_update->update_rates();
  _rates [2*reaction_index] = reaction_to_update->forward_rate();
  _rates [2*reaction_index+1] = reaction_to_update->backward_rate();
}

#endif // RATE_MANAGER_H
