

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


// ==================
//  Project Includes
// ==================
//
#include "macros.h"

#include "forwarddeclarations.h"

/**
 * @brief The BoundUnit class describes a bound unit.
 *
 * It contains the unit's binding site, its current position and its current
 * reading frame position.
 */
class BoundUnit
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor
   * @param binding_site Position to which the unit bound.
   */
  BoundUnit (const BindingSite& binding_site);

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
   * @brief Rebind bound unit at a different location.
   * @param binding_site Position to which the unit bound.
   */
  void rebind (const BindingSite& binding_site);

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
   * @brief Return unit's original binding site.
   * @return Position to which the unit bound.
   */
  const BindingSite& binding_site (void) const;

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
   * @brief Accessor to strand identifier.
   * @return Integer identifier of strand or NO_STRAND if none specified.
   */
  int strand (void) const;

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
  /** @brief Position to which the unit bound. */
  const BindingSite* _binding_site;

  /** @brief ChemicalSequence on which unit is bound. */
  ChemicalSequence* _location;

  /** @brief First position spanned by unit. */
  int _first;

  /** @brief Last position spanned by unit. */
  int _last;
  
  /** @brief Position where unit can potentially read its sequence. */
  int _reading_frame;

  /** @brief Specific strand on which the bound unit is located. */
  int _strand;
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

inline const BindingSite& BoundUnit::binding_site (void) const
{
  return *_binding_site;
}

inline ChemicalSequence& BoundUnit::location (void) const
{
  return *_location;
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

#endif // BOUND_UNIT_H
