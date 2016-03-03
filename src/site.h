

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
#include "simulatorinput.h"
#include "macros.h"

/**
 * @brief Class that represents sites on chemical sequences.
 *
 * The Site class contains two types of information. Each instance
 * belongs to a family of sites (e.g. Ribosome Binding Site) but also
 * has a specific location along a specific chemical sequence.
 */
class Site : public SimulatorInput
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
   * @param first Relative position of first base.
   * @param last Relative position of last base.
   */
  Site (SiteFamily& family, ChemicalSequence& location, int first, int last);

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // Site (Site& other_site);
  // /* @brief Assignment operator. */
  // Site& operator= (Site& other_site);

  /** @brief Destructor. */
  virtual ~Site (void) {}

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Move site along the sequence.
   * @param step_size Step by which to move the binding site.
   */
  virtual void move (int step_size);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //  
  /**
   * @brief Family accessor.
   * @return Reference to family the site belongs to.
   */
  const SiteFamily& family (void) const;
 
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
   * @brief Accessor to number of available sites.
   * @return Number of sites currently available.
   */
  int number_available_sites (void) const;

  /**
   * @brief Check whether site overlaps with a given segment.
   * @param a Start of the segment to check against.
   * @param b End of the segment to check against.
   */
  bool overlaps (int a, int b) const;   

 protected:
  // ============
  //  Attributes
  // ============
  //
  /** @brief Family to which the site belongs */
  SiteFamily& _family;

  /** @brief Chemical on which the site is located. */
  ChemicalSequence& _location;

  /** @brief First position of the site along the sequence. */
  int _first;
  
  /** @brief Last position of site along the sequence. */
  int _last;

  // =================
  //  Private Methods
  // =================
  //
};

// ======================
//  Inline declarations
// ======================
//
inline const SiteFamily& Site::family (void) const
{
  return _family;
}

inline int Site::first (void) const
{
  return _first;
}

inline int Site::last (void) const
{
  return _last;
}

inline ChemicalSequence& Site::location (void) const
{
  return _location;
}

inline bool Site::overlaps (int a, int b) const
{
  /** @pre a must be smaller or equal to b. */
  REQUIRE (a <= b);
  return (((_first <= a) && (_last >= a)) || ((_first >= a) && (b >= _first)));
}

#endif // SITE_H
