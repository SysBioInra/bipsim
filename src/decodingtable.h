

/**
 * @file decodingtable.h
 * @brief Header for the DecodingTable class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef DECODINGTABLE_H
#define DECODINGTABLE_H

// ==================
//  General Includes
// ==================
//
#include <map> // std::map
#include <vector> // std::vector
#include <string> // std::string
#include <set> // std::set

// ==================
//  Project Includes
// ==================
//
#include "forwarddeclarations.h"
#include "simulatorinput.h"
#include "macros.h" // REQUIRE


/**
 * @brief Table that stores base-template pairings.
 *
 * Templates are user-defined strings and bases are any chemical that can be
 * used for synthesis. It inherits class SimulatorInput.
 */
class DecodingTable : public SimulatorInput
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Default constructor
   * @param template_length Length of templates used for decoding.
   */
  DecodingTable (int template_length);
  

  // Not needed for this class (use of default copy constructor) !
  // /*
  //  * @brief Copy constructor
  //  */
  // DecodingTable ( const DecodingTable& other_decoding_table );

  /**
   * @brief Destructor
   */
  virtual ~DecodingTable (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //
  /**
   * @brief Add pairing to the decoding table.
   *
   * If template is already defined, the corresponding base is overwritten.
   * @param template_ String template to decode.
   * @param corresponding_base Chemical that matches the template.
   * @param occupied_polymerase Occupied polymerase when it has loaded the base.
   * @param loading_rate Rate at which a base is loaded onto its template.
   */
  void add_template (const std::string& template_, Chemical& corresponding_base, BoundChemical& occupied_polymerase, double loading_rate);


  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Number of templates loaded in the map.
   * @return Number of templates loaded in the map.
   */
  int size (void) const;

  /**
   * @brief Get index corresponding to template.
   * @param template_ Template to retrieve in the table.
   * @return Index given to the template. DecodingTable::UNKNOWN_TEMPLATE if unknown.
   */
  int template_index (const std::string& template_) const;


  /**
   * @brief Get base corresponding to template.
   * @param template_index Template index.
   * @return Reference to chemical corresponding to template.
   */
  Chemical& decode (int template_index) const;

  /**
   * @brief Get occupied polymerase state corresponding to template.
   * @param template_index Template index.
   * @return Reference to occupied polymerase corresponding to template.
   */
  BoundChemical& occupied_polymerase (int template_index) const;

  /**
   * @brief Get loading rate corresponding to template.
   * @param template_index Template index.
   * @return Loading rate corresponding to template.
   */
  double loading_rate (int template_index) const;

  /**
   * @brief Length of template.
   * @return Lenght of template.
   */
  int template_length (void) const;


  /**
   * @brief Accessor to all loadable bases.
   * @return Set of all loadable bases.
   */
  const std::set<Chemical*> bases (void) const;

  /**
   * @brief Accessor to all possible occupied polymerases.
   * @return Set of all possible occupied polymerases.
   */
  const std::set<BoundChemical*> occupied_polymerases (void) const;

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
  // DecodingTable& operator= ( const DecodingTable& other_decoding_table );
  
  /**
   * @brief Standard output.
   * @return A reference to the stream containing the output.
   * @param output Stream where output should be written.
   * @param decoding_table Reference to the table whose information should be written.
   */
  friend std::ostream& operator<< (std::ostream& output, const DecodingTable& decoding_table);


  // ==================================
  //  Public Methods - Class invariant
  // ==================================
  //
  /**
   * @brief Check class invariant.
   * @return True if class invariant is preserved
   */
  virtual bool check_invariant (void) const;

  
  // ==================
  //  Public Constants
  // ==================
  //
  /**
   * @brief Index associated with unknown templates.
   */
  static const int UNKNOWN_TEMPLATE = -1;

  
private:

  // ============
  //  Attributes
  // ============
  //
  /** @brief Loading rates associated with each template. */
  std::vector< double > _loading_rates;
  
  /** @brief Vector of loadable chemicals. */
  std::vector< Chemical* > _bases;

  /** @brief Vector of occupied polymerases. */
  std::vector< BoundChemical* > _occupied_polymerases;

  /** @brief Template to index map. */
  std::map< std::string, int > _template_index;

  /** @brief Template length. */
  int _template_length;

  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Print class content.
   * @param output Stream where output should be written.
   */
  void print ( std::ostream& output ) const;

};

// ======================
//  Inline declarations
// ======================
//

inline int DecodingTable::template_index (const std::string& template_) const
{
  std::map< std::string, int >::const_iterator it = _template_index.find (template_);
  if (it != _template_index.end()) // Template  exists.
    {
      return it->second;
    }
  else
    {
      return DecodingTable::UNKNOWN_TEMPLATE;
    }
}

inline Chemical& DecodingTable::decode (int template_index) const
{
  REQUIRE( template_index >= 0 ); /** @pre Template index must be nonnegative. */
  REQUIRE( template_index < _bases.size() ); /** @pre Template index must be smaller than number of templates. */
  return *(_bases [template_index]);
}

inline BoundChemical& DecodingTable::occupied_polymerase (int template_index) const
{
  REQUIRE( template_index >= 0 ); /** @pre Template index must be nonnegative. */
  REQUIRE( template_index < _bases.size() ); /** @pre Template index must be smaller than number of templates. */
  return *(_occupied_polymerases [template_index]);
}

inline double DecodingTable::loading_rate (int template_index) const
{
  REQUIRE( template_index >= 0 ); /** @pre Template index must be nonnegative. */
  REQUIRE( template_index < _loading_rates.size() ); /** @pre Template index must be smaller than number of templates. */
  return _loading_rates [template_index];
}

inline int DecodingTable::size (void) const
{
  return _template_index.size();
}

inline int DecodingTable::template_length (void) const
{
  return _template_length;
}

inline const std::set<Chemical*> DecodingTable::bases (void) const
{
  return std::set<Chemical*> (_bases.begin(), _bases.end());
}

inline const std::set<BoundChemical*> DecodingTable::occupied_polymerases (void) const
{
  return std::set<BoundChemical*> (_occupied_polymerases.begin(), _occupied_polymerases.end());
}


#endif // DECODINGTABLE_H
