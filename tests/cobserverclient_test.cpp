/**
 * @file cobserverclient_test.cpp
 * @brief Unit testing for CObserverClient class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream> // std::cerr
#include <cstdlib> // EXIT_SUCCESS EXIT_FAILURE

#include "../src/cobserverclient.h"
#include "../src/chemical.h"

class Dummy : public CObserverClient
{
 public:
  Dummy (void) : _last_message (-1) {}
  void update (int message) { _last_message = message; }
  int last_message (void) { return _last_message; }
  void add_observer (Chemical& chemical, int msg)
  { CObserverClient::add_observer (chemical, msg); }

 private:
  int _last_message;
};

// ==================
//  Project Includes
// ==================
//
#define FAILURE(msg) {std::cerr << "TEST FAILED: " << msg << std::endl; return EXIT_FAILURE;}


int main (int argc, char *argv[])
{ 
  Chemical A, B;
  int A_msg = 0;
  int B_msg = 1;
  Dummy dummy;
  dummy.add_observer (A, A_msg);
  dummy.add_observer (B, B_msg);

  A.add (1);
  if (dummy.last_message() != A_msg)
    { FAILURE ("Observer did not notify correct value."); }

  B.add (1);
  if (dummy.last_message() != B_msg)
    { FAILURE ("Observer did not notify correct value."); }

  return EXIT_SUCCESS;
}
