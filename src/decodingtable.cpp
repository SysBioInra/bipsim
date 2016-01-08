

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

// Not needed for this class (use of default copy constructor) !
// DecodingTable::DecodingTable ( const DecodingTable& other_decoding_table );

DecodingTable::~DecodingTable (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//

void DecodingTable::add_template (const std::string& template_, Chemical& corresponding_base, BoundChemical& occupied_polymerase, double loading_rate)
{
  REQUIRE( template_.size() == _template_length ); /** @pre template_ size must be consistent with _template_length. */
  REQUIRE( _template_index.count(template_) == 0 ); /** @pre Template must not exist already. */
  
  // DANGER: when the left hand side is called, the map size is already effectively increased
  // so when the right hand side is evaluated, we get the size INCLUDING the new template
  // because the vector indices start at 0, we substract 1
  _template_index [template_] = _template_index.size()-1;
  _bases.push_back (&corresponding_base);
  _occupied_polymerases.push_back (&occupied_polymerase);
  _loading_rates.push_back (loading_rate);
}


// ============================
//  Public Methods - Accessors
// ============================
//
void DecodingTable::print ( std::ostream& output ) const
{
  output << "Decoding table:" << std::endl;
  for ( std::map<std::string,int>::const_iterator template_ = _template_index.begin();
	template_ != _template_index.end(); template_++ )
    {
      std::string motif = template_->first;
      int index = template_->second;
      output << "\tMotif " << motif 
	     << " loads chemical "
	     << _bases [index]
	     << " and yields occupied polymerase "
	     << _occupied_polymerases [index]
	     << "." << std::endl;
    }
}


// ==========================
//  Public Methods - Setters
// ==========================
//


// =======================================
//  Public Methods - Operator overloading
// =======================================
//
// Not needed for this class (use of default overloading) !
// DecodingTable& DecodingTable::operator= ( const DecodingTable& other_decoding_table );

std::ostream& operator<< (std::ostream& output, const DecodingTable& decoding_table)
{
  decoding_table.print (output);
  return output;
}



// =================
//  Private Methods
// =================
//
