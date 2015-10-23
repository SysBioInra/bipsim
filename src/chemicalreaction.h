


/**
 * @file chemicalreaction.h
 * @authors Marc Dinh, Stephan Fischer
 * @brief Header for the ChemicalReaction class.
 * 
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
   * @param components
   *  Vector of chemicals involved in the reaction.
   * @param stoichiometry
   *  Vector of stoichiometry of the chemicals, positive for products, negative for
   *  reactants.
   * @param forward_rate_constant Forward rate constant.
   * @param backward_rate_constant Backward rate constant.
   */
  ChemicalReaction (std::vector<Chemical*>& components,
		    std::vector<int>& stoichiometry,
		    double forward_rate_constant,
		    double backward_rate_constant);
  
  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // ChemicalReaction (ChemicalReaction& other_chemicalreaction);

  /**
   * @brief Destructor
   */
  virtual ~ChemicalReaction (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Update chemical quantities according to the forward reaction.
   */
  virtual void perform_forward (void);
  
  /**
   * @brief Update chemical quantities according to the backward reaction.
   */
  virtual void perform_backward (void);


  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /** @brief Check whether there are enough reactants to perform forwardreaction. */
  virtual bool is_forward_reaction_possible (void) const;

  /** @brief Check whether there are enough products to perform backward reaction. */
  virtual bool is_backward_reaction_possible (void) const;


  // ==========================
  //  Public Methods - Setters
  // ==========================
  //


  // =======================================
  //  Public Methods - Operator overloading
  // =======================================
  //
  // Not needed for this class (use of default overloading) !
  // /*
  //  * @brief Assignment operator
  //  */
  // ChemicalReaction& operator= (ChemicalReaction& other_chemicalreaction);

 protected:
  // ===================
  //  Protected Methods
  // ===================
  //

 private:
  // ============
  //  Attributes
  // ============
  //
  /** @brief Stoichiometry of the forward reactants. */
  std::vector <int> _forward_stoichiometry;

  /** @brief Stoichiometry of the backward reactants. */
  std::vector <int> _backward_stoichiometry;  

  /** @brief Number of free chemicals among forward reactants. */
  int _free_reactant_number;

  /** @brief Number of free chemicals among forward reactants. */
  int _free_product_number;
  
  /** @brief Bound reactant of the reaction (0 if none). */
  BoundChemical* _bound_reactant;

  /** @brief Bound product of the reaction (0 if none). */
  BoundChemical* _bound_product;
  
  /** @brief Forward rate constant k_1. */
  double _k_1;

  /** @brief Forward rate constant k_-1. */
  double _k_m1;

  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Compute current forward rate.
   * @return Current forward rate.
   */
  virtual double compute_forward_rate (void) const;

  /**
   * @brief Compute current backward rate.
   * @return Current backward rate.
   */
  virtual double compute_backward_rate (void) const;

  /**
   * @brief Print class content.
   * @param output Stream where output should be written.
   */
  virtual void print (std::ostream& output) const;

  /**
   * @brief Looks for bound chemicals in the reaction and isolates them.
   *
   * If no bound chemical is found, pointers to bound elements will be null. If
   * some are found the routine performs checks (no more than 1 bound
   * product/reactant, if there is a bound product there needs to be a bound
   * reactant and vice versa, the stoichiometry coefficient has to be 1). Bound
   * elements are moved to the end of the forward/backward_reactants vectors
   * for simplicity.
   */
  void isolate_bound_components (void);

  /**
   * @brief Cast forward reactant to chemical.
   * @param index Index of the reactant.
   * @return Reactant cast to pointer to Chemical.
   */
  Chemical* forward_chemical (int index);

  /**
   * @brief Cast backward reactant to chemical.
   * @param index Index of the reactant.
   * @return Reactant cast to pointer to Chemical.
   */
  Chemical* backward_chemical (int index);

  /**
   * @brief Cast forward reactant to chemical.
   * @param index Index of the reactant.
   * @return Reactant cast to pointer to Chemical.
   */
  const Chemical* forward_chemical (int index) const;

  /**
   * @brief Cast backward reactant to chemical.
   * @param index Index of the reactant.
   * @return Reactant cast to pointer to Chemical.
   */
  const Chemical* backward_chemical (int index) const;
};

// ======================
//  Inline declarations
// ======================
//
#include "chemical.h"

inline Chemical* ChemicalReaction::forward_chemical (int index) 
{
  return static_cast <Chemical*> (_forward_reactants [index]);
}

inline Chemical* ChemicalReaction::backward_chemical (int index) 
{
  return static_cast <Chemical*> (_backward_reactants [index]);
}

inline const Chemical* ChemicalReaction::forward_chemical (int index) const
{
  return static_cast <const Chemical*> (_forward_reactants [index]);
}

inline const Chemical* ChemicalReaction::backward_chemical (int index) const
{
  return static_cast <const Chemical*> (_backward_reactants [index]);
}

#endif // CHEMICALREACTION_H
