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
 * @file boundunit.h
 * @brief Header for the BoundUnit class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef BOUND_UNIT_H
#define BOUND_UNIT_H

// ==================
//  General Includes
// ==================
//
#include <vector> // std::vector

// ==================
//  Project Includes
// ==================
//
#include "macros.h"

#include "forwarddeclarations.h"

/**
 * @brief Class enabling restricted access to BoundUnit.
 * @details BoundUnitListKey uses the pass-key idiom to grant
 *  BoundUnitList exclusive access to some of its members.
 */
class BoundUnitListKey
{
 private:
  friend class BoundUnitList;
  /**
   * @brief Default constructor (can only be created by friends).
   */
  BoundUnitListKey (void) {}
};

/**
 * @brief The BoundUnit class describes a bound unit.
 */
class BoundUnit
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor from binding site.
   * @param site BindingSite the unit bound to.
   */
  BoundUnit (const BindingSite& site);

  /**
   * @brief Constructor from location.
   * @param location ChemicalSequence to which unit bound.
   * @param first First position occupied by unit.
   * @param last Last position occupied by unit.
   * @param reading_frame Reading frame of new unit.
   */
  BoundUnit (ChemicalSequence& location, int first, 
	     int last, int reading_frame);

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor */
  // BoundUnit (const BoundUnit& other);
  // /* @brief Assignment operator */
  // BoundUnit& operator= (const BoundUnit& other);
  // /* @brief Destructor */
  // ~BoundUnit (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Move the unit along the sequence according to a certain step.
   * @param step_size Size of the step.
   */  
  void move (int step_size);
  
  /**
   * @brief Rebind bound unit at a different binding site.
   * @param site BindingSite the unit bound to.
   */
  void rebind (const BindingSite& site);

  /**
   * @brief Rebind bound unit at a different location.
   * @param location ChemicalSequence to which unit bound.
   * @param first First position occupied by unit.
   * @param last Last position occupied by unit.
   * @param reading_frame Reading frame of new unit.
   */
  void rebind (ChemicalSequence& location, int first, 
	       int last, int reading_frame);

  /**
   * @brief Specify position as being on particular strand.
   * @param strand_id Integer identifier of strand.
   */
  void set_strand (int strand_id);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Return reading frame of unit at binding.
   * @return Reading frame of unit at binding.
   */
  int initial_reading_frame (void) const;

  /**
   * @brief Return location of unit.
   * @return ChemicalSequence on which unit is bound.
   */
  ChemicalSequence& location (void) const;

  /**
   * @brief Return first position spanned by unit.
   * @return First position spanned by unit.
   */
  int first (void) const;

  /**
   * @brief Return last position spanned by unit.
   * @return Last position spanned by unit.
   */
  int last (void) const;
  
  /**
   * @brief Return reading frame position.
   * @return Position where unit can potentially read its sequence.
   */
  int reading_frame (void) const;

  /**
   * @brief Return site the unit bound to.
   * @return Pointer to BindingSite the unit bound to.
   */
  const BindingSite* binding_site (void) const;

  /**
   * @brief Accessor to strand identifier.
   * @return Integer identifier of strand or NO_STRAND if none specified.
   */
  int strand (void) const;

  /**
   * @brief Accessor to position of unit within BoundUnitList.
   * @param key used to ensure that this function can only be called by a
   *  BoundUnitList.
   * @return List of indices of unit within the various BoundUnitLists in
   *  which it is stored.
   * @details The list of position is very useful to locate the unit very
   *  quickly within BoundUnitLists.
   */
  std::vector<int>& list_indices(const BoundUnitListKey& key) const;

  // ==================
  //  Public Constants
  // ==================
  //
  /** @brief Constant used when unit is not bound to a specific strand. */
  static const int NO_STRAND = -1;
  
private:
  // =================
  //  Private Methods
  // =================
  //

  // ============
  //  Attributes
  // ============
  //
  /** @brief Reading frame of unit at binding. */
  int _initial_reading_frame;
  /** @brief ChemicalSequence on which unit is bound. */
  ChemicalSequence* _location;
  /** @brief BindingSite to which unit bound (0 if no specific site). */
  const BindingSite* _binding_site;
  /** @brief First position spanned by unit. */
  int _first;
  /** @brief Last position spanned by unit. */
  int _last;
  /** @brief Position where unit can potentially read its sequence. */
  int _reading_frame;
  /** @brief Specific strand on which the bound unit is located. */
  int _strand;
  /** @brief Index of unit in various BoundUnitLists. */
  mutable std::vector<int> _list_indices;
};

// ======================
//  Inline declarations
// ======================
//
inline void BoundUnit::move (int step_size)
{
  _first += step_size;
  _last += step_size;
  _reading_frame += step_size;
}

inline void BoundUnit::set_strand (int strand_id)
{
  /** @pre strand_id must be positive. */
  REQUIRE (strand_id >= 0);
  _strand = strand_id;
}

inline int BoundUnit::initial_reading_frame (void) const
{
  return _initial_reading_frame;
}

inline ChemicalSequence& BoundUnit::location (void) const
{
  return *_location;
}

inline const BindingSite* BoundUnit::binding_site (void) const
{
  return _binding_site;
}

inline int BoundUnit::first (void) const
{
  return _first;
}

inline int BoundUnit::last (void) const
{
  return _last;
}
  
inline int BoundUnit::reading_frame (void) const
{
  return _reading_frame;
}

inline int BoundUnit::strand (void) const
{
  return _strand;
}

inline std::vector<int>& BoundUnit::list_indices(const BoundUnitListKey& key) const
{
  return _list_indices;
}


#endif // BOUND_UNIT_H
