

/**
 * @file sitehandler.h
 * @brief Header for the SiteHandler class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef SITEHANDLER_H
#define SITEHANDLER_H

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

typedef std::list< const Site* > SiteList;
typedef std::map< int, SiteList > SiteFamilyMap;

/**
 * @brief The BindingSiteHandler class manages binding sites.
 *
 * It creates, destroys and classifies binding sites in different families. It
 * also enables other classes to access to these binding sites.
 */
class SiteHandler
{
public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor
   */
  SiteHandler ( void ) {}
   
  /**
   * @brief Copy constructor
   */
  SiteHandler ( const SiteHandler& other_site_handler );

  /**
   * @brief Destructor
   */
  virtual ~SiteHandler ( void );

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

  /**
   * @brief for fun
   */
  void print ( void ) const;

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Returns id corresponding to family name.
   * @return Integer identfier of the site family
   *  (BindingSiteHandler::NOT_FOUND if unknown family).
   * @param  family_name Name of the site family.   
   */
  int retrieve_id ( std::string family_name ) const;

  /**
   * @brief Returns family name corresponding to id.
   * @return Name of the site family.
   *  (empty if unknown family).
   * @param family_id Integer identfier of the site family
   */
  std::string retrieve_name ( int family_id ) const;
  

  // ==========================
  //  Public Methods - Setters
  // ==========================
  //


  // =======================================
  //  Public Methods - Operator overloading
  // =======================================
  //
  /**
   * @brief Assignment operator
   */
  SiteHandler& operator= ( SiteHandler& other_site_handler );


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
  static const int NOT_FOUND = IdentifiedList::NOT_FOUND;

protected:

  // ============
  //  Attributes
  // ============
  //
  /** @brief The list that contains the family identifiers. */
  IdentifiedList _family_ids;
  /** @brief The map that contains the references to binding sites. */
  SiteFamilyMap _families;

  // =================
  //  Private Methods
  // =================
  //
  void copy_family_map ( const SiteHandler& other_site_handler );
  void create_site ( int family_id, const Site& site );
  void clear_sites ( void );
  
};

// ======================
//  Inline declarations
// ======================
//
inline int SiteHandler::retrieve_id ( std::string family_name ) const
{
  return _family_ids.id ( family_name );
}

inline std::string SiteHandler::retrieve_name ( int family_id ) const
{
  return _family_ids.name ( family_id );
}

#endif // SITEHANDLER_H
