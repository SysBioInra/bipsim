

/**
 * @file chemicallogger.cpp
 * @brief Implementation of the ChemicalLogger class.
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
#include "chemicallogger.h"
#include "chemical.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
ChemicalLogger::ChemicalLogger (const std::string& filename,
				const std::vector <const Chemical*>& chemicals,
				const std::vector <std::string>& names, 
				bool overwrite /*= true*/)
 : _chemicals (chemicals)
{
  /** @pre Vector of names must be as long as vector of references. */
  REQUIRE (chemicals.size() == names.size());
  
  // open file
  if (overwrite) { _output.open (filename.c_str()); }
  else { _output.open (filename.c_str(), std::ofstream::app); }
  
  // write header (if necessary)
  if (overwrite) { write_header (names); }
}

// Forbidden
// ChemicalLogger::ChemicalLogger (const ChemicalLogger& logger);
// ChemicalLogger& ChemicalLogger::operator= (const ChemicalLogger& other_logger);

ChemicalLogger::~ChemicalLogger (void)
{
  _output.close();
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void ChemicalLogger::log (double simulation_time)
{
  _output << simulation_time;
  for (std::vector <const Chemical*>::iterator chemical_it = _chemicals.begin();
       chemical_it != _chemicals.end(); ++chemical_it)
    {
      _output << "\t" << (*chemical_it)->number();
    }
  _output << "\n";
}


// =================
//  Private Methods
// =================
//
void ChemicalLogger::write_header (const std::vector <std::string>& names)
{
  _output << "time";
  for (std::vector <std::string>::const_iterator name_it = names.begin();
       name_it != names.end(); ++name_it)
    {
      _output << "\t" << *name_it;
    }
  _output << "\n";
}
