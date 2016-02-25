

/**
 * @file doublestrand.h
 * @brief Header for the DoubleStrand class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef DOUBLE_STRAND_H
#define DOUBLE_STRAND_H

// ==================
//  General Includes
// ==================
//


// ======================
//  Forward declarations
// ======================
//
#include "forwarddeclarations.h"

// ==================
//  Project Includes
// ==================
//
#include "chemical.h"
#include "bindingsitefamily.h"
#include "freeendfactory.h"

/**
 * @brief Class representing double strand sequences.
 *
 * A DoubleStrand appariates two ChemicalSequence. It creates factories for
 * BindingSite corresponding to free ends that are passed to each sequence.
 */
class DoubleStrand : public Chemical
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor.
   */
  DoubleStrand (ChemicalSequence& sense,
		ChemicalSequence& antisense);


  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // DoubleStrand (const DoubleStrand& other);
  // /* @brief Assignment operator. */
  // DoubleStrand& operator= (const DoubleStrand& other);
  // /* @brief Destructor. */
  // ~DoubleStrand (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // Redefined from Chemical
  void add (int number);
  void remove (int number);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //

private:
  // ============
  //  Attributes
  // ============
  //
  /** @brief Sense strand. */
  ChemicalSequence& _sense;

  /** @brief Antisense strand. */
  ChemicalSequence& _antisense;

  // order is important here
  /** @brief Family containing sites created by left free ends. */
  BindingSiteFamily _left_ends;
  
  /** @brief Family containing sites created by right free ends. */
  BindingSiteFamily _right_ends;

  /** @brief Factory used to create free ends properly on sense strand. */
  FreeEndFactory _sense_factory;

  /** @brief Factory used to create free ends properly on antisense strand. */
  FreeEndFactory _antisense_factory;

  // =================
  //  Private Methods
  // =================
  //
};

// ======================
//  Inline declarations
// ======================
//


#endif // DOUBLE_STRAND_H
