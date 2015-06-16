

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
#include <string> // std::string

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "macros.h"
#include "chemical.h"
#include "bindable.h"

/**
 * @brief A simple list of SiteLocation.
 */
typedef std::list<SiteLocation> SiteLocationList;

/**
 * @brief A map that stores the location of all BoundChemical.
 *
 * Keys of the map are references to all BoundChemical currently located on
 * the chemical sequence, data is a SiteLocationList that stores the location
 * of all chemicals designated by the key.
 */
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
   * @param sequence Sequence of the chemical
   */
  ChemicalSequence (const std::string& sequence);

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
   */
  void bind_unit ( const BoundChemical& chemical_to_bind );


  /**
   * @brief Unbinds one of the chemical elements that is bound to the bindable
   *  element.
   * @param  chemical_to_unbind
   *  The element to unbind.
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


  /**
   * @return Print class content.
   * @param output Stream where output should be written.
   */
  virtual void print (std::ostream& output) const;


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

  /**
   * @brief Returns the sequence between two specific positions.
   * @return String sequence between two positions.
   * @param first_position
   *  Position of the first base of the sequence to return (included).
   * @param last_position
   *  Position of the last base of the sequence to return (included).
   */
  const std::string get_sequence (int first_position, int last_position) const;


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

  /** @brief Sequence of the chemical. */
  std::string _sequence;

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
inline const std::string ChemicalSequence::get_sequence (int first_position, int length) const
{
  /* @pre First position must be positive. */
  REQUIRE( first_position > 0 );
  /* @pre Length must be positive. */
  REQUIRE( length > 0 );
  /* @pre Requested sequence must not exceed sequence length. */
  REQUIRE( first_position + length - 1 <= this->length() );

  return _sequence.substr (first_position-1, length);
}


#endif // CHEMICALSEQUENCE_H
