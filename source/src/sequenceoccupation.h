// 
// Copyright 2017 INRA
// Authors: M. Dinh, S. Fischer
// Last modification: 2017-09-19
// 
// 
// Licensed under the GNU General Public License.
// You should have received a copy of the GNU General Public License
// along with BiPSim.  If not, see <http://www.gnu.org/licenses/>.
// 


/**
 * @file sequenceoccupation.h
 * @brief Header for the SequenceOccupation class.
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
#include <stack> // std::stack

// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"

/**
 * @brief Class handling occupation levels on ChemicalSequence.
 * @details SequenceOccupation has two roles: it stores occupation levels at 
 * every base of the sequence and the chemicals bound to every position. It 
 * also handles SiteAvailability objects that warn BindingSite about change in
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
   */
  SequenceOccupation (int length);

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
   * @param first First base occupied by element.
   * @param last Last base occupied by element.
   */
  void add_element (int first, int last);
  
  /**
   * @brief Register element removed from sequence.
   * @param first First base occupied by element.
   * @param last Last base occupied by element.
   */
  void remove_element (int first, int last);

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
   * @brief Extend strand at given position.
   * @param strand_id Integer identifer of strand to extend.
   * @param position Position where extension should happen.
   * @return True if extension worked.
   */
  bool extend_strand (int strand_id, int position);

  /**
   * @brief Release id and memory associated with given partial strand.
   * @param strand_id Integer identifer of strand to release.
   */
  void release_strand_id (int strand_id);

  /**
   * @brief Register static site.
   * @param site BindingSite to notify when a change happens on the site.
   */
  void watch_site (BindingSite& site);

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
   * @brief Accessor to partial strand at given position.
   * @param position Position of interest.
   * @return Integer identifier of the oldest strand not synthesized at
   *  given position (or new strand).
   */
  int partial_strand_id (int position) const;

  /**
   * @brief Check whether partial strand is completed.
   * @param strand_id Identifier of strand to check.
   * @return True if strand is fully polymerized.
   */
  bool strand_completed (int strand_id) const;

  /**
   * @brief Composition of partial strands.
   * @return Each entry of the list represents a different strand. Each strand
   *  is represented by a vector of integers giving the position of the segments
   *  composing the strand. E.g. {[1 10], [1 5 10 20]} means there are two
   *  partial strands. Strand 1 is composed of a single segment [1 10], strand
   *  2 of segments [1 5] and [10 20].
   */
  std::list <std::vector <int> > partial_strands (void) const;

private:
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
  std::size_t position_to_group (int position) const;

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
   * @param index Index of the group to start the fusion from, typically a group
   *  that was just extended towards the end of the sequence.
   *
   *  This function should be called whenever a group is extended. It tries to
   *  fuse the group recursively with following groups, as it assumes that the
   *  first group was extended towards the end of the sequence.
   */
  void fuse_groups (std::size_t index);

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

  /** @brief Groups of sites whose availability needs to be maintained. */
  std::vector <SiteGroup*> _site_groups;

  /** @brief Vector of partial strands (access by identifier). */
  std::vector <PartialStrand*> _partials;  
  /** @brief List of partial strand identifiers in creation order. */
  mutable std::list <int> _partial_creation_order;  
  /** @brief List of unused partial strands. */
  mutable std::stack <int, std::vector <int> > _unused_partials;
};

// ======================
//  Inline declarations
// ======================
//
#include "macros.h" // REQUIRE
#include "partialstrand.h"

inline bool SequenceOccupation::strand_completed (int strand_id) const
{
  /** @pre strand_id must be a valid id. */
  REQUIRE ((strand_id >= 0) && (strand_id < _partials.size()));
  return _partials [strand_id]->completed();
}


#endif // SEQUENCE_OCCUPATION_H
