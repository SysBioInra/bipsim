
/**
 * @file chemicalreaction.h
 * @authors Marc Dinh, Stephan Fischer
 * @brief Header for the ChemicalReaction class.
 */

// Multiple include protection
//
#ifndef CHEMICALREACTION_H
#define CHEMICALREACTION_H

// ==================
//  General Includes
// ==================
//
#include <vector> // std::vector

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "bidirectionalreaction.h"

/**
 * @brief This class represents classical chemical reactions.
 *
 * A classical chemical reaction is defined by reactants, producs and their
 * stoichiometry. It usually involves covalent modifications and formation
 * of new elements, which distinguishes it from other reactions.
 */
class ChemicalReaction : public BidirectionalReaction
{
public:
  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor
   * @param components Vector of chemicals involved in the reaction.
   * @param stoichiometry Vector of stoichiometry of the chemicals, 
   *  positive for products, negative for reactants.
   * @param orders Vector of orders of the chemicals.
   * @param forward_rate_constant Forward rate constant.
   * @param backward_rate_constant Backward rate constant.
   * @param forward_bound BoundChemical reactant (if applicable).
   * @param backward_bound BoundChemical product (if applicable).
   */
  ChemicalReaction (const std::vector<FreeChemical*>& components,
		    const std::vector<int>& stoichiometry,
		    const std::vector<int>& orders,
		    double forward_rate_constant,
		    double backward_rate_constant,
		    BoundChemical* forward_bound = 0,
		    BoundChemical* backward_bound = 0);
  
  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor */
  // ChemicalReaction (ChemicalReaction& other_reaction);
  // /* @brief Assignment operator */
  // ChemicalReaction& operator= (ChemicalReaction& other_reaction);
  // /* @brief Destructor */
  // virtual ~ChemicalReaction (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // Redefinitions from BidirectionalReaction
  void perform_forward (void);
  void perform_backward (void);
  void handle_volume_change (double volume);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  // Redefinitions from BidirectionalReaction
  bool is_forward_reaction_possible (void) const;
  bool is_backward_reaction_possible (void) const;

 private:
  // =================
  //  Private Methods
  // =================
  //
  // redefinitions from BidirectionalReaction
  double compute_forward_rate (void) const;
  double compute_backward_rate (void) const;
  void print (std::ostream& output) const;

  static double contribution (int number, int order);
  
  // ============
  //  Attributes
  // ============
  //
  struct CRFree
  {
    FreeChemical* chemical;
    int stoichiometry;
    int order;
  };

  /** @brief Free forward reactants. */
  std::vector <CRFree> _free_forward;

  /** @brief Free backward reactants. */
  std::vector <CRFree> _free_backward;
  
  /** @brief Bound reactant of the reaction (0 if none). */
  BoundChemical* _bound_reactant;

  /** @brief Bound product of the reaction (0 if none). */
  BoundChemical* _bound_product;
  
  /** @brief Forward rate constant k_1. */
  double _k_1;

  /** @brief Forward rate constant k_-1. */
  double _k_m1;

  /** @brief Current forward rate including volume. */
  double _forward_constant;

  /** @brief Current backward rate including volume. */
  double _backward_constant;

  /** @brief Total order of forward reactants. */
  int _forward_order;

  /** @brief Total order of backward reactants. */
  int _backward_order;
};

// ======================
//  Inline declarations
// ======================
//
#include <cmath> // pow
#include "freechemical.h"

inline double 
ChemicalReaction::contribution (int number, int order)
{  
  double result = 1;
  order = number - order;
  while (number > order) { result *= number; --number; }
  return result;      
}

inline void ChemicalReaction::handle_volume_change (double volume)
{
  _forward_constant = _k_1 / pow (volume, _forward_order);
  _backward_constant = _k_m1 / pow (volume, _backward_order);
}

#endif // CHEMICALREACTION_H
