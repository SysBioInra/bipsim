

/**
 * @file loader.h
 * @brief Header for the Loader class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef LOADER_H
#define LOADER_H

// ==================
//  General Includes
// ==================
//
#include <map> // std::map
#include <vector> // std::vector
#include <set> // std::set

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "boundchemical.h"
#include "decodingtable.h"
#include "ratevalidity.h"
#include "updatedtotalratevector.h"
#include "boundunitlist.h"

/**
 * @brief Chemical that loads a new base during sequence synthesis.
 *
 * The most important characteristics of this type of chemical is its
 * ability to decode a template a find the appropriate chemical to load
 * onto the elongating strand.
 * This class inherits class BoundChemical.
 */
class Loader : public BoundChemical
{
public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor
   * @param decoding_table
   *  The table that contains the template-chemical association that the
   *  Loader uses to import new elements onto the matrix strand.
   */
  Loader (const DecodingTable& decoding_table);

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor */
  // Loader (Loader& other_loader);
  // /* @brief Assignment operator */
  // Loader& operator= (Loader& other_loader);
  // /* @brief Destructor */
  // ~Loader (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // Redefined from BoundChemical
  void remove_focused_unit (void);
  void focus_random_unit (void);
  void focus_random_unit (const BindingSiteFamily& family);

  /**
   * @brief Focus random unit based on their loading_rates.
   */
  void focus_random_unit_from_loading_rates (void);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Loading rate summed over all elements.
   * @return Loading rate summed over all elements.
   */
   double loading_rate (void);

  /**
   * @brief Chemical that the focused loader tries to load.
   * @return Chemical that the focused loader tries to load.
   */
  Chemical& focused_chemical_to_load (void) const;

  /**
   * @brief Occupied state of the focused loader.
   * @return Occupied state of the focused loader.
   */
  BoundChemical& focused_occupied_state (void) const;

  /**
   * @brief Accessor to list of chemicals loaded by the loader.
   * @return List of chemicals loaded by the loader.
   */
  const std::set<Chemical*> chemicals_loaded (void) const;

  /**
   * @brief Accessor to list of occupied states yielded by the base loader.
   * @return List of occupied states yielded by the base loader.
   */
  const std::set<BoundChemical*> occupied_states (void) const;

private:
  // ============
  //  Attributes
  // ============
  //
  /** @brief The table of template-chemical associations. */
  const DecodingTable& _decoding_table;

  /**
   * @brief Pointers to chemicals indexed by template they are reading.
   */
  std::vector <BoundUnitList> _unit_map;

  /** @brief Loading rates associated with each template. */
  UpdatedTotalRateVector _loading_rates;

  /** @brief */
  RateValidity _rate_validity;

  /** @brief Index of template that the focused unit is reading. */
  int _focused_template_index;
  
  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Update _focused_template_index according to current _focused_unit.
   */
  void update_focused_template (void);

  /**
   * @brief Update loading rates.
   */
  void update_rates (void);

  /**
   * @brief Add a new unit in place of an existing new chemical.
   *
   * This function MUST be called every time a unit is added.
   * @param binding_site The binding site to which it bound.
   * @param position Current position.
   * @param reading_frame Reading frame position.
   */
  virtual void add_unit (const BindingSite& binding_site, int position, int reading_frame);
  

  // ===================
  //  Private Constants
  // ===================
  //
  /** @brief Template index if template read by focused unit is unknown. */
  static const int UNKNOWN_TEMPLATE = DecodingTable::UNKNOWN_TEMPLATE;
};

// ======================
//  Inline declarations
// ======================
//
inline double Loader::loading_rate (void)
{ 
  update_rates();
  return _loading_rates.total_rate();
}

inline Chemical& Loader::focused_chemical_to_load (void) const
{
  /** @pre Template must be recognized by the loader. */
  REQUIRE (_focused_template_index != UNKNOWN_TEMPLATE); 
  return _decoding_table.chemical_to_load (_focused_template_index);
}

inline BoundChemical& Loader::focused_occupied_state (void) const
{
  /** @pre Template must be recognized by the loader. */
  REQUIRE (_focused_template_index != UNKNOWN_TEMPLATE); 
  return _decoding_table.occupied_polymerase (_focused_template_index);
}

inline const std::set<Chemical*> Loader::chemicals_loaded (void) const
{
  return _decoding_table.chemicals_loaded();
}

inline const std::set<BoundChemical*> Loader::occupied_states (void) const
{
  return _decoding_table.occupied_polymerases();
}

#endif // LOADER_H
