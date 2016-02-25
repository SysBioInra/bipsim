

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
#include "processivechemical.h"
#include "loader.h"
#include "chemicalsequence.h"
#include "doublestrand.h"

#include "compositiontable.h"
#include "decodingtable.h"
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
  // first word of line must be "unit"
  if (check_tag (_line_stream, "unit") == false) { return false; }

  // try to create unit
  std::string tag = read <std::string> (_line_stream);
  if (tag == "BindingSite") { create_binding_site(); }
  else if (tag == "TerminationSite") { create_termination_site(); }
  else if (tag == "FreeChemical") { create_free_chemical(); }
  else if (tag == "BoundChemical") { create_bound_chemical(); }
  else if (tag == "ChemicalSequence") { create_chemical_sequence(); }
  else if (tag == "DoubleStrandSequence") { create_double_strand_sequence(); }
  else if (tag == "ProcessiveChemical") { create_processive_chemical(); }
  else if (tag == "Loader") { create_loader(); }
  else if (tag == "CompositionTable") { create_composition_table(); }
  else if (tag == "DecodingTable") { create_decoding_table(); }
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
  int reading_frame = 0;
  BindingSite* binding_site;
  if (not (_line_stream >> reading_frame)) // no reading frame
    { 
      binding_site = new BindingSite (*family, *location, 
				      location->relative (start), 
				      location->relative (end),
				      k_on, k_off); 
    }
  else
    { 
      binding_site = new BindingSite (*family, *location, 
				      location->relative (start), 
				      location->relative (end),
				      k_on, k_off, 
				      location->relative (reading_frame)); 
    }
  store (binding_site);
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
  CompositionTable* table = fetch_or_create <CompositionTable> (_line_stream);
  
  // read letter and associated chemicals
  char letter = read <char> (_line_stream);
  std::list <Chemical*> chemicals;
  chemicals.push_back (fetch <FreeChemical> (_line_stream));
  std::string chemical_name;
  while (_line_stream >> chemical_name)
    { chemicals.push_back (fetch <FreeChemical> (chemical_name)); }

  table->add_rule (letter, chemicals);
}


void UnitFactory::create_decoding_table (void)
{
  std::string name = read <std::string> (_line_stream);
  std::list <std::string> template_list;
  std::list <Chemical*> base_list;
  std::list <BoundChemical*> polymerase_list;
  std::list <double> rate_list;

  std::string template_, base, polymerase; double rate;
  while (_line_stream >> template_ >> base >> polymerase >> rate)
    {
      template_list.push_back (template_);
      base_list.push_back (fetch <FreeChemical> (base));
      polymerase_list.push_back (fetch <BoundChemical> (polymerase));
      rate_list.push_back (rate);
    }
  if (template_list.size() == 0) { throw FormatException(); }

  // create and store table
  std::list<std::string>::iterator template_it = template_list.begin();
  std::list<Chemical*>::iterator base_it = base_list.begin();
  std::list<BoundChemical*>::iterator polymerase_it = polymerase_list.begin();
  std::list<double>::iterator rate_it = rate_list.begin();
  int template_length = template_it->size();
  DecodingTable* table = new DecodingTable (template_length);
  while (template_it != template_list.end())
    {
      if (template_it->size() == template_length)
	{
	  table->add_template (*template_it, **base_it,
			       **polymerase_it, *rate_it);
	}
      else
	{
	  delete table;
	  throw ParserException ("Trying to define a decoding table with "
				 "templates of variable length.");
	}
      template_it++; base_it++; polymerase_it++; rate_it++;
    }
  store (table, name);
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
  std::string input_motif = read <std::string> (_line_stream);
  std::string output_motif = read <std::string> (_line_stream);

  TransformationTable* table = new TransformationTable (input_motif.size()); 
  table->add_rule (input_motif, output_motif);
  while (_line_stream >> input_motif >> output_motif)
    {
      if (input_motif.size() == table->input_motif_length())
	{ table->add_rule (input_motif, output_motif); }
      else
	{ delete table; throw FormatException(); }
    }
  store (table, name);
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
      store (new ChemicalSequence (read <std::string> (_line_stream))); 
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

void UnitFactory::create_loader (void)
{
  std::string name = read <std::string> (_line_stream);
  DecodingTable* decoding_table = fetch <DecodingTable> (_line_stream);
  store (new Loader (*decoding_table), name);
}


void UnitFactory::create_processive_chemical (void)
{
  std::string name = read <std::string> (_line_stream);
  BoundChemical* stalled = fetch <BoundChemical> (_line_stream);
  
  // parse termination sites
  std::string family;
  std::list <SiteFamily*> families;
  while (_line_stream >> family)
    { families.push_back (fetch <SiteFamily> (family)); }
	
  ProcessiveChemical* chemical = new ProcessiveChemical (*stalled);
  for (std::list <SiteFamily*>::iterator family_it = families.begin();
       family_it != families.end(); ++family_it)
    { chemical->add_recognized_termination_site (**family_it); }
  store (chemical, name);
}
