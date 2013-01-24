#include "MatlabHelper.h"
#include <cassert>
#include <vector>
#include <string>
#include <sstream>
using std::vector;
using std::string;
using std::ostringstream;

/*******************************************************/
mxArray* Bool_Cpp2Matlab(bool val)
/*******************************************************/
{
    return(mxCreateDoubleScalar((val ? 1.0 : 0.0)));
}

/**********************************************************/
mxArray* Build1x2DoubleCellArray(double v1, double v2)
/**********************************************************/
{
    mwSize dims[] = {1,2};
    mxArray* toReturn = mxCreateCellArray(2, dims);
    mxSetCell(toReturn, 0, Double_Cpp2Matlab(v1));
    mxSetCell(toReturn, 1, Double_Cpp2Matlab(v2));
    return(toReturn);
}

/*******************************************************/
mxArray* Double_Cpp2Matlab(double val)
/*******************************************************/
{
    return(mxCreateDoubleScalar(val));
}
/*******************************************************/
bool IsMatlabString(const mxArray* val)
/*******************************************************/
{
    /* input must be a string and must be a row vector*/
    return((mxIsChar(val) == 1) && (mxGetM(val) ==1));
}

/*******************************************************/
void SafePrint(const string& s)
/*******************************************************/
{
    //TODO: Protect this with mutex or semaphore
    mexPrintf("%s",s.c_str());
}

/*******************************************************/
void SafePrint(const ostringstream& s)
/*******************************************************/
{
    SafePrint(s.str());
}

/*******************************************************/
mxArray* String_Cpp2Matlab(const string& s)
/*******************************************************/
{
    return(mxCreateString(s.c_str()));
}

/*******************************************************/
string String_Matlab2Cpp(const mxArray* s)
/*******************************************************/
{
    assert(IsMatlabString(s));

    /* get the length of the input string */
    int buflen = (mxGetM(s) * mxGetN(s)) + 1;

    /* copy the string data from s into a C string input_ buf.    */
    char* input_buf = mxArrayToString(s);
    
    if(input_buf == NULL)
    {
        mexErrMsgTxt("Could not convert input to string.");
        throw;
    }

    string toReturn(input_buf);
    mxFree(input_buf);
    return(toReturn);
}

/*******************************************************/
bool VerifyNumberOfInputParameters(int numberExpected, int numberPassed)
/*******************************************************/
{
    if(numberExpected != numberPassed)
    {
        mexPrintf("Incorrect number of parameters passed, %i expected but %i passed\n", numberExpected, numberPassed);
        return(false);
    }
    return(true);
}