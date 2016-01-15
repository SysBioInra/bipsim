

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
   * @param current_position Position it currently occupies.
   * @param reading_frame Position where it can potentially read its sequence.
   */
  BoundUnit (const BindingSite& binding_site, int current_position, int reading_frame)
    : _binding_site (binding_site)
    , _current_position (current_position)
    , _reading_frame (reading_frame)
  {
  }

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor */
  // BoundUnit ( const BoundUnit& other_bound_unit );
  // /* @brief Assignment operator */
  // BoundUnit& operator= ( const BoundUnit& other_bound_unit );
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
   * @brief Return current position.
   * @return Position where unit is currently bound.
   */
  int current_position (void) const;
  
  /**
   * @brief Return reading frame position.
   * @return Position where unit can potentially read its sequence.
   */
  int reading_frame (void) const;
  
private:
  // ============
  //  Attributes
  // ============
  //
  /**
   * @brief Position to which the unit bound.
   */
  const BindingSite& _binding_site;

  /**
   * @brief Position where unit is currently bound.
   */
  int _current_position;
  
  /**
   * @brief Position where unit can potentially read its sequence.
   */
  int _reading_frame ;
  

  // =================
  //  Private Methods
  // =================
  //

};

// ======================
//  Inline declarations
// ======================
//
inline const BindingSite& BoundUnit::binding_site (void) const
{
  return _binding_site;
}

inline int BoundUnit::current_position (void) const
{
  return _current_position;
}
  
inline  int BoundUnit::reading_frame (void) const
{
  return _reading_frame;
}

inline void BoundUnit::move (int step_size)
{
  _current_position += step_size;
  _reading_frame += step_size;
}


#endif // BOUND_UNIT_H
