

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

#include "simulatorexception.h"

#include "chemicalreaction.h"
#include "complexation.h"
#include "sequencebinding.h"
#include "loading.h"
#include "release.h"
#include "translocation.h"
#include "forwardreaction.h"
#include "backwardreaction.h"

#include "boundchemical.h"
#include "freechemical.h"
#include "processivechemical.h"
#include "loader.h"
#include "chemicalsequence.h"

#include "cellstate.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
ReactionFactory::ReactionFactory (CellState& cell_state)
  : _cell_state (cell_state)
{
}

// Not needed for this class (use of compiler generated versions)
// ReactionFactory::ReactionFactory (const ReactionFactory& other_factory);
// ReactionFactory& ReactionFactory::operator= (const ReactionFactory& other_factory);
// ReactionFactory::~ReactionFactory (void);

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
			     || create_sequence_binding (remaining)
			     || create_translocation (remaining)
			     || create_release (remaining)
			     || create_degradation (remaining)
			     || create_loading (remaining));
			     
  return creation_succeeded;
}

// ============================
//  Public Methods - Accessors
// ============================
//


// =================
//  Private Methods
// =================
//
bool ReactionFactory::create_chemical_reaction (const std::string& line)
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
      if (chemical_ptr == 0) { throw DependencyException (chemical); }

      if (not (line_stream >> stoichiometry)) { throw FormatException(); }

      chemicals.push_back (chemical_ptr);
      stoichiometries.push_back (stoichiometry);
    }

  // read rates
  double k_1, k_m1;
  if ((chemicals.size() == 0) || (not (line_stream >> k_1 >> k_m1)))
    { throw FormatException(); }

  // create and store
  ChemicalReaction* reaction = new ChemicalReaction (chemicals, stoichiometries,
						     k_1, k_m1);
  _cell_state.store (reaction);
  if (k_1 > 0) { _cell_state.store (new ForwardReaction (*reaction)); }
  if (k_m1 > 0) { _cell_state.store (new BackwardReaction (*reaction)); } 
  return true;
}



bool ReactionFactory::create_loading (const std::string& line)
{
  std::istringstream line_stream (line);
  if (check_tag (line_stream, "Loading") == false) { return false; }

  // read base data
  std::string loader;
  if (not (line_stream >> loader)) { throw FormatException(); }

  // check that the chemicals are known and valid
  Loader* loader_ptr = _cell_state.find <Loader> (loader);
  if (loader_ptr == 0) { throw DependencyException (loader); }
    
  // create and store
  _cell_state.store (new Loading (*loader_ptr));
  return true;
}



bool ReactionFactory::create_complexation (const std::string& line)
{
  std::istringstream line_stream (line);
  if (check_tag (line_stream, "Complexation") == false) { return false; }

  // read base data
  std::string component_a, component_b, complex;
  double k_on, k_off;
  if (not (line_stream >> component_a >> component_b >> complex
	   >> k_on >> k_off))
    { throw FormatException(); }

  // check that the chemicals are known
  Chemical* component_a_ptr = _cell_state.find <Chemical> (component_a); 
  if (component_a_ptr == 0) { throw DependencyException (component_a); }
  Chemical* component_b_ptr = _cell_state.find <Chemical> (component_b); 
  if (component_b_ptr == 0)  { throw DependencyException (component_b); }
  Chemical* complex_ptr = _cell_state.find <Chemical> (complex); 
  if (complex_ptr == 0)  { throw DependencyException (complex); }
  
  // create and store
  Complexation* reaction = new Complexation (*component_a_ptr, 
					     *component_b_ptr,
					     *complex_ptr, k_on, k_off);
  _cell_state.store (reaction);
  if (k_on > 0) { _cell_state.store (new ForwardReaction (*reaction)); }
  if (k_off > 0) { _cell_state.store (new BackwardReaction (*reaction)); } 
  return true;
}



bool ReactionFactory::create_translocation (const std::string& line)
{
  std::istringstream line_stream (line);
  if (check_tag (line_stream, "Translocation") == false) { return false; }

  // read base data
  std::string chemical, second_chemical;
  int step_size ;
  double rate;
  if (not (line_stream >> chemical >> second_chemical >> step_size >> rate))
    { throw FormatException(); }

  // check that the chemicals are valid
  ProcessiveChemical* 
    processive_chemical = _cell_state.find <ProcessiveChemical> (chemical);
  if (processive_chemical == 0)  { throw DependencyException (chemical); }
  BoundChemical* 
    chemical_after_step = _cell_state.find <BoundChemical> (second_chemical);
  if (chemical_after_step == 0)  throw DependencyException (second_chemical);

  // create and store
  _cell_state.store (new Translocation (*processive_chemical,
					*chemical_after_step,
					step_size, rate));
  return true;
}



bool ReactionFactory::create_sequence_binding (const std::string& line)
{
  std::istringstream line_stream (line);
  if (check_tag (line_stream, "SequenceBinding") == false) { return false; }

  // read base data
  std::string unit_to_bind, binding_result, binding_site;
  if (not (line_stream >> unit_to_bind >> binding_result >> binding_site))
    { throw FormatException(); }

  // check that the data is valid
  FreeChemical* 
    unit_to_bind_ptr = _cell_state.find <FreeChemical> (unit_to_bind);
  if (unit_to_bind_ptr == 0)  { throw DependencyException (unit_to_bind); }
  BoundChemical*
    binding_result_ptr = _cell_state.find <BoundChemical> (binding_result);
  if (binding_result_ptr == 0)  { throw DependencyException (binding_result); }
  BindingSiteFamily* family_ptr = 
    _cell_state.find <BindingSiteFamily> (binding_site);
  if (family_ptr == 0) { throw DependencyException (binding_site); }

  // create and store
  SequenceBinding* reaction = 
    new SequenceBinding (*unit_to_bind_ptr, *binding_result_ptr, *family_ptr);
  _cell_state.store (reaction);
  _cell_state.store (new ForwardReaction (*reaction));
  _cell_state.store (new BackwardReaction (*reaction)); 
  return true;
}

bool ReactionFactory::create_release (const std::string& line)
{
  std::istringstream line_stream (line);
  if (check_tag (line_stream, "Release") == false) { return false; }

  // read base data
  std::vector <Chemical*> chemicals;
  std::vector <int> stoichiometries;
  std::string chemical;
  int stoichiometry;

  // get the chemical to release
  if (not (line_stream >> chemical)) { throw FormatException(); }

  BoundChemical* unit_to_release = _cell_state.find <BoundChemical> (chemical);
  if (unit_to_release == 0)  { throw DependencyException (chemical); }

  // get other components of the reaction
  while ((line_stream >> chemical) && (chemical != std::string ("rate")))
    {
      FreeChemical* chemical_ptr = _cell_state.find <FreeChemical> (chemical);
      if (chemical_ptr == 0)  { throw DependencyException (chemical); }

      if (not (line_stream >> stoichiometry)) { throw FormatException(); }

      chemicals.push_back (chemical_ptr);
      stoichiometries.push_back (stoichiometry);
    }

  // get rates
  double rate;
  if ((chemicals.size() == 0) || (not (line_stream >> rate)))
    { throw FormatException(); }
  
  // get associated products (if applicable)
  ProductTable* table = 0;
  if (check_tag (line_stream, "produces"))
    {
      std::string table_name;
      if (not (line_stream >> table_name)) { throw FormatException(); }
      table = _cell_state.find <ProductTable> (table_name);
      if (table == 0) { throw DependencyException (table_name); } // throw error
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
    { throw FormatException(); }

  // check that the names are valid
  ChemicalSequence* 
    sequence = _cell_state.find <ChemicalSequence> (sequence_name);
  if (sequence == 0) { throw DependencyException (sequence_name); }
  CompositionTable* 
    table = _cell_state.find <CompositionTable> (table_name);
  if (table == 0) { throw DependencyException (table_name); }
  
  // get sequence composition
  std::map <Chemical*, int>
    composition = table->composition (sequence->sequence());
  int num_el = composition.size();
  if (num_el == 0)
    { 
      std::ostringstream message;
      message << "Could not compute composition after degradation of "
	      << sequence_name << ", check table " << table_name
	      << " consistency";
      throw ParserException (message.str());
    }

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
  ChemicalReaction* reaction = new ChemicalReaction (chemicals, stoichiometry,
						     rate, 0);
  _cell_state.store (reaction);
  _cell_state.store (new ForwardReaction (*reaction));
  return true;
}
