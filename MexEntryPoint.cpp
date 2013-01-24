// Filename: MexEntryPoint.h
/*****************************************************************************/
// Description: The mex function enters at void mexFunction(...) Rather than
// create a mex function for each cpp function we want matlab clients to have
// access to, a single function is provided (called "MatlabSQLite") where the
// user passes as the first argument a string indicating the function they want
// invoked.
//
// Author: Mirza A. Shah
/*****************************************************************************/

#include "FunctionImplementations.h"
#include <string>
using std::string;

namespace CppFunctionNames
{
    const string CFN_OPENDB          = "OPEN";
    const string CFN_PREPARE         = "PREPARE";
    const string CFN_STEP            = "STEP";
    const string CFN_GETCOLUMNINT    = "GETCOLUMNINT";
    const string CFN_GETCOLUMNFLOAT  = "GETCOLUMNFLOAT";
    const string CFN_GETCOLUMNSTRING = "GETCOLUMNSTRING";
    const string CFN_GETNUMCOLUMNS   = "GETNUMCOLUMNS";
    const string CFN_FINALIZE        = "FINALIZE";
    const string CFN_CLOSE           = "CLOSE";
};

/*******************************************/
static mxArray* InvokeAppropriateCppFunction(string functionName, int numArgs, const mxArray* args[])
/*******************************************/
{
    if(functionName == CppFunctionNames::CFN_OPENDB)
        return(MatlabSQLite_Open(numArgs - 1, args));
    else if (functionName == CppFunctionNames::CFN_PREPARE)
        return(MatlabSQLite_Prepare(numArgs - 1, args));
    else if (functionName == CppFunctionNames::CFN_STEP)
        return(MatlabSQLite_Step(numArgs - 1, args));
    else if (functionName == CppFunctionNames::CFN_GETCOLUMNINT)        
        return(MatlabSQLite_GetColumnInt(numArgs - 1, args));
    else if (functionName == CppFunctionNames::CFN_GETCOLUMNFLOAT)
        return(MatlabSQLite_GetColumnFloat(numArgs - 1, args));
    else if (functionName == CppFunctionNames::CFN_GETCOLUMNSTRING)
        return(MatlabSQLite_GetColumnString(numArgs - 1, args));
    else if (functionName == CppFunctionNames::CFN_GETNUMCOLUMNS)
        return(MatlabSQLite_GetNumColumns(numArgs - 1, args));
    else if (functionName == CppFunctionNames::CFN_FINALIZE)
        return(MatlabSQLite_Finalize(numArgs - 1, args));
    else if (functionName == CppFunctionNames::CFN_CLOSE)
        return(MatlabSQLite_Close(numArgs - 1, args));
    else
    {
        SafePrint("Error: Matlab client attempting to invoke non-existent function " + functionName + ".\n");
        return(Bool_Cpp2Matlab(false));
    }
}

/*******************************************/
extern "C" void mexFunction(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
/*******************************************/
{
    //Verify parameter count
    if(nrhs < 1)
    {
        SafePrint("Not enough parameters, need at least 1 string parameter to indicate C++ function name.\n");
        plhs[0] = Bool_Cpp2Matlab(false);
        return;
    }

    //Determine function user wants to invoke
    string functionName = String_Matlab2Cpp(prhs[0]);

    //Handle user request, remember that plhs[0] is the return value
    plhs[0] = InvokeAppropriateCppFunction(functionName, nrhs, prhs);
}
