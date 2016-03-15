

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
CompositionTable::CompositionTable (const std::vector <char>& letters, 
				    const std::vector <std::list<FreeChemical*> >& compositions)
{
  /** @pre Vectors must have same length. */
  REQUIRE (letters.size() == compositions.size());
  /** @pre Vectors must not be empty. */
  REQUIRE (letters.size() != 0);

  for (int i = 0; i < letters.size(); ++i) 
    {
      _rules [letters [i]] = compositions [i];
    }
}

// Not needed for this class (use of compiler-generated versions)
// CompositionTable::CompositionTable (const CompositionTable& other);
// CompositionTable& CompositionTable::operator= (const CompositionTable& other);
// CompositionTable::~CompositionTable (void);

// ===========================
//  Public Methods - Commands
// ===========================
//
std::map <FreeChemical*, int>
CompositionTable::composition (const std::string& sequence) const
{
  std::istringstream seq_stream (sequence);
  std::map <FreeChemical*, int> result;

  // naive version: we simply go through the letters and update the result
  char next_letter;
  while (seq_stream.get (next_letter))
    {
      /** @pre A composition rule must be attached to every letter. */
      std::map <char, std::list <FreeChemical*> >::const_iterator
	rule_it = _rules.find (next_letter);
      REQUIRE (rule_it != _rules.end());

      const std::list <FreeChemical*>& chemical_list = rule_it->second;
      for (std::list <FreeChemical*>::const_iterator
	     chemical_it = chemical_list.begin();
	   chemical_it != chemical_list.end(); ++chemical_it)
	{
	  // look if chemical is already in the map
	  std::map <FreeChemical*, int>::iterator
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
