

/**
 * @file decodingtable.cpp
 * @brief Implementation of the DecodingTable class.
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
#include "decodingtable.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
DecodingTable::DecodingTable (int template_length)
  : _template_length (template_length)
{
}

// Not needed for this class (use of compiler-generated versions)
// DecodingTable::DecodingTable (const DecodingTable& other_table);
// DecodingTable& DecodingTable::operator= (const DecodingTable& other_table);
// DecodingTable::~DecodingTable (void);

// ===========================
//  Public Methods - Commands
// ===========================
//

void DecodingTable::add_template (const std::string& template_, 
				  Chemical& chemical_to_load, 
				  BoundChemical& occupied_polymerase, 
				  double loading_rate)
{
  /** @pre template_ size must be consistent with _template_length. */
  REQUIRE( template_.size() == _template_length ); 
  /** @pre Template must not exist already. */
  REQUIRE( _template_index.count(template_) == 0 ); 
  
  // DANGER: when the left hand side is called, the map size is already 
  // effectively increased so when the right hand side is evaluated, we get
  // the size INCLUDING the new template because the vector indices start 
  // at 0, we substract 1
  _template_index [template_] = _template_index.size()-1;
  _chemicals_to_load.push_back (&chemical_to_load);
  _occupied_polymerases.push_back (&occupied_polymerase);
  _loading_rates.push_back (loading_rate);
}


// ============================
//  Public Methods - Accessors
// ============================
//
std::ostream& operator<< (std::ostream& output, 
			  const DecodingTable& decoding_table)
{
  decoding_table.print (output);
  return output;
}



// =================
//  Private Methods
// =================
//
void DecodingTable::print (std::ostream& output) const
{
  output << "Decoding table:" << std::endl;
  for (std::map<std::string,int>::const_iterator 
	 template_ = _template_index.begin();
	template_ != _template_index.end(); template_++)
    {
      std::string motif = template_->first;
      int index = template_->second;
      output << "\tMotif " << motif 
	     << " loads chemical "
	     << _chemicals_to_load [index]
	     << " and yields occupied polymerase "
	     << _occupied_polymerases [index]
	     << "." << std::endl;
    }
}
