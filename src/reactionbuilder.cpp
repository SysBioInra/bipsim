
/**
 * @file reactionbuilder.cpp
 * @brief Implementation of the ChemicalReactionBuilder, ProductLoadingBuilder,
 *  DoubleStrandLoading, TranslocationBuilder, SequenceBindingBuilder 
 *  ReleaseBuilder and DegradationBuilder classes.
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
#include "reactionbuilder.h"

#include "compositiontable.h"
#include "loadingtable.h"
#include "producttable.h"
#include "transformationtable.h"

#include "bindingsitefamily.h"
#include "freechemical.h"
#include "boundchemical.h"
#include "chemicalsequence.h"
#include "doublestrand.h"

#include "chemicalreaction.h"
#include "sequencebinding.h"
#include "loading.h"
#include "release.h"
#include "translocation.h"
#include "doublestrandrecruitment.h"
#include "forwardreaction.h"
#include "backwardreaction.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
ChemicalReactionBuilder::ChemicalReactionBuilder (CellState& cell_state)
  : Builder (cell_state)
  , _format (TagToken ("ChemicalReaction")
	     + Iteration (MemToken <std::string> (_chemical_names) 
			  + MemToken <int> (_stoichiometries_read), 
			  TagToken ("rates"))
	     + DblToken (_k_1) + DblToken (_k_m1))
{
}

ProductLoadingBuilder::ProductLoadingBuilder (CellState& cell_state)
  : Builder (cell_state)
  , _format (TagToken ("ProductLoading") + StrToken (_loader_name) 
	     + StrToken (_table_name))
{
}

DoubleStrandLoadingBuilder::DoubleStrandLoadingBuilder (CellState& cell_state)
  : Builder (cell_state)
  , _format (TagToken ("DoubleStrandLoading") + StrToken (_loader_name) 
	     + StrToken (_table_name) + StrToken (_stalled_name))
{
}

TranslocationBuilder::TranslocationBuilder (CellState& cell_state)
  : Builder (cell_state)
  , _format (TagToken ("Translocation") + StrToken (_processive_name)
	     + StrToken (_step_name) + StrToken (_stalled_name) 
	     + IntToken (_step_size) + DblToken (_rate)
	     + Iteration (MemToken <std::string> (_family_names)))
{
}

SequenceBindingBuilder::SequenceBindingBuilder (CellState& cell_state)
  : Builder (cell_state)
  , _format (TagToken ("SequenceBinding") + StrToken (_unit_to_bind) 
	     + StrToken (_bound_unit) + StrToken (_family))
{
}

ReleaseBuilder::ReleaseBuilder (CellState& cell_state)
  : Builder (cell_state)
  , _format (TagToken ("Release") + StrToken (_unit_to_release)
	     + Iteration (MemToken <std::string> (_chemical_names) 
			  + MemToken <int> (_stoichiometries),
			  TagToken ("rate") + DblToken (_rate)))
  , _product_format (TagToken ("produces") + StrToken (_table_name))
{
}

DegradationBuilder::DegradationBuilder (CellState& cell_state)
  : Builder (cell_state)
  , _format (TagToken ("Degradation") + StrToken (_sequence_name) 
	     + StrToken (_table_name) + DblToken (_rate))
{
}

DoubleStrandRecruitmentBuilder::
DoubleStrandRecruitmentBuilder (CellState& cell_state)
  : Builder (cell_state)
  , _format (TagToken ("DoubleStrandRecruitment") + StrToken (_recruiter) 
	     + StrToken (_recruit) + StrToken (_bound_recruit) 
	     + DblToken (_rate))
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//
bool ChemicalReactionBuilder::match (InputLine& text_input)
{
  _chemical_names.clear(); _stoichiometries_read.clear(); 
  if (!_format.match (text_input)) { return false; }
  parse_chemicals (_chemical_names, _stoichiometries_read);

  ChemicalReaction* reaction = 
    new ChemicalReaction (_free_chemicals, _stoichiometries, _orders,
			  _k_1, _k_m1, _bound_reactant, _bound_product);
  store (reaction);
  if (_k_1 > 0) { store (new ForwardReaction (*reaction)); }
  if (_k_m1 > 0) { store (new BackwardReaction (*reaction)); } 
  return true;
}

bool ProductLoadingBuilder::match (InputLine& text_input)
{
  if (!_format.match (text_input)) { return false; }
  store (new ProductLoading (fetch <BoundChemical> (_loader_name),
			     fetch <LoadingTable> (_table_name)));
  return true;
}

bool DoubleStrandLoadingBuilder::match (InputLine& text_input)
{
  if (!_format.match (text_input)) { return false; }
  store (new DoubleStrandLoading (fetch <BoundChemical> (_loader_name),
				  fetch <LoadingTable> (_table_name), 
				  fetch <BoundChemical> (_stalled_name)));
  return true;
}

bool TranslocationBuilder::match (InputLine& text_input)
{
  _family_names.clear();
  if (!_format.match (text_input)) { return false; }
  std::vector <const SiteFamily*> families;
  for (std::vector <std::string>::iterator it = _family_names.begin();
       it != _family_names.end(); ++it)
    { families.push_back (&(fetch <SiteFamily> (*it))); }

  store (new Translocation (fetch <BoundChemical> (_processive_name), 
			    fetch <BoundChemical> (_step_name), 
			    fetch <BoundChemical> (_stalled_name),
			    _step_size, _rate, families));
  return true;
}

bool SequenceBindingBuilder::match (InputLine& text_input)
{
  if (!_format.match (text_input)) { return false; }
  SequenceBinding* reaction = 
    new SequenceBinding (fetch <FreeChemical> (_unit_to_bind), 
			 fetch <BoundChemical> (_bound_unit), 
			 fetch <BindingSiteFamily> (_family));
  store (reaction);
  store (new ForwardReaction (*reaction));
  store (new BackwardReaction (*reaction)); 
  return true;
}

bool ReleaseBuilder::match (InputLine& text_input)
{
  _chemical_names.clear(); _stoichiometries.clear();
  if (!_format.match (text_input)) { return false; }
  _chemicals.clear(); _orders.clear();
  for (int i = 0; i < _chemical_names.size(); ++i)
    {
      _chemicals.push_back (&(fetch <FreeChemical> (_chemical_names [i])));
      _orders.push_back (abs (_stoichiometries [i]));
    }
	 
  // get associated product (if applicable)
  ProductTable* table = 0;
  if (_product_format.match (text_input))
    { table = &(fetch <ProductTable> (_table_name)); }

  store (new Release (fetch <BoundChemical> (_unit_to_release), _chemicals, 
		      _stoichiometries, _orders, _rate, table));
  return true;
}


bool DegradationBuilder::match (InputLine& text_input)
{
  if (!_format.match (text_input)) { return false; }
  ChemicalSequence& sequence = fetch <ChemicalSequence> (_sequence_name);
  CompositionTable& table = fetch <CompositionTable> (_table_name);

  // get sequence composition
  std::map <FreeChemical*, int>
    composition = table.composition (sequence.sequence());
  if (composition.size() == 0)
    { 
      std::ostringstream message;
      message << "Could not compute composition after degradation of "
	      << _sequence_name << ", check table " << _table_name
	      << " consistency";
      throw ParserException (message.str());
    }

  // create chemical and stoichiometry vector to represent reaction
  std::vector <FreeChemical*> chemicals (composition.size()+1);
  std::vector <int> stoichiometry (chemicals.size());
  std::vector <int> orders (chemicals.size());
  chemicals [0] = &sequence; stoichiometry [0] = -1; orders [0] = 1;
  int i = 1;
  for (std::map <FreeChemical*, int>::iterator comp_it = composition.begin();
       comp_it != composition.end(); ++comp_it, ++i)
    {
      chemicals [i] = comp_it->first; 
      stoichiometry [i] = comp_it->second;
      orders [i] = comp_it->second;
    }

  ChemicalReaction* reaction = 
    new ChemicalReaction (chemicals, stoichiometry, orders, _rate, 0);
  store (reaction);
  store (new ForwardReaction (*reaction));
  return true;
}

bool DoubleStrandRecruitmentBuilder::match (InputLine& text_input)
{
  if (!_format.match (text_input)) { return false; }
  store (new DoubleStrandRecruitment (fetch <BoundChemical> (_recruiter), 
				      fetch <FreeChemical> (_recruit),
				      fetch <BoundChemical> (_bound_recruit),
				      _rate));
  return true;
}

// ============================
//  Public Methods - Accessors
// ============================
//

// =================
//  Private Methods
// =================
//
void ChemicalReactionBuilder::
parse_chemicals (const std::vector <std::string>& names, 
		 const std::vector <int>& stoichiometries)
{
  _bound_product = 0; _bound_reactant = 0;
  _free_chemicals.clear(); _stoichiometries.clear(); _orders.clear();
  for (int i = 0; i < names.size(); ++i)
    {
      int stoichiometry = stoichiometries [i];
      Chemical* chemical = &(fetch <Chemical> (names [i]));
      FreeChemical* free_chemical = dynamic_cast <FreeChemical*> (chemical);
      BoundChemical* bound_chemical = dynamic_cast <BoundChemical*> (chemical);
      if (free_chemical)
	{
	  _free_chemicals.push_back (free_chemical);
	  _stoichiometries.push_back (stoichiometry);
	  _orders.push_back (abs (stoichiometry));
	}
      else if (bound_chemical && (stoichiometry == 1) && (_bound_product == 0))
	{ 
	  _bound_product = bound_chemical; 
	}
      else if (bound_chemical && (stoichiometry == -1) && (_bound_reactant == 0))
	{
	  _bound_reactant = bound_chemical;
	}
      else
	{
	  throw ParserException ("A chemical reaction can only contain free "
				 "chemicals or bound chemicals. It contains"
				 " at most one bound chemical on each side of"
				 " the reaction with unitary stoichiometry.");
	}
    }
  if (((_bound_reactant == 0) && (_bound_product != 0))
      || ((_bound_reactant != 0) && (_bound_product == 0)))
    {
      throw ParserException ("If a chemical reaction contains bound elements,"
			     " it must have exactly one bound reactant and one"
			     " bound product.");
    }
}
