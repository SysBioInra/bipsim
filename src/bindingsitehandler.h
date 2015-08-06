

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
  virtual void create_site ( std::string family_name, ChemicalSequence& location, int position,
			     int length, double k_on, double k_off,
			     int reading_frame = NO_READING_FRAME );


  /**
   * @brief Update binding rate contributions to reflect current site occupancy.
   *
   * This function must be called before updating binding rates, else binding rates
   * will not reflect current site occupancy...
   */
  void update_all_binding_rate_contributions (void);

  /**
   * @brief Update binding rate contributions to reflect current site occupancy given that only one sequence was changed.
   * @param modified_sequence Chemical sequence that was modified since last update.
   *
   * This function must be called before updating binding rates, else binding rates
   * will not reflect current site occupancy...
   */
  void update_binding_rate_contributions (const ChemicalSequence& modified_sequence);


  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Return a random available site belonging to a specific family.
   * @param family_id Integer identifier of the binding site family.
   * @return Random available site belonging to a specific family.
   */
  const BindingSite& get_random_available_site ( int family_id );

  /**
   * @brief Return total contribution to binding rate for a specific family.
   * @param family_id Integer identifier of the binding site family.
   * @return Contribution to binding rate for a specific family.
   */
  double get_total_binding_rate_contribution ( int family_id );



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

  /** @brief Map classifying binding sites according to the sequence they belong to. */
  std::map< const ChemicalSequence*, std::list<BindingSite*> > _location_map;

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

inline double BindingSiteHandler::get_total_binding_rate_contribution ( int family_id )
{
  REQUIRE (exists (family_id)); /** @pre Family identifier must exist. */
  return _families [family_id].total_rate_contribution();
}

inline const BindingSite& BindingSiteHandler::get_random_available_site ( int family_id )
{
  REQUIRE (exists (family_id)); /** @pre Family identifier must exist. */
  return _families [family_id].get_random_available_site();
}

#endif // BINDINGSITEHANDLER_H
