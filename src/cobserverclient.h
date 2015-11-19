

/**
 * @file cobserverclient.h
 * @brief Header for the CObserverClient class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef COBSERVER_CLIENT_H
#define COBSERVER_CLIENT_H

// ==================
//  General Includes
// ==================
//
#include <list> // std::list

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"

/**
 * @brief Abstract class enabling handling of a pool of ConcentrationObserver.
 *
 * CObserverClient handles a pool of ConcentrationObserver. Classes that inherit
 * it can add new observers or clear the whole pool. At creation, they specify
 * a target for the observer and an identifier the observer should send each
 * time it receives an update. This identifier is then transmitted via the 
 * update() function that needs to be implemented by the inheriting class.
 */
class CObserverClient
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor.
   */
  CObserverClient (void) {}

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor.
  //  */
  // CObserverClient (const CObserverClient& other_class_name);

  /**
   * @brief Destructor.
   */
  ~CObserverClient (void) { clear_observers(); }

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Function called by observers to notify an update.
   * @param identifier Identifier of the observer having received an update.
   */
  virtual void update (int identifier) = 0;

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
  // CObserverClient& operator= (const CObserverClient& other_class_name);

protected:
  // ======================
  //  Protected Attributes
  // ======================
  //

  // ===================
  //  Protected Methods
  // ===================
  //
  /**
   * @brief Add an observer to the pool.
   * @param reactant Reactant to observer.
   * @param identifier Integer identifier of the observer. Should be passed
   *  to the parent each time an update occurs.
   */
  void add_observer (Reactant& reactant, int identifier);

  /**
   * @brief Clear the pool of observers.
   */
  void clear_observers (void);


private:

  // ============
  //  Attributes
  // ============
  //
  /** Observer pool. */
  std::list <ConcentrationObserver*> _observers;

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
#include "concentrationobserver.h"
inline void CObserverClient::add_observer (Reactant& reactant, int identifier)
{
  _observers.push_back (new ConcentrationObserver (*this, &reactant,
						   identifier));
}

inline void CObserverClient::clear_observers (void)
{
  // unsubscribe reactant notifications by deleting observers
  for (std::list<ConcentrationObserver*>::iterator obs_it = _observers.begin();
       obs_it != _observers.end(); ++obs_it)
    {
      delete *obs_it; // this automatically detaches the observer
    }
  _observers.clear();
}

#endif // COBSERVER_CLIENT_H
