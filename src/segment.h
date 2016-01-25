

/**
 * @file segment.h
 * @brief Header for the Segment class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef SEGMENT_H
#define SEGMENT_H

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
 * @brief Class representing a segment of sequence.
 *
 * 
 */
class Segment
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /** @brief Constructor. */
  Segment (int first, int last) : first (first), last (last) {}

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // Segment (const Segment& other_segment);
  // /* @brief Assignment operator. */
  // Segment& operator= (const Segment& other_segment);
  // /* @brief Destructor. */
  // ~Segment (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //


  // ============================
  //  Public Methods - Accessors
  // ============================
  //

  // ===================
  //  Public Attributes
  // ===================
  //
  /** @brief First base spanned by the segment. */
  int first;

  /** @brief Last base spanned by the segment. */
  int last;

 private:
  // ============
  //  Attributes
  // ============
  //

  // =================
  //  Private Methods
  // =================
  //
};

// ======================
//  Inline declarations
// ======================
//

#endif // SEGMENT_H
