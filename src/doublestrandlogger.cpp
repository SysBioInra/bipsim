
/**
 * @file doublestrandlogger.cpp
 * @brief Implementation of the DoubleStrandLogger class.
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
#include "doublestrandlogger.h"
#include "doublestrand.h"
#include "chemicalsequence.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
DoubleStrandLogger::DoubleStrandLogger (const std::string& filename,
					const DoubleStrand& double_strand, 
					bool overwrite /*= true*/)
  : _double_strand (double_strand)
{
  // open file
  if (overwrite) { _output.open (filename.c_str()); }
  else { _output.open (filename.c_str(), std::ofstream::app); }
}

// Forbidden
// DoubleStrandLogger::DoubleStrandLogger (const DoubleStrandLogger& logger);
// DoubleStrandLogger& DoubleStrandLogger::operator= (const DoubleStrandLogger& other_logger);

DoubleStrandLogger::~DoubleStrandLogger (void)
{
  _output.close();
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void DoubleStrandLogger::log (double simulation_time)
{
  _output << simulation_time;
  _output << " Sense\n";
  write_sites (_double_strand.sense());
  _output << " Antisense\n";
  write_sites (_double_strand.antisense());
  _output << "\n";
}

// =================
//  Private Methods
// =================
//
void DoubleStrandLogger::write_sites (const ChemicalSequence& sequence)
{
  std::list <std::vector <int> > strands = sequence.partial_strands();
  int id = 1;
  for (std::list <std::vector <int> >::iterator it = strands.begin();
       it != strands.end(); ++it, ++id)
    {
      _output << "Strand " << id << ":";
      for (int i = 0; i < it->size(); i += 2)
	{
	  _output << "[" << (*it)[i] << " " << (*it)[i+1] << "]";
	}
      _output << "\n";
    }
}
