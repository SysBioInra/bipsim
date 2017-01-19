

/**
 * @file templatefilter.h
 * @brief Header for the TemplateFilter class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef TEMPLATE_FILTER_H
#define TEMPLATE_FILTER_H

// ==================
//  General Includes
// ==================
//


// ==================
//  Project Includes
// ==================
//
#include "boundunitfilter.h"
#include "ratevalidity.h"
#include "updatedtotalratevector.h"
// #include "flyratevector.h"
#include "vectorlist.h"


// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"

/**
 * @brief Class filtering bound units according to template they are reading.
 *
 * TemplateFilter is a filter class for bound units. It is used by Loading
 * reactions to select a unit according to the template it is reading and
 * its associated loading rate. TemplateFilter inherits BoundUnitFilter.
 */
class TemplateFilter : public BoundUnitFilter
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor. 
   * @param table LoadingTable containing templates according to which units
   *  should be segregated as well as loading rates.
   */
  TemplateFilter (const LoadingTable& table);

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // TemplateFilter (const TemplateFilter& other);
  // /* @brief Assignment operator. */
  // TemplateFilter& operator= (const TemplateFilter& other);
  // /* @brief Destructor. */
  // ~TemplateFilter (void);
  
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
  /**
   * @brief Pick random unit weighted by loading rate associated with its
   * template.
   * @return Random BoundUnit stored by the filter.
   */
  BoundUnit& random_unit (void) const;
  
  /**
   * @brief Loading rate summed over all units.
   * @return Loading rate summed over all units.
   */
  double loading_rate (void) const;

private:
  // =================
  //  Private Methods
  // =================
  //
  /** @brief Update rates that need to be updated. */
  void update_rates (void) const;

  // ============
  //  Attributes
  // ============
  //
  /** @brief The table of template-chemical associations. */
  const LoadingTable& _table;

  /** @brief Unit lists indexed by template they are reading. */
  std::vector <VectorList <BoundUnit*> > _unit_map;

  /** @brief Loading rates associated with each template. */
  mutable UpdatedTotalRateVector _loading_rates;
  // mutable FlyRateVector _loading_rates;

  /** @brief Object handling rate indices to update. */
  mutable RateValidity _rate_validity;
};

// ======================
//  Inline declarations
// ======================
//
inline BoundUnit& TemplateFilter::random_unit (void) const
{
  update_rates(); 
  _loading_rates.update_cumulates();
  return *(_unit_map [_loading_rates.random_index()].random_element());
}

inline double TemplateFilter::loading_rate (void) const
{ 
  update_rates();
  return _loading_rates.total_rate();
}

#endif // TEMPLATE_FILTER_H
