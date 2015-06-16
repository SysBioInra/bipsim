

/**
 * @file bindable.h
 * @brief Header for the Bindable class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef BINDABLE_H
#define BINDABLE_H

// ==================
//  General Includes
// ==================
//
#include <list> // std::list
#include <map> // std::map

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "macros.h"

/**
 * @brief Service class for bindable chemical elements.
 *
 * The Bindable class is a service class that contains basic methods for chemical
 * elements that can be bound by other elements. As a service class, it should be
 * inherited by these bindable elements.
 */
class Bindable
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor
   */
  Bindable (void);

  // Not needed for this class  (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // Bindable (Bindable& other_bindable);

  /**
   * @brief Destructor
   */
  virtual ~Bindable (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Binds a chemical element to a binding site of the bindable element.
   * @param chemical_to_bind
   *  The chemical element to bind.
   * @sa BindingSite
   */
  virtual void bind_unit ( const BoundChemical& chemical_to_bind ) = 0;


  /**
   * @brief Unbinds one of the chemical elements that is bound to the bindable
   *  element.
   * @param  chemical_to_unbind
   *  The element to unbind.
   */
  virtual void unbind_unit ( const BoundChemical& chemical_to_unbind ) = 0;


  /**
   * @brief Changes the nature of a bound unit.
   * @param old_chemical
   *  The original nature of the bound unit.
   * @param new_chemical
   *  The new nature of the bound unit.
   */
  virtual void replace_bound_unit ( const BoundChemical& old_chemical, const BoundChemical& new_chemical ) = 0;


  /**
   * @brief Moves a processive element by a given amount of steps.
   * @param chemical_to_move
   *  The processive element that is moving along the bindable element.
   * @param number_steps
   *  The number of steps by which it moves.
   */
  virtual void move_bound_unit ( ProcessiveChemical& chemical_to_move, int number_steps ) = 0;


  /**
   * @brief Add termination site on element.
   * @param termination_site
   *  Termination site located on bindable element.
   */
  void add_termination_site ( const Site& termination_site );


  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Returns the number of available sites at a given position.
   * @return int
   *  Number of available sites.
   * @param position Position of the site.
   * @param length Length of the site.
   * @sa BindingSite
   */
  virtual int number_available_sites ( int position, int length ) const = 0;
 
  /**
   * @brief Returns whether the given site can be logically found on the sequence.
   * @return True if position + length exceeds sequence length or position is negative.
   * @param position Position of the site.
   * @param length Length of the site.
   */
  bool is_out_of_bounds ( int position, int length ) const;
    
  /**
   * @brief Returns whether a specific termination site can be found at a given position.
   * @return True if a requested termination site is present at requested position.
   * @param position Position to look at.
   * @param termination_site_families 
   *  List of termination sites to look for.
   */
  bool is_termination_site ( int position, const std::list<int>& termination_site_families ) const;

  /**
   * @brief Returns length of bindable element.
   * @return Length of bindable element.
   */
  int length ( void ) const;

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
  // Bindable& operator= (Bindable& other_binding);


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
  /** @brief Length of the bindable element. */
  int _length;

  /** @brief Termination sites on the bindable element. */
  std::map< int, std::list<int> > _termination_sites;
  
  // =================
  //  Private Methods
  // =================
  //

};

// ======================
//  Inline declarations
// ======================
//
inline int Bindable::length ( void ) const { return _length; }

inline bool Bindable::is_out_of_bounds ( int position, int length ) const
{
  return ( ( position + length > _length ) || ( position < 0 ) );
}

#endif // BINDABLE_H
