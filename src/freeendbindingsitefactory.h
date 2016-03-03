

/**
 * @file freeendbindingsitefactory.h
 * @brief Header for the FreeEndBindingSiteFactory class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef FREE_END_BINDING_SITE_FACTORY_H
#define FREE_END_BINDING_SITE_FACTORY_H

// ==================
//  General Includes
// ==================
//


// ==================
//  Project Includes
// ==================
//


// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"

/**
 * @brief Class used to create binding sites due to free ends on double strands.
 *
 * FreeEndBindingSiteFactory is a factory class creating binding sites
 * due to a free end on the opposite strand.
 */
class FreeEndBindingSiteFactory
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /** @brief Constructor. */
  FreeEndBindingSiteFactory (ChemicalSequence& location,
			     BindingSiteFamily& left_family,
			     BindingSiteFamily& right_family);

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // FreeEndBindingSiteFactory (const FreeEndBindingSiteFactory& other);
  // /* @brief Assignment operator. */
  // FreeEndBindingSiteFactory& operator= (const FreeEndBindingSiteFactory& other);
  // /* @brief Destructor. */
  // ~FreeEndBindingSiteFactory (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Create binding site due to a left free end on opposite strand.
   * @param position Position of binding site.
   *
   * Everything is expressed from the point of view of the strand carrying the
   * binding site. Left means the beginning of the appariated segment even
   * though from the point of view of the appariated strand it is going to
   * be the end (i.e. the right end) of the segment.
   */
  BindingSite* create_left (int position) const;

  /**
   * @brief Create binding site due to a right free end on opposite strand.
   * @param position Position of binding site.
   *
   * Everything is expressed from the point of view of the strand carrying the
   * binding site. Right means the end of the appariated segment even
   * though from the point of view of the appariated strand it is going to
   * be the beginnig (i.e. the left end) of the segment.
   */
  BindingSite* create_right (int position) const;

  // ============================
  //  Public Methods - Accessors
  // ============================
  //


private:
  // ============
  //  Attributes
  // ============
  //
  /** @brief Strand on which the free end is. */
  ChemicalSequence& _location;

  /** @brief Family to which left free end binding sites belong. */
  BindingSiteFamily& _left_family;

  /** @brief Family to which riqht free end binding sites belong. */
  BindingSiteFamily& _right_family;

  // =================
  //  Private Methods
  // =================
  //
};

// ======================
//  Inline declarations
// ======================
//

#endif // FREE_END_BINDING_SITE_FACTORY_H
