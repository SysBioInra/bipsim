
/**
 * @file reactant.h
 * @brief Header for the Reactant class.
 * @authors Marc Dinh, Stephan Fischer
 */

// Multiple include protection
//
#ifndef REACTANT_H
#define REACTANT_H

// ==================
//  General Includes
// ==================
//
#include <list> // std::list

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"

/**
 * @brief Abstract class that contains all possible reactants.
 */
class Reactant
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  // Not needed for this class (use of compiler-generated versions)
  // /* @brief Constructor. */
  // Reactant (void);
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // Reactant (const Reactant& other);
  // /* @brief Assignment operator. */
  // Reactant& operator= (const Reactant& other);

  /** @brief Destructor. */
  virtual ~Reactant (void) = 0;

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Attach an observer for notification when a change occurs.
   * @param observer RateInvalidator to add to observer list.
   */
  void attach (RateInvalidator& observer);

  /**
   * @brief Detach a previously added observer from observer list.
   * @param observer RateInvalidator to remove from observer list.
   */
  void detach (RateInvalidator& observer);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  
 protected:
  // ===================
  //  Protected Methods
  // ===================
  //
  /** @brief Notify all observers. */
  void notify_change (void);

 private:
  // =================
  //  Private Methods
  // =================
  //

  // ============
  //  Attributes
  // ============
  //  
  /** @brief Set of observers to notify. */
  std::list <RateInvalidator*> _observers;
};

// ======================
//  Inline declarations
// ======================
//
#include "rateinvalidator.h"

inline Reactant::~Reactant (void) {}

inline void Reactant::attach (RateInvalidator& observer) 
{
  _observers.push_back (&observer); 
}

inline void Reactant::detach (RateInvalidator& observer) 
{
  _observers.remove (&observer);
}

inline void Reactant::notify_change (void)
{
  for (std::list <RateInvalidator*>::iterator obs_it = _observers.begin();
       obs_it != _observers.end(); ++obs_it)
    { (*obs_it)->update(); }
}

#endif // REACTANT_H
