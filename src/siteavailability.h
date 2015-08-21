

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
 * @brief Class computing number of available sites on a sequence and sending notifications to observers on changes.
 *
 * SiteAvailability computes availability of one site in a pool of identical sequences. Each time
 * its notify() member is called, it checks how many instances of its site are available along the pool and,
 * if this number has changed since last update, it sends an update() to its observer with the new number of
 * available sites.
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
   * @param first_position Position of the first base of the site to monitor.
   * @param length Length of the site to monitor.
   * @param observer_to_notify Observer to notify when the number of available sites changes.
   */
  SiteAvailability (int first_position, int length, SiteObserver& observer_to_notify);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor.
  //  */
  // SiteAvailability ( const SiteAvailability& other_site_availability );

  /**
   * @brief Destructor.
   */
  virtual ~SiteAvailability (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Check current number available sites and notify observer if a change occurred since last check.
   * @param current_number_sequences Current number of identical sequences in the pool.
   * @param current_occupancy Number of bases occupied for each base of the sequence along the pool.
   */
  void notify (int current_number_sequences, const std::vector<int>& current_occupancy);

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
  // SiteAvailability& operator= ( const SiteAvailability& other_site_availability );

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

  // ======================
  //  Forbidden Operations
  // ======================
  //

};

// ======================
//  Inline declarations
// ======================
//

#endif // SITE_AVAILABILITY_H
