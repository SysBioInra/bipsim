

/**
 * @file reactant.h
 * @brief Header for the Reactant class.
 * 
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
#include <set> // std::set

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "dependencyratemanager.h"

/**
 * @brief Class representing any entity that can be involved in a reaction.
 *
 * Reactant mainly provides an interface for handling reaction rate updates.
 * The type inheritance allows to list all components of a reaction as a list
 * of Reactant, while the implementation inheritance provides a way to communicate
 * with a DependencyRateManager automatically whenever a change in concentration occurs.
 */
class Reactant
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor.
   */
  Reactant (void) {}

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor.
  //  */
  // Reactant ( const Reactant& other_reactant );

  /**
   * @brief Destructor.
   */
  virtual ~Reactant (void) {}

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Attach a DependencyRateManager for notification when concentration changes.
   * @param rate_manager RateManager to add to observer list.
   */
  void attach (DependencyRateManager& rate_manager)
  {
    _observers.insert (&rate_manager);
  }

  /**
   * @brief Detach a previously added RateManager from observer list.
   * @param rate_manager RateManager to remove from observer list.
   */
  void detach (DependencyRateManager& rate_manager)
  {
    _observers.erase (&rate_manager);
  }

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
  // Reactant& operator= ( const Reactant& other_reactant );

  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @brief Check class invariant.
   * @return True if class invariant is preserved.
   */
  virtual bool check_invariant (void) const {return true;}

 protected:
  // ===================
  //  Protected Methods
  // ===================
  //
  /**
   * @brief Notify all observers that concentration has changed.
   */
  void notify_concentration_change (void)
  {
    for (std::set <DependencyRateManager*>::iterator obs_it = _observers.begin();
	 obs_it != _observers.end(); obs_it++)
      {
	(*obs_it)->update (this);
      }
  }


 private:

  // ============
  //  Attributes
  // ============
  //
  /**
   * @brief List of observers to notify when concentration changes.
   */
  std::set <DependencyRateManager*> _observers;

  // =================
  //  Private Methods
  // =================
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

#endif // REACTANT_H
