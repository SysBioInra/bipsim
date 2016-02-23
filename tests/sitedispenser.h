
#ifndef SITE_DISPENSER_H
#define SITE_DISPENSER_H

#include <vector>
#include <map>
#include <cmath>

#include "../src/bindingsitefamily.h"
#include "../src/bindingsite.h"
#include "../src/chemicalsequence.h"

class MockBindingSite : public BindingSite
{
 public:
  MockBindingSite (BindingSiteFamily& bsf, ChemicalSequence& cs,
		   int first, int last)
    : BindingSite (bsf, cs, first, last, 1, 1)
    , _updated (false) {}
  bool was_updated (void) { return _updated; }
  void update (void) { _updated = true; }
  void reset_update (void) { _updated = false; }

 private:
  bool _updated;
};

class SiteDispenser
{
 public:
  SiteDispenser (int sequence_length)
    : _cs (std::string (sequence_length, 'a'))
    {}

  ~SiteDispenser (void) 
  {
    for (std::vector <BindingSite*>::iterator site_it = _sites.begin();
	 site_it != _sites.end(); ++site_it)
      { delete *site_it; }
  }

  MockBindingSite& new_site (int first, int last) 
  {
    MockBindingSite* mbs = new MockBindingSite (_bsf, _cs, first, last);
    mbs->reset_update();
    _sites.push_back (mbs);
    return *mbs;
  }

 private:
  BindingSiteFamily _bsf;
  ChemicalSequence _cs;
  std::vector <BindingSite*> _sites;
};

#endif // SITE_DISPENSER_H
