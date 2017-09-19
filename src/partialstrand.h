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
 * @file partialstrand.h
 * @brief Header for the PartialStrand class.
 * @authors Marc Dinh, Stephan Fischer
 */

// Multiple include protection
//
#ifndef PARTIAL_STRAND_H
#define PARTIAL_STRAND_H

// ==================
//  General Includes
// ==================
//
#include <vector> // std::vector

// ==================
//  Project Includes
// ==================
//


// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"

/**
 * @brief Class representing a strand composed of subsegments.
 */
class PartialStrand
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /** @brief Constructor. */
  PartialStrand (int length);


  // /* @brief Copy constructor. */
  // PartialStrand (const PartialStrand& other);
  // /* @brief Assignment operator. */
  // PartialStrand& operator= (const PartialStrand& other);
  // /* @brief Destructor. */
  // ~PartialStrand (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Occupy given *unoccupied* position.
   * @param position Position that should be occupied.
   */
  void occupy (int position);

  /**
   * @brief Reset whole strand (becomes empty).
   */
  void reset (void);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Check whether strand has been completed.
   * @return True if strand has been completed.
   */
  bool completed (void) const;

  /**
   * @brief Check whether strand is empty.
   * @return True if strand is empty.
   */
  bool empty (void) const;

  /**
   * @brief Check whether given position is occupied
   * @param position Position along the strand.
   * @return True if position occupied, false if position unoccupied.
   */
  bool occupied (int position) const;

  /**
   * @brief Positon of segments composing partial strand.
   * @return Vector containing positions of segments. Numbers are to be read
   *  two by two, each pair representing a new segment. E.g. [1, 10, 15, 20]
   *  means that the strand is composed of segment [1 10] and [15 20].
   */
  std::vector <int> segments (void) const;

private:
  // =================
  //  Private Methods
  // =================
  //

  // ============
  //  Attributes
  // ============
  //
  /** @brief Length of the strand. */
  int _length;
  /** @brief Number of bases currently occupied. */
  int _number_occupied;
  /** @brief List of sequence segments. */
  std::vector <bool> _occupied;
};

// ======================
//  Inline declarations
// ======================
//
#include <algorithm> // std::fill
#include "macros.h"

inline void PartialStrand::occupy (int position)
{
  /** @pre Position must not be occupied already. */
  REQUIRE (_occupied [position] == false);
  _occupied [position] = true;
  ++_number_occupied;
  /** @post Number of position occupied should be reasonable. */
  ENSURE ((_number_occupied > 0) && (_number_occupied <= _length));
}

inline void PartialStrand::reset (void)
{
  std::fill (_occupied.begin(), _occupied.end(), false);
  _number_occupied = 0;
}

inline bool PartialStrand::completed (void) const
{
  return _number_occupied == _length;
}

inline bool PartialStrand::empty (void) const
{
  return _number_occupied == 0;
}

inline bool PartialStrand::occupied (int position) const
{
  return _occupied [position];
}


#endif // PARTIAL_STRAND_H
