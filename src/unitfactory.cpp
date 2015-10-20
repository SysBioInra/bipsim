

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

#include "site.h"
#include "bindingsite.h"
#include "bindingsitefamily.h"

#include "chemical.h"
#include "boundchemical.h"
#include "processivechemical.h"
#include "baseloader.h"
#include "chemicalsequence.h"

#include "decodingtable.h"
#include "producttable.h"
#include "transformationtable.h"

#include "cellstate.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
UnitFactory::UnitFactory (CellState& cell_state)
  : _cell_state (cell_state)
{
}

// Not needed for this class (use of default copy constructor) !
// UnitFactory::UnitFactory (const UnitFactory& other_unit_factory);

UnitFactory::~UnitFactory (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//
bool UnitFactory::handle (const std::string& line)
{
  // parse the first word and hand the rest of the line over to 
  // appropriate creator
  std::istringstream line_stream (line);  
  // first word of line must be "unit"
  if (check_tag (line_stream, "unit") == false) { return false; }

  // try to create unit
  // (c++ is "clever", whenever a function returns true, remaining functions are
  // not evaluated because true || anything = true)
  std::string remaining;
  std::getline (line_stream, remaining);
  bool creation_succeeded = (create_binding_site (remaining)
			     || create_chemical (remaining)
			     || create_termination_site (remaining)
			     || create_bound_chemical (remaining)
			     || create_chemical_sequence (remaining)
			     || create_processive_chemical (remaining)
			     || create_base_loader (remaining)
			     || create_decoding_table (remaining)
			     || create_product_table (remaining)
			     || create_transformation_table (remaining));

  // if (creation_succeeded == false)  { TODO throw error !!! }
  return creation_succeeded;
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
// UnitFactory& UnitFactory::operator= (const UnitFactory& other_unit_factory);


// =================
//  Private Methods
// =================
//
bool UnitFactory::create_binding_site (const std::string& line)
{
  std::istringstream line_stream (line);  
  if (check_tag (line_stream, "BindingSite") == false) { return false; }

  // read base data
  std::string family_name, location;
  int start, end;
  double k_on, k_off;
  if ( not (line_stream >> family_name >> location >> start >> end
	    >> k_on >> k_off) )
    {  
      // TODO throw error
      return false;
    }

  // look for location
  ChemicalSequence* sequence = _cell_state.find <ChemicalSequence> (location);
  if (sequence == 0) { return false; } // TODO throw error ?

  // check position consistency
  if (end < start)
    {
      std::cerr << "Site of family " << family_name 
		<< " at position [" << start << "," << end << "]:"
		<< "starting position is smaller than end..." << std::endl;
      return false;
    }
  if (sequence->is_out_of_bounds (start, end-start+1))
    {
      std::cerr << "Site of family " << family_name 
		<< " at position [" << start << "," << end << "]"
		<< " is not within bound of " << location << std::endl;
      return false;
    }

  // get family ref/id (create family if necessary)
  BindingSiteFamily* family =
    _cell_state.find <BindingSiteFamily> (family_name);
  if (family == 0)
    {
      family = new BindingSiteFamily;
      _cell_state.store (family, family_name);
    }
  int family_id = _cell_state.find_id (family_name);

  // read reading frame (optional)
  int reading_frame = 0;
  BindingSite* binding_site;
  if (not (line_stream >> reading_frame)) // no reading frame
    { binding_site = new BindingSite (family_id, *sequence, start, end-start+1,
				      k_on, k_off); }
  else
    { binding_site = new BindingSite (family_id, *sequence, start, end-start+1,
				      k_on, k_off, reading_frame); }

  // add created unit to family and cell state
  family->add (binding_site);
  _cell_state.store (binding_site);
  return true;
}

bool UnitFactory::create_termination_site (const std::string& line)
{
  std::istringstream line_stream (line);  
  if (check_tag (line_stream, "TerminationSite") == false) { return false; }

  // read base data
  std::string family_name, location;
  int start, end;
  if (not (line_stream >> family_name >> location >> start >> end))
    {
      // TODO throw error
      return false;
    }

  // check location
  ChemicalSequence* sequence = _cell_state.find <ChemicalSequence> (location);
  if (sequence == 0) return false; // TODO throw error ?

  // get family ref/id (create family if necessary)
  SiteFamily* family = _cell_state.find <SiteFamily> (family_name);
  if (family == 0)
    {
      family = new SiteFamily;
      _cell_state.store (family, family_name);
    }
  int family_id = _cell_state.find_id (family_name);

  // create and store entity
  Site* site = new Site (family_id, *sequence, start, end-start+1);
  family->add (site);
  sequence->add_termination_site (*site);
  _cell_state.store (site);
  return true;
}

bool UnitFactory::create_decoding_table (const std::string& line)
{
  std::istringstream line_stream (line);  
  if (check_tag (line_stream, "DecodingTable") == false) { return false; }

  // read base data
  std::string name, template_, base, polymerase;
  double rate;
  std::list <std::string> template_list;
  std::list <Chemical*> base_list;
  std::list <BoundChemical*> polymerase_list;
  std::list <double> rate_list;
  if (not (line_stream >> name)) 
    {
      // TODO throw error
      return false;
    }

  while (line_stream >> template_ >> base >> polymerase >> rate)
    {
      // we check whether the base and polymerase are already known
      Chemical* base_ptr = _cell_state.find <Chemical> (base);
      if (base_ptr == 0) return false; // TODO throw error ?
      BoundChemical* polymerase_ptr = _cell_state.find <BoundChemical> (polymerase);
      if (polymerase_ptr == 0) return false; // TODO throw error ?

      // add line to lists
      template_list.push_back (template_);
      base_list.push_back (base_ptr);
      rate_list.push_back (rate);
      polymerase_list.push_back (polymerase_ptr);
    }

  if (template_list.size() == 0) return false; // TODO throw error

  // create and store table
  std::list<std::string>::const_iterator template_it = template_list.begin();
  std::list<Chemical*>::const_iterator base_it = base_list.begin();
  std::list<BoundChemical*>::const_iterator polymerase_it = polymerase_list.begin();
  std::list<double>::const_iterator rate_it = rate_list.begin();
  int template_length = template_it->size();
  DecodingTable* table = new DecodingTable (template_length);
  while (template_it != template_list.end())
    {
      if (template_it->size() == template_length)
	{
	  table->add_template (*template_it, **base_it, **polymerase_it, *rate_it);
	}
      else
	{
	  std::cerr << "ERROR: trying to define a decoding table with templates of variable "
		    << "length." << std::endl;
	}
      template_it++; base_it++; polymerase_it++; rate_it++;
    }
  _cell_state.store (table, name);
  return true;
}

bool UnitFactory::create_product_table (const std::string& line)
{
  std::istringstream line_stream (line);  
  if (check_tag (line_stream, "ProductTable") == false) { return false; }

  // read base data
  std::string name, table_name;
  if (not (line_stream >> name >> table_name)) 
    {
      // TODO throw error
      return false;
    }

  TransformationTable* table =
    _cell_state.find <TransformationTable> (table_name);
  if (table == 0) return false; // TODO throw error ?

  _cell_state.store (new ProductTable (*table), name);
  return true;
}

bool UnitFactory::create_transformation_table (const std::string& line)
{
  std::istringstream line_stream (line);
  if (check_tag (line_stream, "TransformationTable") == false) { return false; }

  // read base data
  std::string name, input_motif, output_motif;
  if (not (line_stream >> name)) 
    {
      // TODO throw error
      return false;
    }

  TransformationTable* table = 0;
  if (line_stream >> input_motif >> output_motif)
    {
      // create table
      table = new TransformationTable (input_motif.size()); 
      table->add_rule (input_motif, output_motif);
    }
  else
    {
      // TODO throw error
      return false;
    }
  
  while (line_stream >> input_motif >> output_motif)
    {
      if (input_motif.size() == table->input_motif_length())
	{ table->add_rule (input_motif, output_motif); }
      else
	{
	  // TODO throw error
	  delete table;
	  return false;
	}
    }

  // store table
  _cell_state.store (table, name);
  return true;
}


bool UnitFactory::create_chemical ( const std::string& line )
{
  std::istringstream line_stream (line);  
  if (check_tag (line_stream, "Chemical") == false) { return false; }

  // read base data
  std::string name;
  if (not (line_stream >> name))
    {
      // TODO throw error
      return false;
    }

  int initial_quantity;
  if (not (line_stream >> initial_quantity)) { initial_quantity = 0; }

  // create and store
  Chemical* chemical = new Chemical;
  chemical->add (initial_quantity);
  _cell_state.store (chemical, name);
  return true;
}


bool UnitFactory::create_chemical_sequence ( const std::string& line )
{
  std::istringstream line_stream (line);  
  if (check_tag (line_stream, "ChemicalSequence") == false) { return false; }

  // read base data
  std::string name, keyword;
  if ( not (line_stream >> name >> keyword) )
    {
      // TODO throw error
      return false;
    }

  // check if chemical is defined by sequence or product_of
  std::string sequence;
  ChemicalSequence* chemical = 0;
  if (keyword == "sequence")
    {
      if (not(line_stream >> sequence)) { return false; } // TODO throw error
      chemical = new ChemicalSequence (sequence);
    }
  else if (keyword == "product_of")
    {
      // gather parent
      std::string parent_name, table_name; 
      int pos1, pos2;
      if (not(line_stream >> parent_name >> pos1 >> pos2 >> table_name))
	{ return false; } // TODO throw error

      ChemicalSequence* parent = 
	_cell_state.find <ChemicalSequence> (parent_name);
      if (parent == 0) { return false; } // temporarily

      ProductTable* table =
	_cell_state.find <ProductTable> (table_name);
      if (table == 0) { return false; }

      // check position consistency
        if (parent->is_out_of_bounds (pos1, pos2-pos1+1))
	  {
	    std::cerr << "Product " << name 
		      << " at position [" << pos1 << "," << pos2 << "]"
		      << " is not within bound of " << parent_name << std::endl;
	    return false;
	  }

      // check whether sequence is already listed
      chemical = _cell_state.find <ChemicalSequence> (name);
      if (chemical != 0)
	{
	  // if it is we simply need to signal that it is also
	  // the product of another parent
	  sequence = table->generate_child_sequence (*parent, pos1, pos2);
	  if (sequence != chemical->sequence())
	    {
	      std::cerr << "product " << name << " defined multiple times,"
			<< " and inferred sequence\n" << sequence
			<< "\ndoes not match previous definition\n"
			<< chemical->sequence() << std::endl;
	      return false;
	    }
	  table->add (*parent, pos1, pos2, *chemical);
	  return true;
	}
      
      sequence = table->generate_child_sequence (*parent, pos1, pos2);
      if (sequence == "") { return false; } // TODO throw error
      chemical = new ChemicalSequence (sequence, pos1);
      table->add (*parent, pos1, pos2, *chemical);
    }
  else { return false; } // TODO throw error

  int initial_quantity;
  if (not (line_stream >> initial_quantity)) { initial_quantity = 0; }
  chemical->add (initial_quantity);

  _cell_state.store (chemical, name);
  return true;
}


bool UnitFactory::create_bound_chemical ( const std::string& line )
{
  std::istringstream line_stream (line);  
  if (check_tag (line_stream, "BoundChemical") == false) { return false; }

  // read base data
  std::string name;
  if (not (line_stream >> name))
    {
      // TODO throw error
      return false;
    }

  int initial_quantity;
  if (not (line_stream >> initial_quantity)) { initial_quantity = 0; }

  BoundChemical* chemical = new BoundChemical;
  chemical->add (initial_quantity);
  _cell_state.store (chemical, name);
  return true;
}

bool UnitFactory::create_base_loader ( const std::string& line )
{
  std::istringstream line_stream (line);  
  if (check_tag (line_stream, "BaseLoader") == false) { return false; }

  // read base data
  std::string name, decoding_table;
  if ( not (line_stream >> name >> decoding_table) )
    {
      // TODO throw error
      return false;
    }

  // check whether the decoding table is known and valid
  DecodingTable* decoding_table_ptr = _cell_state.find <DecodingTable> (decoding_table);
  if (decoding_table_ptr == 0) return false; // TODO throw error ?

  _cell_state.store (new BaseLoader (*decoding_table_ptr), name);
  return true;
}


bool UnitFactory::create_processive_chemical ( const std::string& line )
{
  std::istringstream line_stream (line);  
  if (check_tag (line_stream, "ProcessiveChemical") == false) { return false; }

  // read base data
  std::string name, stalled_name;
  if ( not (line_stream >> name >> stalled_name) )
    {
      // TODO throw error
      return false;
    }

  // check whether the stalled form is known
  BoundChemical* stalled = _cell_state.find <BoundChemical> (stalled_name);
  if (stalled == 0) return false; // TODO throw error ?

  // parse termination sites
  std::string site_name;
  std::list <int> site_ids;
  while (line_stream >> site_name)
    {
      // check whether termination site is already known
      int site_id = _cell_state.find_id (site_name);
      if (site_id != CellState::NOT_FOUND) { site_ids.push_back (site_id);}
      else return false; // TODO throw error ?
    }
  
  // create and store
  ProcessiveChemical* proc_chemical = new ProcessiveChemical (*stalled);
  for (std::list <int>::iterator site_it = site_ids.begin();
       site_it != site_ids.end(); ++site_it)
    { proc_chemical->add_recognized_termination_site (*site_it); }
  _cell_state.store (proc_chemical, name);
  return true;
}
