

/**
 * @file bindingsite.h
 * @brief Header for the BindingSite class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef BINDINGSITE_H
#define BINDINGSITE_H

// ==================
//  General Includes
// ==================
//

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "bindable.h"
#include "macros.h"

/**
 * @brief Class that represents binding sites on bindable elements.
 *
 * The BindingSite class contains two types of information. Each instance
 * belongs to a family of binding sites (e.g. Ribosome Binding Site) but also
 * has a specific location along a specific chemical sequence.
 */
class BindingSite
{
public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor
   */
  BindingSite (void);

  // Not needed for this class (use of default copy constructor) ! 
  // /*
  //  * @brief Copy constructor
  //  */
  // BindingSite (BindingSite& other_binding_site);

  /**
   * @brief Destructor
   */
  virtual ~BindingSite (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Bind a chemical at the binding site.
   * @param unit_to_bind Chemical to bind.
   */
  void bind_unit ( BoundChemical& unit_to_bind );

  /**
   * @brief Unbind a chemical at the binding site.
   * @param unit_to_unbind Chemical to unbind.
   */
  void unbind_unit ( BoundChemical& unit_to_unbind );

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Affinity accessor.
   * @return Affinity value.
   */
  double affinity ( void );
  
  /**
   * @brief Family accessor.
   * @return Integer family identifier.
   * @sa BindingSiteList
   */
  int family ( void );
 
  /**
   * @brief Number of available sites in the cell.
   * @return Number of available sites in the cell.
   */
  int number_available_sites ( void );

  // ==========================
  //  Public Methods - Setters
  // ==========================
  //
  /**
   * @brief Set family (e.g. Ribosome Binding Site).
   * @param family_id Integer family identifier.
   * @sa BindingSiteList
   */
  void set_family ( int family_id );

  /**
   * @brief Set on which chemical the site is.
   * @param location Chemical sequence containing the site.
   * @param position Position along the sequence.
   * @param length Length of the binding site.
   */
  void set_physical_context ( Bindable& location, int position, int lenght );

  /**
   * @brief Set affinity.
   * @param affinity Affinity value.
   */
  void set_affinity (double affinity);

  // =======================================
  //  Public Methods - Operator overloading
  // =======================================
  //
  // Not needed for this class (use of default overloading) !
  // /*
  //  * @brief Assignment operator
  //  */
  // CLASSNAME& operator= (CLASSNAME& other_binding);


  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @brief Checks the class invariant.
   * @return True if class invariant is preserved
   */
  virtual bool check_invariant (void);
  

 private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Family to which the binding site belongs */
  int _family;

  /** @brief Chemical on which the binding site is located. */
  Bindable* _location;

  /** @brief Exact position of the binding site along the sequence. */
  int _position;

  /** @brief Length of the binding motif. */
  int _length;

  /** @brief Affinity of the motif. */
  double _affinity;

  // =================
  //  Private Methods
  // =================
  //

};

// ======================
//  Inline declarations
// ======================
//
inline void BindingSite::bind_unit ( BoundChemical& unit_to_bind )
{
  REQUIRE( _location != 0 ); /** @pre _location must be defined. */
  _location->bind_unit ( unit_to_bind, _position, _length );
}

inline void BindingSite::unbind_unit ( BoundChemical& unit_to_unbind )
{
  REQUIRE( _location != 0 ); /** @pre _location must be defined. */    
  _location->unbind_unit ( unit_to_unbind, _position, _length );
}


inline double BindingSite::affinity (void)
{ 
  return _affinity;
}

inline int BindingSite::family ( void )
{
  return _family;
}

inline void  BindingSite::set_family (int family_id)
{
  _family = family_id;
}

inline void  BindingSite::set_physical_context ( Bindable& location, int position, int length )
{
  REQUIRE( position > 0 ); /** @pre position > 0 */
  REQUIRE( length > 0 ); /** @pre length > 0 */
  _location = &location;
  _position = position;
  _length = length;
}

inline void  BindingSite::set_affinity (double affinity)
{
  REQUIRE( affinity >= 0 ); /** @pre affinity >= 0 */
  _affinity = affinity;
}


#endif // BINDINGSITE_H
