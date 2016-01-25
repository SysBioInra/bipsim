

/**
 * @file complexation.h
 * @authors Marc Dinh, Stephan Fischer
 * @brief Header for the Complexation class.
 * 
 */


// Multiple include protection
//
#ifndef COMPLEXATION_H
#define COMPLEXATION_H

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

/**
 * @brief This class represents complexation reactions.
 *
 * A complexation is defined by two chemical elements and an affinity between
 * them. They can combine into a new entity.
 */
class Complexation : public BidirectionalReaction
{
public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor
   * @param component_a Reference to the first chemical involved.
   * @param component_b Reference to the second chemical involved.
   * @param complex Reference to the complex.
   * @param k_on Association constant.
   * @param k_off Dissociation constant.
   */
  Complexation (Chemical& component_a, Chemical& component_b, 
		Chemical& complex, double k_on, double k_off);


 private:
  // Forbidden
  /** @brief Copy constructor */
  Complexation (Complexation& other_complexation);
  /** @brief Assignment operator */
  Complexation& operator= (Complexation& other_complexation);
 public:
  
  /** @brief Destructor */
  ~Complexation (void);

  // ===========================
  //  Public Methods - Commands
  // =========================== 
  //
  // redefined from Reaction
  void perform_forward (void);
  void perform_backward (void);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  // redefined from Reaction
  bool is_forward_reaction_possible (void) const;
  bool is_backward_reaction_possible (void) const;

 private:
  // ============
  //  Attributes
  // ============
  //
  /** @brief Implementation depending on presence of bound elements. */
  ComplexationImp* _imp;
  
  /** @brief on-rate of complexation reaction. */
  double _k_on;
  
  /** @brief off-rate of complexation reaction. */
  double _k_off;
  
  // =================
  //  Private Methods
  // =================
  //
  // redefined from Reaction
  double compute_forward_rate (void) const;
  double compute_backward_rate (void) const;
  void print (std::ostream& output) const;
};

// ======================
//  Inline declarations
// ======================
//


#endif // COMPLEXATION_H
