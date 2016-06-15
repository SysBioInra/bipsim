
/**
 * @file tablebuilder.cpp
 * @brief Implementation of the CompositionTableBuilder, LoadingTableBuilder, 
 *  ProductTableBuilder, TransformationTableBuilder classes.
 * @authors Marc Dinh, Stephan Fischer
 */

// ==================
//  General Includes
// ==================
//

// ==================
//  Project Includes
// ==================
//
#include "tablebuilder.h"

#include "compositiontable.h"
#include "loadingtable.h"
#include "producttable.h"
#include "transformationtable.h"

#include "freechemical.h"
#include "boundchemical.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
CompositionTableBuilder::CompositionTableBuilder (CellState& cell_state)
  : Builder (cell_state)
  , _header_format (TagToken ("CompositionTable") + StrToken (_name))
  , _composition_format (MemToken <char> (_letters)
			 + Iteration (MemToken <std::string> (_chemical_names),
				      TagToken (",")))
{
}

LoadingTableBuilder::LoadingTableBuilder (CellState& cell_state)
  : Builder (cell_state)
  , _header_format (TagToken ("LoadingTable") + StrToken (_name))
  , _line_format (StrToken (_template) + StrToken (_base_name) 
		  + StrToken (_occupied_name) + DblToken (_rate))
  , _comma_token (",")
{
}

ProductTableBuilder::ProductTableBuilder (CellState& cell_state)
  : Builder (cell_state)
  , _format (TagToken ("ProductTable") + StrToken (_name) 
	     + StrToken (_table_name))
{
}

TransformationTableBuilder::TransformationTableBuilder (CellState& cell_state)
  : Builder (cell_state)
  , _header_format (TagToken ("TransformationTable") + StrToken (_name))
  , _line_format (StrToken (_input) + StrToken (_output)) 
  , _comma_token (",")
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//
bool CompositionTableBuilder::match (InputLine& text_input)
{
  int mark = text_input.mark();
  if (!_header_format.match (text_input)) { return false; }
  _letters.clear(); _chemical_lists.clear(); _chemical_names.clear();
  while (_composition_format.match (text_input))
    {
      if (_chemical_names.size() == 0) 
	{ text_input.go_to (mark); return false; }
      _chemical_lists.resize (_chemical_lists.size() + 1);
      for (std::vector <std::string>::iterator it = _chemical_names.begin();
	   it != _chemical_names.end(); ++it)
	{ _chemical_lists.back().push_back (&(fetch <FreeChemical> (*it))); }
      _chemical_names.clear();
    }
  if (_letters.size() == 0) { text_input.go_to (mark); return false; }
  if (!text_input.end_of_line()) { text_input.go_to (mark); return false; }
  store (new CompositionTable (_letters, _chemical_lists), _name);
  return true;
}

bool LoadingTableBuilder::match (InputLine& text_input)
{
  int mark = text_input.mark();
  if (!_header_format.match (text_input)) { return false; } 
  _templates.clear(); _bases.clear(); _occupied.clear(); _rates.clear();
  bool next = true;
  do
    {
      if (!_line_format.match (text_input)) 
	{ text_input.go_to (mark); return false; }
      _templates.push_back (_template);      
      if (_templates.back().length() != _templates.front().length())
	{
	  throw ParserException ("Trying to define a loading table with "
				 "templates of variable length.");
	}
      _bases.push_back (&(fetch <FreeChemical> (_base_name)));
      _occupied.push_back (&(fetch <BoundChemical> (_occupied_name))); 
      _rates.push_back (_rate);
    }
  while (_comma_token.match (text_input)); // quadruplets separated by commas
  if (!text_input.end_of_line()) { text_input.go_to (mark); return false; }
  store (new LoadingTable (_templates, _bases, _occupied, _rates), _name);  
  return true;
}

bool ProductTableBuilder::match (InputLine& text_input)
{
  if (!_format.match (text_input)) { return false; }
  store (new ProductTable (fetch <TransformationTable> (_table_name)), _name);
  return true;
}

bool TransformationTableBuilder::match (InputLine& text_input)
{
  int mark = text_input.mark();
  if (!_header_format.match (text_input)) { return false; }
  std::vector <std::string> input_motifs, output_motifs;
  do
    {
      if (!_line_format.match (text_input)) 
	{ text_input.go_to (mark); return false; }
      input_motifs.push_back (_input);
      if (input_motifs.back().length() != input_motifs.front().length())
	{ throw ParserException ("Motifs must have same length"); }
      output_motifs.push_back (_output);
    }
  while (_comma_token.match (text_input)); // duplets are separated by commas
  if (!text_input.end_of_line()) { text_input.go_to (mark); return false; }
  store (new TransformationTable (input_motifs, output_motifs), _name);
  return true;
}
