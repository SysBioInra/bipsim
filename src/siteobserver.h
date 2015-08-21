

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
 * @brief Watch a specific Site and receive notifications when its availability changed.
 *
 * SiteObserver is an abstract class designed to be inherited by classes that need to 
 * know when a specific site availability has changed. The observer class subscribes to a
 * the chemical sequence of interest and receives notifications via update() function
 * calls of the current number of available sites.
 */
class SiteObserver
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor.
   */
  SiteObserver (void) {}

  /**
   * @brief Destructor.
   */
  virtual ~SiteObserver (void) {}

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Perform necessary actions when availability of the observed site has changed.
   * @param number_available_sites Number of sites currently available.
   */
  virtual void update (int number_available_sites) = 0;

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

  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @brief Check class invariant.
   * @return True if class invariant is preserved.
   */
  virtual bool check_invariant (void) const
  {
    return true;
  }

private:

  // ============
  //  Attributes
  // ============
  //
  

  // =================
  //  Private Methods
  // =================
  //

  // ======================
  //  Forbidden Operations
  // ======================
  //
  /**
   * @brief Copy constructor forbidden.
   */
  SiteObserver (const SiteObserver& other_site_observer);

  /**
   * @brief Assignment operator forbidden.
   */
  SiteObserver& operator= ( const SiteObserver& other_site_observer );
};

// ======================
//  Inline declarations
// ======================
//

#endif // SITE_OBSERVER_H
