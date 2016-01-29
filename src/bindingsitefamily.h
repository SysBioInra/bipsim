

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
#include "reactant.h"
#include "sitefamily.h"
#include "updatedtotalratevector.h"

/**
 * @brief Class handling a family of binding sites.
 *
 * BindingSiteFamily stores references to all binding sites of
 * a same family (which try to load the same chemical). It computes
 * the contribution to binding rates of each site depending on their
 * occupation and affinity.
 */
class BindingSiteFamily : public Reactant, public SiteFamily
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  //
  /** @brief Default constructor */
  BindingSiteFamily (void);

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor */
  // BindingSiteFamily (const BindingSiteFamily& other_family);
  // /* @brief Assignment operator */
  // BindingSiteFamily& operator= (const BindingSiteFamily& other_family);
  // /* @brief Destructor */
  // ~BindingSiteFamily (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Add a binding site to the family.
   * @param binding_site Pointer to the site to add.
   */
  void add (BindingSite* binding_site);

  /**
   * @brief Update contributions given that a binding site availability has
   *  changed.
   * @param site_index Index of the binding site that has changed.
   * @param number_available_sites New number of available sites.
   */
  void update (int site_index, int number_available_sites);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Return total binding rate contribution as computed during the last
   *  update.
   * @return Total binding rate contribution reflecting global affinity for
   *  ligand and availability.
   */
  double total_binding_rate_contribution (void) const;

  /**
   * @brief Return a random available site based on contributions computed
   *  during the last update.
   * @return Available site drawn according to a biased wheel favoring high
   *  affinity and availability.
   */
  const BindingSite& get_random_available_site (void) const;

  /**
   * @brief Checks whether a site from this family is currently available.
   * @return True if there is at least one site available.
   *
   * Note that compared to the rate accessors, this accessor is based on the
   * current state of the cell, not that at the time of the last 
   * update_rate_contributions(). Some computations are involved in this
   * accessor, but it is less expensive than update_rate_contributions().
   */
  bool is_site_available (void) const;

private:
  // ============
  //  Attributes
  // ============
  /**
   * @brief Binding sites belonging to the family.
   */
  std::vector<const BindingSite*> _binding_sites;

  /**
   * @brief Contribution of each binding site to the binding rate.
   */
  mutable UpdatedTotalRateVector _rate_contributions;  

  // =================
  //  Private Methods
  // =================
  //
};

// ======================
//  Inline declarations
// ======================
//

inline double BindingSiteFamily::total_binding_rate_contribution (void) const
{
  return _rate_contributions.total_rate();
}

inline const BindingSite& BindingSiteFamily::get_random_available_site (void) const
{
  _rate_contributions.update_cumulates();
  return *(_binding_sites [_rate_contributions.random_index()]);
}

#endif // BINDING_SITE_FAMILY_H
