
/**
 * @file release.h
 * @brief Header for the Release class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef RELEASE_H
#define RELEASE_H

// ==================
//  General Includes
// ==================
//

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "chemicalreaction.h"

// ===================
//  Class Declaration
// ===================
//
/**
 * @brief Class for representing release reactions.
 *
 * A release reaction is defined by the release of a chemical that has been
 * synthesizing a template. By releasing it, its product is also released.
 * However, note that other components can take part in this reaction. In
 * particular, the reaction should be specify in what form the main chemical is
 * released (it can bea free form of the chemical, but it could also break down
 * into several subparts).
 * This class inherits class Reaction.
 * @sa Reaction
 */
class Release: public Reaction
{
public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor
   * @param unit_to_release Unit that is released from its template.
   * @param other_components
   *  Vector of other chemicals involved in the reaction.
   * @param stoichiometry
   *  Vector of stoichiometry of the other chemicals, positive for products,
   *  negative for reactants.
   * @param rate Reaction rate constant.
   * @param product_table If bound chemical releases a product, table listing
   *  molecules it may produce depending on its binding and release sites.
   * @sa ChemicalReaction
   */
  Release (BoundChemical& unit_to_release,
	   std::vector<Chemical*>& other_components,
	   std::vector<int>& stoichiometry, double rate,
	   ProductTable* product_table = 0);
    

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // Release (Release& other_release);

  /**
   * @brief Destructor
   */
  virtual ~Release (void);
  
  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  //

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Returns whether there are enough chemicals to perform reaction.
   * @return True if there are enough reactant, false otherwise.
   */
  virtual bool is_reaction_possible (void) const;


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
  // Release& operator= (Release& other_release);


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
  /** @brief Chemical element to release. */
  BoundChemical& _unit_to_release;

  /** @brief Side reaction representing other components that are involved in the release other than the unit to release. */
  ChemicalReaction _side_reaction;
  
  /** @brief Table of molecules that the released chemical may have produced. */
  ProductTable* _product_table;
  
  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Update chemical quantities according to the reaction.
   */
  virtual void do_reaction (void);
  
  /**
   * @brief Compute current reaction rate.
   * @return Current reaction rate.
   */
  virtual double compute_rate (void) const;

  /**
   * @brief Print class content.
   * @param output Stream where output should be written.
   */
  virtual void print (std::ostream& output) const;
};

// =====================
//  Inline Declarations
// =====================
//

#endif // RELEASE_H
