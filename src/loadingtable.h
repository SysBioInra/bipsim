

/**
 * @file loadingtable.h
 * @brief Header for the LoadingTable class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// Multiple include protection
//
#ifndef LOADING_TABLE_H
#define LOADING_TABLE_H

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
 * @brief Table that stores chemical-template pairings.
 *
 * Templates are user-defined strings and chemical are any chemical that can be
 * used for synthesis. It inherits class SimulatorInput.
 */
class LoadingTable : public SimulatorInput
{
 public:

  // ==========================
  //  Constructors/Destructors
  // ==========================
  //
  /**
   * @brief Constructor. 
   * @param templates Vector of string templates to decode.
   * @param chemicals_to_load Vector of FreeChemical that match the templates.
   * @param occupied_states Vector of BoundChemical representing complex of
   *  chemical performing loading with loaded chemical.
   * @param loading_rates Vector of rates at which a chemical is loaded 
   *  onto its template.
   */
  LoadingTable (const std::vector <std::string>& templates,
		const std::vector <FreeChemical*>& chemicals_to_load,
		const std::vector <BoundChemical*>& occupied_states, 
		const std::vector <double>& loading_rates);
  
  // Not needed for this class (use of compiler-generated versions)
  // (3-0 rule: either define all 3 following or none of them)
  // /* @brief Copy constructor. */
  // LoadingTable (const LoadingTable& other_loading_table);
  // /* @brief Assignment operator. */
  // LoadingTable& operator= ( const LoadingTable& other_loading_table );
  // /* @brief Destructor. */
  // ~LoadingTable (void);

  // ===========================
  //  Public Methods - Commands
  // ===========================
  //

  // ============================
  //  Public Methods - Accessors
  // ============================
  //
  /**
   * @brief Number of templates in the table.
   * @return Number of templates in the table.
   */
  int size (void) const;

  /**
   * @brief Get index corresponding to template.
   * @param template_ Template to retrieve in the table.
   * @return Index given to the template. LoadingTable::UNKNOWN_TEMPLATE 
   *  if unknown.
   */
  int template_index (const std::string& template_) const;

  /**
   * @brief Get chemical to load ont the template.
   * @param template_index Template index.
   * @return Reference to chemical corresponding to template.
   */
  FreeChemical& chemical_to_load (int template_index) const;

  /**
   * @brief Get occupied state state corresponding to template.
   * @param template_index Template index.
   * @return Reference to occupied state corresponding to template.
   */
  BoundChemical& occupied_state (int template_index) const;

  /**
   * @brief Get loading rate corresponding to template.
   * @param template_index Template index.
   * @return Loading rate corresponding to template.
   */
  double loading_rate (int template_index) const;

  /**
   * @brief Length of templates.
   * @return Lenght of templates.
   */
  int template_length (void) const;

  /**
   * @brief Accessor to all loadable chemicals.
   * @return Set of all loadable chemicals.
   */
  const std::set <FreeChemical*> chemicals_loaded (void) const;

  /**
   * @brief Accessor to all possible occupied states.
   * @return Set of all possible occupied states.
   */
  const std::set <BoundChemical*> occupied_states (void) const;
  
  /**
   * @brief Standard output.
   * @return A reference to the stream containing the output.
   * @param output Stream where output should be written.
   * @param loading_table Reference to the table whose information should be
   *  written.
   */
  friend std::ostream& operator<< (std::ostream& output,
				   const LoadingTable& loading_table);

  
  // ==================
  //  Public Constants
  // ==================
  //
  /**
   * @brief Index associated with unknown templates.
   */
  static const int UNKNOWN_TEMPLATE = -1;

private:
  // =================
  //  Private Methods
  // =================
  //
  /**
   * @brief Print class content.
   * @param output Stream where output should be written.
   */
  void print (std::ostream& output) const;

  // ============
  //  Attributes
  // ============
  //
  /** @brief Loading rates associated with each template. */
  std::vector <double> _loading_rates;
  
  /** @brief Vector of loadable chemicals. */
  std::vector <FreeChemical*> _chemicals_to_load;

  /** @brief Vector of occupied states. */
  std::vector <BoundChemical*> _occupied_states;

  /** @brief Template to index map. */
  std::map <std::string, int> _template_index;

  /** @brief Template length. */
  int _template_length;
};

// ======================
//  Inline declarations
// ======================
//

inline int LoadingTable::template_index (const std::string& template_) const
{
  std::map <std::string, int>::const_iterator it = 
    _template_index.find (template_);
  if (it != _template_index.end()) { return it->second; }
  else { return UNKNOWN_TEMPLATE; }
}

inline FreeChemical& LoadingTable::chemical_to_load (int template_index) const
{
  /** @pre Template index must be nonnegative. */
  REQUIRE (template_index >= 0); 
  /** @pre Template index must be smaller than number of templates. */
  REQUIRE (template_index < _chemicals_to_load.size()); 
  return *(_chemicals_to_load [template_index]);
}

inline BoundChemical& LoadingTable::occupied_state (int template_index) const
{
  /** @pre Template index must be nonnegative. */
  REQUIRE (template_index >= 0); 
  /** @pre Template index must be smaller than number of templates. */
  REQUIRE (template_index < _chemicals_to_load.size()); 
  return *(_occupied_states [template_index]);
}

inline double LoadingTable::loading_rate (int template_index) const
{
  /** @pre Template index must be nonnegative. */
  REQUIRE (template_index >= 0); 
  /** @pre Template index must be smaller than number of templates. */
  REQUIRE (template_index < _loading_rates.size()); 
  return _loading_rates [template_index];
}

inline int LoadingTable::size (void) const
{
  return _template_index.size();
}

inline int LoadingTable::template_length (void) const
{
  return _template_length;
}

inline const std::set<FreeChemical*> LoadingTable::chemicals_loaded (void) const
{
  return std::set<FreeChemical*> (_chemicals_to_load.begin(), 
				  _chemicals_to_load.end());
}

inline const std::set<BoundChemical*> LoadingTable::occupied_states (void) const
{
  return std::set<BoundChemical*> (_occupied_states.begin(), 
				   _occupied_states.end());
}


#endif // LOADING_TABLE_H
