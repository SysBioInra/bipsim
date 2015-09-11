

/**
 * @file eventparser.cpp
 * @brief Implementation of the EventParser class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream> // std::cout, std::cerr
#include <fstream> // std::ifstream
#include <sstream>
#include <string>

// ==================
//  Project Includes
// ==================
//
#include "eventparser.h"
#include "chemicalhandler.h"
#include "addevent.h"
#include "removeevent.h"
#include "setevent.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
EventParser::EventParser (const std::string& event_file, const ChemicalHandler& chemical_handler)
  : _file (event_file.c_str())
  , _chemical_handler (chemical_handler)
  , _line (0)
{
  // check file
  if (_file.fail())
    {
      std::cerr << "Warning: could not open event file\n" << event_file << "\nNo events loaded." << std::endl;
    }
}

Event* EventParser::create_next_event (void)
{
  bool looking_for_next_event = true;
  // parse until next valid event is found
  std::string line;
  std::string event_tag;
  std::string target_name;
  Chemical* target = 0;
  int quantity;
  double time;

  while (looking_for_next_event)
    {
      if (!std::getline (_file, line)) return 0; // end of file -> no event to create
      ++_line;
      
      // check if line is empty or comment
      if (is_empty (line)) continue; // look for event at next line
	

      // check that line is valid
      std::istringstream line_stream (line);
      if (line_stream >> time >> event_tag >> target_name >> quantity)
	{
	  // get target reference
	  if (_chemical_handler.exists (target_name))
	    {
	      target = &_chemical_handler.reference (target_name);
	      // check tag
	      if (is_tag_valid (event_tag))
		{
		  // everythin is valid: we found the next event !
		  looking_for_next_event = false;
		}
	      else
		{
		  std::cerr << "Warning: unrecognized event tag on line " << _line << "\n"
			    << line << "n"
			    << "in event file, event ignored\n";	      		  
		}
	    }
	  else
	    {
	      std::cerr << "Warning: unrecognized chemical on line " << _line << "\n"
			<< line << "n"
			<< "in event file, event ignored\n";	      
	    }
	}
      else // parsing failed
	{
	  std::cerr << "Warning: unrecognized event on line " << _line << "\n"
		    << line << "n"
		    << "in event file, line ignored\n";
	}
    }
  
  // create event and return
  return create_event (time, event_tag, *target, quantity);
}

// Not needed for this class (use of default copy constructor) !
// EventParser::EventParser (const EventParser& other_event_parser);

EventParser::~EventParser (void)
{
}

// ===========================
//  Public Methods - Commands
// ===========================
//


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
// EventParser& EventParser::operator= (const EventParser& other_event_parser);


// =================
//  Private Methods
// =================
//
bool EventParser::is_empty (const std::string& line)
{
  std::istringstream line_stream (line);
  std::string first_word;
  if (!(line_stream >> first_word) // line is empty
      || (first_word [0] == '#')) // line is commented
    {
      return true;
    }

  return false;
}

bool EventParser::is_tag_valid (const std::string& event_tag)
{
  if ((event_tag == "ADD") || (event_tag == "REMOVE") || (event_tag == "SET")) return true;
  else return false;
}

Event* EventParser::create_event (double time, const std::string& event_tag, Chemical& target, int quantity)
{
  REQUIRE (is_tag_valid (event_tag)); /** @pre Tag must already have been checked. */

  if (event_tag == "ADD")
    {
      return new AddEvent (time, target, quantity);
    }
  else if (event_tag == "REMOVE")
    {
      return new RemoveEvent (time, target, quantity);
    }
  else if (event_tag == "SET")
    {
      return new SetEvent (time, target, quantity);
    }
  else
    {
      std::cerr << "Programer note: unvalid tag in routine of EventParser, this should never happen..." << std::endl;
    }
}
