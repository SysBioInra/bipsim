

/**
 * @file sitefamily.h
 * @brief Header for the SiteFamily class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef SITE_FAMILY_H
#define SITE_FAMILY_H

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
#include "simulatorinput.h"

/**
 * @brief Abstract class representing site families.
 *
 * SiteFamily inherits SimulatorInput.
 */
class SiteFamily : public SimulatorInput
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  // Not needed for this class (use of compiler-generated versions)
  // /* @brief Default constructor. */
  // SiteFamily (void);
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // SiteFamily (const SiteFamily& other_site_family);
  // /* @brief Assignment operator. */
  // SiteFamily& operator= (const SiteFamily& other_site_family);
  /** @brief Destructor. */
  virtual ~SiteFamily (void) {}

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //

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

#endif // SITE_FAMILY_H
