

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
   * @param is_static Parameter specifying if the site remains at the same
   *  position on the sequence over time (true by default).
   */
  BindingSite (BindingSiteFamily& family, ChemicalSequence& location, 
	       int first, int last, double k_on, double k_off,
	       int reading_frame = NO_READING_FRAME,
	       bool is_static = true);

  
 private:
  /** @brief Copy constructor (forbidden). */
  BindingSite (BindingSite& other);
  /** @brief Assignment operator (forbidden). */
  BindingSite& operator= (BindingSite other);
 public:
  
  /** @brief Destructor */
  ~BindingSite (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // redefined from Site
  void move (int step_size);

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
   */
  virtual void update (void);

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

  /** @brief Last availability value notified. */
  int _last_availability;

  /** @brief Attribute storing whether site is static or not. */
  bool _static;

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
#include "bindingsitefamily.h"

inline void BindingSite::move (int step_size)
{
  /** @pre Instance must have been declared as non static. */
  REQUIRE (_static == false);

  Site::move (step_size);
  if (_reading_frame != NO_READING_FRAME) { _reading_frame += step_size; }
  update(); // availability may have changed

  /** @post Reading frame must stay within site limits. */
  ENSURE ((_reading_frame == NO_READING_FRAME) ||
	  ((_reading_frame >= first()) && (_reading_frame <= last())));
}		     

inline void BindingSite::set_update_id (int new_id)
{
  /** @pre new_id must be positive. */
  REQUIRE (new_id >= 0);

  _update_id = new_id;
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
