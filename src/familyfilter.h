
/**
 * @file familyfilter.h
 * @brief Header for the FamilyFilter class.
 * @authors Marc Dinh, Stephan Fischer
 */

// Multiple include protection
//
#ifndef FAMILY_FILTER_H
#define FAMILY_FILTER_H

// ==================
//  General Includes
// ==================
//


// ==================
//  Project Includes
// ==================
//
#include "boundunitfilter.h"
#include "vectorlist.h"

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"

/**
 * @brief Class filtering bound units belonging to a specific BindingSiteFamily.
 *
 * FamilyFilter only stores bound units belonging to a specific 
 * BindingSiteFamily. It inherits BoundUnitFilter.
 */
class FamilyFilter : public BoundUnitFilter
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor. 
   * @param family Filter will only keep units that bound to this 
   *  specific BindingSiteFamily
   */
  FamilyFilter (const BindingSiteFamily& family);
  
  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // FamilyFilter (const FamilyFilter& other);
  // /* @brief Assignment operator. */
  // FamilyFilter& operator= (const FamilyFilter& other);
  // /* @brief Destructor. */
  // ~FamilyFilter (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // redefined from BoundUnitFilter
  void add (BoundUnit& unit);
  void remove (BoundUnit& unit);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  // redefined from BoundUnitFilter
  BoundUnit& random_unit (void) const;

  /**
   * @brief Number of units filtered.
   * @return Number of units retained by the filter.
   */
  int number (void) const ;

  /**
   * @brief Returns the total unbinding rate for units stored in filter.
   * @return Total unbinding rate.
   */
  double total_unbinding_rate (void) const;

private:
  // =================
  //  Private Methods
  // =================
  //

  // ============
  //  Attributes
  // ============
  //
  /** @brief Family the units should have bounded to. */
  const BindingSiteFamily* _family;

  /** @brief Units bounded to family provided at construction. */
  VectorList <BoundUnit*> _units;
};

// ======================
//  Inline declarations
// ======================
//
#include "macros.h"

inline BoundUnit& FamilyFilter::random_unit (void) const
{
  /** @pre Filter must not be empty. */
  REQUIRE (number() > 0);
  return *(_units.random_element());
}

inline int FamilyFilter::number (void) const
{
  return _units.size();
}

#endif // FAMILY_FILTER_H
