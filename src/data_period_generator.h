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
#ifndef DATA_PERIOD_GENERATOR_H_
#define DATA_PERIOD_GENERATOR_H_

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
#include <map>
#include <stdlib.h>

#include "data_generator.h"

namespace anomaly
{
  
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
class data_period_generator
{
public:

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------
  data_period_generator(data_generator const &dg)
  {
    for (int i = dg.time_start(); i < dg.time_end(); )
    {
      // do anomalous based on probability
      if (dg.is_anomaly_based_on_probability())
      {
        int duration = dg.generate_anomaly_duration();
        
        if (dg.respect_windows())
        {
          if (duration + i > dg.time_end()) duration = dg.time_end() - i;
        }

        anomaly_periods_[i] = duration;
        
        i += duration;
      }
      else
      {
        int duration = dg.generate_normal_duration();
        
        if (dg.respect_windows())
        {
          if (duration + i > dg.time_end()) duration = dg.time_end() - i;
        }

        normal_periods_[i] = duration;
        
        i += duration;
      }
    }
  } 

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------
  std::map<int, int> & normal_periods() { return normal_periods_; }
  std::map<int, int> & anomaly_periods() { return anomaly_periods_; }

  //--------------------------------------------------------------------------
  //--------------------------------------------------------------------------
  void output_periods()
  {
    using namespace std;

    cout << "normal periods: " << endl;
    int count = 0;
    for (std::map<int, int>::iterator i = normal_periods().begin();
        i != normal_periods().end(); ++i)
    {
      cout << "\t" << ++count << "\t" << i->first << "\t" 
           << i->first + i->second << endl;
    }

    cout << "anomaly periods: " << endl;
    count = 0;
    for (std::map<int, int>::iterator i = anomaly_periods().begin();
        i != anomaly_periods().end(); ++i)
    {
      cout << "\t" << ++count << "\t" << i->first << "\t" 
           << i->first + i->second << endl;
    }
  }

private:
  std::map<int, int> normal_periods_;
  std::map<int, int> anomaly_periods_;
};

} // namespace anomaly

#endif // DATA_SCHEDULE_GENERATOR_H_




