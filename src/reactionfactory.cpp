

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
  : Factory (cell_state)
{
}

// Not needed for this class (use of compiler generated versions)
// ReactionFactory::ReactionFactory (const ReactionFactory& other);
// ReactionFactory& ReactionFactory::operator= (const ReactionFactory& other);
// ReactionFactory::~ReactionFactory (void);

// ===========================
//  Public Methods - Commands
// ===========================
//
bool ReactionFactory::handle (const std::string& line)
{
  // parse the first word and hand the rest of the line over to 
  // appropriate creator
  _line_stream.str (line);
  _line_stream.clear();
  // first word of line must be "reaction"
  if (check_tag (_line_stream, "reaction") == false) { return false; }

  // try to create reaction
  std::string remaining;
  std::string tag = read <std::string> (_line_stream);
  if (tag == "ChemicalReaction") { create_chemical_reaction(); }
  else if (tag == "Complexation") { create_complexation(); }
  else if (tag == "SequenceBinding") { create_sequence_binding(); }
  else if (tag == "Translocation") { create_translocation(); }
  else if (tag == "Release") { create_release(); }
  else if (tag == "Degradation") { create_degradation(); }
  else if (tag == "Loading") { create_loading(); }
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
void ReactionFactory::create_chemical_reaction (void)
{
  std::vector <Chemical*> chemicals;
  std::vector <int> stoichiometries;
  std::string next = read <std::string> (_line_stream);
  while (next != std::string ("rates"))
    {
      chemicals.push_back (fetch <Chemical> (next));
      stoichiometries.push_back (read <int> (_line_stream));
      next = read <std::string> (_line_stream);
    }
  if (chemicals.size() == 0) { throw FormatException(); }
  double k_1 = read <double> (_line_stream);
  double k_m1 = read <double> (_line_stream);

  ChemicalReaction* reaction = new ChemicalReaction (chemicals, stoichiometries,
						     k_1, k_m1);
  store (reaction);
  if (k_1 > 0) { store (new ForwardReaction (*reaction)); }
  if (k_m1 > 0) { store (new BackwardReaction (*reaction)); } 
}

void ReactionFactory::create_loading (void)
{
  Loader* loader = fetch <Loader> (_line_stream);
  store (new Loading (*loader));
}

void ReactionFactory::create_complexation (void)
{
  Chemical* component_a = fetch <Chemical> (_line_stream); 
  Chemical* component_b = fetch <Chemical> (_line_stream); 
  Chemical* complex = fetch <Chemical> (_line_stream); 
  double k_on = read <double> (_line_stream);
  double k_off = read <double> (_line_stream);

  Complexation* reaction = new Complexation (*component_a, *component_b,
					     *complex, k_on, k_off);
  store (reaction);
  if (k_on > 0) { store (new ForwardReaction (*reaction)); }
  if (k_off > 0) { store (new BackwardReaction (*reaction)); } 
}

void ReactionFactory::create_translocation (void)
{
  ProcessiveChemical* processive = fetch <ProcessiveChemical> (_line_stream);
  BoundChemical* chemical_after_step = fetch <BoundChemical> (_line_stream);
  int step_size = read <int> (_line_stream);
  double rate = read <double> (_line_stream);

  store (new Translocation (*processive, *chemical_after_step,
			     step_size, rate));
}

void ReactionFactory::create_sequence_binding (void)
{
  FreeChemical* unit_to_bind = fetch <FreeChemical> (_line_stream);
  BoundChemical* binding_result = fetch <BoundChemical> (_line_stream);
  BindingSiteFamily* family = fetch <BindingSiteFamily> (_line_stream);

  SequenceBinding* reaction = 
    new SequenceBinding (*unit_to_bind, *binding_result, *family);
  store (reaction);
  store (new ForwardReaction (*reaction));
  store (new BackwardReaction (*reaction)); 
}

void ReactionFactory::create_release (void)
{
  BoundChemical* unit_to_release = fetch <BoundChemical> (_line_stream);
  std::vector <Chemical*> chemicals;
  std::vector <int> stoichiometries;
  std::string next = read <std::string> (_line_stream);
  while (next != std::string ("rate"))
    {
      chemicals.push_back (fetch <FreeChemical> (next));
      stoichiometries.push_back (read <int> (_line_stream));
      next = read <std::string> (_line_stream);
    }
  if (chemicals.size() == 0) { throw FormatException(); }
  double rate = read <double> (_line_stream);
  
  // get associated products (if applicable)
  ProductTable* table = 0;
  if (check_tag (_line_stream, "produces"))
    { table = fetch <ProductTable> (_line_stream); }

  store (new Release (*unit_to_release, chemicals,
		       stoichiometries, rate, table));
}


void ReactionFactory::create_degradation (void)
{
  std::string sequence_name = read <std::string> (_line_stream);
  ChemicalSequence* sequence = fetch <ChemicalSequence> (sequence_name);
  std::string table_name = read <std::string> (_line_stream);
  CompositionTable* table = fetch <CompositionTable> (table_name);
  double rate = read <double> (_line_stream);

  // get sequence composition
  std::map <Chemical*, int>
    composition = table->composition (sequence->sequence());
  if (composition.size() == 0)
    { 
      std::ostringstream message;
      message << "Could not compute composition after degradation of "
	      << sequence_name << ", check table " << table_name
	      << " consistency";
      throw ParserException (message.str());
    }

  // create chemical and stoichiometry vector to represent reaction
  std::vector <Chemical*> chemicals (composition.size()+1);
  std::vector <int> stoichiometry (chemicals.size());
  chemicals [0] = sequence; stoichiometry [0] = -1;
  int i = 1;
  for (std::map <Chemical*, int>::iterator comp_it = composition.begin();
       comp_it != composition.end(); ++comp_it, ++i)
    {
      chemicals [i] = comp_it->first; stoichiometry [i] = comp_it->second;
    }

  ChemicalReaction* reaction = 
    new ChemicalReaction (chemicals, stoichiometry, rate, 0);
  store (reaction);
  store (new ForwardReaction (*reaction));
}
