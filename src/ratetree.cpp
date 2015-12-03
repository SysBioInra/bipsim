

/**
 * @file ratetree.cpp
 * @brief Implementation of the RateTree class.
 * 
 * @authors Marc Dinh, Stephan Fischer
 */


// ==================
//  General Includes
// ==================
//
#include <iostream> // std::cout
#include <queue> // std::queue

// ==================
//  Project Includes
// ==================
//
#include "ratetree.h"
#include "sumnode.h"
#include "dummynode.h"

// ==========================
//  Constructors/Destructors
// ==========================
//
RateTree::RateTree (int number_rates)
  // care: number of nodes can be larger than number of rates because of dummy
  // nodes !!! Twice the number of rates is a very generous overestimate.
  : _update_queue (2*number_rates) 
{
  // first create leaves
  _leaves.reserve (number_rates);
  std::queue <RateNode*>* current_layer = new std::queue <RateNode*>;
  for (int i = 0; i < number_rates; ++i)
    {
      ReactionNode* node = new ReactionNode (i);
      _nodes.push_back (node);
      _leaves.push_back (node);
      current_layer->push (node);
    }

  // create parents from leaves
  std::queue <RateNode*>* next_layer = new std::queue <RateNode*>;
  std::queue <RateNode*>* tmp = 0;
  RateNode* left, *right;
  while (current_layer->size() > 1)
    {
      while (current_layer->empty() == false)
	{
	  left = current_layer->front();
	  current_layer->pop();
	  if (current_layer->empty() == false)
	    {
	      right = current_layer->front();
	      current_layer->pop();
	      _nodes.push_back (new SumNode (*left, *right));
	      left->set_parent (_nodes.back());
	      right->set_parent (_nodes.back());
	    }
	  else // only one element left
	    {
	      _nodes.push_back (new DummyNode (*left));
	      left->set_parent (_nodes.back());
	    }
	  next_layer->push (_nodes.back());
	}
      tmp = current_layer;
      current_layer = next_layer;
      next_layer = tmp;
    }
  delete current_layer;
  delete next_layer;
  _root = _nodes.back();
}

// Not needed for this class (use of default copy constructor) !
// RateTree::RateTree (const RateTree& other_class_name);

RateTree::~RateTree (void)
{
  for (std::list<RateNode*>::iterator node_it = _nodes.begin();
       node_it != _nodes.end(); ++node_it)
    { delete *node_it; }
}

// ===========================
//  Public Methods - Commands
// ===========================
//
void RateTree::update_cumulates (void)
{
  while (_update_queue.end() == false)
    {
      RateNode* next = _update_queue.item()->update();
      if (next != 0) { _update_queue.push (next); }
      _update_queue.next();
    }
  _update_queue.clear();
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
// RateTree& RateTree::operator= (const RateTree& other_class_name);


// =================
//  Private Methods
// =================
//
