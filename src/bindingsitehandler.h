

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
#include <string>  //std::string

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "sitehandler.h"
#include "bindingsitefamily.h"


/**
 * @brief Class managing binding sites.
 *
 * It creates, destroys and classifies binding sites in different families. It
 * also enables other classes to access to these binding sites.
 */
class BindingSiteHandler : public SiteHandler
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
   * @param k_on
   *  on-rate of the binding site.
   * @param k_off
   *  off-rate of the binding site.
   * @param reading_frame Position of the reading frame (if applicable).
   */
  void create_site ( std::string family_name, ChemicalSequence& location, int position,
		     int length, double k_on, double k_off,
		     int reading_frame = NO_READING_FRAME );



  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Accessor to binding site families.
   * @param identifier Integer identifier to the family.
   * @return Reference to the binding site family.
   */
  BindingSiteFamily& binding_site_family (int identifier);

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
   * @brief Check class invariant.
   * @return True if class invariant is preserved
   */
  virtual bool check_invariant ( void ) const;

  // ==================
  //  Public Constants
  // ==================
  //
  /** @brief Constant value used to specify that there is no reading frame associated with a binding site. */
  static const int NO_READING_FRAME = BindingSite::NO_READING_FRAME;

private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Map classifying binding sites in families. */
  std::map<int,BindingSiteFamily> _families;

  /** @brief List of all binding sites created (for memory management). */
  std::list<BindingSite*> _binding_site_list;

  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Print class content.
   * @param output Stream where output should be written.
   */
  virtual void print (std::ostream& output) const;

  /**
   * @brief Erase all sites.
   */
  void clear_sites (void);
  
  // ======================
  //  Forbidden Operations
  // ======================
  //
  /**
   * @brief Copy constructor forbidden (declared private)
   */
  BindingSiteHandler ( BindingSiteHandler& other_binding_site_handler );

  /**
   * @brief Assignment operator forbidden (declared private)
   */
  BindingSiteHandler& operator= ( BindingSiteHandler& other_binding_site_handler );
  
};

// ======================
//  Inline declarations
// ======================
//
inline BindingSiteFamily& BindingSiteHandler::binding_site_family (int identifier)
{
  /** @pre Family identfier must exist. */
  REQUIRE (_families.find(identifier) != _families.end()); 

  return _families [identifier];
}

#endif // BINDINGSITEHANDLER_H
