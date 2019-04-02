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
#include <iomanip>
#include "random_vars/random_vars.h"
#include "function_vars/function_vars.h"
#include "sine/sine.h"
#include "space_shuttle/space_shuttle.h"
#include "speed_bump/speed_bump.h"
#include "ecg/ecg.h"
#include "nary/nary_anomaly.h"
#include "step/step_anomaly.h"
#include "exact/exact_anomaly.h"

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
int main()
{
  srand(time(NULL));

  example_exact_anomaly();
  //example_step_anomaly();
  //example_nary_anomaly();
  //example_random_vars();
  //example_function_vars();
  //example_space_shuttle();
  //example_sine();
  //example_ecg();

  return 0;
}
