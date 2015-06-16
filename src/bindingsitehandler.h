

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
#include "identifiedlist.h"
#include "sitehandler.h"

/**
 * @brief The BindingSiteHandler class manages binding sites.
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
  // Not needed here !
  // /*
  //  * @brief Default constructor
  //  */
  // BindingSiteHandler ( void );
   
  // /*
  //  * @brief Copy constructor
  //  */
  // BindingSiteHandler ( BindingSiteHandler& other_binding_site_handler );

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
   */
  virtual void create_site ( std::string family_name, Bindable& location, int position,
			     int length, double k_on, double k_off );

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Return a random available site belonging to a specific family.
   * @param family_id Integer identifier of the binding site family.
   * @return Random available site belonging to a specific family.
   */
  const BindingSite& get_random_available_site ( int family_id ) const;

  /**
   * @brief Return total contribution to binding rate for a specific family.
   * @param family_id Integer identifier of the binding site family.
   * @return Contribution to binding rate for a specific family.
   */
  double get_total_binding_rate_contribution ( int family_id ) const;

  // ==========================
  //  Public Methods - Setters
  // ==========================
  //


  // =======================================
  //  Public Methods - Operator overloading
  // =======================================
  //
  // /*
  //  * @brief Assignment operator
  //  */
  // BindingSiteHandler& operator= ( BindingSiteHandler& other_binding_site_handler );


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
  // static const int NO_SPECIFIC_SITE_ID = -1;

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

#endif // BINDINGSITEHANDLER_H
