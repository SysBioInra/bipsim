

/**
 * @file bindingsiteobserver.h
 * @brief Header for the BindingSiteObserver class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef BINDING_SITE_OBSERVER_H
#define BINDING_SITE_OBSERVER_H

// ==================
//  General Includes
// ==================
//


// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "siteobserver.h"

/**
 * @brief Watch a specific BindingSite and receive notifications when its availability changed.
 *
 * BindingSiteObserver subscribes to a
 * chemical sequence of interest and receives notifications via update() function
 * calls of the current number of available binding sites. It then notifies the binding site's
 * family of the new number of available sites.
 * This class inherits abstract class SiteObserver.
 */
class BindingSiteObserver : public SiteObserver
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor.
   * @param site_to_watch BindingSite whose availability needs to be watched.
   * @param family_to_notify BindingSiteFamily that needs to be notified when availability changes.
   * @param site_identifier Integer identifier of the site within the BindingSiteFamily passed along in the notification.
   * @sa BindingSiteFamily
   */
  BindingSiteObserver (const BindingSite& site_to_watch, BindingSiteFamily& family_to_notify, int site_identifier);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor.
  //  */
  // BindingSiteObserver ( const BindingSiteObserver& other_binding_site_observer );

  /**
   * @brief Destructor.
   */
  virtual ~BindingSiteObserver (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Notify BindingSiteFamily that the number of available site has changed.
   * @param number_available_sites Number of sites currently available.
   */
  virtual void update (int number_available_bindingsites);

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
  // BindingSiteObserver& operator= ( const BindingSiteObserver& other_bindingsite_observer );

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
  /** @brief Family to notify when availability of the site has changed. */
  int _site_id;

  /** @brief Family to notify when availability of the site has changed. */
  BindingSiteFamily& _family;

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

#endif // BINDING_SITE_OBSERVER_H
