
/**
 * @file chemicalsequence.h
 * @brief Header for the ChemicalSequence class.
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
#include "freechemical.h"
#include "sequenceoccupation.h"

/**
 * @brief This class stores chemicals that can be described by a sequence.
 * @details Usually polymers based on a specific alphabet. Typical examples 
 *  include DNA, RNA and proteins.
 */
class ChemicalSequence : public FreeChemical
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
   * @param is_circular Indicates whether sequence is circular or not.
   */
  ChemicalSequence (const std::string& sequence, int starting_position = 1,
		    bool is_circular = false);

 private:
  /** @brief Copy constructor */
  ChemicalSequence (ChemicalSequence& other_chemical);
  /** @brief Assignment operator */
  ChemicalSequence& operator= (ChemicalSequence& other_chemical);
 public:
  
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
   * @brief Bind an element to the sequence.
   * @param first First position occupied by element.
   * @param last Last position occupied by element.
   */
  void bind_unit (int first, int last);

  /**
   * @brief Unbinds an element from sequence.
   * @param first First position occupied by element.
   * @param last Last position occupied by element.
   */
  void unbind_unit (int first, int last);

  /**
   * @brief Extend partial strand at given position.
   * @param strand_id Integer identifier provided at strand creation.
   * @param position Position where extension should take place.
   * @return True if extension worked, false if position already occupied.
   */
  bool extend_strand (int strand_id, int position);

  /**
   * @brief Register binding site to update when its availability changes. 
   * @param site BindingSite to update when availability changes. Its position
   *  is supposed to not change over time.
   */
  void watch_site (BindingSite& site);

  /**
   * @brief Add termination site on element.
   * @param termination_site
   *  Termination site located on sequence.
   */
  void add_termination_site (const Site& termination_site);

  /**
   * @brief Declare a sequence to which the sequence is appariated.
   * @param sequence Antisense sequence to which the sequence is appariated.
   */
  void set_appariated_sequence (ChemicalSequence& sequence);
  
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
   * @brief Composition of partial strands.
   * @return Each entry of the list represents a different strand. Each strand
   *  is represented by a vector of integers giving the position of the segments
   *  composing the strand. E.g. {[1 10], [1 5 10 20]} means there are two
   *  partial strands. Strand 1 is composed of a single segment [1 10], strand
   *  2 of segments [1 5] and [10 20].
   */
  std::list <std::vector <int> > partial_strands (void) const;

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
  bool is_termination_site 
    (int position, 
     const std::vector <const SiteFamily*>& termination_site_families) const;

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
   * @brief Accessor to partial strand.
   * @param position Position of interest.
   * @return Integer identifier of the oldest strand not synthesized at
   *  given position (or new strand).
   */
  int partial_strand_id (int position) const;

  /**
   * @brief Get chemical's appariated strand.
   * @return Pointer to appariated strand or 0 if sequence is not double strand.
   */
  ChemicalSequence* appariated_strand (void) const;

  /**
   * @brief Get corresponding position on antisense sequence.
   * @param position Position on sense sequence.
   * @return Position of corresponding base on antisense sequence.
   */
  int complementary (int position) const;

private:
  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Print class content.
   * @param output Stream where output should be written.
   */
  void print (std::ostream& output) const;

  // ============
  //  Attributes
  // ============
  //
  /** @brief Length of the sequence. */
  int _length;

  /** @brief Starting position of the sequence (allows absolute positioning). */
  int _starting_position;

  /** @brief Sequence of the chemical. */
  std::string _sequence;  

  /** @brief Circularity. */
  bool _is_circular;

  /** @brief Termination sites on the sequence. */
  std::map <int, std::list <const SiteFamily*> > _termination_sites;

  /** @brief Appariated sequence. */
  ChemicalSequence* _appariated_sequence;

  /** @brief Sequence occupation of the chemical. */
  SequenceOccupation _occupation;
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
  return _occupation.number_sites (first, last);
}

inline 
int ChemicalSequence::number_available_sites (int first, int last) const
{
  /** @pre first must be smaller than last. */
  REQUIRE (first <= last);
  /** @pre first and last must be within sequence bound. */
  REQUIRE (!is_out_of_bounds (first, last));
  return _occupation.number_available_sites (first, last);
}

inline 
std::list <std::vector <int> > ChemicalSequence::partial_strands (void) const
{
  return _occupation.partial_strands();
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
  REQUIRE (!is_out_of_bounds (first, last));

  return _sequence.substr (first, last-first+1);
}

inline int ChemicalSequence::relative (int absolute_position) const
{ 
  return absolute_position - _starting_position;
}

inline int ChemicalSequence::partial_strand_id (int position) const
{
  /** @pre Position must be consistent with sequence length. */
  REQUIRE (is_out_of_bounds (position, position) == false);
  return _occupation.partial_strand_id (position);
}

inline ChemicalSequence* ChemicalSequence::appariated_strand (void) const
{
  return _appariated_sequence;
}

inline int ChemicalSequence::complementary (int position) const
{
  /** @pre position must be within sequence bounds. */
  REQUIRE (!is_out_of_bounds (position, position));
  /** @pre A pairing must have been defined. */
  REQUIRE (_appariated_sequence != 0);
  /** @post Returned value must be within bounds of appariated strand. */
  ENSURE (!_appariated_sequence->is_out_of_bounds (_length-position-1,
						   _length-position-1));
  return _length-position-1;
}


#endif // CHEMICALSEQUENCE_H
