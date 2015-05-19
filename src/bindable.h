

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
#include <list>

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
   * @param position Position of binding site.
   * @param length Size occupied by bound element on sequence.
   * @sa BindingSite
   */
  virtual void bind_unit ( BoundChemical& chemical_to_bind, int position, int length ) = 0;


  /**
   * @brief Unbinds one of the chemical elements that is bound to the bindable
   *  element.
   * @param  chemical_to_unbind
   *  The element to unbind.
   */
  virtual void unbind_unit ( BoundChemical& chemical_to_unbind, int position, int length ) = 0;


  /**
   * @brief Changes the nature of a bound unit.
   * @param old_chemical
   *  The original nature of the bound unit.
   * @param new_chemical
   *  The new nature of the bound unit.
   */
  virtual void replace_bound_unit ( BoundChemical& old_chemical, BoundChemical& new_chemical ) = 0;


  /**
   * @brief Moves a processive element by a given amount of steps.
   * @param chemical_to_move
   *  The processive element that is moving along the bindable element.
   * @param number_steps
   *  The number of steps by which it moves.
   */
  virtual void move_bound_unit ( ProcessiveChemical& chemical_to_move, int number_steps ) = 0;

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
  virtual int number_available_sites ( int position, int length ) = 0;
 
  /**
   * @brief Returns length of bindable element.
   * @return Length of bindable element.
   */
  int length ( void );

  // ==========================
  //  Public Methods - Setters
  // ==========================
  //
  /**
   * @brief Sets element length.
   * @param length Length of bindable element.
   */
  virtual void set_length ( int length );
  
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
  virtual bool check_invariant (void);

private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Length of the bindable element. */
  int _length;
  
  // =================
  //  Private Methods
  // =================
  //

};

// ======================
//  Inline declarations
// ======================
//
inline int Bindable::length ( void ) { return _length; }

inline void Bindable::set_length ( int length )
{
  REQUIRE( length > 0 ); /** @pre Length must be positive. */
  _length = length;
}

#endif // BINDABLE_H
