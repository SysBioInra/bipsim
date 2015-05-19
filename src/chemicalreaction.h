


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
#include <map>

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations"

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
   * @brief Default constructor
   */
  ChemicalReaction (void);

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
   * @brief Adds a component to the reaction.
   * @param  chemical
   *  A chemical, product or reactant, involved in the reaction.
   * @param  stoichiometry
   *  The stoichiometry of the chemical, positive for products, negative for
   *  reactants.
   */
  void add_chemical (Chemical& chemical, int stoichiometry);


  // ============================
  //  Public Methods - Accessors
  // ============================
  //


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
   * @return True if class invariant is preserved
   */
  bool check_invariant (void);


private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief */
  std::map<Chemical*,int> _reaction_components;

  // =================
  //  Private Methods
  // =================
  //

};

// ======================
//  Inline declarations
// ======================
//


#endif // CHEMICALREACTION_H
