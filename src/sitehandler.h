

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
#include <string>  //std::string

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "identifiedlist.h"


/**
 * @brief Abstract class for site management.
 *
 * SiteHandler is an abstract class that handles the family
 * classification of sites and the associated identifiers.
 * Creation of sites is handled by inheriting classes.
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
  SiteHandler ( void );
   
  /**
   * @brief Destructor
   */
  virtual ~SiteHandler ( void ) = 0;

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Returns whether family name is known.
   * @return True if family is known.
   * @param  family_name Name of the site family.   
   */
  bool exists (std::string family_name) const;

  /**
   * @brief Returns id corresponding to family name.
   * @return Integer identfier of the site family
   *  (SiteHandler::NOT_FOUND if unknown family).
   * @param  family_name Name of the site family.   
   */
  int retrieve_id (std::string family_name) const;

  /**
   * @brief Returns family name corresponding to id.
   * @return Name of the site family.
   *  (empty if unknown family).
   * @param family_id Integer identfier of the site family
   */
  std::string retrieve_name (int family_id) const;
  

  // ==========================
  //  Public Methods - Setters
  // ==========================
  //


  // =======================================
  //  Public Methods - Operator overloading
  // =======================================
  //
  /**
   * @brief Standard output.
   * @return A reference to the stream containing the output.
   * @param output Stream where output should be written.
   * @param site_handler Reference to the handler whose information should be written.
   */
  friend std::ostream& operator<< (std::ostream& output, const SiteHandler& site_handler);


  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @brief Check class invariant.
   * @return True if class invariant is preserved
   */
  virtual bool check_invariant ( void ) const;

  // ==================
  //  Public Constants
  // ==================
  //
  /**
   * @brief Identifier associated with unknown site families.
   */
  static const int NOT_FOUND = IdentifiedList::NOT_FOUND;
  
 protected:

  // ===================
  //  Protected Methods
  // ===================
  //
  /**
   * @brief Returns id corresponding to family name or create it if nonexistent.
   * @return Integer identfier of the site family (created if not already existent).
   * @param  family_name Name of the site family.   
   */
  int get_or_create_family_identifier (std::string& family_name);
    
 private:
  
  // ============
  //  Attributes
  // ============
  //
  /** @brief The list that contains the family identifiers. */
  IdentifiedList _family_ids;

  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Print class content.
   * @param output Stream where output should be written.
   */
  virtual void print (std::ostream& output) const = 0;

  // ======================
  //  Forbidden Operations
  // ======================
  //
  /**
   * @brief Assignment operator forbidden (declared private)
   */
  SiteHandler& operator= ( SiteHandler& other_site_handler );  

  /**
   * @brief Copy constructor forbidden (declared private)
   */
  SiteHandler ( const SiteHandler& other_site_handler );
  
};

// ======================
//  Inline declarations
// ======================
//
inline bool SiteHandler::exists ( std::string family_name ) const
{
  return _family_ids.exists ( family_name );
}

inline int SiteHandler::retrieve_id ( std::string family_name ) const
{
  return _family_ids.id ( family_name );
}

inline std::string SiteHandler::retrieve_name ( int family_id ) const
{
  return _family_ids.name ( family_id );
}

#endif // SITEHANDLER_H
