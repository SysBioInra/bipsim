

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
#include "config.h"
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
   * @param family Family the site belongs to.
   * @param location Chemical sequence containing the site.
   * @param first Relative first position of site.
   * @param last Relative last position of site.
   * @param k_on On rate of binding on the site.
   * @param k_off Off rate of unbinding on the site.
   * @param reading_frame Absolute position of the reading frame 
   *  (if applicable, NO_READING_FRAME by default).
   */
  BindingSite (BindingSiteFamily& family, ChemicalSequence& location, 
	       int first, int last, double k_on, double k_off,
	       int reading_frame = NO_READING_FRAME);

  
 private:
  /** @brief Copy constructor (forbidden). */
  BindingSite (BindingSite& other);
  /** @brief Assignment operator (forbidden). */
  BindingSite& operator= (BindingSite other);
 public:

  // /* @brief Destructor */
  // ~BindingSite (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Perform necessary actions when availability of the observed site 
   *  has changed.
   */
#ifndef PERFORM_TESTS
  void update (void);
#else
  virtual void update (void);
#endif

  /**
   * @brief Change update_id to pass along when update occurs.
   * @param new_id Positive integer representing new id to pass along.
   */
  void set_update_id (int new_id);

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
   * @brief Binding rate accessor.
   * @return Product of k_on and number of available sites.
   */
  double binding_rate (void) const;

  /**
   * @brief Reading frame accessor.
   * @return Reading frame on the binding site, BindingSite::NO_READING_FRAME 
   *  if there is none.
   */
  int reading_frame (void) const;
  
  // ==================
  //  Public Constants
  // ==================
  //
  /** @brief Constant value used to specify that there is no reading frame 
   *   associated with the binding site. */
  static const int NO_READING_FRAME = -1;

  /** @brief Default update identifier sent when an update occurs. */
  static const int DEFAULT_ID = -1;

 private:
  // =================
  //  Private Methods
  // =================
  //

  // ============
  //  Attributes
  // ============
  //
  /** @brief On-rate constant of the motif. */
  double _k_on;
  
  /** @brief Off-rate constant of the motif. */
  double _k_off;

  /** @brief Reading frame position (NO_READING_FRAME if there is none). */
  int _reading_frame;

  /** @brief Identifier to send when an update occurs. */
  int _update_id;
};

// ======================
//  Inline declarations
// ======================
//
#include "macros.h"
#include "bindingsitefamily.h"

inline void BindingSite::update (void)
{
  static_cast <BindingSiteFamily&> (Site::family()).update (_update_id);  
}
 
inline void BindingSite::set_update_id (int new_id)
{
  _update_id = new_id;
}

inline const BindingSiteFamily& BindingSite::family (void) const
{
  return static_cast <const BindingSiteFamily&> (Site::family());
}

inline double BindingSite::k_on (void) const
{
  return _k_on;
}

inline double BindingSite::k_off (void) const
{
  return _k_off;
}

inline double BindingSite::binding_rate (void) const
{
  return _k_on * number_available_sites();
}

inline int BindingSite::reading_frame (void) const
{
  return _reading_frame;
}


#endif // BINDINGSITE_H
