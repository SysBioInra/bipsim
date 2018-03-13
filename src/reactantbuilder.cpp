// 
// Copyright 2017 INRA
// Authors: M. Dinh, S. Fischer
// Last modification: 2017-09-19
// 
// 
// Licensed under the GNU General Public License.
// You should have received a copy of the GNU General Public License
// along with BiPSim.  If not, see <http://www.gnu.org/licenses/>.
// 


/**
 * @file reactantbuilder.cpp
 * @brief Implementation of the BindingSiteBuilder, TerminationSiteBuilder,
 *  FreeChemicalBuilder, BoundChemicalBuilder, ChemicalSequenceBuilder,
 *  DoubleStrandBuilder classes.
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
#include "reactantbuilder.h"

#include "bindingsite.h"
#include "bindingsitefamily.h"
#include "switch.h"

#include "producttable.h"
#include "transformationtable.h"

#include "freechemical.h"
#include "boundchemical.h"
#include "chemicalsequence.h"
#include "doublestrand.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
BindingSiteBuilder::BindingSiteBuilder (CellState& cell_state)
  : Builder (cell_state)
  , _format (TagToken ("BindingSite") + StrToken (_family_name)
	     + StrToken (_location_name) + IntToken (_start) + IntToken (_end)
	     + DblToken (_k_on) + DblToken (_k_off))
  , _rf_format (_reading_frame)
{
}

SwitchBuilder::SwitchBuilder (CellState& cell_state)
  : Builder (cell_state)
  , _format (TagToken ("Switch") + StrToken (_name)
	     + StrToken (_input) + StrToken (_output))
{
}

SwitchSiteBuilder::SwitchSiteBuilder (CellState& cell_state)
  : Builder (cell_state)
  , _format (TagToken ("SwitchSite") + StrToken (_location)
	     + IntToken (_position) + StrToken (_switch_name))
{
}

FreeChemicalBuilder::FreeChemicalBuilder (CellState& cell_state)
  : Builder (cell_state)
  , _format (TagToken ("FreeChemical") + StrToken (_name))
  , _constant_tag ("CONSTANT")
{
}

BoundChemicalBuilder::BoundChemicalBuilder (CellState& cell_state)
  : Builder (cell_state)
  , _format (TagToken ("BoundChemical") + StrToken (_name))
{
}

ChemicalSequenceBuilder::ChemicalSequenceBuilder (CellState& cell_state)
  : Builder (cell_state)
  , _header_format (TagToken ("ChemicalSequence") + StrToken (_name))
  , _sequence_format (TagToken ("sequence") + StrToken (_sequence))
  , _product_format (TagToken ("product_of") + StrToken (_parent_name)
		     + IntToken (_pos1) + IntToken (_pos2) 
		     + StrToken (_table_name))
{
}

DoubleStrandBuilder::DoubleStrandBuilder (CellState& cell_state)
  : Builder (cell_state)
  , _format (TagToken ("DoubleStrandSequence") + StrToken (_name)
	     + StrToken (_sense_name) + StrToken (_sequence)
	     + StrToken (_antisense_name) + StrToken (_table_name)
	     + StrToken (_circularity))
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//
bool BindingSiteBuilder::match (InputLine& text_input)
{
  if (_format.match (text_input) == false) { return false; }
  ChemicalSequence& location = fetch <ChemicalSequence> (_location_name);
  if (_rf_format.match (text_input))
    {  
      if (_end < _reading_frame) { _end = _reading_frame; }
      if (_start > _reading_frame) { throw ParserException ("wtf?"); }
      _reading_frame = location.relative (_reading_frame);
    }
  else
    { _reading_frame = BindingSite::NO_READING_FRAME; }
  check_positions (location);
  store (new BindingSite (fetch_or_create <BindingSiteFamily> (_family_name), 
			  location, location.relative (_start),
			  location.relative (_end), _k_on, _k_off, 
			  _reading_frame));
  return true;
}

bool SwitchBuilder::match (InputLine& text_input)
{
  if (_format.match (text_input) == false) { return false; }
  BoundChemical& input = fetch <BoundChemical> (_input);
  BoundChemical& output = fetch <BoundChemical> (_output);
  Switch* switch_ = new Switch (input, output);
  store (switch_, _name);
  input.add_switch (*switch_);
  return true;
}

bool SwitchSiteBuilder::match (InputLine& text_input)
{
  if (_format.match (text_input) == false) { return false; }
  ChemicalSequence& location = fetch <ChemicalSequence> (_location);
  Switch& switch_ = fetch <Switch> (_switch_name);
  location.add_switch_site (location.relative (_position), switch_.id());
  return true;
}

bool FreeChemicalBuilder::match (InputLine& text_input)
{
  if (!_format.match (text_input)) { return false; }
  FreeChemical* chemical = new FreeChemical;
  store (chemical, _name);
  chemical->add (read_initial_quantity (text_input));
  chemical->set_constant (_constant_tag.match (text_input));
  return true;
}

bool BoundChemicalBuilder::match (InputLine& text_input)
{
  if (!_format.match (text_input)) { return false; }
  store (new BoundChemical, _name);
  return true;
}

bool ChemicalSequenceBuilder::match (InputLine& text_input)
{
  int mark = text_input.mark();
  if (!_header_format.match (text_input)) { return false; }
  ChemicalSequence* chemical;
  bool is_new = true;
  if (_sequence_format.match (text_input))
    { chemical = new ChemicalSequence (_sequence); }
  else if (_product_format.match (text_input))
    { 
      ChemicalSequence& parent = fetch <ChemicalSequence> (_parent_name);
      check_out_of_bounds (parent);
      ProductTable& table = fetch <ProductTable> (_table_name);
      _sequence = table.generate_child_sequence 
	(parent, parent.relative (_pos1), parent.relative (_pos2));
      check_child_sequence();

      chemical = find <ChemicalSequence> (_name);
      if (chemical == 0) // chemical does not exist: create it
	{ chemical = new ChemicalSequence (_sequence, _pos1); }
      else // chemical exists: signal it is product of another parent
	{ is_new = false; check_product_consistency (*chemical); }
      table.add (parent, parent.relative (_pos1), 
		 parent.relative (_pos2), *chemical);      
    }
  else { text_input.go_to (mark); return false; }
  if (is_new)
    {
      store (chemical, _name);
      chemical->add (read_initial_quantity (text_input));
    }
  return true;
}

bool DoubleStrandBuilder::match (InputLine& text_input)
{
  if (!_format.match (text_input)) { return false; }
  bool circular = false;
  if (_circularity == "CIRCULAR") { circular = true; }
  else if (_circularity != "LINEAR")
    {
      throw ParserException (_circularity + ": wrong keyword for DoubleStrand"
			     " circularity, must be CIRCULAR or LINEAR");
    }
  std::string antisequence = fetch <TransformationTable> (_table_name).
    transform (std::string (_sequence.rbegin(),_sequence.rend()));
  ChemicalSequence* sense = new ChemicalSequence (_sequence, 1, circular); 
  store (sense, _sense_name);
  ChemicalSequence* antisense = new ChemicalSequence (antisequence, 1, circular); 
  store (antisense, _antisense_name);
  DoubleStrand* chemical = new DoubleStrand (*sense, *antisense);
  store (chemical, _name);
  chemical->add (read_initial_quantity (text_input));
  return true;
}


// =================
//  Private Methods
// =================
//
void BindingSiteBuilder::check_positions (const ChemicalSequence& location)
{
  if (_end < _start)
    {
      std::ostringstream message;
      message << "Site of family " << _family_name 
	      << " at position [" << _start << "," << _end << "]:"
	      << "starting position is smaller than end...";
      throw ParserException (message.str());
    }
  if (location.is_out_of_bounds (location.relative (_start), 
				 location.relative (_end)))
    {
      std::ostringstream message;
      message << "Site of family " << _family_name 
	      << " at position [" << _start << "," << _end << "]"
	      << " is not within bound of " << _location_name;
      throw ParserException (message.str());
    }
}

void ChemicalSequenceBuilder::
  check_out_of_bounds (const ChemicalSequence& parent)
{
  if (parent.is_out_of_bounds (parent.relative (_pos1), 
				 parent.relative (_pos2)))
    {
      std::ostringstream message;
      message << "Product " << _name 
	      << " at position [" << _pos1 << "," << _pos2 << "]"
	      << " is not within bound of " << _parent_name;
      throw ParserException (message.str());
    }
}

void ChemicalSequenceBuilder::check_child_sequence (void)
{
  if (_sequence == "")
    { 
      std::ostringstream message;
      message << "Applying table " << _table_name << " on "
	      << _parent_name << " between positions " << _pos1
	      << " and " << _pos2 << " did not yield a valid child sequence,"
	      << " check positions and table content";
      throw ParserException (message.str()); 
    }
}

void ChemicalSequenceBuilder::
  check_product_consistency (const ChemicalSequence& chemical)
{
  // check that product sequence is consistent with previous definitions
  if (_sequence != chemical.sequence())
    {
      std::ostringstream message;
      message << "product " << _name << " defined multiple times,"
	      << " and inferred sequence\n" << _sequence
	      << "\ndoes not match previous definition\n"
	      << chemical.sequence();
      throw ParserException (message.str());
    }
}

