

/**
 * @file doublestrandlogger.cpp
 * @brief Implementation of the DoubleStrandLogger class.
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
  _output << " sense";
  write_sites (_double_strand.sense());
  _output << " antisense";
  write_sites (_double_strand.antisense());
  _output << "\n";
}

// =================
//  Private Methods
// =================
//
void DoubleStrandLogger::write_sites (const ChemicalSequence& sequence)
{
  int current = sequence.number_sites (0, 0);
  _output << " " << 1 << " " << current;
  for (int i = 1; i < sequence.length(); ++i)
    {
      if (sequence.number_sites (i,i) != current)
	{
	  current = sequence.number_sites (i,i);
	  _output << " " << i+1 << " " << current;
	}
    }
}
