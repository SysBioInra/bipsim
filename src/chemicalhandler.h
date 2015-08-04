

/**
 * @file chemicalhandler.h
 * @brief Header for the ChemicalHandler class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef CHEMICALHANDLER_H
#define CHEMICALHANDLER_H

// ==================
//  General Includes
// ==================
//
#include <string> // std::string
#include <map> // std::map
#include <list> // std::list
#include <iostream>

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "identifiedlist.h"
#include "macros.h" // REQUIRE

/**
 * @brief This class maintains a list of all chemicals.
 *
 * Every chemical is originally designated by name. This class attributes every
 * chemical an integer identifier and keeps a reference toward every chemical.
 */
class ChemicalHandler
{
public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  ChemicalHandler (void);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // ChemicalHandler (ChemicalHandler& other_chemical_handler);

  /**
   * @brief Destructor
   */
  virtual ~ChemicalHandler (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Add new chemical to the list.
   * @param chemical_name Name usually given to the chemical.
   * @param initial_quantity Number of molecules present at beginning of simulation.
   */
  void create_chemical (std::string chemical_name, int initial_quantity = 0);

  /**
   * @brief Add new bound chemical to the list.
   * @param chemical_name Name usually given to the chemical.
   * @param initial_quantity Number of molecules present at beginning of simulation.
   */
  void create_bound_chemical (std::string chemical_name, int initial_quantity = 0);

  /**
   * @brief Add new base_loader to the list.
   * @param chemical_name Name usually given to the chemical.
   * @param decoding_table Table used by the BaseLoader to match chemicals with templates.
   * @param initial_quantity Number of molecules present at beginning of simulation.
   */
  void create_base_loader (std::string chemical_name, const DecodingTable& decoding_table, int initial_quantity = 0);

  /**
   * @brief Add new processive chemical to the list.
   * @param chemical_name Name usually given to the chemical.
   * @param stalled_name Name given to stalled form of the chemical.
   * @param initial_quantity Number of molecules present at beginning of simulation.
   */
  void create_processive_chemical (std::string chemical_name, std::string stalled_name, int initial_quantity = 0);

  /**
   * @brief Add new processive chemical to the list.
   * @param chemical_name Name usually given to the chemical.
   * @param sequence Sequence of the chemical.
   * @param initial_quantity Number of molecules present at beginning of simulation.
   */
  void create_chemical_sequence (std::string chemical_name, const std::string& sequence, int initial_quantity = 0);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Return wether chemical name is already known.
   * @return True if the chemical is already in the list of known
   *  chemicals.
   * @param  chemical_name The name usually given to the chemical.
   */
  bool exists (const std::string& chemical_name) const;

  /**
   * @brief Return name associated with chemical identifier.
   * @return Name of the chemical corresponding to chemical_id.
   *  Empty if chemical_id is unknown.
   * @param  chemical_id Identifier of the chemical in the list.
   */
  std::string name (int chemical_id) const;

  /**
   * @brief Return reference to chemical associated with chemical
   *  identifier.
   * @return Reference to the chemical corresponding to chemical_id.
   * @param chemical_id Identifier of the chemical in the list.
   */
  Chemical& reference (int chemical_id) const;

  /**
   * @brief Return reference to chemical associated with chemical
   *  name.
   * @return Reference to the chemical corresponding to chemical_name.
   * @param chemical_name Name of chemical.
   */
  Chemical& reference (const std::string& chemical_name) const;

  /**
   * @brief Return identifier associated with chemical name.
   * @return Integer identifier of the chemical
   *  (ChemicalHandler::NOT_FOUND if it is not found).
   * @param  chemical_name The name usually given to the chemical
   */
  int id (const std::string& chemical_name) const;

  /**
   * @brief Return last identifier created.
   * @return Identifier corresponding to last entity created.
   */
  int last_id_created (void) const;

  /**
   * @brief Return last reference created.
   * @return Reference to last entity created.
   */
  Chemical& last_reference_created (void) const;


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
  // ChemicalHandler& operator= (ChemicalHandler& other_chemical_handler);

  /**
   * @brief Standard output.
   * @return A reference to the stream containing the output.
   * @param output Stream where output should be written.
   * @param chemical_handler Reference to the chemical handler whose information should be written.
   */
  friend std::ostream& operator<< (std::ostream& output,const ChemicalHandler& chemical_handler);

  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @return True if class invariant is preserved
   */
  virtual bool check_invariant (void) const;

  // ==================
  //  Public Constants
  // ==================
  //
  /**
   * @brief Standard identifier for unknown chemicals.
   */
  static const int NOT_FOUND = IdentifiedList::NOT_FOUND;


private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief List of identifiers attributes to chemicals. */
  IdentifiedList _identifiers;

  /** @brief Map of chemical references. */
  std::map< int, Chemical* > _references;

  /** @brief Identifer of the last entity created. */
  int _last_id;

  /** @brief Reference to last entity created. */
  Chemical* _last_reference;

  // =================
  //  Private Methods
  // =================
  //

};

// ======================
//  Inline declarations
// ======================
//
inline bool ChemicalHandler::exists (const std::string& chemical_name) const
{
  return _identifiers.exists (chemical_name);
}

inline std::string ChemicalHandler::name (int chemical_id) const
{
  return _identifiers.name (chemical_id);
}

inline Chemical& ChemicalHandler::reference (int chemical_id) const
{
  std::map< int, Chemical* >::const_iterator ref = _references.find ( chemical_id );
  REQUIRE( ref != _references.end() ); /** @pre chemical_id is already defined */
  return *(ref->second);
}

inline Chemical& ChemicalHandler::reference (const std::string& chemical_name) const
{
  REQUIRE( exists (chemical_name) ); /** @pre Chemical name is already known. */
  return reference (id (chemical_name));
}


inline int ChemicalHandler::id (const std::string& chemical_name) const
{
  return _identifiers.id (chemical_name);
}

inline int ChemicalHandler::last_id_created (void) const
{
  return _last_id;
}

inline Chemical& ChemicalHandler::last_reference_created (void) const
{
  return *_last_reference;
}



#endif // CHEMICALHANDLER_H
