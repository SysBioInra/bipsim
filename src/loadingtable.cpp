

/**
 * @file loadingtable.cpp
 * @brief Implementation of the LoadingTable class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream>

// ==================
//  Project Includes
// ==================
//
#include "loadingtable.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
LoadingTable::LoadingTable (const std::vector <std::string>& templates,
			    const std::vector <FreeChemical*>& chemicals_to_load,
			    const std::vector <BoundChemical*>& occupied_states,
			    const std::vector <double>& loading_rates)
  : _loading_rates (loading_rates)
  , _chemicals_to_load (chemicals_to_load)
  , _occupied_states (occupied_states)
{
  /** @pre Vectors size must match. */
  REQUIRE ((templates.size() == chemicals_to_load.size())
	   && (templates.size() == occupied_states.size())
	   && (templates.size() == loading_rates.size()));
  /** @pre Vectors must not be empty. */
  REQUIRE (templates.size() != 0) ;

  _template_length = templates [0].length();
  for (std::size_t i = 0; i < templates.size(); ++i)
    {
      /** @pre All templates must have same length. */
      REQUIRE (templates[i].length() == _template_length); 
      /** @pre Template must not exist already. */
      REQUIRE (_template_index.count (templates[i]) == 0); 

      _template_index [templates[i]] = i;
    }
}

// Not needed for this class (use of compiler-generated versions)
// LoadingTable::LoadingTable (const LoadingTable& other_table);
// LoadingTable& LoadingTable::operator= (const LoadingTable& other_table);
// LoadingTable::~LoadingTable (void);

// ===========================
//  Public Methods - Commands
// ===========================
//

// ============================
//  Public Methods - Accessors
// ============================
//
std::ostream& operator<< (std::ostream& output, 
			  const LoadingTable& loading_table)
{
  loading_table.print (output);
  return output;
}



// =================
//  Private Methods
// =================
//
void LoadingTable::print (std::ostream& output) const
{
  output << "Loading table:" << std::endl;
  for (std::map<std::string,int>::const_iterator 
	 template_ = _template_index.begin();
	template_ != _template_index.end(); template_++)
    {
      std::string motif = template_->first;
      int index = template_->second;
      output << "\tMotif " << motif 
	     << " loads chemical "
	     << _chemicals_to_load [index]
	     << " and yields occupied state "
	     << _occupied_states [index]
	     << "." << std::endl;
    }
}
