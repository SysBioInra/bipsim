

/**
 * @file chemicalsequence.h
 * @brief Header for the ChemicalSequence class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef CHEMICALSEQUENCE_H
#define CHEMICALSEQUENCE_H

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
#include "macros.h"
#include "chemical.h"
#include "bindable.h"

typedef std::list<SiteLocation> SiteLocationList;
typedef std::map< const BoundChemical*, SiteLocationList > ChemicalMap;

/**
 * @brief This class stores chemicals that can be described by a sequence.
 *
 * Usually polymers based on a specific alphabet. Typical examples include DNA,
 * RNA and proteins.
 */
class ChemicalSequence : public Chemical, public Bindable
{
public:


  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor
   */
  ChemicalSequence (void);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // ChemicalSequence (ChemicalSequence& other_chemical_sequence);

  /**
   * @brief Destructor
   */
  virtual ~ChemicalSequence (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief If the sequence is a template, elongate nascent product
   */
  void elongate_nascent (void);

  /**
   * @brief If the sequence is a template, terminate nascent product
   */
  void terminate_nascent (void);

  /**
   * @brief Binds a chemical element to a binding site of the bindable element.
   * @param chemical_to_bind
   *  The chemical element to bind.
   * @param position Position of binding site.
   * @param length Size occupied by bound element on sequence.
   * @sa BindingSite
   */
  void bind_unit ( const BoundChemical& chemical_to_bind );


  /**
   * @brief Unbinds one of the chemical elements that is bound to the bindable
   *  element.
   * @param  chemical_to_unbind
   *  The element to unbind.
   * @param position Position of binding site.
   * @param length Size occupied by bound element on sequence.
   * @sa BindingSite
   */
  void unbind_unit ( const BoundChemical& chemical_to_unbind );


  /**
   * @brief Changes the nature of a bound unit.
   * @param old_chemical
   *  The original nature of the bound unit.
   * @param new_chemical
   *  The new nature of the bound unit.
   */
  void replace_bound_unit ( const BoundChemical& old_chemical, const BoundChemical& new_chemical );


  /**
   * @brief Move a bound unit
   * @param chemical_to_move
   *  The processive element that is moving along the bindable element.
   * @param number_steps
   *  The number of steps by which it moves.
   */
  void move_bound_unit ( ProcessiveChemical& chemical_to_move, int number_steps );


  /**
   * @brief Adds a given quantity of chemical to the existing pool.
   * @param quantity Amount to add
   */
  void add ( int quantity );


  /**
   * @brief Remove a given quantity of chemical to the existing pool.
   * @param quantity Amount to remove.
   */
  void remove ( int quantity );

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Returns the number of available sites at a given position.
   * @return Number of available sites.
   * @param position Position of the site.
   * @param length Length of the site.
   * @sa BindingSite
   */
  int number_available_sites ( int position, int length ) const;


  // ==========================
  //  Public Methods - Setters
  // ==========================
  //
  /**
   * @brief Sets element length.
   * @param length Length of sequence.
   */
  void set_length ( int length );


  // =======================================
  //  Public Methods - Operator overloading
  // =======================================
  //
  // Not needed for this class (use of default overloading) !
  // /*
  //  * @brief Assignment operator
  //  */
  // ChemicalSequence& operator= (ChemicalSequence& other_chemical_sequence);

  
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
  /** @brief Tracks available positions along the sequence. */
  std::vector<int> _occupancy_map;

  /** @brief Map of bound chemicals. */
  ChemicalMap _chemical_map;

  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Remove a specific reference from the bound chemical map.
   * @param chemical The type of chemical to remove.
   * @param position Its position along the sequence.
   * @param length Its length.
   */
  void remove_reference_from_map ( const BoundChemical& chemical, int position, int length );

};

// ======================
//  Inline declarations
// ======================
//


#endif // CHEMICALSEQUENCE_H
