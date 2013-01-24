// Filename: FunctionImplementations.h
/*****************************************************************************/
// Description: This is where each possible function that can be invoked 
// from Matlab is implemented. See MexEntryPoint.cpp to see where these 
// are used.
//
// Author: Mirza A. Shah
/*****************************************************************************/

#ifndef FUNCTION_IMPLEMENTATIONS_H_DEFINED
#define FUNCTION_IMPLEMENTATIONS_H_DEFINED

#include "MatlabHelper.h"

mxArray* MatlabSQLite_Open(int numArgs, const mxArray* args[]);
mxArray* MatlabSQLite_Prepare(int numArgs, const mxArray* args[]);
mxArray* MatlabSQLite_Step(int numArgs, const mxArray* args[]);
mxArray* MatlabSQLite_GetColumnInt(int numArgs, const mxArray* args[]);
mxArray* MatlabSQLite_GetColumnFloat(int numArgs, const mxArray* args[]);
mxArray* MatlabSQLite_GetColumnString(int numArgs, const mxArray* args[]);
mxArray* MatlabSQLite_GetNumColumns(int numArgs, const mxArray* args[]);
mxArray* MatlabSQLite_Finalize(int numArgs, const mxArray* args[]);
mxArray* MatlabSQLite_Close(int numArgs, const mxArray* args[]);

#endif
