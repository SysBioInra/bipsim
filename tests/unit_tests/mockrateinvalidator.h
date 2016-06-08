
#ifndef MOCK_RATE_INVALIDATOR_H
#define MOCK_RATE_INVALIDATOR_H

#include "rateinvalidator.h"

class MockRateInvalidator : public RateInvalidator
{
 public:
  MockRateInvalidator (RateValidity& rate_validity, int index)
    : RateInvalidator (rate_validity, index)
    , _updated (false) {}
  bool was_updated (void) { return _updated; }
  void update (void) { _updated = true; }
  void reset_update (void) { _updated = false; }

 private:
  bool _updated;
};

#endif // MOCK_RATE_INVALIDATOR_H
