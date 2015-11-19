

/**
 * @file baseloader.h
 * @brief Header for the BaseLoader class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef BASELOADER_H
#define BASELOADER_H

// ==================
//  General Includes
// ==================
//
#include <map> // std::map
#include <list> // std::list
#include <vector> // std::vector
#include <set> // std::set

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "boundchemical.h"
#include "decodingtable.h"
#include "subratevector.h"
#include "cobserverclient.h"

/**
 * @brief Chemical that loads a new base during sequence synthesis.
 *
 * The most important characteristics of this type of chemical is its
 * ability to decode a template a find the appropriate chemical to load
 * onto the elongating strand.
 * This class inherits class BoundChemical.
 */
class BaseLoader : public BoundChemical, public CObserverClient
{
public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor
   * @param decoding_table
   *  The table that contains the template-base association that the
   *  BaseLoader uses to import new elements onto the elongating strand.
   */
  BaseLoader (const DecodingTable& decoding_table);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // BaseLoader (BaseLoader& other_base_loader);

  /**
   * @brief Destructor
   */
  virtual ~BaseLoader (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  // 
  /**
   * @brief Add a new unit in place of an existing new chemical.
   *
   * This function MUST be called every time a unit is added.
   * @param binding_site The binding site to which it bound.
   * @param position Current position.
   * @param reading_frame Reading frame position.
   */
  virtual void add_unit (const BindingSite& binding_site, int position, int reading_frame);

  /**
   * @brief Remove focused unit.
   */
  virtual void remove_focused_unit (void);

  /**
   * @brief Focus a unit randomly.
   */
  virtual void focus_random_unit (void);

  /**
   * @brief Focus a unit that bound to a specific binding site family.
   * @param binding_site_family The binding site family to inspect.
   */
  virtual void focus_random_unit (int binding_site_family);

  /**
   * @brief Focus random unit based on their loading_rates.
   */
  void focus_random_unit_from_loading_rates (void);

  /**
   * @brief Update loading rate corresponding to a specific index.
   * @param index Integer associated with a specific base-codon pairing.
   */
  void update (int index);

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
   * @brief Base that the focused chemical tries to load.
   * @return Base that the focused chemical tries to load.
   */
  Chemical& focused_base_to_load (void) const;

  /**
   * @brief Occupied state of the focused loader.
   * @return Occupied state of the focused loader.
   */
  BoundChemical& focused_occupied_state (void) const;

  /**
   * @brief Accessor to list of bases loaded by the base loader.
   * @return List of bases loaded by the base loader.
   */
  const std::set<Chemical*> bases_loaded (void) const;

  /**
   * @brief Accessor to list of occupied states yielded by the base loader.
   * @return List of occupied states yielded by the base loader.
   */
  const std::set<BoundChemical*> occupied_states (void) const;

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
  // BaseLoader& operator= (BaseLoader& other_base_loader);

private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief The table of template-base associations. */
  const DecodingTable& _decoding_table;

  /**
   * @brief Iterators to chemicals indexed by template they are reading.
   */
  std::vector< std::list<BoundUnitList::iterator> > _unit_map;

  /** @brief Loading rates associated with each template. */
  SubRateVector _loading_rates;

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
inline double BaseLoader::loading_rate (void)
{ 
  return _loading_rates.total_rate();
}

inline Chemical& BaseLoader::focused_base_to_load (void) const
{
  REQUIRE( _focused_template_index != UNKNOWN_TEMPLATE ); /** @pre Template must be recognized by the base loader. */
  return _decoding_table.decode (_focused_template_index);
}

inline BoundChemical& BaseLoader::focused_occupied_state (void) const
{
  REQUIRE( _focused_template_index != UNKNOWN_TEMPLATE ); /** @pre Template must be recognized by the base loader. */
  return _decoding_table.occupied_polymerase (_focused_template_index);
}

inline const std::set<Chemical*> BaseLoader::bases_loaded (void) const
{
  return _decoding_table.bases();
}

inline const std::set<BoundChemical*> BaseLoader::occupied_states (void) const
{
  return _decoding_table.occupied_polymerases();
}

#endif // BASELOADER_H
