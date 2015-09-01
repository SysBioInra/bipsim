


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
#include "reaction.h"

/**
 * @brief This class represents classical chemical reactions.
 *
 * A classical chemical reaction is defined by reactants, producs and their
 * stoichiometry. It usually involves covalent modifications and formation
 * of new elements, which distinguishes it from other reactions.
 */
class ChemicalReaction : public Reaction
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
		    std::vector<int>& stoichiometry, double forward_rate_constant,
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
   * @brief Update reaction rates.
   */
  virtual void update_rates ( void );


  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Print class content.
   * @param output Stream where output should be written.
   */
  virtual void print (std::ostream& output) const;


  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /** @brief Check whether there are enough reactants to perform reaction. */
  virtual bool is_forward_reaction_possible (void) const;

  /** @brief Check whether there are enough products to perform backwards reaction. */
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


  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @brief Check class invariant.
   * @return True if class invariant is preserved
   */
  bool check_invariant (void) const;

 protected:
  // ===================
  //  Protected Methods
  // ===================
  //
  /**
   * @brief Update chemical quantities according to the forward reaction.
   */
  virtual void do_forward_reaction (void);
  
  /**
   * @brief Update chemical quantities according to the backward reaction.
   */
  virtual void do_backward_reaction (void);


 private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Number of components of the reaction. */
  int _number_components;

  /** @brief Components of the reaction. */
  std::vector<Chemical*> _component_vector;
  
  /** @brief Stoichiometry of the reaction. */
  std::vector<int> _stoichiometry;

  /** @brief Index of the bound product of the reaction (_number_components if none). */
  int _bound_product_index;
  
  /** @brief Index of the bound reactant of the reaction (_number_components if none). */
  int _bound_reactant_index;

  /** @brief Forward rate constant k_1. */
  double _k_1;

  /** @brief Forward rate constant k_-1. */
  double _k_m1;

  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Looks for bound chemicals in the reaction and updates corresponding indices.
   *
   * If no bound chemical is found, the indices for the bound reactant and product will be
   * set to _number_components for algorithmic reasons. The routine also performs some other
   * checks (no more than 1 bound product/reactant, if there is a bound product there needs
   * to be a bound reactant and vice versa, the stoichiometry coefficient has to be 1).
   */
  void compute_bound_component_indices ( void );

};

// ======================
//  Inline declarations
// ======================
//


#endif // CHEMICALREACTION_H
