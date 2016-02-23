

/**
 * @file freeendfactory.h
 * @brief Header for the FreeEndFactory class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef FREE_END_FACTORY_H
#define FREE_END_FACTORY_H

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
 * FreeEndFactory is a factory class representing free ends on a strand. It
 * automatically creates and registers a binding site on the opposite strand.
 */
class FreeEndFactory
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /** @brief Constructor. */
  FreeEndFactory (ChemicalSequence& location,
		  ChemicalSequence& opposite_strand,
		  BindingSiteFamily& left_family,
		  BindingSiteFamily& right_family);

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // FreeEndFactory (const FreeEndFactory& other);
  // /* @brief Assignment operator. */
  // FreeEndFactory& operator= (const FreeEndFactory& other);
  // /* @brief Destructor. */
  // ~FreeEndFactory (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Declare a free end on the left of a segment, generating a binding 
   *  site on the opposite strand.
   * @param position Position of first *unoccupied* base on strand carrying
   *  the free end (*not* opposite strand)
   */
  BindingSite* create_left (int position) const;

  /**
   * @brief Declare a free end on the right of a segment, generating a binding 
   *  site on the opposite strand.
   * @param position Position of first *unoccupied* base on strand carrying 
   *  the free end (*not* opposite strand)
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

  /** @brief Strand on which a binding site appears. */
  ChemicalSequence& _opposite;

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

#endif // FREE_END_FACTORY_H
