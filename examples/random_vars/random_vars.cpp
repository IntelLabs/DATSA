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
#include "../../src/data_generator.h"
#include "../../src/random_variable.h"
#include "../../src/event_data.h"
#include <iomanip>

using namespace anomaly;

//----------------------------------------------------------------------------
//
// return rand() % anomaly_prob == 0;
// 
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
data_list gen_example_random_vars()
{
  int time_start = 1, time_end = 10000;

  int normal_lowerbound_duration = 20;
  int normal_upperbound_duration = 20;

  int anomaly_lowerbound_duration = 10;
  int anomaly_upperbound_duration = 10;

  long prob_num = 1;
  long prob_den = 2;

  data_generator data_gen(time_start, time_end, normal_lowerbound_duration,
    normal_upperbound_duration, anomaly_lowerbound_duration,
    anomaly_upperbound_duration, prob_num, prob_den);

  random_variable *v = new random_variable("x");

  v->insert_normal_range(1000, 2000);
  v->insert_anomaly_range(1, 10);

  data_gen.anomaly_label(1.0);
  data_gen.normal_label(0.0);

  data_gen.insert_variable(v);
  
  //data_gen.normal_uniqueness().set_kind(kHardUniqueness);
  //data_gen.normal_uniqueness().set_max_failures(100);

  //data_gen.anomaly_uniqueness().set_kind(kHardUniqueness);
  //data_gen.anomaly_uniqueness().set_max_failures(100);
  
  data_gen.generate_data();
  return data_gen.generated_data();
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void example_random_vars()
{
  data_list data = gen_example_random_vars();
  
  //--------------------------------------------------------------------------
  // print out the header information
  //--------------------------------------------------------------------------
  std::cout << "label" << "\t" << "time-stamp" << "\t";
  
  std::map<std::string, double>::iterator it = data.begin()->variable_values().begin();

  for (; it != data.begin()->variable_values().end(); ++it)
  {
    std::cout << it->first << "\t";
  }
  std::cout << std::endl;

  //--------------------------------------------------------------------------
  // print out the actual generated data
  //--------------------------------------------------------------------------
  data_list::iterator i = data.begin();
  
  for (; i != data.end(); ++i)
  {
    std::cout << i->label() << ",\t";
    std::cout << i->time_stamp();
    
    std::map<std::string, double>::iterator it = i->variable_values().begin();
    
    std::cout << std::setprecision(4);
    for (; it != i->variable_values().end(); ++it)
    {
      std::cout << ",\t";
      std::cout << std::fixed << it->second;
    }
    std::cout << std::setprecision(0);
    
    std:: cout << std::endl;
  }  
}









