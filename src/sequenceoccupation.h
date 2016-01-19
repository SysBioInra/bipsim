

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
#include <map> // std::map

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
 * handles SiteAvailability objects that warn SiteObserver about change in
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
  SequenceOccupation (const SequenceOccupation& other_sequence_occupation);
  /** @brief Assignment operator. */
  SequenceOccupation& operator= (const SequenceOccupation& other_sequence_occupation);

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
   * @brief Attach site watcher to a specific site.
   *
   *  This member creates an object of type SiteAvailability that will send out
   *  notifications every time a change occurs along the sequence (i.e. every
   *  time another member of this class is called).
   *
   * @param first First base of the site to watch.
   * @param last Last base of the site to watch.
   * @param observer SiteObserver to notify when a change happens on the site.
   */
  void add_watcher (int first, int last, SiteObserver& observer);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //

private:
  // ============
  //  Attributes
  // ============
  //
  /** @brief Number of sequences in the pool. */
  int _number;

  /** @brief Tracks available positions along the sequence. */
  std::vector<int> _occupancy_map;

  /** @brief A simple list of SiteLocation. */
  typedef std::list<SiteLocation> SiteLocationList;
  
  /**
   * @brief A map that stores the location of all BoundChemical.
   *
   * Keys of the map are references to all BoundChemical currently located on
   * the chemical sequence, data is a SiteLocationList that stores the location
   * of all chemicals designated by the key.
   */
  typedef std::map< const BoundChemical*, SiteLocationList > ChemicalMap;

  /** @brief Map of bound chemicals. */
  ChemicalMap _chemical_map;

  /** @brief List of sites whose availability needs to be checked. */
  std::vector <WatcherGroup*> _watcher_groups;
  

  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Remove a specific reference from the bound chemical map.
   * @param chemical The type of chemical to remove.
   * @param first First position occupied by element.
   * @param last Last position occupied by element.
   */
  void remove_reference_from_map (const BoundChemical& chemical,
				  int first, int last);

  /**
   * @brief Convert a position along the sequence to a wtcher group index.
   * @param position Position along the sequence.
   * @return Index of the WatcherGroup that contains this position (as watcher
   *  groups are non-overlapping, it is necessarily unique). If no group
   *  contains the position, returns the index of the next group along the
   *  sequence or the number of groups if there are no groups between the 
   *  position and the end of the sequence.
   */
  int position_to_group (int position);

  /**
   * @brief Send notifications knowing that a change occurred between two
   *  precise positions.
   * @param a First position affected by change.
   * @param b Last position affected by change.
   */
  void notify_change (int a, int b);

  /**
   * @brief Send notifications to all observers due to a global change.
   */
  void notify_all_sites (void);

  /**
   * @brief Fuse overlapping watcher groups starting from a specific index.
   *
   *  This function should be called whenever a group is extended. It tries to
   *  fuse the group recursively with following groups, as it assumes that the
   *  first group was extended towards the end of the sequence.
   *
   * @param index Index of the group to start the fusion from, typically a group
   *  that was just extended towards the end of the sequence.
   */
  void fuse_groups (int index);
};

// ======================
//  Inline declarations
// ======================
//

#endif // SEQUENCE_OCCUPATION_H
