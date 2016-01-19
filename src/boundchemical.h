


/**
 * @file boundchemical.h
 * @brief Header for the BoundChemical class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef BOUNDCHEMICAL_H
#define BOUNDCHEMICAL_H

// ==================
//  General Includes
// ==================
//
#include <map>  // std::map

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "chemical.h"
#include "boundunitlist.h"


/**
 * @brief The BoundChemical class describes chemicals in a bound form.
 *
 * Bound chemicals may be linked with specific reactions (i.e. different from
 * their free counterparts) and collide with other bound elements.
 * Inherits class Chemical.
 * @sa Chemical
 */
class BoundChemical : public Chemical
{
public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  /** @brief Default constructor */
  BoundChemical (void);
  
 private:
  // Forbidden
  /** @brief Copy constructor */
  BoundChemical (BoundChemical& other_bound_chemical);
  /** @brief Assignment operator */
  BoundChemical& operator= (BoundChemical& other_chemical);
 public:

  /**
   * @brief Destructor
   */
  virtual ~BoundChemical (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // redefined from Chemical
  /**
   * @brief Adds a given quantity of chemical to the existing pool.
   * @param quantity The amount to add
   * @details Cannot be called directly, will be ignored. 
   */
  void add (int quantity);

  /**
   * @brief Remove a given quantity of chemical to the existing pool.
   * @param quantity The amount to remove.
   * @details Cannot be called directly, will be ignored. 
   */
  void remove (int quantity);

  /**
   * @brief Add a new unit specific binding site.
   * @param binding_site The binding site to which it bound.
   */
  void add_unit_at_site (const BindingSite& binding_site);

  /**
   * @brief Add a new unit in place of an existing new chemical.
   */
  void add_unit_in_place_of (const BoundChemical& precursor);
  
  /**
   * @brief Remove focused unit.
   */
  virtual void remove_focused_unit (void);

  /**
   * @brief Focus a unit randomly.
   */
  virtual void focus_random_unit (void);

  /**
   * @brief Focus a unit that bound to a specific binding site family.
   * @param binding_site_family The binding site family to inspect.
   */
  virtual void focus_random_unit (int binding_site_family);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Returns the number of elements bound to a specific binding site
   *  family.
   * @return Number of elements bound to the binding site family.
   * @param binding_site_family The binding site family to inspect.
   */
  int number_bound_to_family (int binding_site_family) const;

  /**
   * @brief Returns the binding site of focused unit.
   * @return Binding site of focused unit.
   */
  const BindingSite& focused_unit_binding_site (void) const;

  /**
   * @brief Accessor of first position of focused unit.
   * @return First position of focused unit.
   */
  int focused_unit_first (void) const;

  /**
   * @brief Accessor to last position of focused unit.
   * @return Last position of focused unit.
   */
  int focused_unit_last (void) const;

  /**
   * @brief Accessor to reading frame of focused unit.
   * @return Position where unit can potentially read its sequence.
   */
  int focused_unit_reading_frame (void) const;

  /**
   * @brief Returns the location of focused unit.
   * @return ChemicalSequence that bears the bound chemical.
   */
  ChemicalSequence& focused_unit_location (void) const;

  /**
   * @brief Returns the total unbinding rate for a subset of chemicals.
   * @param binding_site_family
   *   Binding site identifier to which the chemicals of interest must be
   *   bound.
   * @return Total unbinding rate for chemicals bound to given family.
   */
  double unbinding_rate_contribution (int binding_site_family) const;

 protected:
  // ============
  //  Attributes
  // ============
  //
  /** @brief Pointer to the focused unit. */
  BoundUnit* _focused_unit;

  // =================
  //  Protected Methods
  // =================
  //
  /**
   * @brief Add a new unit.
   *
   * This function MUST be called every time a unit is added.
   * @param binding_site The binding site to which it bound.
   * @param position Current position.
   * @param reading_frame Reading frame position.
   */
  virtual void add_unit (const BindingSite& binding_site,
			 int position, int reading_frame);
  

 private:
  // ============
  //  Attributes
  // ============
  //  
  /**
   * @brief A map that classifies all BoundChemical depending on the family of
   *  BindingSite they bound to.
   */
  typedef std::map <int, BoundUnitList> UnitFamilyMap;

  /** @brief Binding sites to which chemicals bound and current position (sorted by family). */
  UnitFamilyMap _family_map;

    
  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Print class content.
   * @param output Stream where output should be written.
   */
  virtual void print (std::ostream& output) const;
};

// ======================
//  Inline declarations
// ======================
//
#include <iostream>
#include "boundunit.h"

inline void BoundChemical::add (int quantity)
{
  std::cerr << "ERROR: cannot directly add molecules of type BoundChemical."
	    << " Command ignored.\n";
}

inline void BoundChemical::remove (int quantity)
{
  std::cerr << "ERROR: cannot directly remove molecules of type BoundChemical."
	    << " Command ignored.\n";
}

inline const BindingSite& BoundChemical::focused_unit_binding_site (void) const
{
  return _focused_unit->binding_site();
}

inline int BoundChemical::focused_unit_last (void) const
{
  return (_focused_unit->binding_site()).last()
    + _focused_unit->current_position() 
    - (_focused_unit->binding_site()).first();
}

inline ChemicalSequence& BoundChemical::focused_unit_location (void) const
{
  return (_focused_unit->binding_site()).location();
}

inline int BoundChemical::focused_unit_first (void) const
{
  return _focused_unit->current_position();
}

inline int BoundChemical::focused_unit_reading_frame (void) const
{
  return _focused_unit->reading_frame();
}



#endif // BOUNDCHEMICAL_H
