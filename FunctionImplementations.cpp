#ifndef FUNCTION_IMPLEMENTATIONS_H_DEFINED
#define FUNCTION_IMPLEMENTATIONS_H_DEFINED

#include "FunctionImplementations.h"
#include "MatlabHelper.h"
#include "SQLiteSimpleWrapper.h"
#include <map>
using std::map;

/****************************************************************/
/****************************************************************/
//This section is to enable persistence of pointers used by SQLite
//on C side.
/****************************************************************/
/****************************************************************/

typedef map<int, sqlite3*>       DatabaseHandleMap;
typedef map<int, sqlite3_stmt*>  PreparedStatementHandleMap;

static DatabaseHandleMap          g_databaseHandles;
static PreparedStatementHandleMap g_preparedStatementHandles;
static int                        g_databaseId = 0;
static int                        g_preparedStatementId = 0;

int NextDatabaseID(){return(++g_databaseId);}
int NextPreparedStatementID(){return(++g_preparedStatementId);}
sqlite3* GetDatabasePointer(int id){ return(g_databaseHandles[id]);}
sqlite3_stmt* GetPreparedStatementPointer(int id){return(g_preparedStatementHandles[id]);}

void SetDatabasePointer(int id, sqlite3* dbHandle)
{
   sqlite3* current = g_databaseHandles[id];
   //TODO: Delete/close current?
   g_databaseHandles[id] = dbHandle;   
}

void SetPreparedStatementPointer(int id, sqlite3_stmt* psHandle)
{
   sqlite3_stmt* current = g_preparedStatementHandles[id];
   //TODO: Delete/close current?
   g_preparedStatementHandles[id] = psHandle;
}

int SetDatabasePointer(sqlite3* dbHandle){int id = NextDatabaseID(); SetDatabasePointer(id, dbHandle); return(id);}
int SetPreparedStatementPointer(sqlite3_stmt* psHandle){int id = NextPreparedStatementID(); SetPreparedStatementPointer(id, psHandle); return(id);}

/****************************************************************/
/****************************************************************/
//END PERSISTENT POINTER CODE
/****************************************************************/
/****************************************************************/

/****************************************************************/
mxArray* MatlabSQLite_Open(int numArgs, const mxArray* args[])
/****************************************************************/
{   
    //Arg1 = dbFilename
    if(!VerifyNumberOfInputParameters(1, numArgs))
    {
        SafePrint("MatlabSQLite_Open Error: Incorrect number of arguments passed.\n");
        return(Double_Cpp2Matlab(-1));
    }

    if(!IsMatlabString(args[1]))
    {
        SafePrint("MatlabSQLite_Open Error: First argument (database filename) is not a Matlab string\n");
        return(Double_Cpp2Matlab(-1));
    }

    string dbFilename = String_Matlab2Cpp(args[1]);        
    
    try
    {
        return(Double_Cpp2Matlab(SetDatabasePointer(SQLite_OpenDB(dbFilename))));
    }
    catch(SQLiteException e)
    {
        SafePrint("MatlabSQLite_Open Error: " + e.Error() + "\n");
        return(Double_Cpp2Matlab(-1));
    }
}

/****************************************************************/
mxArray* MatlabSQLite_Prepare(int numArgs, const mxArray* args[])
/****************************************************************/
{
    //Arg1 = dbHandle, Arg2 = sqlExp
    if(!VerifyNumberOfInputParameters(2, numArgs))
    {
        SafePrint("MatlabSQLite_Prepare Error: Incorrect number of arguments passed.\n");
        return(Double_Cpp2Matlab(-1));
    }
   
    sqlite3* dbHandle = GetDatabasePointer((int)(mxGetScalar(args[1])));       
    string   sqlStmt  = String_Matlab2Cpp(args[2]);

    if(dbHandle == NULL)
    {
        SafePrint("MatlabSQLite_Prepare Error: Null database handle.\n");
        return(Double_Cpp2Matlab(-1));
    }

    try
    {
        sqlite3_stmt* preparedStmt = SQLite_Prepare(dbHandle, sqlStmt);
        return(Double_Cpp2Matlab(SetPreparedStatementPointer(preparedStmt)));
    }
    catch(SQLiteException e)
    {
        SafePrint("MatlabSQLite_Prepare Error: " + e.Error() + "\n");
        return(Double_Cpp2Matlab(-1));
    }
}

/****************************************************************/
mxArray* MatlabSQLite_Step(int numArgs, const mxArray* args[])
/****************************************************************/
{
    //Arg1 - preparedStmt
    if(!VerifyNumberOfInputParameters(1, numArgs))
    {
        SafePrint("MatlabSQLite_Step Error: Incorrect number of arguments passed.\n");
        return(Bool_Cpp2Matlab(false));
    }

    sqlite3_stmt* preparedStmtHandle = GetPreparedStatementPointer((int)(mxGetScalar(args[1])));       
    
    try
    {
        return(Bool_Cpp2Matlab(SQLite_Step(preparedStmtHandle)));
    }
    catch(SQLiteException e)
    {
        SafePrint("MatlabSQLite_Step Error: " + e.Error() + "\n");
        return(Double_Cpp2Matlab(-1));
    }
}

/****************************************************************/
mxArray* MatlabSQLite_GetColumnInt(int numArgs, const mxArray* args[])
/****************************************************************/
{
    //Arg1 - preparedStmt, Arg2 = colIndex
    if(!VerifyNumberOfInputParameters(2, numArgs))
    {
        SafePrint("MatlabSQLite_GetColumnInt Error: Incorrect number of arguments passed.\n");
        return(Bool_Cpp2Matlab(false));
    }

    sqlite3_stmt* preparedStmtHandle = GetPreparedStatementPointer((int)(mxGetScalar(args[1])));       
    int           index              = (int)(mxGetScalar(args[2]));

    try
    {
        return(Double_Cpp2Matlab(SQLite_Column_Integer(preparedStmtHandle, index)));
    }
    catch(SQLiteException e)
    {
        SafePrint("MatlabSQLite_GetColumnInt Error: " + e.Error() + "\n");
        return(Double_Cpp2Matlab(0));
    }

}

/****************************************************************/
mxArray* MatlabSQLite_GetColumnFloat(int numArgs, const mxArray* args[])
/****************************************************************/
{
    //Arg1 - preparedStmt, Arg2 = colIndex
    if(!VerifyNumberOfInputParameters(2, numArgs))
    {
        SafePrint("MatlabSQLite_GetColumnFloat Error: Incorrect number of arguments passed.\n");
        return(Bool_Cpp2Matlab(false));
    }

    sqlite3_stmt* preparedStmtHandle = GetPreparedStatementPointer((int)(mxGetScalar(args[1])));       
    int           index              = (int)(mxGetScalar(args[2]));

    try
    {
        return(Double_Cpp2Matlab(SQLite_Column_Float(preparedStmtHandle, index)));
    }
    catch(SQLiteException e)
    {
        SafePrint("MatlabSQLite_GetColumnFloat Error: " + e.Error() + "\n");
        return(Double_Cpp2Matlab(0));
    }}

/****************************************************************/
mxArray* MatlabSQLite_GetColumnString(int numArgs, const mxArray* args[])
/****************************************************************/
{
    //Arg1 - preparedStmt, Arg2 = colIndex
    if(!VerifyNumberOfInputParameters(2, numArgs))
    {
        SafePrint("MatlabSQLite_GetColumnString Error: Incorrect number of arguments passed.\n");
        return(Bool_Cpp2Matlab(false));
    }

    sqlite3_stmt* preparedStmtHandle = GetPreparedStatementPointer((int)(mxGetScalar(args[1])));       
    int           index              = (int)(mxGetScalar(args[2]));

    try
    {
        return(String_Cpp2Matlab(SQLite_Column_Text(preparedStmtHandle, index)));
    }
    catch(SQLiteException e)
    {
        SafePrint("MatlabSQLite_GetColumnString Error: " + e.Error() + "\n");
        return(Double_Cpp2Matlab(0));
    }
}

/****************************************************************/
mxArray* MatlabSQLite_GetNumColumns(int numArgs, const mxArray* args[])
/****************************************************************/
{
    //Arg1 - preparedStmt
    if(!VerifyNumberOfInputParameters(1, numArgs))
    {
        SafePrint("MatlabSQLite_GetNumColumns Error: Incorrect number of arguments passed.\n");
        return(Bool_Cpp2Matlab(false));
    }

    sqlite3_stmt* preparedStmtHandle = GetPreparedStatementPointer((int)(mxGetScalar(args[1])));       

    try
    {
        return(Double_Cpp2Matlab(SQLite_NumColumns(preparedStmtHandle)));
    }
    catch(SQLiteException e)
    {
        SafePrint("MatlabSQLite_GetNumColumns Error: " + e.Error() + "\n");
        return(Double_Cpp2Matlab(0));
    }
}

/****************************************************************/
mxArray* MatlabSQLite_Finalize(int numArgs, const mxArray* args[])
/****************************************************************/
{
    //Arg1 - preparedStmt
    if(!VerifyNumberOfInputParameters(1, numArgs))
    {
        SafePrint("MatlabSQLite_Finalize Error: Incorrect number of arguments passed.\n");
        return(Bool_Cpp2Matlab(false));
    }

    int stmtId = (int)(mxGetScalar(args[1]));
    sqlite3_stmt* preparedStmtHandle = GetPreparedStatementPointer(stmtId);
    SetPreparedStatementPointer(stmtId, NULL);

    try
    {
        SQLite_Finalize(preparedStmtHandle);        
    }
    catch(SQLiteException e)
    {
        SafePrint("MatlabSQLite_Finalize Error: " + e.Error() + "\n");       
        return(Double_Cpp2Matlab(-1));
    }

    return(Double_Cpp2Matlab(1));
}

/****************************************************************/
mxArray* MatlabSQLite_Close(int numArgs, const mxArray* args[])
/****************************************************************/
{
    //Arg1 - dbHandle
    if(!VerifyNumberOfInputParameters(1, numArgs))
    {
        SafePrint("MatlabSQLite_Close Error: Incorrect number of arguments passed.\n");
        return(Bool_Cpp2Matlab(false));
    }
    
    int dbID = (int)(mxGetScalar(args[1]));
    sqlite3* dbHandle = GetDatabasePointer(dbID);     
    SetDatabasePointer(dbID, NULL);
            
    try
    {        
        SQLite_CloseDB(dbHandle);        
    }
    catch(SQLiteException e)
    {
        SafePrint("MatlabSQLite_Close Error: " + e.Error() + "\n");       
        return(Double_Cpp2Matlab(-1));
    }

    return(Double_Cpp2Matlab(1));

}

#endif
