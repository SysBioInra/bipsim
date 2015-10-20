

/**
 * @file transformationtable.cpp
 * @brief Implementation of the TransformationTable class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream> // std::cout
#include <sstream> // std::istringstream

// ==================
//  Project Includes
// ==================
//
#include "transformationtable.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
TransformationTable::TransformationTable (int input_motif_length)
  : _motif_length (input_motif_length)
{
}

// Not needed for this class (use of default copy constructor) !
// TransformationTable::TransformationTable (const TransformationTable& other_transformation_table);

TransformationTable::~TransformationTable (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//
std::string TransformationTable::transform (const std::string& sequence) const
{
  // check that sequence has valid length
  if ((sequence.length() % _motif_length) != 0) return "";

  std::istringstream seq_stream (sequence);
  std::string result;
  char next_motif [_motif_length+1];
  std::map <std::string, std::string>::const_iterator rule;
  while (seq_stream.get (next_motif, _motif_length+1))
    {
      rule = _rules.find (next_motif);
      if (rule == _rules.end()) return ""; // unknown input motif
      result += rule->second;
    }
  return result;
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
// TransformationTable& TransformationTable::operator= (const TransformationTable& other_transformation_table);


// =================
//  Private Methods
// =================
//
