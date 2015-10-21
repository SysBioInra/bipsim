

/**
 * @file reactionfactory.cpp
 * @brief Implementation of the ReactionFactory class.
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
#include "reactionfactory.h"

#include "chemicalreaction.h"
#include "complexation.h"
#include "binding.h"
#include "baseloading.h"
#include "release.h"
#include "elongation.h"

#include "cellstate.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
ReactionFactory::ReactionFactory (CellState& cell_state)
  : _cell_state (cell_state)
{
}

// Not needed for this class (use of default copy constructor) !
// ReactionFactory::ReactionFactory (const ReactionFactory& other_factory);

ReactionFactory::~ReactionFactory (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//
bool ReactionFactory::handle (const std::string& line)
{
  // parse the first word and hand the rest of the line over to 
  // appropriate creator
  std::istringstream line_stream (line);
  // first word of line must be "reaction"
  if (check_tag (line_stream, "reaction") == false) { return false; }


  // try to create reaction
  // (c++ is "clever", whenever a function returns true, remaining functions are
  // not evaluated because true || anything = true)
  std::string remaining;
  std::getline (line_stream, remaining);
  bool creation_succeeded = (create_chemical_reaction (remaining)
			     || create_complexation (remaining)
			     || create_binding (remaining)
			     || create_elongation (remaining)
			     || create_release (remaining)
			     || create_degradation (remaining)
			     || create_base_loading (remaining));
			     
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
// ReactionFactory& ReactionFactory::operator= (const ReactionFactory& other_factory);


// =================
//  Private Methods
// =================
//
bool ReactionFactory::create_chemical_reaction ( const std::string& line )
{
  std::istringstream line_stream (line);
  if (check_tag (line_stream, "ChemicalReaction") == false) { return false; }

  // read base data
  std::vector <Chemical*> chemicals;
  std::vector <int> stoichiometries;
  std::string chemical;
  int stoichiometry;
  while ( (line_stream >> chemical) && (chemical != std::string ("rates")) )
    {
      // check that the chemical is known
      Chemical* chemical_ptr = _cell_state.find <Chemical> (chemical);
      if (chemical_ptr == 0) return false; // TODO throw error ?

      if (not (line_stream >> stoichiometry))
	{
	  // TODO throw error
	  return false;
	}

      chemicals.push_back (chemical_ptr);
      stoichiometries.push_back (stoichiometry);
    }

  // read rates
  double k_1, k_m1;
  if ((chemicals.size() == 0) || (not (line_stream >> k_1 >> k_m1)))
    {
      // TODO throw error
      return false;
    }

  // create and store
  _cell_state.store (new ChemicalReaction (chemicals, stoichiometries,
					   k_1, k_m1));
  return true;
}



bool ReactionFactory::create_base_loading ( const std::string& line )
{
  std::istringstream line_stream (line);
  if (check_tag (line_stream, "BaseLoading") == false) { return false; }

  // read base data
  std::string base_loader;
  if (not (line_stream >> base_loader))
    {
      // TODO throw error
      return false;
    }

  // check that the chemicals are known and valid
  BaseLoader* base_loader_ptr = _cell_state.find <BaseLoader> (base_loader);
  if (base_loader_ptr == 0) return false; // TODO throw error ?
    
  // create and store
  _cell_state.store (new BaseLoading (*base_loader_ptr));
  return true;
}



bool ReactionFactory::create_complexation ( const std::string& line )
{
  std::istringstream line_stream (line);
  if (check_tag (line_stream, "Complexation") == false) { return false; }

  // read base data
  std::string component_a, component_b, complex;
  double k_on, k_off;
  if (not (line_stream >> component_a >> component_b >> complex
	   >> k_on >> k_off))
    {
      // TODO throw error
      return false;
    }

  // check that the chemicals are known
  Chemical* component_a_ptr = _cell_state.find <Chemical> (component_a); 
  if (component_a_ptr == 0) return false; // TODO throw error ?
  Chemical* component_b_ptr = _cell_state.find <Chemical> (component_b); 
  if (component_b_ptr == 0) return false; // TODO throw error ?
  Chemical* complex_ptr = _cell_state.find <Chemical> (complex); 
  if (complex_ptr == 0) return false; // TODO throw error ?
  
  // create and store
  _cell_state.store (new Complexation (*component_a_ptr, *component_b_ptr,
				       *complex_ptr, k_on, k_off));
  return true;
}



bool ReactionFactory::create_elongation ( const std::string& line )
{
  std::istringstream line_stream (line);
  if (check_tag (line_stream, "Elongation") == false) { return false; }

  // read base data
  std::string chemical, second_chemical;
  int step_size ;
  double rate;
  if (not (line_stream >> chemical >> second_chemical >> step_size >> rate))
    {
      // TODO throw error
      return false;
    }

  // check that the chemicals are valid
  ProcessiveChemical* 
    processive_chemical = _cell_state.find <ProcessiveChemical> (chemical);
  if (processive_chemical == 0) return false; // TODO throw error ?
  BoundChemical* 
    chemical_after_step = _cell_state.find <BoundChemical> (second_chemical);
  if (chemical_after_step == 0) return false; // TODO throw error ?

  // create and store
  _cell_state.store (new Elongation (*processive_chemical, *chemical_after_step,
				     step_size, rate));
  return true;
}



bool ReactionFactory::create_binding ( const std::string& line )
{
  std::istringstream line_stream (line);
  if (check_tag (line_stream, "Binding") == false) { return false; }

  // read base data
  std::string unit_to_bind, binding_result, binding_site;
  if (not (line_stream >> unit_to_bind >> binding_result >> binding_site))
    {
      // TODO throw error
      return false;
    }

  // check that the data is valid
  Chemical* unit_to_bind_ptr = _cell_state.find <Chemical> (unit_to_bind);
  if (unit_to_bind_ptr == 0) return false; // TODO throw error ?
  BoundChemical*
    binding_result_ptr = _cell_state.find <BoundChemical> (binding_result);
  if (binding_result_ptr == 0) return false; // TODO throw error ?
  int binding_site_family = _cell_state.find_id (binding_site);
  if (binding_site_family == CellState::NOT_FOUND) return false; // TODO throw error ?

  // create and store
  _cell_state.store (new Binding (*unit_to_bind_ptr, *binding_result_ptr,
				  *(_cell_state.find <BindingSiteFamily> (binding_site)),
				  binding_site_family));
  return true;
}

bool ReactionFactory::create_release ( const std::string& line )
{
  std::istringstream line_stream (line);
  if (check_tag (line_stream, "Release") == false) { return false; }

  // read base data
  std::vector <Chemical*> chemicals;
  std::vector <int> stoichiometries;
  std::string chemical;
  int stoichiometry;

  // get the chemical to release
  if (not (line_stream >> chemical)) 
    {
      // TODO throw error
      return false;
    }
  BoundChemical* unit_to_release = _cell_state.find <BoundChemical> (chemical);
  if (unit_to_release == 0) return false; // TODO throw error ?

  // get other components of the reaction
  while ((line_stream >> chemical) && (chemical != std::string ("rate")))
    {
      Chemical* chemical_ptr = _cell_state.find <Chemical> (chemical);
      if (chemical_ptr == 0) return false; // TODO throw error ?

      if (not (line_stream >> stoichiometry))
	{
	  // TODO throw error
	  return false;
	}

      chemicals.push_back (chemical_ptr);
      stoichiometries.push_back (stoichiometry);
    }

  // get rates
  double rate;
  if ((chemicals.size() == 0) || (not (line_stream >> rate)))
    {
      // TODO throw error
      return false;
    }
  
  // get associated products (if applicable)
  ProductTable* table = 0;
  if (check_tag (line_stream, "produces"))
    {
      std::string table_name;
      if (not (line_stream >> table_name)) { return false; } // throw error
      table = _cell_state.find <ProductTable> (table_name);
      if (table == 0) { return false; } // throw error
    }

  // create and store
  _cell_state.store (new Release (*unit_to_release, chemicals,
				  stoichiometries, rate, table));
  return true;
}


bool ReactionFactory::create_degradation (const std::string& line)
{
  std::istringstream line_stream (line);
  if (check_tag (line_stream, "Degradation") == false) { return false; }

  // read base data
  std::string sequence_name, table_name;
  double rate;
  if (not (line_stream >> sequence_name >> table_name >> rate))
    {
      // TODO throw error
      return false;
    }

  // check that the names are valid
  ChemicalSequence* 
    sequence = _cell_state.find <ChemicalSequence> (sequence_name);
  if (sequence == 0) return false; // TODO throw error ?
  CompositionTable* 
    table = _cell_state.find <CompositionTable> (table_name);
  if (table == 0) return false; // TODO throw error ?
  
  // get sequence composition
  std::map <Chemical*, int>
    composition = table->composition (sequence->sequence());
  int num_el = composition.size();
  if (num_el == 0) { return false; } // TODO throw error

  // create chemical and stoichiometry vector to represent reaction
  std::vector <Chemical*> chemicals (num_el+1);
  std::vector <int> stoichiometry (num_el+1);
  chemicals [0] = sequence;
  stoichiometry [0] = -1;
  int i = 1;
  for (std::map <Chemical*, int>::iterator comp_it = composition.begin();
       comp_it != composition.end(); ++comp_it, ++i)
    {
      chemicals [i] = comp_it->first;
      stoichiometry [i] = comp_it->second;
    }

  // create and store
  _cell_state.store (new ChemicalReaction (chemicals, stoichiometry, rate, 0));
  return true;
}
