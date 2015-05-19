


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

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "chemical.h"

typedef std::list< BindingSite* > BindingSiteList;
typedef std::map< int, BindingSiteList > ReferenceMap;

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
  //
  /**
   * @brief Default constructor
   */
  BoundChemical (void);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // BoundChemical (BoundChemical& other_bound_chemical);

  /**
   * @brief Destructor
   */
  virtual ~BoundChemical (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Actions to be taken when the chemical has collided with another
   * chemical.
   */
  void handle_collision (void);

  /**
   * @brief Adds a given quantity of chemical to the existing pool at a 
   *  specific binding site.
   * @param quantity The amount to add.
   * @param binding_site The binding site to which it bound.
   */
  void add ( int quantity, BindingSite& binding_site );

  /**
   * @brief Removes a random chemical bound to a specific binding site family.
   * @param binding_site_family 
   */
  void unbind_random ( int family_id );

  // ============================
  //  Public Methods - Accessors
  // ============================
  //


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
  virtual bool check_invariant (void);


private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Binding sites to which the chemical is bound (sorted by family). */
  ReferenceMap _binding_sites;

  // =================
  //  Private Methods
  // =================
  //

};

// ======================
//  Inline declarations
// ======================
//

#endif // BOUNDCHEMICAL_H
