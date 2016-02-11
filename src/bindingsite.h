

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
#include "macros.h"
#include "site.h"
#include "bindingsitefamily.h"

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
   * @param family Family the site belongs to.
   * @param location Chemical sequence containing the site.
   * @param first First position of site.
   * @param last Last position of site.
   * @param k_on On rate of binding on the site.
   * @param k_off Off rate of unbinding on the site.
   * @param reading_frame Position of the reading frame (if applicable).
   */
  BindingSite (BindingSiteFamily& family, ChemicalSequence& location, 
	       int first, int last, double k_on, double k_off,
	       int reading_frame = NO_READING_FRAME );

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor */
  // BindingSite (BindingSite& other_binding_site);
  // /* @brief Assignment operator */
  // BindingSite& operator= (BindingSite other_binding_site);
  // /* @brief Destructor */
  // ~BindingSite (void);

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

  /**
   * @brief Perform necessary actions when availability of the observed site 
   *  has changed.
   * @param number_available_sites Number of sites currently available.
   */
  void update (int number_available_sites);

  /**
   * @brief Change message to pass along when update occurs.
   * @param new_message Positive integer representing new message to pass along.
   */
  void set_update_message (int new_message);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  // Redefined from Site
  const BindingSiteFamily& family (void) const;

  /**
   * @brief k_on accessor.
   * @return on-rate constant of the binding site.
   */
  double k_on (void) const;

  /**
   * @brief k_off accessor.
   * @return off-rate constant of the binding site.
   */
  double k_off (void) const;

  /**
   * @brief Reading frame accessor.
   * @return Reading frame on the binding site, BindingSite::NO_READING_FRAME if there is none.
   */
  int reading_frame (void) const;
  
  // ==================
  //  Public Constants
  // ==================
  //
  /** @brief Constant value used to specify that there is no reading frame 
   *   associated with the binding site. */
  static const int NO_READING_FRAME = -1;

  /** @brief Default message sent when an update occurs. */
  static const int DEFAULT_MESSAGE = -1;

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

  /** @brief Message to send when an update occurs. */
  int _message;

  // =================
  //  Private Methods
  // =================
  //
};

// ======================
//  Inline declarations
// ======================
//
#include "macros.h"
#include "chemicalsequence.h"

inline BindingSite::BindingSite (BindingSiteFamily& family, 
				 ChemicalSequence& location, 
				 int first, int last, 
				 double k_on, double k_off,
				 int reading_frame /*= NO_READING_FRAME*/)
  : Site (family, location, first, last)
  , _k_on (k_on)
  , _k_off (k_off)
  , _reading_frame (reading_frame)
{
  /** @pre If defined, reading frame must be within site. */
  REQUIRE ((reading_frame == NO_READING_FRAME)
	   || ((reading_frame >= first) && (reading_frame <= last)));

  // add binding site to family
  static_cast <BindingSiteFamily&> (_family).add (this);

  // demand availability notifications
  location.notify_site_availability (*this);
}

inline void BindingSite::update (int number_of_available_sites)
{
  /** @pre Number of available sites must be positive); */
  REQUIRE (number_of_available_sites >= 0);
  
  static_cast <BindingSiteFamily&> (_family).
    update (_message, _k_on * number_of_available_sites);
}

inline void BindingSite::set_update_message (int new_message)
{
  /** @pre new_message must be positive. */
  REQUIRE (new_message >= 0);

  _message = new_message;
}

inline const BindingSiteFamily& BindingSite::family (void) const
{
  return static_cast <const BindingSiteFamily&> (_family);
}

inline double BindingSite::k_on (void) const
{
  return _k_on;
}

inline double BindingSite::k_off (void) const
{
  return _k_off;
}

inline int BindingSite::reading_frame (void) const
{
  return _reading_frame;
}


#endif // BINDINGSITE_H
