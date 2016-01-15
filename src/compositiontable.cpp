

/**
 * @file compositiontable.cpp
 * @brief Implementation of the CompositionTable class.
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
#include "compositiontable.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
// Not needed for this class (use of compiler-generated versions)
// CompositionTable::CompositionTable (void);
// CompositionTable::CompositionTable (const CompositionTable& other_composition_table);
// CompositionTable& CompositionTable::operator= (const CompositionTable& other_composition_table);
// CompositionTable::~CompositionTable (void);

// ===========================
//  Public Methods - Commands
// ===========================
//
std::map <Chemical*, int>
CompositionTable::composition (const std::string& sequence) const
{
  std::istringstream seq_stream (sequence);
  std::map <Chemical*, int> result;

  // naive version: we simply go through the letters and update the result
  char next_letter;
  while (seq_stream.get (next_letter))
    {
      /** @pre A composition rule must be attached to every letter. */
      std::map <char, std::list <Chemical*> >::const_iterator
	rule_it = _rules.find (next_letter);
      REQUIRE (rule_it != _rules.end());

      const std::list <Chemical*>& chemical_list = rule_it->second;
      for (std::list <Chemical*>::const_iterator
	     chemical_it = chemical_list.begin();
	   chemical_it != chemical_list.end(); ++chemical_it)
	{
	  // look if chemical is already in the map
	  std::map <Chemical*, int>::iterator
	    result_it = result.find (*chemical_it);
	  if (result_it != result.end())
	    { ++(result_it->second); }
	  else
	    { result [*chemical_it] = 1; } 	  
	}
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
