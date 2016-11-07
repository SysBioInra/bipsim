
/**
 * @file release.h
 * @brief Header for the Release class.
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
#include "reaction.h"
#include "chemicalreaction.h"

// ===================
//  Class Declaration
// ===================
//
/**
 * @brief Class representing release reactions.
 * @details A release reaction is defined by the release of a 
 *  chemical that has been
 *  synthesizing a template. By releasing it, its product is also released.
 *  However, note that other components can take part in this reaction. In
 *  particular, the reaction should be specify in what form the main chemical is
 *  released (it can bea free form of the chemical, but it could also break down
 *  into several subparts).
 *  This class inherits class Reaction.
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
   * @param releasing_polymerase Chemical releasing a product.
   * @param empty_polymerase Chemical after releasing the product.
   * @param fail_polymerase Chemical "after release" in case release failed
   *  (because no product was found in the product table).
   * @param product_table Table listing molecules produced given binding 
   *  and release sites.
   * @param rate Reaction rate constant.
   */
  Release (BoundChemical& releasing_polymerase, 
	   BoundChemical& empty_polymerase,
	   BoundChemical& fail_polymerase,
	   const ProductTable& product_table, double rate);
    

  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // Release (Release& other_release);
  // /* @brief Assignment operator. */
  // Release& operator= (Release& other_release);
  // /* @brief Destructor */
  // ~Release (void);
  
  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  // Redefined from Reaction
  void handle_volume_change (double volume);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  // Redefined from Reaction
  bool is_reaction_possible (void) const;

 private:  
  // =================
  //  Private Methods
  // =================
  //
  // Redefined from Reaction.
  void do_reaction (void);
  double compute_rate (void) const;
  void print (std::ostream& output) const;

  // ============
  //  Attributes
  // ============
  //
  /** @brief Chemical yielding a product. */
  BoundChemical& _releasing_polymerase;
  /** @brief Empty chemical obtained after product has been yielded. */
  BoundChemical& _empty_polymerase;
  /** @brief Chemical obtained if release failed. */
  BoundChemical& _fail_polymerase;
  /** @brief Table of molecules that the released chemical may produce. */
  const ProductTable& _product_table;

  /** @brief Reaction rate. */
  double _rate;
  /** @brief Volume constant. */
  double _volume_constant;
};

// =====================
//  Inline Declarations
// =====================
//
inline void Release::handle_volume_change (double volume)
{
  // no volume dependency: _volume_constant = _rate;
}

#endif // RELEASE_H
