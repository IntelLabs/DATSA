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

*/

//----------------------------------------------------------------------------
#ifndef STOCHASTIC_DURATION_H_
#define STOCHASTIC_DURATION_H_

#include <stdlib.h>

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
namespace anomaly
{
  inline const int random(int const x) { return x < 1 ? 0 : rand() % (x+1); }

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
class stochastic_duration
{
public:
  
  stochastic_duration(int lowerbound, int upperbound) :
  lowerbound_(lowerbound),
  upperbound_(upperbound)
  {}
  
  void set_lowerbound(int const rhs) { lowerbound_ = rhs; }
  void set_upperbound(int const rhs) { upperbound_ = rhs; }
  
  int generate_stochastic_duration() const
  {
    return random(upperbound() - lowerbound()) + lowerbound();
  }
 
private:

  int lowerbound() const { return lowerbound_; }
  int upperbound() const { return upperbound_; }

  int lowerbound_;
  int upperbound_; 
};

}//namespace anomaly

#endif //STOCHASTIC_DURATION_H_

