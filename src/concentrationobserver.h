

/**
 * @file concentrationobserver.h
 * @brief Header for the ConcentrationObserver class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef CONCENTRATION_OBSERVER_H
#define CONCENTRATION_OBSERVER_H

// ==================
//  General Includes
// ==================
//


// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"

/**
 * @brief Class observing reactants and noticing parents when concentration
 *  changes.
 *
 * ConcentrationObserver uses an observer pattern to receive notifications from 
 * reactants when concentration changes and sends an update() command to its
 * owner, passing along a user-defined message, i.e. identifiers of the
 * reactions to update in the rate manager.
 * @sa Reactant
 */
class ConcentrationObserver
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor.
   * @param parent Reference to the rate manager to warn when a change occurs.
   * @param reactant Reference to the reactant to observe.
   * @param identifier Identifier used by the parent, passed along as message 
   *  when an update happens.
   */
  ConcentrationObserver (CObserverClient& parent, Reactant* reactant, 
			 int identifier);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor.
  //  */
  // ConcentrationObserver ( const ConcentrationObserver& other_concentration_observer );

  /**
   * @brief Destructor.
   */
  ~ConcentrationObserver (void);
    

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Called to warn about concentration changed.
   *
   * The call is simply redirected to the parent along with a reference of the reactant that changed.
   */
  void update (void);
  
  /**
   * @brief Called to signal that observed subject is destructed.
   */
  void deregister (void);

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
  // ConcentrationObserver& operator= (const ConcentrationObserver& other_concentration_observer);


private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Parent to warn when concentration changes. */
  CObserverClient& _parent;
  
  /** @brief Reactant to observe. */
  Reactant* _reactant;
  
  /** @brief Message to pass along with concentration change notifications. */
  int _message;

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
#include "macros.h"
#include "cobserverclient.h"

inline void ConcentrationObserver::update (void)
{
  /** @pre Observed reactant must not have deregistered. */
  REQUIRE (_reactant != 0);
  _parent.update (_message);
}

inline void ConcentrationObserver::deregister (void)
{
  _reactant = 0;
}

#endif // CONCENTRATION_OBSERVER_H
