

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
 * @brief Watch a specific BindingSite and receive notifications when its
 *  availability changed.
 *
 * BindingSiteObserver subscribes to a chemical sequence of interest and
 * receives notifications via update() function calls of the current number of 
 * available binding sites. It then notifies the binding site's
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
   * @param family_to_notify BindingSiteFamily that needs to be notified when
   *  availability changes.
   * @param site_identifier Integer identifier of the site within the
   *  BindingSiteFamily passed along in the notification.
   * @sa BindingSiteFamily
   */
  BindingSiteObserver (const BindingSite& site_to_watch,
		       BindingSiteFamily& family_to_notify,
		       int site_identifier);

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // BindingSiteObserver (const BindingSiteObserver& other_observer);
  // /* @brief Assignment operator. */
  // BindingSiteObserver& operator= (const BindingSiteObserver& other_observer);
  // /* @brief Destructor. */
  // virtual ~BindingSiteObserver (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // redefined from SiteObserver
  /**
   * @brief Perform necessary actions when availability of the observed site 
   *  has changed.
   * @param number_available_sites Number of sites currently available.
   *
   * Notify BindingSiteFamily that the number of available site has changed. 
   */
  void update (int number_available_sites);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //

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
};

// ======================
//  Inline declarations
// ======================
//
#include "bindingsitefamily.h"

inline 
BindingSiteObserver::BindingSiteObserver (const BindingSite& site_to_watch,
					  BindingSiteFamily& family_to_notify,
					  int site_identifier)
  : _site_id (site_identifier)
  , _family (family_to_notify)
{
  site_to_watch.location().watch_site_availability
    (site_to_watch.first(), site_to_watch.last(), *this);
}

inline void BindingSiteObserver::update (int number_of_available_sites)
{
  /** @pre Number of available sites must be positive); */
  REQUIRE (number_of_available_sites >= 0);
  _family.update (_site_id, number_of_available_sites);
}

#endif // BINDING_SITE_OBSERVER_H
