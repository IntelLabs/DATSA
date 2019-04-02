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
#ifndef DATA_GENERATION_SCHEDULER_H_
#define DATA_GENERATION_SCHEDULER_H_

#include "data_generator.h"

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
namespace anomaly
{

class data_generator;

//----------------------------------------------------------------------------
// Data generation scheduler interface -- Halide-like extensible schedulers
// 
// data_generation_scheduler is the base class for which all schedulers are
// derived. They are required to implement execute, which is intended to
// generate all the data in whatever way the scheduler deems appropriate.
//
// all needed anomaly information is available from the data_generator that
// is held (protected) in this base class, which they can access directly.
//----------------------------------------------------------------------------
class data_generation_scheduler
{
public:
  
  data_generation_scheduler(data_generator &dg) : dg_(dg) {}
  
  virtual ~data_generation_scheduler() {}
  
  void virtual execute() = 0;
    
protected:
  
  data_generator &dg_;
};

  
} // namespace anomaly


#endif // SCHEDULE_H_






