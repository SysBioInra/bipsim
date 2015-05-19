

/**
 * @file bindingsitehandler.h
 * @brief Header for the BindingSiteHandler class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef BINDINGSITEHANDLER_H
#define BINDINGSITEHANDLER_H

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

typedef std::list< BindingSite* > BindingSiteList;
typedef std::map< int, BindingSiteList > ReferenceMap;

/**
 * @brief The BindingSiteHandler class manages binding sites.
 *
 * It creates, destroys and classifies binding sites in different families. It
 * also enables other classes to access to these binding sites.
 */
class BindingSiteHandler
{
public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor
   */
  BindingSiteHandler ( void );
   
  /**
   * @brief Copy constructor
   */
  BindingSiteHandler ( BindingSiteHandler& other_binding_site_handler );

  /**
   * @brief Destructor
   */
  virtual ~BindingSiteHandler ( void );

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Create a new binding site.
   * @param family_name
   *  Name of the family the binding site belongs to (e.g. Ribosome Binding
   *  Site).
   * @param location
   *  Chemical element that bears the binding site.
   * @param position
   *  Position of the binding site on the bearing element.
   * @param length
   *  Length of the binding site.
   * @param affinity
   *  Affinity of the binding site.
   */
  void create_binding_site ( std::string family_name, Bindable& location, int position,
			     int length, double affinity );

  /**
   * @brief Bind element on a random available site belonging to a specific family.
   * @param family_id Integer identifier of the binding site family.
   * @param unit_to_bind Element to bind.
   */
  void add_unit_on_random_available_site ( int family_id, BoundChemical& unit_to_bind );

  /**
   * @brief for fun
   */
  void print ( void );

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Returns id corresponding to family name.
   * @return Integer identfier of the binding site family
   *  (BindingSiteHandler::NOT_FOUND if unknown family).
   * @param  family_name Name of the binding site family.   
   */
  int retrieve_id ( std::string family_name );

  /**
   * @brief Returns family name corresponding to id.
   * @return Name of the binding site family.
   *  (empty if unknown family).
   * @param family_id Integer identfier of the binding site family
   */
  std::string retrieve_name ( int family_id );
  

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
  BindingSiteHandler& operator= ( BindingSiteHandler& other_binding_site_handler );


  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @return True if class invariant is preserved
   */
  virtual bool check_invariant ( void );

  // ==================
  //  Public Constants
  // ==================
  //
  static const int NOT_FOUND = IdentifiedList::NOT_FOUND;

private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief The list that contains the family identifiers. */
  IdentifiedList _family_ids;
  /** @brief The map that contains the references to binding sites. */
  ReferenceMap _references;

  // =================
  //  Private Methods
  // =================
  //
  void copy_reference_map ( BindingSiteHandler& other_binding_site_handler );
  void create_binding_site ( int family_id, BindingSite& binding_site );
  void clear_binding_sites ( void );
  
};

// ======================
//  Inline declarations
// ======================
//
inline int BindingSiteHandler::retrieve_id ( std::string family_name )
{
  return _family_ids.id ( family_name );
}

inline std::string BindingSiteHandler::retrieve_name ( int family_id )
{
  return _family_ids.name ( family_id );
}

#endif // BINDINGSITEHANDLER_H
