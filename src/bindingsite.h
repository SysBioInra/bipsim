

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
#include "chemicalsequence.h"
#include "macros.h"
#include "site.h"

/**
 * @brief Class that represents binding sites on chemical sequences.
 *
 * The BindingSite class contains two types of information. Each instance
 * belongs to a family of binding sites (e.g. Ribosome Binding Site) but also
 * has a specific location along a specific chemical sequence.
 */
class BindingSite : public Site
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
   * @param length Length of the binding site.
   * @param k_on On rate of binding on the site.
   * @param k_off Off rate of unbinding on the site.
   * @param reading_frame Position of the reading frame (if applicable).
   * @sa BindingSiteHandler
   */
  BindingSite (int family_id, ChemicalSequence& location, int position,
	       int length, double k_on, double k_off,
	       int reading_frame = NO_READING_FRAME );

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
  void bind_unit (BoundChemical& unit_to_bind) const;

  /**
   * @brief Unbind a chemical at the binding site.
   * @param unit_to_unbind Chemical to unbind.
   */
  void unbind_unit (BoundChemical& unit_to_unbind) const;

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief k_on accessor.
   * @return on-rate constant of the binding site.
   */
  double k_on ( void ) const;

  /**
   * @brief k_off accessor.
   * @return off-rate constant of the binding site.
   */
  double k_off ( void ) const;

  /**
   * @brief Reading frame accessor.
   * @return Reading frame on the binding site, BindingSite::NO_READING_FRAME if there is none.
   */
  int reading_frame ( void ) const;

  /**
   * @brief Binding rate contributon.
   * @return Contribution to binding rate coming from this site.
   */
  double rate_contribution (void) const;
  

  
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
  // CLASSNAME& operator= (CLASSNAME& other_binding);


  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @brief Checks the class invariant.
   * @return True if class invariant is preserved
   */
  virtual bool check_invariant (void) const;
  
  // ==================
  //  Public Constants
  // ==================
  //
  /** @brief Constant value used to specify that there is no reading frame associated with the binding site. */
  static const int NO_READING_FRAME = -1;

 private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief on-rate constant of the motif. */
  double _k_on;
  
  /** @brief off-rate constant of the motif. */
  double _k_off;

  /** @brief Reading frame position (NO_READING_FRAME if there is none). */
  int _reading_frame;

  // =================
  //  Private Methods
  // =================
  //

};

// ======================
//  Inline declarations
// ======================
//
inline double BindingSite::k_on ( void ) const
{
  return _k_on;
}

inline double BindingSite::k_off ( void ) const
{
  return _k_off;
}

inline int BindingSite::reading_frame ( void ) const
{
  return _reading_frame;
}



#endif // BINDINGSITE_H
