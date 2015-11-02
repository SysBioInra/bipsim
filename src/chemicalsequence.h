

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
#include "chemical.h"
#include "sequenceoccupation.h"

/**
 * @brief This class stores chemicals that can be described by a sequence.
 *
 * Usually polymers based on a specific alphabet. Typical examples include DNA,
 * RNA and proteins.
 */
class ChemicalSequence : public Chemical
{
public:


  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor
   * @param sequence Sequence of the chemical
   * @param starting_position Starting position (allows absolute positionning).
   */
  ChemicalSequence (const std::string& sequence, int starting_position = 1);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // ChemicalSequence (ChemicalSequence& other_chemical_sequence);

  /**
   * @brief Destructor
   */
  ~ChemicalSequence (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Binds a chemical element to a binding site of the sequence.
   * @param chemical_to_bind
   *  The chemical element to bind.
   */
  void bind_unit (const BoundChemical& chemical_to_bind);


  /**
   * @brief Unbinds one of the chemical elements that is bound to the sequence
   *  element.
   * @param  chemical_to_unbind
   *  The element to unbind.
   */
  void unbind_unit (const BoundChemical& chemical_to_unbind);


  /**
   * @brief Changes the nature of a bound unit.
   * @param old_chemical
   *  The original nature of the bound unit.
   * @param new_chemical
   *  The new nature of the bound unit.
   */
  void replace_bound_unit (const BoundChemical& old_chemical,
			   const BoundChemical& new_chemical);


  /**
   * @brief Move a bound unit
   * @param chemical_to_move
   *  The processive element that is moving along the sequence.
   * @param number_steps
   *  The number of steps by which it moves.
   */
  void move_bound_unit (ProcessiveChemical& chemical_to_move, int number_steps);


  /**
   * @brief Adds a given quantity of chemical to the existing pool.
   * @param quantity Amount to add
   */
  void add (int quantity);


  /**
   * @brief Remove a given quantity of chemical to the existing pool.
   * @param quantity Amount to remove.
   */
  void remove (int quantity);

  /**
   * @brief Watch availability of a specific site and notify an observer when it changes.
   * @param first First absolute position of the site.
   * @param lasth Last absolute position of the site.
   * @param site_observer SiteObserver to update with the current number of available sites.
   */
  void watch_site_availability (int first, int last,
				SiteObserver& site_observer);


  /**
   * @brief Add termination site on element.
   * @param termination_site
   *  Termination site located on sequence.
   */
  void add_termination_site (const Site& termination_site);


  /**
   * @brief Print class content.
   * @param output Stream where output should be written.
   */
  void print (std::ostream& output) const;


  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Returns whether the given site can be logically found on the
   *  sequence.
   * @param first Absolute position of first base of site.
   * @param last Asolute position of last base of site.
   * @return True if site is within sequence.
   */
  bool is_out_of_bounds (int first, int last) const;
    
  /**
   * @brief Returns whether a specific termination site can be found at a given 
   *  position.
   * @param position Absolute position to look at.
   * @param termination_site_families 
   *  List of termination sites to look for.
   * @return True if a requested termination site is present at requested
   *  position.
   */
  bool is_termination_site (int position, const std::list<int>& termination_site_families) const;

  /**
   * @brief Returns length of sequence.
   * @return Length of sequence.
   */
  int length (void) const;

  /**
   * @brief Returns the whole sequence.
   * @return String representing chemical sequence.
   */
  const std::string& sequence (void) const;

  /**
   * @brief Returns the sequence between two specific positions.
   * @return String sequence between two positions.
   * @param first
   *  Absolute position of the first base of the sequence to return (included).
   * @param last
   *  Absolute position of the last base of the sequence to return (included).
   */
  const std::string sequence (int first, int last) const;


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

private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Starting position of the sequence (allows absolute positionning). */
  int _starting_position;

  /** @brief Length of the sequence. */
  int _length;

  /** @brief Termination sites on the sequence. */
  std::map< int, std::list<int> > _termination_sites;

  /** @brief Sequence of the chemical. */
  std::string _sequence;  

  /** @brief Sequence occupation of the chemical. */
  SequenceOccupation _sequence_occupation;

  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Transform absolute positions to relative [0,length) positions.
   * @param absolute_position Absolute position to transform.
   * @return Relative position along the sequence, 0 being the starting
   *  position.
   */
  int relative (int absolute_position) const;
};

// ======================
//  Inline declarations
// ======================
//
#include "macros.h"

inline int ChemicalSequence::length (void) const
{ 
  return _length;
}

inline bool ChemicalSequence::is_out_of_bounds (int first, int last ) const
{
  /** @pre first must be smaller than last. */
  REQUIRE (first <= last);
  return ((relative (first) < 0) || (relative (last) >= _length));
}

inline const std::string& ChemicalSequence::sequence (void) const
{
  return _sequence;
}

inline const std::string ChemicalSequence::sequence (int first,
						     int last) const
{
  /** @pre first must be smaller than last. */
  REQUIRE (first <= last);
  /** @pre Requested sequence must be within sequence bounds. */
  REQUIRE (is_out_of_bounds (first, last) == false);

  return _sequence.substr (relative (first), last-first+1);
}

inline int ChemicalSequence::relative (int absolute_position) const
{ 
  return absolute_position - _starting_position;
}


#endif // CHEMICALSEQUENCE_H
