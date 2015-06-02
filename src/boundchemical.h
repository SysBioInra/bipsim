


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
#include <list>  // std::list
#include <map>  // std::map
#include <utility> // std::pair

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "chemical.h"
#include "bindingsite.h"

typedef std::pair< const BindingSite *, int > BoundUnit;
typedef std::list< BoundUnit > BoundUnitList;
typedef std::map< int, BoundUnitList > UnitFamilyMap;

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
  // Not needed here
  // /*
  // * @brief Default constructor
  // */
  // BoundChemical (void);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // BoundChemical (BoundChemical& other_bound_chemical);

  /**
   * @brief Destructor
   */
  virtual ~BoundChemical ( void );

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Actions to be taken when the chemical has collided with another
   * chemical.
   */
  void handle_collision ( void );

  /**
   * @brief Add a new unit specific binding site.
   * @param binding_site The binding site to which it bound.
   */
  void add_unit_at_site ( const BindingSite& binding_site );

  /**
   * @brief Add a new unit in place of an existing new chemical.
   */
  void add_unit_in_place_of ( const BoundChemical& precursor );
  
  /**
   * @brief Remove focused unit.
   */
  void remove_focused_unit ( void );

  /**
   * @brief Focus a unit randomly.
   */
  void focus_random_unit ( void );

  /**
   * @brief Focus a unit that bound to a specific binding site family.
   * @param binding_site The binding site family to inspect.
   */
  void focus_random_unit ( int binding_site_family );

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Returns the number of elements bound to a specific binding site family.
   * @return Number of elements bound to the binding site family.
   * @param binding_site The binding site family to inspect.
   */
  int number_bound_to_family ( int binding_site_family ) const;

  /**
   * @brief Returns the binding site of focused unit.
   * @return Binding site of focused unit.
   */
  const BindingSite& focused_unit_binding_site ( void ) const;

  /**
   * @brief Returns the position of focused unit.
   * @return Position of focused unit.
   */
  int focused_unit_position ( void ) const;

  /**
   * @brief Returns the position of focused unit.
   * @return Length of focused unit.
   */
  int focused_unit_length ( void ) const;

  /**
   * @brief Returns the location of focused unit.
   * @return Bindable that bears the bound chemical.
   */
  Bindable& focused_unit_location ( void ) const;

  /**
   * @brief Returns the total unbinding rate for a subset of chemicals.
   * @param binding_site_family
   *   Binding site identifier to which the chemicals of interest must be
   *   bound.
   * @return Total unbinding rate for chemicals bound to given family.
   */
  double get_total_unbinding_rate_contribution ( int binding_site_family ) const;

  /**
   * @return Print class content.
   * @param output Stream where output should be written.
   */
  virtual void print (std::ostream& output) const;


  // ==========================
  //  Public Methods - Setters
  // ==========================
  //


  // =======================================
  //  Public Methods - Operator overloading
  // =======================================
  //
  // Not needed for this class (use of default overloading) !
  // /*
  //  * @brief Assignment operator
  //  */
  // CLASSNAME& operator= (CLASSNAME& other_CLASSNAME);

  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @return True if class invariant is preserved
   */
  virtual bool check_invariant (void) const;


 protected:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Binding site to which the focused unit bound and current position. */
  BoundUnitList::iterator _focused_unit;


 private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Binding sites to which chemicals bound and current position (sorted by family). */
  UnitFamilyMap _family_map;
    
  // =================
  //  Private Methods
  // =================
  //

};

// ======================
//  Inline declarations
// ======================
//
inline const BindingSite& BoundChemical::focused_unit_binding_site ( void ) const
{
  return *(_focused_unit->first);
}

inline int BoundChemical::focused_unit_length ( void ) const
{
  return (_focused_unit->first)->length();
}

inline Bindable& BoundChemical::focused_unit_location ( void ) const
{
  return (_focused_unit->first)->location();
}

inline int BoundChemical::focused_unit_position ( void ) const
{
  return _focused_unit->second;
}



#endif // BOUNDCHEMICAL_H
