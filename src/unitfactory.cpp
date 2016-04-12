

/**
 * @file unitfactory.cpp
 * @brief Implementation of the UnitFactory class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream> // std::cout
#include <cstdlib> // atof

// ==================
//  Project Includes
// ==================
//
#include "unitfactory.h"

#include "simulatorexception.h"

#include "site.h"
#include "bindingsite.h"
#include "bindingsitefamily.h"

#include "freechemical.h"
#include "boundchemical.h"
#include "chemicalsequence.h"
#include "doublestrand.h"

#include "compositiontable.h"
#include "loadingtable.h"
#include "producttable.h"
#include "transformationtable.h"

#include "cellstate.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
UnitFactory::UnitFactory (CellState& cell_state)
  : Factory (cell_state)
{
}

// Not needed for this class (use of compiler generated versions)
// UnitFactory::UnitFactory (const UnitFactory& other_unit_factory);
// UnitFactory& UnitFactory::operator= (const UnitFactory& other_unit_factory);
// UnitFactory::~UnitFactory (void);


// ===========================
//  Public Methods - Commands
// ===========================
//
bool UnitFactory::handle (const std::string& line)
{
  // parse the first word and hand the rest of the line over to 
  // appropriate creator
  _line_stream.str (line);  
  _line_stream.clear();

  // try to create unit
  std::string tag = read <std::string> (_line_stream);
  if (tag == "BindingSite") { create_binding_site(); }
  else if (tag == "TerminationSite") { create_termination_site(); }
  else if (tag == "FreeChemical") { create_free_chemical(); }
  else if (tag == "BoundChemical") { create_bound_chemical(); }
  else if (tag == "ChemicalSequence") { create_chemical_sequence(); }
  else if (tag == "DoubleStrandSequence") { create_double_strand_sequence(); }
  else if (tag == "CompositionTable") { create_composition_table(); }
  else if (tag == "LoadingTable") { create_loading_table(); }
  else if (tag == "ProductTable") { create_product_table(); }
  else if (tag == "TransformationTable") { create_transformation_table(); }
  else { return false; }
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
void UnitFactory::create_binding_site (void)
{
  std::string family_name = read <std::string> (_line_stream);
  BindingSiteFamily* family = 
    fetch_or_create <BindingSiteFamily> (family_name);
  ChemicalSequence* location = fetch <ChemicalSequence> (_line_stream);
  int start = read <int> (_line_stream); int end = read <int> (_line_stream);
  double k_on = read <double> (_line_stream); 
  double k_off = read <double> (_line_stream); 

  // check position consistency
  if (end < start)
    {
      std::ostringstream message;
      message << "Site of family " << family_name 
	      << " at position [" << start << "," << end << "]:"
	      << "starting position is smaller than end...";
      throw ParserException (message.str());
    }
  if (location->is_out_of_bounds (location->relative (start), 
				  location->relative (end)))
    {
      std::ostringstream message;
      message << "Site of family " << family_name 
	      << " at position [" << start << "," << end << "]"
	      << " is not within bound of " << location;
      throw ParserException (message.str());
    }

  // read reading frame (optional)
  int reading_frame;
  if (not (_line_stream >> reading_frame))
    { reading_frame = BindingSite::NO_READING_FRAME; }
  else { reading_frame = location->relative (reading_frame); }

  store (new BindingSite (*family, *location, location->relative (start), 
			  location->relative (end), k_on, k_off, reading_frame));
}

void UnitFactory::create_termination_site (void)
{
  // read base data
  SiteFamily* family = fetch_or_create <SiteFamily> (_line_stream);
  ChemicalSequence* location = fetch <ChemicalSequence> (_line_stream);
  int start = read <int> (_line_stream); int end = read <int> (_line_stream);

  // create and store entity
  Site* site = new Site (*family, *location, location->relative (start), 
			 location->relative (end));
  location->add_termination_site (*site);
  store (site);
}

void UnitFactory::create_composition_table (void)
{
  std::string name = read <std::string> (_line_stream);
  std::vector <char> letters;
  std::vector <std::list <FreeChemical*> > chemical_lists;

  bool next = true;
  while (next)
    {
      // read letter and associated chemicals
      letters.push_back (read <char> (_line_stream));      
      chemical_lists.resize (chemical_lists.size() + 1);
      std::list <FreeChemical*>& chemicals = chemical_lists.back();

      std::string chemical_name = read <std::string> (_line_stream);
      bool comma = check_comma (chemical_name);
      chemicals.push_back (fetch <FreeChemical> (chemical_name));
      while (next && (comma == false))
	{
	  if (_line_stream >> chemical_name)
	    { 
	      comma = check_comma (chemical_name);
	      if (chemical_name != "")
		{ chemicals.push_back (fetch <FreeChemical> (chemical_name)); }
	    }
	  else { next = false; }
	}
    }
  store (new CompositionTable (letters, chemical_lists), name);
}


void UnitFactory::create_loading_table (void)
{
  std::string name = read <std::string> (_line_stream);
  std::vector <std::string> templates;
  std::vector <FreeChemical*> bases;
  std::vector <BoundChemical*> occupied;
  std::vector <double> rates;

  bool next = true;
  while (next)
    {
      templates.push_back (read <std::string> (_line_stream));
      if (templates.back().length() != templates.front().length())
	{
	  throw ParserException ("Trying to define a loading table with "
				 "templates of variable length.");
	}
      bases.push_back (fetch <FreeChemical> (_line_stream));
      occupied.push_back (fetch <BoundChemical> (_line_stream)); 

      // quadruplets are separated by commas, if there is no comma
      // then the list should end here
      std::string rate_str = read <std::string> (_line_stream);
      if (check_comma (rate_str) == false)
	{
	  char next_char;
	  if (not (_line_stream >> next_char)) { next = false; }
	  else if (next_char != ',') { throw FormatException(); }
	}
      rates.push_back (atof (rate_str.c_str()));
    }
  store (new LoadingTable (templates, bases, occupied, rates), name);  
}

void UnitFactory::create_product_table (void)
{
  std::string name = read <std::string> (_line_stream);
  TransformationTable* table = fetch <TransformationTable> (_line_stream);

  store (new ProductTable (*table), name);
}

void UnitFactory::create_transformation_table (void)
{
  std::string name = read <std::string> (_line_stream);
  std::vector <std::string> input_motifs;
  std::vector <std::string> output_motifs; 

  bool next = true;
  while (next)
    {
      input_motifs.push_back (read <std::string> (_line_stream));
      if (input_motifs.back().length() != input_motifs.front().length())
	{ throw ParserException ("Motifs must have same length"); }

      // duplets are separated by commas, if there is no comma
      // then the list should end here
      std::string motif = read <std::string> (_line_stream);
      if (check_comma (motif) == false)
	{
	  char next_char;
	  if (not (_line_stream >> next_char)) { next = false; }
	  else if (next_char != ',') { throw FormatException(); }
	}      
      output_motifs.push_back (motif);
    }
  store (new TransformationTable (input_motifs, output_motifs), name);
}

void UnitFactory::create_free_chemical (void)
{
  std::string name = read <std::string> (_line_stream);
  FreeChemical* chemical = new FreeChemical;
  store (chemical, name);
  chemical->add (read_initial_quantity (_line_stream));
}


void UnitFactory::create_chemical_sequence (void)
{
  std::string name = read <std::string> (_line_stream);

  // check if chemical is defined by sequence or product_of
  std::string keyword = read <std::string> (_line_stream);
  if (keyword == "sequence")
    { 
      ChemicalSequence* chemical = 
	new ChemicalSequence (read <std::string> (_line_stream));
      store (chemical, name); 
      chemical->add (read_initial_quantity (_line_stream));
      return;
    }
  if (keyword != "product_of") { throw FormatException(); }
  
  std::string parent_name = read <std::string> (_line_stream);
  ChemicalSequence* parent = fetch <ChemicalSequence> (parent_name);
  int pos1 = read <int> (_line_stream);
  int pos2 = read <int> (_line_stream);
  std::string table_name = read <std::string> (_line_stream);
  ProductTable* table = fetch <ProductTable> (table_name);
  // check position consistency
  if (parent->is_out_of_bounds (parent->relative (pos1), 
				parent->relative (pos2)))
    {
      std::ostringstream message;
      message << "Product " << name 
	      << " at position [" << pos1 << "," << pos2 << "]"
	      << " is not within bound of " << parent_name;
      throw ParserException (message.str());
    }
  std::string sequence = table->
    generate_child_sequence (*parent, parent->relative (pos1), 
			     parent->relative (pos2));
  if (sequence == "")
    { 
      std::ostringstream message;
      message << "Applying table " << table_name << " on "
	      << parent_name << " between positions " << pos1
	      << " and " << pos2 << " did not yield a valid child sequence,"
	      << " check positions and table content";
      throw ParserException (message.str()); 
    }

  ChemicalSequence* chemical = cell_state().find <ChemicalSequence> (name);
  if (chemical == 0) // chemical does not exist: create it
    { 
      chemical = new ChemicalSequence (sequence, pos1); 
      store (chemical, name);
      chemical->add (read_initial_quantity (_line_stream));
    }
  else // chemical exists: signal it is product of another parent
    {
      // check that product sequence is consistent with previous definitions
      if (sequence != chemical->sequence())
	{
	  std::ostringstream message;
	  message << "product " << name << " defined multiple times,"
		  << " and inferred sequence\n" << sequence
		  << "\ndoes not match previous definition\n"
		  << chemical->sequence();
	  throw ParserException (message.str());
	}
    }
  table->add (*parent, parent->relative (pos1), 
	      parent->relative (pos2), *chemical);
}

void UnitFactory::create_double_strand_sequence (void)
{
  std::string name = read <std::string> (_line_stream);
  std::string sense_name = read <std::string> (_line_stream);
  std::string sequence = read <std::string> (_line_stream);
  ChemicalSequence* sense = new ChemicalSequence (sequence); 
  store (sense, sense_name);
  std::string antisense_name = read <std::string> (_line_stream);
  TransformationTable* table = fetch <TransformationTable> (_line_stream);
  std::string antisequence = 
    table->transform (std::string (sequence.rbegin(), sequence.rend()));
  ChemicalSequence* antisense = new ChemicalSequence (antisequence); 
  store (antisense, antisense_name);
  DoubleStrand* chemical = new DoubleStrand (*sense, *antisense);
  store (chemical, name);
  chemical->add (read_initial_quantity (_line_stream));
}

void UnitFactory::create_bound_chemical (void)
{
  std::string name = read <std::string> (_line_stream);
  store (new BoundChemical, name);
}
