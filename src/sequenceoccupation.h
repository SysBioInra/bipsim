

/**
 * @file sequenceoccupation.h
 * @brief Header for the SequenceOccupation class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef SEQUENCE_OCCUPATION_H
#define SEQUENCE_OCCUPATION_H

// ==================
//  General Includes
// ==================
//
#include <vector> // std::vector
#include <list> // std::list

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"

/**
 * @brief Class handling occupation levels on ChemicalSequence.
 *
 * SequenceOccupation has two roles: it stores occupation levels at every
 * base of the sequence and the chemicals bound to every position. It also
 * handles SiteAvailability objects that warn BindingSite about change in
 * availability at specific sites.
 */
class SequenceOccupation
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor.
   * @param length Length of the sequence whose occupation should be handled.
   * @param number Number of sequences initially present.
   */
  SequenceOccupation (int length, int number);

 private:
  // Forbidden
  /** @brief Copy constructor. */
  SequenceOccupation (const SequenceOccupation& other);
  /** @brief Assignment operator. */
  SequenceOccupation& operator= (const SequenceOccupation& other);

 public:
  /**
   * @brief Destructor.
   */
  ~SequenceOccupation (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Register new bound element.
   * @param element Element that was bound to the sequence.
   * @param first First base occupied by element.
   * @param last Last base occupied by element.
   */
  void add_element (const BoundChemical& element, int first, int last);
  
  /**
   * @brief Register element removed from sequence.
   * @param element Element that was removed from the sequence.
   * @param first First base occupied by element.
   * @param last Last base occupied by element.
   */
  void remove_element (const BoundChemical& element, int first, int last);

  /**
   * @brief Register new sequence instances added to the pool.
   * @param quantity Number of instances of sequence added to pool.
   */
  void add_sequence (int quantity);

  /**
   * @brief Register sequence instances removed from pool.
   * @param quantity Number of instances of sequence added to pool.
   */
  void remove_sequence (int quantity);

  /**
   * @brief Start segment of sequence.
   * @param position Position where to start the segment.
   */
  void start_segment (int position);

  /**
   * @brief Extend a segment.
   * @param position Position where extension should happen.
   *
   * Extension can only take place if a segment has already been started that
   * extends until position-1. If there are several segments meeting this
   * requirement, the last segment accessed will be extended.
   */
  void extend_segment (int position);

  /**
   * @brief Register static site.
   * @param site BindingSite to notify when a change happens on the site.
   */
  void register_site (BindingSite& site);

  /**
   * @brief Register moving site.
   * @param site BindingSite to notify when a change happens on the site.
   */
  void register_moving_site (BindingSite& site);

  /**
   * @brief Deregister moving site.
   * @param site BindingSite to stop watching.
   */
  void deregister_moving_site (BindingSite& site);

  /**
   * @brief Define a free end factory for partial strands.
   * @param factory Factory used to generate free ends of partial strands.
   */
  void set_free_end_factory (const FreeEndFactory& factory);

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

private:
  // ============
  //  Attributes
  // ============
  //
  /** @brief Number of sequences in the pool. */
  int _number_sequences;

  /** @brief Tracks number of segments spanning each base. */
  std::vector <int> _number_segments;

  /** @brief Tracks occupied positions along the sequence. */
  std::vector <int> _occupancy;

  /** @brief Groups of static sites whose availability needs to be checked. */
  std::vector <SiteGroup*> _site_groups;

  /** @brief List of moving sites whose availability needs to be checked. */
  std::list <BindingSite*> _moving_sites;

  /** @brief List of partial strands. */
  std::list <PartialStrand*> _partials;  

  /** @brief Free end factory. */
  const FreeEndFactory* _free_end_factory; 

  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Convert a position along the sequence to a wtcher group index.
   * @param position Position along the sequence.
   * @return Index of the SiteGroup that contains this position (as site
   *  groups are non-overlapping, it is necessarily unique). If no group
   *  contains the position, returns the index of the next group along the
   *  sequence or the number of groups if there are no groups between the 
   *  position and the end of the sequence.
   */
  int position_to_group (int position) const;

  /**
   * @brief Send notifications knowing that a change occurred between two
   *  precise positions.
   * @param a First position affected by change.
   * @param b Last position affected by change.
   */
  void notify_change (int a, int b) const;

  /**
   * @brief Send notifications to all sites due to a global change.
   */
  void notify_all_sites (void) const;

  /**
   * @brief Fuse overlapping site groups starting from a specific index.
   *
   *  This function should be called whenever a group is extended. It tries to
   *  fuse the group recursively with following groups, as it assumes that the
   *  first group was extended towards the end of the sequence.
   *
   * @param index Index of the group to start the fusion from, typically a group
   *  that was just extended towards the end of the sequence.
   */
  void fuse_groups (int index);

  /**
   * @brief Check if partial strand is complete and add it to full sequences.
   * @param strand_it Partial strand to check.
   */
  void _check_completion (std::list <PartialStrand*>::iterator& strand_it);
};

// ======================
//  Inline declarations
// ======================
//
inline 
void SequenceOccupation::set_free_end_factory (const FreeEndFactory& factory)
{
  _free_end_factory = &factory;
}

#endif // SEQUENCE_OCCUPATION_H
