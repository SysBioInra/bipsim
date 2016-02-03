

/**
 * @file siteavailability.h
 * @brief Header for the SiteAvailability class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef SITE_AVAILABILITY_H
#define SITE_AVAILABILITY_H

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

/**
 * @brief Class computing number of available sites and notifying observers on 
 *  changes.
 *
 * SiteAvailability computes availability of one site in a pool of identical
 * sequences. Each time its notify() member is called, it checks how many
 * instances of its site are available along the pool and, if this number has
 * changed since last update, it sends an update() to its observer with the new 
 * number of available sites.
 */

class SiteAvailability
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor.
   * @param site_to_notify BindingSite to notify when the number of 
   * available sites changes.
   */
  SiteAvailability (BindingSite& site_to_notify);

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // SiteAvailability (const SiteAvailability& other_site_availability);
  // /* @brief Assignment operator. */
  // SiteAvailability& operator= (const SiteAvailability& other_site_availability);
  // /* @brief Destructor. */
  // ~SiteAvailability (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Notify site if a change occurred since last notification.
   * @param number_sites Number of sites currently available.
   */
  void notify (int number_sites);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Accessor to site being watched.
   * @return BindingSite being watched.
   */
  const BindingSite& site (void) const;

  /**
   * @brief Check whether site is affected by a change on a specific segment.
   * @param a First position of segment that changed.
   * @param b Last position of segment that changed.
   */
  bool is_affected (int a, int b) const;

private:
  // ============
  //  Attributes
  // ============
  //
  /** @brief Site to modify when changes occur. */
  BindingSite& _site;

  /** @brief Last value sent to site. */
  int _last_value_notified;

  // =================
  //  Private Methods
  // =================
  //
};

// ======================
//  Inline declarations
// ======================
//
#include "bindingsite.h"

inline const BindingSite& SiteAvailability::site (void) const
{
  return _site;
}

inline bool SiteAvailability::is_affected (int a, int b) const
{
  return !(((_site.relative_first() < a) && (_site.relative_last() < a)) 
	   || ((_site.relative_first() > b) && (_site.relative_last() > b)));
}


#endif // SITE_AVAILABILITY_H
