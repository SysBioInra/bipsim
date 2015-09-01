

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
#include "dependencyratemanager.h"

/**
 * @brief Class observing reactants and noticing parents when concentration changes.
 *
 * ConcentrationObserver uses an observer pattern to receive notifications from reactants
 * when concentration changes and sends an update() command to its owner, passing along
 * a user-defined message, i.e. identifiers of the reactions to update in the rate manager.
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
   * @brief Default constructor.
   * @param parent Reference to the rate manager to warn when a change occurs.
   * @param reactant_to_observe Reference to the reactant to observe.
   * @param message Identifiers used by the rate manager to determine what it needs to do.
   */
  ConcentrationObserver (DependencyRateManager& parent, Reactant* reactant_to_observe, const std::list<int>& message);

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

  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @brief Check class invariant.
   * @return True if class invariant is preserved.
   */
  bool check_invariant (void) const;


private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Parent to warn when concentration changes. */
  DependencyRateManager& _parent;

  /** @brief Reactant to observe. */
  Reactant* _reactant;
  
  /** @brief Message to pass along with concentration change notifications. */
  std::list<int> _message;

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
inline void ConcentrationObserver::update (void)
{
  _parent.update (_message);
}

#endif // CONCENTRATION_OBSERVER_H
