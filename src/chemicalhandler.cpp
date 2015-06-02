

/**
 * @file chemicalhandler.cpp
 * @brief Implementation of the ChemicalHandler class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream>

// ==================
//  Project Includes
// ==================
//
#include "chemicalhandler.h"
#include "chemical.h"
#include "boundchemical.h"
#include "chemicalsequence.h"
#include "processivechemical.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
ChemicalHandler::ChemicalHandler (void)
  : _last_id (NOT_FOUND)
  , _last_reference (0)
{
}

// Not needed for this class (use of default copy constructor) !
// ChemicalHandler::ChemicalHandler ( const ChemicalHandler& other_chemical_handler );

ChemicalHandler::~ChemicalHandler (void)
{
  std::map< int, Chemical* >::iterator chemical = _references.begin();
  while ( chemical != _references.end() )
    {
      delete chemical->second;
      chemical++;
    }
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void ChemicalHandler::create_chemical (std::string chemical_name, int initial_quantity /*= 0*/)
{
  // create chemical if it does not already exist and append it to the reference map
  if ( not _identifiers.exists (chemical_name) )
    {
      // get a new id for the entity
      _identifiers.create_id (chemical_name);
      _last_id = _identifiers.id (chemical_name);

      // create it and store the reference
      Chemical* new_chemical= new Chemical;
      new_chemical->add (initial_quantity);
      _references [_last_id] = new_chemical;
      _last_reference = new_chemical;
    }
}

void ChemicalHandler::create_bound_chemical (std::string chemical_name, int initial_quantity /*= 0*/)
{
  // create bound chemical if it does not already exist and append it to the reference map
  if ( not _identifiers.exists (chemical_name) )
    {
      // get a new id for the entity
      _identifiers.create_id (chemical_name);
      _last_id = _identifiers.id (chemical_name);

      // create it and store the reference
      BoundChemical* new_chemical= new BoundChemical;
      new_chemical->add (initial_quantity);
      _references [_last_id] = new_chemical;
      _last_reference = new_chemical;
    }  
}

void ChemicalHandler::create_processive_chemical (std::string chemical_name, std::string stalled_name, int initial_quantity /*= 0*/)
{
  // create processive chemical if it does not already exist and append it to the reference map
  if ( not _identifiers.exists (chemical_name) )
    {
      // get a new id for the entity
      _identifiers.create_id (chemical_name);
      _last_id = _identifiers.id (chemical_name);

      // load the stalled form
      int stalled_id = _identifiers.id (stalled_name);
      /** @pre Stalled form must be defined already. */
      REQUIRE( stalled_id != IdentifiedList::NOT_FOUND );
      BoundChemical* stalled_form = dynamic_cast< BoundChemical* > (_references[stalled_id]);
      /** @pre Stalled form must be a bound chemical. */
      REQUIRE( stalled_form != 0 );

      // create the entity and store reference
      ProcessiveChemical* new_chemical= new ProcessiveChemical (*stalled_form);
      new_chemical->add (initial_quantity);
      _references [_last_id] = new_chemical;
      _last_reference = new_chemical;
    }
}

void ChemicalHandler::create_chemical_sequence (std::string chemical_name, int length, int initial_quantity /*= 0*/)
{
  // create bound chemical if it does not already exist and append it to the reference map
  if ( not _identifiers.exists (chemical_name) )
    {
      // get a new id for the entity
      _identifiers.create_id (chemical_name);
      _last_id = _identifiers.id (chemical_name);

      // create it and store the reference
      ChemicalSequence* new_chemical= new ChemicalSequence;
      new_chemical->set_length (length);
      new_chemical->add (initial_quantity);
      _references [_last_id] = new_chemical;
      _last_reference = new_chemical;
    }
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
// ChemicalHandler& ChemicalHandler::operator= ( const ChemicalHandler& other_chemical_handler );

std::ostream& operator<< (std::ostream& output, const ChemicalHandler& chemical_handler)
{
  for (std::map<int, Chemical*>::const_iterator next_reference = chemical_handler._references.begin();
       next_reference != chemical_handler._references.end();
       next_reference++ )
    {
      output << chemical_handler._identifiers.name (next_reference->first) << ": "
	     << *(next_reference->second) << std::endl;
    }
  
  return output;
}

// ==================================
//  Public Methods - Class invariant
// ==================================
//
/**
 * Checks all the conditions that must remain true troughout the life cycle of
 * every object.
 */
bool ChemicalHandler::check_invariant (void) const
{
  bool result = true;
  return result;
}


// =================
//  Private Methods
// =================
//
