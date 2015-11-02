

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

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // SiteLocation (SiteLocation& other_site_location);

  /**
   * @brief Destructor.
   */
  virtual ~SiteLocation (void) {}

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
  int first (void) const;


  /**
   * @brief Returns last position of site.
   * @return Last position of site.
   */
  int last (void) const;

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
  //  * @brief Assignment operator
  //  */
  // SiteLocation& operator= (SiteLocation& other_site_location);



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
inline int SiteLocation::first (void) const { return _first; }

inline int SiteLocation::last (void) const { return _last; }


#endif // SITELOCATION_H
