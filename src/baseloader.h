

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

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "boundchemical.h"
#include "decodingtable.h"

/**
 * @brief Chemical that loads a new base during sequence synthesis.
 *
 * The most important characteristics of this type of chemical is its
 * ability to decode a template a find the appropriate chemical to load
 * onto the elongating strand.
 * This class inherits class BoundChemical.
 */
class BaseLoader : public BoundChemical
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
   */
  virtual void add_unit (const BindingSite& binding_site, int position);

  /**
   * @brief Remove focused unit.
   */
  virtual void remove_focused_unit (void);

  /**
   * @brief Focus random unit based on their loading_rates.
   */
  void focus_random_unit_from_loading_rates (void);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Loading rate averaged over all elements.
   * @return Loading rate averaged over all elements.
   */
   double loading_rate (void);

  /**
   * @brief Base that the focused chemical tries to load.
   * @return Base that the focused chemical tries to load (0 if there is no
   *   chemical that matches the template it reads).
   */
  Chemical& focused_base_to_load (void) const;

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

  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @return True if class invariant is preserved
   */
  virtual bool check_invariant (void) const;


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
  std::vector< double > _loading_rates;

  /** @brief Index of template that the focused unit is reading. */
  int _focused_template_index;

  /** @brief Loading rate. */
  double _total_loading_rate;
  
  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Updates loading rates associated with each template and total loading rate.
   */
  void update_loading_rates (void);

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
  // update loading rates first
  update_loading_rates();
  return _total_loading_rate;
}

inline Chemical& BaseLoader::focused_base_to_load (void) const
{
  return _decoding_table.decode (_focused_template_index);
}


#endif // BASELOADER_H
