

/**
 * @file site.h
 * @brief Header for the Site class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef SITE_H
#define SITE_H

// ==================
//  General Includes
// ==================
//

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "macros.h"

/**
 * @brief Class that represents sites on chemical sequences.
 *
 * The Site class contains two types of information. Each instance
 * belongs to a family of sites (e.g. Ribosome Binding Site) but also
 * has a specific location along a specific chemical sequence.
 */
class Site
{
public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor
   * @param family_id Integer family identifier.
   * @param location Chemical sequence containing the site.
   * @param position Position along the sequence.
   * @param length Length of the site.
   * @sa SiteHandler
   */
  Site ( int family_id, ChemicalSequence& location, int position, int length )
    : _family ( family_id )
    , _location ( location )
    , _position ( position )
    , _length ( length ) {}

  // Not needed for this class (use of default copy constructor) ! 
  // /*
  //  * @brief Copy constructor
  //  */
  // Site (Site& other_site);

  /**
   * @brief Destructor
   */
  virtual ~Site (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //

  // ============================
  //  Public Methods - Accessors
  // ============================
  //  
  /**
   * @brief Family accessor.
   * @return Integer family identifier.
   * @sa SiteList
   */
  int family ( void ) const;
 
  /**
   * @brief Site location.
   * @return ChemicalSequence that carries the site.
   */
  ChemicalSequence& location ( void ) const;

  /**
   * @brief Position accessor.
   * @return Position along the bearer.
   */
  int position ( void ) const;
 
  /**
   * @brief Length accessor.
   * @return Length of the motif.
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
  // CLASSNAME& operator= (CLASSNAME& other_site);


  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @brief Checks the class invariant.
   * @return True if class invariant is preserved
   */
  virtual bool check_invariant (void) const;
  

 protected:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Family to which the site belongs */
  int _family;

  /** @brief Chemical on which the site is located. */
  ChemicalSequence& _location;

  /** @brief Exact position of the site along the sequence. */
  int _position;
  
  /** @brief Length of the motif. */
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
inline int Site::family ( void ) const
{
  return _family;
}

inline int Site::position ( void ) const
{
  return _position;
}

inline int Site::length ( void ) const
{
  return _length;
}

inline ChemicalSequence& Site::location ( void ) const
{
  return _location;
}



#endif // SITE_H
