

/**
 * @file  sitelocation.h
 * @brief Header for the SiteLocation class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef SITELOCATION_H
#define SITELOCATION_H

// ==================
//  General Includes
// ==================
//


// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "macros.h"

/**
 * @brief Class storing site location variables.
 *
 * Simply stores and gives access to (position,length). Can be used for
 * simplicity and clarity by other classes.
 */
class SiteLocation
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor based on position and length.
   * @param first First position of site.
   * @param last Last position of site.
   */
  SiteLocation (int first, int last)
    : _first (first)
    , _last (last) {}

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // SiteLocation (SiteLocation& other_site_location);
  // /* @brief Assignment operator. */
  // SiteLocation& operator= (SiteLocation& other_site_location);
  // /* @brief Destructor. */
  // ~SiteLocation (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //


  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Returns first position of site.
   * @return First position of site.
   */
  int first (void) const { return _first; }

  /**
   * @brief Returns last position of site.
   * @return Last position of site.
   */
  int last (void) const { return _last; }

private:
  // ============
  //  Attributes
  // ============
  //
  /** @brief First position of site. */
  int _first;

  /** @brief Last position of site. */
  int _last;

  // =================
  //  Private Methods
  // =================
  //

};

// ======================
//  Inline declarations
// ======================
//


#endif // SITELOCATION_H
