

/**
 * @file bindingsitefamily.h
 * @brief Header for the BindingSiteFamily class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef BINDING_SITE_FAMILY_H
#define BINDING_SITE_FAMILY_H

// ==================
//  General Includes
// ==================
//
#include <vector> // std::vector

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "bindingsite.h"
#include "reactant.h"

/**
 * @brief Class handling a family of binding sites.
 *
 * BindingSiteFamily stores references to all binding sites of
 * a same family (which try to load the same chemical). It computes
 * the contribution to binding rates of each site depending on their
 * occupation and affinity.
 */
class BindingSiteFamily : public Reactant
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor
   */
  BindingSiteFamily (void);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // BindingSiteFamily ( const BindingSiteFamily& other_CLASSNAME );

  /**
   * @brief Destructor
   */
  ~BindingSiteFamily (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Add a binding site to the family.
   * @param binding_site Pointer to the site to add.
   */
  void add_binding_site (const BindingSite* binding_site);

  /**
   * @brief Update contributions given that a binding site availability has changed.
   * @param site_index Index of the binding site that has changed.
   * @param number_available_sites New number of available sites.
   */
  void update (int site_index, int number_available_sites);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Return total binding rate contribution as computed during the last update.
   * @return Total binding rate contribution reflecting global affinity for ligand and availability.
   */
  double total_binding_rate_contribution (void) const;

  /**
   * @brief Return a random available site based on contributions computed during the last update.
   * @return Available site drawn according to a biased wheel favoring high affinity and availability.
   */
  const BindingSite& get_random_available_site (void) const;

  /**
   * @brief Checks whether a site from this family is currently available.
   * @return True if there is at least one site available.
   *
   * Note that compared to the rate accessors, this accessor is based on the current state of the cell,
   * not that at the time of the last update_rate_contributions(). Some computations are involved in this
   * accessor, but it is less expensive than update_rate_contributions().
   */
  bool is_site_available (void) const;

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
  //  * @brief Assignment operator
  //  */
  // BindingSiteFamily& operator= ( const BindingSiteFamily& other_CLASSNAME );

  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @brief Check class invariant.
   * @return True if class invariant is preserved
   */
  bool check_invariant (void) const;


private:

  // ============
  //  Attributes
  // ============
  //
  /**
   * @brief Total contribution to the binding rate.
   */
  double _total_rate_contribution;

  /**
   * @brief Binding sites belonging to the family.
   */
  std::vector<const BindingSite*> _binding_sites;

  /**
   * @brief Contribution of each binding site to the binding rate.
   */
  std::vector<double> _rate_contributions;
  
  /**
   * @brief Observers monitoring availability of all the sites.
   */
  std::list<BindingSiteObserver*> _binding_site_observers;
  

  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Compute total rate contribution from the _rate_contributions vector
   */
  void compute_total_rate_contribution (void);

  // ======================
  //  Forbidden Operations
  // ======================
  //

};

// ======================
//  Inline declarations
// ======================
//

inline double BindingSiteFamily::total_binding_rate_contribution (void) const
{
  return _total_rate_contribution;
}


#endif // BINDING_SITE_FAMILY_H
