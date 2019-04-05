/*

MIT License

Copyright 2018, Intel Corporation

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in 
the Software without restriction, including without limitation the rights to 
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
of the Software, and to permit persons to whom the Software is furnished to do 
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all 
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
SOFTWARE.

Justin Gottschlich, Intel Labs (justin.gottschlich@intel.com)

*/

//----------------------------------------------------------------------------
#ifndef INTEGER_PROBABILITY_H_
#define INTEGER_PROBABILITY_H_

#include <stdlib.h>

//----------------------------------------------------------------------------
// within the anomaly namespace
//----------------------------------------------------------------------------
namespace anomaly
{
  
//----------------------------------------------------------------------------
// class that calculates the probability of an event provided an integer for
// the numerator and denominator.
//----------------------------------------------------------------------------
class integer_probability
{
public:

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------
  integer_probability(long const numerator, long const denominator)
  :
  numerator_(numerator),
  denominator_(denominator)
  {}

  //--------------------------------------------------------------------------
  // roll - determines if event has occurred. if so, it returns true,
  // otherwise false.
  //--------------------------------------------------------------------------
  bool roll() const
  {
    //------------------------------------------------------------------------
    // roll on the denominator - if the result is smaller than or equal to
    // the denominator, the event occurred.
    //------------------------------------------------------------------------
    int random = 1 + (rand() % denominator_);
    return random <= numerator_ ? true : false;
  }

private:
  
  long numerator_;
  long denominator_;

};

} //namespace anomaly

#endif // INTEGER_PROBABILITY_H_

