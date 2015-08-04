

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
 * @brief Class managing termination sites.
 *
 * It creates, destroys and classifies termination sites in different families. It
 * also enables other classes to access to these termination sites.
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
   *  Name of the family the site belongs to (e.g. stem-loop).
   * @param location
   *  Chemical element that bears the termination site.
   * @param position
   *  Position of the termination site on the bearing element.
   * @param length
   *  Length of the termination site.
   */
  virtual void create_site ( std::string family_name, ChemicalSequence& location,
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
  /** @brief A list of references to termination sites. */
  typedef std::list< const Site* > SiteList;
  
  /** @brief A map associating a family integer identifer with a SiteList. */
  typedef std::map< int, SiteList > SiteFamilyMap;

  /** @brief The map that contains the references to sites. */
  SiteFamilyMap _families;

  // =================
  //  Private Methods
  // =================
  //
  /**
   * @return Print class content.
   * @param output Stream where output should be written.
   */
  virtual void print (std::ostream& output) const ;

  /**
   * @brief Erase all sites.
   */
  void clear_sites ( void );

};

// ======================
//  Inline declarations
// ======================
//

#endif // TERMINATIONSITEHANDLER_H
