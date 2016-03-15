
/**
 * @file sequencebinding.h
 * @brief Header for the SequenceBinding class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef SEQUENCE_BINDING_H
#define SEQUENCE_BINDING_H

// ==================
//  General Includes
// ==================
//

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "bidirectionalreaction.h"
#include "familyfilter.h"

// ===================
//  Class Declaration
// ===================
//
/**
 * @brief Class for representing binding reactions.
 *
 * The SequenceBinding class can represent the binding of chemical elements onto
 * binding sites. A binding reaction is represented by an element to bind, the
 * same element in its bound form and the type of binding sites it can bind
 * onto.
 * This class inherits class Reaction.
 */
class SequenceBinding: public BidirectionalReaction
{
public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor
   * @param unit_to_bind FreeChemical that can potentially bind to a 
   *  binding site.
   * @param binding_result A bound chemical element that corresponds to the 
   *  original chemical in its bound form.
   * @param binding_site_family The family of binding sites the chemical 
   *  can bind onto.
   */
  SequenceBinding (FreeChemical& unit_to_bind, 
		   BoundChemical& binding_result,
		   BindingSiteFamily& binding_site_family);
    

 private:
  // Forbidden
  /** @brief Copy constructor. */
  SequenceBinding (SequenceBinding& other_sequence_binding);
  /** @brief Assignment operator. */
  SequenceBinding& operator= (SequenceBinding& other_sequence_binding);
 public:
  
  /** @brief Destructor. */
  ~SequenceBinding (void);
  
  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  //
  // Redefined from BidirectionReaction
  void perform_forward (void);
  void perform_backward (void);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  // Redefined from BidirectionReaction
  bool is_forward_reaction_possible (void) const;
  bool is_backward_reaction_possible (void) const;

 private:
  // =================
  //  Private Methods
  // =================
  //
  // Redefined from BidirectionReaction
  double compute_forward_rate (void) const;
  double compute_backward_rate (void) const;
  void print (std::ostream& output) const;

  // ============
  //  Attributes
  // ============
  //
  /** @brief FreeChemical that can bind to a binding site. */
  FreeChemical& _unit_to_bind;
  
  /** @brief Corresponding BoundChemical (bound form of unit to bind). */
  BoundChemical& _binding_result;
  
  /** @brief Binding sites the chemical can bind onto. */
  BindingSiteFamily& _family;

  /** @brief Filter used to select units that bound to correct family. */
  FamilyFilter _family_filter;
};

// =====================
//  Inline Declarations
// =====================
//

#endif // BINDING_H
