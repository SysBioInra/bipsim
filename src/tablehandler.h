

/**
 * @file tablehandler.h
 * @brief Header for the TableHandler class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef TABLEHANDLER_H
#define TABLEHANDLER_H

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
 * @brief This class maintains a list of all tables.
 *
 * Every table is originally designated by name. This class attributes every
 * table an integer identifier and keeps a reference toward every table.
 */
class TableHandler
{
public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  TableHandler (void);

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // TableHandler (TableHandler& other_table_handler);

  /**
   * @brief Destructor
   */
  virtual ~TableHandler (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Add new decoding_table to the list.
   * @param table_name Name usually given to the decoding table.
   * @param template_list List of motifs that can be read on a sequence.
   * @param base_list Chemicals that are loaded corresponding to each template.
   * @param polymerase_list Occupied polymerases corresponding to each template.
   * @param initial_quantity Number of molecules present at beginning of simulation.
   * @sa DecodingTable
   */
  void create_decoding_table (std::string table_name, const std::list<std::string>& template_list,
			      const std::list<Chemical*>& base_list, const std::list<BoundChemical*>& polymerase_list,
			      const std::list<double>& rate_list);

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Return wether table name is already known.
   * @return True if the table is already in the list of known
   *  tables.
   * @param  table_name The name usually given to the table.
   */
  bool exists (std::string table_name) const;

  /**
   * @brief Return name associated with table identifier.
   * @return Name of the table corresponding to table_id.
   *  Empty if table_id is unknown.
   * @param  table_id Identifier of the table in the list.
   */
  std::string name (int table_id) const;

  /**
   * @brief Return reference to table associated with table
   *  identifier.
   * @return Reference to the table corresponding to table_id.
   * @param table_id Identifier of the table in the list.
   */
  DecodingTable& reference (int table_id) const;

  /**
   * @brief Return reference to table associated with table
   *  name.
   * @return Reference to the table corresponding to table_name.
   * @param table_name Name of table.
   */
  DecodingTable& reference (std::string table_name) const;

  /**
   * @brief Return identifier associated with table name.
   * @return Integer identifier of the table
   *  (TableHandler::NOT_FOUND if it is not found).
   * @param  table_name The name usually given to the table
   */
  int id (std::string table_name) const;

  /**
   * @brief Return last identifier created.
   * @return Identifier corresponding to last entity created.
   */
  int last_id_created (void) const;

  /**
   * @brief Return last reference created.
   * @return Reference to last entity created.
   */
  DecodingTable& last_reference_created (void) const;


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
  // TableHandler& operator= (TableHandler& other_table_handler);

  /**
   * @brief Standard output.
   * @return A reference to the stream containing the output.
   * @param output Stream where output should be written.
   * @param table_handler Reference to the table handler whose information should be written.
   */
  friend std::ostream& operator<< (std::ostream& output, const TableHandler& table_handler);

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
   * @brief Standard identifier for unknown tables.
   */
  static const int NOT_FOUND = IdentifiedList::NOT_FOUND;


private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief List of identifiers attributed to tables. */
  IdentifiedList _identifiers;

  /** @brief Map of table references. */
  std::map< int, DecodingTable* > _references;

  /** @brief Identifer of the last entity created. */
  int _last_id;

  /** @brief Reference to last entity created. */
  DecodingTable* _last_reference;

  // =================
  //  Private Methods
  // =================
  //

};

// ======================
//  Inline declarations
// ======================
//
inline bool TableHandler::exists (std::string table_name) const
{
  return _identifiers.exists (table_name);
}

inline std::string TableHandler::name (int table_id) const
{
  return _identifiers.name (table_id);
}

inline DecodingTable& TableHandler::reference (int table_id) const
{
  std::map< int, DecodingTable* >::const_iterator ref = _references.find ( table_id );
  REQUIRE( ref != _references.end() ); /** @pre table_id is already defined */
  return *(ref->second);
}

inline DecodingTable& TableHandler::reference (std::string table_name) const
{
  REQUIRE( exists (table_name) ); /** @pre Table name is already known. */
  return reference (id (table_name));
}


inline int TableHandler::id (std::string table_name) const
{
  return _identifiers.id (table_name);
}

inline int TableHandler::last_id_created (void) const
{
  return _last_id;
}

inline DecodingTable& TableHandler::last_reference_created (void) const
{
  return *_last_reference;
}



#endif // TABLEHANDLER_H
