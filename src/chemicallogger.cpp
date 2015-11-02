

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
ChemicalLogger::ChemicalLogger (const std::string& filename, const std::list <const Chemical*>& chemicals, const std::list <std::string>& names,  bool overwrite /*= true*/)
 : _chemicals (chemicals)
{
  /** @pre List of names must be as long as list of references. */
  REQUIRE (chemicals.size() == names.size());
  
  // open file
  if (overwrite)
    _output.open (filename.c_str());
  else
    _output.open (filename.c_str(), std::ofstream::app);
  
  // write header (if necessary)
  if (overwrite) { write_header (names); }
}

// Not needed for this class (use of default copy constructor) !
// ChemicalLogger::ChemicalLogger ( const ChemicalLogger& other_chemical_logger );

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
  for (std::list <const Chemical*>::iterator chemical_it = _chemicals.begin();
       chemical_it != _chemicals.end(); ++chemical_it)
    {
      _output << "\t" << (*chemical_it)->number();
    }
  _output << "\n";
}

// ============================
//  Public Methods - Accessors
// ============================
//


// ==========================
//  Public Methods - Setters
// ==========================
//


// =======================================
//  Public Methods - Operator overloading
// =======================================
//
// Not needed for this class (use of default overloading) !
// ChemicalLogger& ChemicalLogger::operator= ( const ChemicalLogger& other_chemical_logger );


// =================
//  Private Methods
// =================
//
void ChemicalLogger::write_header (const std::list <std::string>& names)
{
  _output << "time";
  for (std::list <std::string>::const_iterator name_it = names.begin();
       name_it != names.end(); ++name_it)
    {
      _output << "\t" << *name_it;
    }
  _output << "\n";
}
