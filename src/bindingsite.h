
/**
 * @file bindingsite.h
 * @brief Header for the BindingSite class.
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
#include "simulatorinput.h"

/**
 * @brief Class that represents binding sites on chemical sequences.
 * @details The BindingSite class contains two types of information. 
 * Each instance
 * belongs to a family of binding sites (e.g. Ribosome Binding Site) but also
 * has a specific location along a specific chemical sequence.
 */
class BindingSite : public SimulatorInput
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
   *  (NO_READING_FRAME by default). If NO_READING_FRAME is provided,
   *  reading frame is arbitrarily placed on first base of site.
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
  /**
   * @brief Site location.
   * @return ChemicalSequence that carries the site.
   */
  ChemicalSequence& location (void) const;

  /**
   * @brief Accessor to starting position on sequence.
   * @return First position of site on sequence.
   */
  int first (void) const;
 
  /**
   * @brief Accessor to ending position on sequence.
   * @return Last position of site on sequence.
   */
  int last (void) const;   

  /**
   * @brief Reading frame accessor.
   * @return Reading frame on the binding site, BindingSite::NO_READING_FRAME 
   *  if there is none.
   */
  int reading_frame (void) const;

  /**
   * @brief Family accessor.
   * @return Reference to BindingSiteFamily the site belongs to.
   */
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
   * @brief Accessor to number of available sites.
   * @return Number of sites currently available.
   */
  int number_available_sites (void) const;

  /**
   * @brief Check whether site overlaps with a given segment.
   * @param a Start of the segment to check against.
   * @param b End of the segment to check against.
   * @return True if segment overlaps site.
   */
  bool overlaps (int a, int b) const;   
  
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
  /** @brief Chemical on which the site is located. */
  ChemicalSequence& _location;
  /** @brief First position of the site along the sequence. */
  int _first;
  /** @brief Last position of site along the sequence. */
  int _last;
  /** @brief Reading frame position (NO_READING_FRAME if there is none). */
  int _reading_frame;

  /** @brief Family to which the site belongs */
  BindingSiteFamily& _family;
  /** @brief On-rate constant of the motif. */
  double _k_on;
  /** @brief Off-rate constant of the motif. */
  double _k_off;

  /** @brief Identifier to send when an update occurs. */
  int _update_id;
};

// ======================
//  Inline declarations
// ======================
//
#include "macros.h"
#include "bindingsitefamily.h"
#include "chemicalsequence.h"

inline ChemicalSequence& BindingSite::location (void) const
{
  return _location;
}

inline int BindingSite::first (void) const
{
  return _first;
}

inline int BindingSite::last (void) const
{
  return _last;
}

inline int BindingSite::reading_frame (void) const
{
  return _reading_frame;
}

inline const BindingSiteFamily& BindingSite::family (void) const
{
  return _family;
}

inline double BindingSite::k_on (void) const
{
  return _k_on;
}

inline double BindingSite::k_off (void) const
{
  return _k_off;
}

inline bool BindingSite::overlaps (int a, int b) const
{
  /** @pre a must be smaller or equal to b. */
  REQUIRE (a <= b);
  return (((_first <= a) && (_last >= a)) || ((_first >= a) && (b >= _first)));
}

inline int BindingSite::number_available_sites (void) const
{
  return _location.number_available_sites (_first, _last);
}

inline void BindingSite::update (void)
{
  _family.update (_update_id);  
}
 
inline void BindingSite::set_update_id (int new_id)
{
  _update_id = new_id;

}

inline double BindingSite::binding_rate (void) const
{
  return _k_on * number_available_sites();
}


#endif // BINDINGSITE_H
