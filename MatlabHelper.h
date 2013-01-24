#ifndef MATLAB_HELPER_H_DEFINED
#define MATLAB_HELPER_H_DEFINED

extern "C" 
{
#include "mex.h"
}
#include <string>
#include <sstream>
using std::string;
using std::ostringstream;

//Converts a C++ bool to a matlab double scalar where 1.0 = true, 0.0 = false
mxArray*          Bool_Cpp2Matlab(bool val);

//Builds a 1x2 cell array with 2 doubles
mxArray*          Build1x2DoubleCellArray(double v1, double v2);

//Converts a C++ double to a Matlab double
mxArray*          Double_Cpp2Matlab(double val);

//Verifies a passed Matlab value is indeed a Matlab string
bool              IsMatlabString(const mxArray* val);

//Thread safe print so Matlab doesn't die
void              SafePrint(const string& s);
void              SafePrint(const ostringstream& s);

//Converts a matlab string into a C++ std::string
string            String_Matlab2Cpp(const mxArray* s);

//Converts a C++ std string to a Matlab string
mxArray*          String_Cpp2Matlab(const string& s);

//Verifies that the number of input parameters == number passed. If so returns true, otherwise false.
//If too few parameters, then function will inform user.
bool              VerifyNumberOfInputParameters(int numberExpected, int numberPassed);

#endif