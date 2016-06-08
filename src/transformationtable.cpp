

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
TransformationTable::TransformationTable 
(const std::vector <std::string>& input_motifs, 
 const std::vector <std::string>& output_motifs)
{
  /** @pre Vector must have equal length. */
  REQUIRE (input_motifs.size() == output_motifs.size());
  /** @pre Vectors must not be empty. */
  REQUIRE (input_motifs.size() != 0);

  _motif_length = input_motifs[0].length();
  for (int i = 0; i < input_motifs.size(); ++i)
    {
      /** @pre All input motifs must have equal length. */
      REQUIRE (input_motifs[i].length() == _motif_length);
      
      _rules [input_motifs[i]] = output_motifs[i];
    }
}

// Not needed for this class (use of compiler generated versions)
// TransformationTable::TransformationTable (const TransformationTable& other_transformation_table);
// TransformationTable& TransformationTable::operator= (const TransformationTable& other_transformation_table);
// TransformationTable::~TransformationTable (void);

// ===========================
//  Public Methods - Commands
// ===========================
//
std::string TransformationTable::transform (const std::string& sequence) const
{
  // check that sequence has valid length
  if ((sequence.length() % _motif_length) != 0) { return ""; }

  std::istringstream seq_stream (sequence);
  std::string result;
  char next_motif [_motif_length+1];
  std::map <std::string, std::string>::const_iterator rule;
  while (seq_stream.get (next_motif, _motif_length+1))
    {
      rule = _rules.find (next_motif);
      if (rule == _rules.end()) { return ""; } // unknown input motif
      result += rule->second;
    }
  return result;
}

// ============================
//  Public Methods - Accessors
// ============================
//

// =================
//  Private Methods
// =================
//
