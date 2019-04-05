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
#ifndef BASIC_SCHEDULER_H_
#define BASIC_SCHEDULER_H_

#include "data_generation_scheduler.h"
#include <iostream>

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
namespace anomaly
{
  
//----------------------------------------------------------------------------
// Basic (single-threaded) data generation scheduler - zero optimizations 
//----------------------------------------------------------------------------
class basic_scheduler : public data_generation_scheduler
{
public:

  basic_scheduler(data_generator &dg) : data_generation_scheduler(dg) {}

  //--------------------------------------------------------------------------
  // generate data for the entire range
  //--------------------------------------------------------------------------
  virtual void execute()
  {
    for (int i = dg_.time_start(); i < dg_.time_end(); )
    {
      anomaly::e_data_types type = 
        dg_.is_anomaly_based_on_probability() ?
        kAnomalyDataType : kNormalDataType;

      int duration = dg_.generate_duration_based_on_type(type);
      
      //----------------------------------------------------------------------
      // if "respect_windows" is enabled, it means that if a time_step will
      // exceed the chosen bounds, it will be retracted to fit into the
      // window size chosen by the system.
      //----------------------------------------------------------------------
      if (dg_.respect_windows())
      {
        if (duration + i > dg_.time_end()) duration = dg_.time_end() - i;
      }

      //----------------------------------------------------------------------
      // generate data for one anomalous or non-anomalous event
      //----------------------------------------------------------------------
      dg_.generate_data_entry(i, duration, type, 
        dg_.variables(), dg_.generated_data(), dg_.time_step());

      i += duration;
    }    
  }
  
private:
  
  // protectedly inherit: 
  // data_generator &dg_;

};

} // namespace anomaly


#endif //BASIC_SCHEDULER_H_


