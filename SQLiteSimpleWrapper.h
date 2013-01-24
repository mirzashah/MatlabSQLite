// Filename: SQLiteSimpleWrapper.h
/*****************************************************************************/
// Description: This is a simple wrapper around some key SQLite calls. 
// Benefits include exception handling instead of error codes, use
// of std::string instead of const char*, and simplified facade interface.
//
// Author: Mirza A. Shah
/*****************************************************************************/


#ifndef SQLITE_SIMPLE_WRAPPER_H_DEFINED
#define SQLITE_SIMPLE_WRAPPER_H_DEFINED

#include <sqlite3.h>
#include <string>
using namespace std;

//Translates an error code to a string description
string SQLite_ErrorCode2String(int code);

//This exception is raised for error handling
struct SQLiteException 
{
   SQLiteException(int errCode){errorCode = errCode; error = SQLite_ErrorCode2String(errCode);}
   SQLiteException(int errCode, string errDescrip){ errorCode = errCode; error = errDescrip;} 
	string Error(){return(error);}
   int    ErrorCode(){return(errorCode);}
	   
   string error;
   int    errorCode;
	
};

//Opens the database
sqlite3*      SQLite_OpenDB(string dbFile);

//Creates a compiled [prepared] version of an sql statement
sqlite3_stmt* SQLite_Prepare(sqlite3* db, string sqlStmt);

//Used for stepping through an sql query result. Returns true if the step operation results in pointing a new valid row, otherwise false.
//Keep calling this after a successful sqlite prepare to iterate across rows of result set.
bool          SQLite_Step(sqlite3_stmt* sqlStmt);

//Used for extracting integer column values
sqlite_int64  SQLite_Column_Integer(sqlite3_stmt* sqlStmt, int colIndex);

//Used for extracting float column values
double        SQLite_Column_Float(sqlite3_stmt* sqlStmt, int colIndex);

//Used for extracting text column values
string        SQLite_Column_Text(sqlite3_stmt* sqlStmt, int colIndex);

//Returns the number of columns in the result set
int           SQLite_NumColumns(sqlite3_stmt* sqlStmt);

//Destroys a prepared statement, use after finished with a query result
void          SQLite_Finalize(sqlite3_stmt* sqlStmt);

//Closes the database
void          SQLite_CloseDB(sqlite3* db);

#endif
