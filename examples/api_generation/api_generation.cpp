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
#include <sstream>

using namespace anomaly;

std::string const kProgramNumberStr = "prog#";
int const kPowerOfTenToPad = 3;
int const kProgramLength = 12;
int const kMaxAPIs = 3;
int const kValueLowerbound = 1;
int const kValueUpperbound = 1000;

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
static double program_api(event_data const &event)
{
  // randomly pick one of the API ids
  return double(rand() % kMaxAPIs);
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
static double program_value(event_data const &event)
{
  // randomly pick a value in the specified value range
  return double(rand() % (kValueUpperbound - kValueLowerbound));
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
static std::string pad_int(int const val)
{
  std::string str;
  
  for (int i = 1; i < kPowerOfTenToPad; ++i)
  {
    if (val < (pow(10, i))) str += "0"; 
  }
  
  str += std::to_string(val);
  
  return str;
} 

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
static void add_variables_to_data_generator(data_generator& data_gen, 
  int program_length)
{
  //--------------------------------------------------------------------------
  // now add as many paired variables (API, values) as is necessary
  // for the program length -- right now, each API assumes only one
  // associated value. This can be changed by adding more "var" values
  //--------------------------------------------------------------------------
  for (int i = 0; i < program_length; ++i)
  {
    std::string api_str = pad_int(i) + "_a";
    function_variable *api = new function_variable(api_str);
    api->normal_function(&program_api);
    api->anomaly_function(&program_api);
    data_gen.insert_variable(api);
    
    std::string val_str = pad_int(i) + "_v";
    function_variable *val = new function_variable(val_str);
    val->normal_function(&program_value);
    val->anomaly_function(&program_value);
    data_gen.insert_variable(val);
  }
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
static data_list generate_programs()
{
  int first_program = 1, until_program = 100;

  // basically, don't let any anomalies happen for now - force one range
  // of non-anomalous programs to exist over the whole segment of programs
  int normal_lowerbound = until_program;
  int normal_upperbound = until_program;
  
  // create a range of 1, just in case
  int anomaly_lowerbound = 1;
  int anomaly_upperbound = 1;

  long prob_numerator = 1;
  long prob_denominator = 10000000;

  data_generator data_gen(first_program, until_program, normal_lowerbound,
    normal_upperbound, anomaly_lowerbound, anomaly_upperbound, prob_numerator, 
    prob_denominator);

  add_variables_to_data_generator(data_gen, kProgramLength);
  
  data_gen.generate_data();  
  return data_gen.generated_data();
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
static std::stringstream&& output_programs(data_list &data)
{
  std::stringstream out;
  
  //--------------------------------------------------------------------------
  // print out the header information
  //--------------------------------------------------------------------------
  out << kProgramNumberStr << "\t";
  
  auto it = data.begin()->variable_values().begin();

  // don't output the "label" column
  for (; it != data.begin()->variable_values().end(); ++it)
  {
    out << it->first << "\t";
  }
  out << std::endl;

  //--------------------------------------------------------------------------
  // print out the actual generated data
  //--------------------------------------------------------------------------
  data_list::iterator i = data.begin();
  
  for (; i != data.end(); ++i)
  {
    out << std::setprecision(0);
    out << i->time_stamp();
    
    std::map<std::string, double>::iterator it = i->variable_values().begin();
    
    out << std::setprecision(0);
    for (; it != i->variable_values().end(); ++it)
    {
      out << ",\t";
      out << std::fixed << it->second;
    }
    out << std::setprecision(0);
    
    out << std::endl;
  }
  
  return out;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void api_program_generation()
{
  data_list data = generate_programs();
  
  std::cout << output_programs(data).str();  
}






