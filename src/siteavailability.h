

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
   * @param first Position of the first base of the site to monitor.
   * @param last Position of the last base of the site to monitor.
   * @param observer_to_notify Observer to notify when the number of available
   *  sites changes.
   */
  SiteAvailability (int first, int last, SiteObserver& observer_to_notify);

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
   * @brief Check current number available sites and notify observer if a 
   *  change occurred since last check.
   * @param a First position affected by change.
   * @param b Last position affected by change.
   * @param current_number_sequences Current number of identical sequences 
   *  in the pool.
   * @param current_occupancy Number of bases occupied for each base of the 
   *  sequence along the pool.
   */
  void notify (int a, int b, int current_number_sequences,
	       const std::vector<int>& current_occupancy);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //

private:
  // ============
  //  Attributes
  // ============
  //
  /** @brief Postion of the first base of the site to monitor. */
  int _first;

  /** @brief Postion of the last base of the site to monitor. */
  int _last;

  /** @brief Observer to modify when changes occur. */
  SiteObserver& _observer;

  /** @brief Last value sent to observer. */
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


#endif // SITE_AVAILABILITY_H
