

/**
 * @file siteobserver.h
 * @brief Header for the SiteObserver class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef SITE_OBSERVER_H
#define SITE_OBSERVER_H

// ==================
//  General Includes
// ==================
//


// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "site.h"
#include "chemicalsequence.h"

/**
 * @brief Watch a specific Site and receive notifications when its 
 *  availability changed.
 *
 * SiteObserver is an abstract class designed to be inherited by classes 
 * that need to know when a specific site availability has changed. The 
 * observer class subscribes to a chemical sequence of interest and receives 
 * notifications via update() function calls of the current number of 
 * available sites.
 */
class SiteObserver
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  // Not needed for this class (use of compiler-generated versions)
  // /* @brief Default constructor. */
  // SiteObserver (void);
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor forbidden. */
  // SiteObserver (const SiteObserver& other_site_observer);
  // /* @brief Assignment operator forbidden. */
  // SiteObserver& operator= (const SiteObserver& other_site_observer);
  /** @brief Destructor. */
  virtual ~SiteObserver (void) {}

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Perform necessary actions when availability of the observed site 
   *  has changed.
   * @param number_available_sites Number of sites currently available.
   */
  virtual void update (int number_available_sites) = 0;

  // ============================
  //  Public Methods - Accessors
  // ============================
  //

private:
  // ============
  //  Attributes
  // ============
  //
  
  // =================
  //  Private Methods
  // =================
  //
};

// ======================
//  Inline declarations
// ======================
//

#endif // SITE_OBSERVER_H
