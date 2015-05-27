

/**
 * @file terminationsitehandler.h
 * @brief Header for the TerminationSiteHandler class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef TERMINATIONSITEHANDLER_H
#define TERMINATIONSITEHANDLER_H

// ==================
//  General Includes
// ==================
//
#include <list>  // std::list
#include <map>  // std::map
#include <string>  //std::string

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "identifiedlist.h"
#include "sitehandler.h"

/**
 * @brief The BindingSiteHandler class manages binding sites.
 *
 * It creates, destroys and classifies binding sites in different families. It
 * also enables other classes to access to these binding sites.
 */
class TerminationSiteHandler : public SiteHandler
{
public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Destructor
   */
  virtual ~TerminationSiteHandler ( void );

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Create a new site.
   * @param family_name
   *  Name of the family the site belongs to (e.g. Ribosome Binding
   *  Site).
   * @param location
   *  Chemical element that bears the binding site.
   * @param position
   *  Position of the binding site on the bearing element.
   * @param length
   *  Length of the binding site.
   */
  virtual void create_site ( std::string family_name, Bindable& location,
			     int position, int length );


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
   * @return True if class invariant is preserved
   */
  virtual bool check_invariant ( void ) const;

  // ==================
  //  Public Constants
  // ==================
  //

protected:

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

#endif // TERMINATIONSITEHANDLER_H
