

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
 * @brief Class storing sites belonging to same family.
 *
 * SiteFamily simply acts as a list but inherits the SimulatorInput
 * type.
 */
class SiteFamily : public SimulatorInput
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor.
   */
  SiteFamily (void) {}

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor.
  //  */
  // SiteFamily (const SiteFamily& other_site_family);

  /**
   * @brief Destructor.
   */
  ~SiteFamily (void) {}

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Add a site to the family.
   * @param site Pointer to the site to add.
   */
  void add (const Site* site)
  {
    _sites.push_back (site);
  }

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
  // SiteFamily& operator= (const SiteFamily& other_site_family);

protected:
  // ======================
  //  Protected Attributes
  // ======================
  //

  // ===================
  //  Protected Methods
  // ===================
  //


private:

  // ============
  //  Attributes
  // ============
  //
  std::list <const Site*> _sites;

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

#endif // SITE_FAMILY_H
