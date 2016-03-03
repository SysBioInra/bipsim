

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
#include <limits> // std::numeric_limits <int>::max()

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
 * @brief Class representing a segment of double strand sequence.
 *
 * Segment represents a part of a double strand sequence. It can be created,
 * extended and ligated to other segments. It automatically generates binding
 * sites on the opposite strand corresponding to its free ends (if applicable).
 */
class Segment
{
 public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /** 
   * @brief Constructor. 
   * @param first Initial starting position of segment.
   * @param last Initial ending position of segment.
   * @param handler FreeEndHandler used to generate binding sites on 
   *  opposite strand.
   */
  Segment (int first, int last, const FreeEndHandler& handler);

 private:
  // Forbidden
  /** @brief Copy constructor. */
  Segment (const Segment& other_segment);
  /** @brief Assignment operator. */
  Segment& operator= (const Segment& other_segment);
 public:

  /** @brief Destructor. */
  ~Segment (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Extend segment one base to the right. 
   */
  void extend_right (void);

  /**
   * @brief Extend segment one base to the left. 
   */
  void extend_left (void);

  /**
   * @brief Ligate (asorb) to another segment. 
   * @param other Other segment whose content will be absorbed (will be an 
   *  empty segment at the end of the operation).
   */
  void absorb_right (Segment& other);

  /**
   * @brief Change position of first base of the segment.
   * @param position New position of first base.
   */
  void set_first (int position);

  /**
   * @brief Change position of las base of the segment.
   * @param position New position of last base.
   */
  void set_last (int position);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /** @brief Position of first base of segment. */
  int first (void) const { return _first; }

  /** @brief Position of last base of segment. */
  int last (void) const { return _last; }
  
  // ==================
  //  Public Constants
  // ==================
  //
  /** 
   * @brief Position smaller than any other position. 
   *
   * If one end of segment is set to START, *no* free end is generated.
   */
  static const int START = -1;

  /** 
   * @brief Position greater than any other position. 
   *
   * If one end of segment is set to END, *no* free end is generated.
   */
  static const int END;

 private:
  // ============
  //  Attributes
  // ============
  //
  /** @brief First base spanned by the segment. */
  int _first;

  /** @brief Last base spanned by the segment. */
  int _last;  

  /** @brief Site associated with the free end on the right of the segment. */
  BindingSite* _right_site;

  /** @brief Site associated with the free end on the left of the segment. */
  BindingSite* _left_site;

  const FreeEndHandler& _handler;

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
