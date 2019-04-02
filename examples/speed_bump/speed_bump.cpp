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
#include "../../src/function_variable.h"
#include "../../src/event_data.h"
#include <iomanip>
#include <cmath>

using namespace anomaly;

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
static double speed_bump_normal_function(event_data const &)
{
  double LO = 8.0, HI = 11.0;
  double data = 
    (static_cast<double>(rand())/static_cast<double>(RAND_MAX)) * (HI-LO)+LO;

  return data;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
static double speed_bump_anomaly_function(event_data const &)
{
  int MULT = 1;

  if(rand() % 2) MULT = -MULT;

  double LO = 8.0, HI = 11.0;
  double data = 
    (static_cast<double>(rand())/static_cast<double>(RAND_MAX)) * (HI-LO)+LO;
  double perturb = 3.0 * MULT;

  return double(data + perturb);
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
static data_list gen_speed_bump_function()
{
  int time_start = 1, time_end = 100001;

  int normal_lowerbound_duration = 50;
  int normal_upperbound_duration = 100;

  int anomaly_lowerbound_duration = 200;
  int anomaly_upperbound_duration = 300;

  long prob_num = 2;
  long prob_den = 100;

  data_generator data_gen(time_start, time_end, normal_lowerbound_duration,
    normal_upperbound_duration, anomaly_lowerbound_duration,
    anomaly_upperbound_duration, prob_num, prob_den);

  function_variable *v = new function_variable("amplitude");
  v->normal_function(&speed_bump_normal_function);
  v->anomaly_function(&speed_bump_anomaly_function);
  data_gen.insert_variable(v);

  //data_gen.enable_multithreaded_data_generation();
  data_gen.generate_data();
  return data_gen.generated_data();
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void speed_bump_function()
{
  data_list data = gen_speed_bump_function();

  //--------------------------------------------------------------------------
  // print out the header information
  //--------------------------------------------------------------------------
  std::cout << "label" << "\t" << "time-stamp" << "\t";

  std::map<std::string, double>::iterator it 
    = data.begin()->variable_values().begin();

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
