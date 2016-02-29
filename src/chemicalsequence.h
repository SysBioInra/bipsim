

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
   * @brief Constructor
   * @param sequence Sequence of the chemical
   * @param starting_position Starting position (allows absolute positionning).
   */
  ChemicalSequence (const std::string& sequence, int starting_position = 1);

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor */
  // ChemicalSequence (ChemicalSequence& other_chemical);
  // /* @brief Assignment operator */
  // ChemicalSequence& operator= (ChemicalSequence& other_chemical);
  // /* @brief Destructor */
  // ~ChemicalSequence (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // redefined from Chemical
  void add (int quantity);
  void remove (int quantity);

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
   * @brief Extend or create new strand at given position.
   * @param position Position where extension should take place.
   **/
  void extend_strand (int position);

  /**
   * @brief Register binding site to update when its availability changes. 
   * @param site BindingSite to update when availability changes. Its position
   *  is supposed to not change over time.
   */
  void register_static_site (BindingSite& site);

  /**
   * @brief Register binding site to update when its availability changes. 
   * @param site BindingSite to update when availability changes. Its position
   *  may change over time.
   */
  void register_dynamic_site (BindingSite& site);

  /**
   * @brief Stop updating a previously registered dynamic site. 
   * @param site BindingSite to stop updating.
   */
  void deregister_dynamic_site (BindingSite& site);

  /**
   * @brief Add termination site on element.
   * @param termination_site
   *  Termination site located on sequence.
   */
  void add_termination_site (const Site& termination_site);

  /**
   * @brief Declare a strand to which the sequence is appariated.
   * @param strand Antisense strand to which the sequence is appariated.
   * @param factory Factory used to generate binding sites due to free ends
   *  in the pairing of sense/antisense strands.
   */
  void set_appariated_strand (ChemicalSequence& strand, 
			      const FreeEndFactory& factory);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Compute the number of sites.
   * @param first Starting position of the site.
   * @param last Ending position of the site.
   * @return Number of sites.
   */
  int number_sites (int first, int last) const;

  /**
   * @brief Compute the number of available sites.
   * @param first Starting position of the site.
   * @param last Ending position of the site.
   * @return Number of unoccupied sites.
   */
  int number_available_sites (int first, int last) const;

  /**
   * @brief Returns whether the given site can be logically found on the
   *  sequence.
   * @param first Relative position of first base of site.
   * @param last Relative position of last base of site.
   * @return True if site is within sequence.
   */
  bool is_out_of_bounds (int first, int last) const;
    
  /**
   * @brief Returns whether a specific termination site can be found at a given 
   *  position.
   * @param position Relative position to look at.
   * @param termination_site_families 
   *  List of termination sites to look for.
   * @return True if a requested termination site is present at requested
   *  position.
   */
  bool is_termination_site (int position, 
			    const std::list <const SiteFamily*>& termination_site_families) const;

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
   *  Relative position of the first base of the sequence to return (included).
   * @param last
   *  Relative position of the last base of the sequence to return (included).
   */
  const std::string sequence (int first, int last) const;

  /**
   * @brief Transform absolute positions to relative [0,length) positions.
   * @param absolute_position Absolute position to transform.
   * @return Relative position along the sequence, 0 being the starting
   *  position.
   */
  int relative (int absolute_position) const;

  /**
   * @brief Get corresponding position on antisense strand.
   * @param position Position on sense strand.
   * @return Position of corresponding base on antisense strand.
   */
  int complementary (int position) const;

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
  std::map <int, std::list <const SiteFamily*> > _termination_sites;

  /** @brief Sequence of the chemical. */
  std::string _sequence;  

  /** @brief Sequence occupation of the chemical. */
  SequenceOccupation _sequence_occupation;

  /** @brief Appariated strand (if applicable). */
  ChemicalSequence* _appariated_strand;

  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Print class content.
   * @param output Stream where output should be written.
   */
  void print (std::ostream& output) const;
};

// ======================
//  Inline declarations
// ======================
//
#include "macros.h"
inline 
int ChemicalSequence::number_sites (int first, int last) const
{
  /** @pre first must be smaller than last. */
  REQUIRE (first <= last);
  /** @pre first and last must be within sequence bound. */
  REQUIRE (!is_out_of_bounds (first, last));
  return _sequence_occupation.number_sites (first, last);
}

inline 
int ChemicalSequence::number_available_sites (int first, int last) const
{
  /** @pre first must be smaller than last. */
  REQUIRE (first <= last);
  /** @pre first and last must be within sequence bound. */
  REQUIRE (!is_out_of_bounds (first, last));
  return _sequence_occupation.number_available_sites (first, last);
}

inline int ChemicalSequence::length (void) const
{ 
  return _length;
}

inline bool ChemicalSequence::is_out_of_bounds (int first, int last) const
{
  /** @pre first must be smaller than last. */
  REQUIRE (first <= last);
  return ((first < 0) || (last >= _length));
}

inline const std::string& ChemicalSequence::sequence (void) const
{
  return _sequence;
}

inline 
const std::string ChemicalSequence::sequence (int first, int last) const
{
  /** @pre first must be smaller than last. */
  REQUIRE (first <= last);
  /** @pre Requested sequence must be within sequence bounds. */
  REQUIRE (is_out_of_bounds (first, last) == false);

  return _sequence.substr (first, last-first+1);
}

inline int ChemicalSequence::relative (int absolute_position) const
{ 
  return absolute_position - _starting_position;
}

inline int ChemicalSequence::complementary (int position) const
{
  /** @pre position must be within sequence bounds. */
  REQUIRE (!is_out_of_bounds (position, position));
  /** @pre A pairing must have been defined. */
  REQUIRE (_appariated_strand != 0);
  /** @post Returned value must be within bounds of appariated strand. */
  ENSURE (!_appariated_strand->is_out_of_bounds (_length-position-1,
						 _length-position-1));
  return _length-position-1;
}


#endif // CHEMICALSEQUENCE_H
