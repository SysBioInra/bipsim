

/**
 * @file chemicallist.h
 * @brief Header for the ChemicalList class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef CHEMICALLIST_H
#define CHEMICALLIST_H

// ==================
//  General Includes
// ==================
//
#include <string>

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations"

/**
 * @brief This class maintains a list of all chemicals.
 *
 * Every chemical is originally designated by name. This class attributes every
 * chemical an integer identifier and keeps a reference toward every chemical.
 */
class ChemicalList
{
public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor
   */
  ChemicalList (void);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // ChemicalList (ChemicalList& other_chemicallist);

  /**
   * @brief Destructor
   */
  virtual ~ChemicalList (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Adds a new chemical to the list.
   * @param chemical_name The name usually given to the chemical.
   */
  void create (string chemical_name);



  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @return True if the chemical is already in the list.
   * @param  chemical_name The name usually given to the chemical.
   */
  bool exists (string chemical_name);

  /**
   * @return The name of the chemical corresponding to chemical_id.
   * @param  chemical_id The identifier of the chemical in the list.
   */
  string name (int chemical_id);

  /**
   * @return A reference to the chemical corresponding to chemical_id.
   * @param  chemical_id The identifier of the chemical in the list.
   */
  Chemical* reference (int chemical_id);

  /**
   * @return The identifier of the chemical in the list (0 if it is not found).
   * @param  chemical_name The name usually given to the chemical
   */
  int id (string chemical_name);


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
  // ChemicalList& operator= (ChemicalList& other_chemicallist);


  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @return True if class invariant is preserved
   */
  virtual bool check_invariant (void);


private:

  // ============
  //  Attributes
  // ============
  //
  

  // =================
  //  Private Methods
  // =================
  //

};

// ======================
//  Inline declarations
// ======================
//


#endif // CHEMICALLIST_H
