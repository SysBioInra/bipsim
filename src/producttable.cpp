

/**
 * @file producttable.cpp
 * @brief Implementation of the ProductTable class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream> // std::cout

// ==================
//  Project Includes
// ==================
//
#include "producttable.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
ProductTable::ProductTable (const TransformationTable& transformation_table)
	     : _transformation_table (transformation_table)
{
}

// Not needed for this class (use of default copy constructor) !
// ProductTable::ProductTable (const ProductTable& other_product_table);

ProductTable::~ProductTable (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//


// ============================
//  Public Methods - Accessors
// ============================
//

ChemicalSequence* ProductTable::product (const ChemicalSequence& parent,
					 int start, int end) const
{
  ParentMap::const_iterator p_it = _products.find (&parent);
  if (p_it == _products.end()) return 0;

  StartMap::const_iterator s_it = (p_it->second).find (start);
  if (s_it == (p_it->second).end()) return 0;

  EndMap::const_iterator e_it = (s_it->second).find (end);
  if (e_it == (s_it->second).end()) return 0;

  return e_it->second;
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
// ProductTable& ProductTable::operator= (const ProductTable& other_product_table);


// =================
//  Private Methods
// =================
//
