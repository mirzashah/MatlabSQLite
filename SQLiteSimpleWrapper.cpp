#include "SQLiteSimpleWrapper.h"

/******************************************************************/
string SQLite_ErrorCode2String(int code)
/******************************************************************/
{
    switch(code)
    {
        case SQLITE_OK:         return("No error");
        case SQLITE_ERROR:      return("SQL error or missing database");
        case SQLITE_INTERNAL:   return("Internal logic error in SQLite");
        case SQLITE_PERM:       return("Access permission denied");
        case SQLITE_ABORT:      return("Callback routine requested an abort");
        case SQLITE_BUSY:       return("The database file is locked");
        case SQLITE_LOCKED:     return("A table in the database is locked");
        case SQLITE_NOMEM:      return("A malloc() failed");
        case SQLITE_READONLY:   return("Attempt to write a readonly database");
        case SQLITE_INTERRUPT:  return("Operation terminated by sqlite3_interrupt()");
        case SQLITE_IOERR:      return("Some kind of disk I/O error occurred");
        case SQLITE_CORRUPT:    return("The database disk image is malformed");
        case SQLITE_NOTFOUND:   return("Unknown opcode in sqlite3_file_control()");
        case SQLITE_FULL:       return("Insertion failed because database is full");
        case SQLITE_CANTOPEN:   return("Unable to open the database file");
        case SQLITE_PROTOCOL:   return("Database lock protocol error");
        case SQLITE_EMPTY:      return("Database is empty");
        case SQLITE_SCHEMA:     return("The database schema changed");
        case SQLITE_TOOBIG:     return("String or BLOB exceeds size limit");
        case SQLITE_CONSTRAINT: return("Abort due to constraint violation");
        case SQLITE_MISMATCH:   return("Data type mismatch");
        case SQLITE_MISUSE:     return("Library used incorrectly");
        case SQLITE_NOLFS:      return("Uses OS features not supported on host");
        case SQLITE_AUTH:       return("Authorization denied");
        case SQLITE_FORMAT:     return("Auxiliary database format error");
        case SQLITE_RANGE:      return("2nd parameter to sqlite3_bind out of range");
        case SQLITE_NOTADB:     return("File opened that is not a database file");
        case SQLITE_ROW:        return("sqlite3_step() has another row ready");
        default:                return("Unknown error code.");
	};
}

/****************************************************************/
sqlite3* SQLite_OpenDB(string dbFile)
/****************************************************************/
{
   sqlite3* dbHandle = NULL;  
   int rc = sqlite3_open_v2(dbFile.c_str(), &dbHandle, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
   if(SQLITE_OK == rc)
      return(dbHandle); 
   else
      throw(SQLiteException(rc));	
}

/****************************************************************/
sqlite3_stmt* SQLite_Prepare(sqlite3* db, string sqlStmt)
/****************************************************************/
{
   sqlite3_stmt* stmtHandle = NULL;
   int rc = sqlite3_prepare_v2(db, sqlStmt.c_str(), sqlStmt.size() + 1,  &stmtHandle, NULL);
   if(stmtHandle != NULL && SQLITE_OK == rc)
      return(stmtHandle);
   else
      throw(SQLiteException(rc));
}

/****************************************************************/
bool SQLite_Step(sqlite3_stmt* sqlStmt)
/****************************************************************/
{
   int rc = sqlite3_step(sqlStmt);
   switch(rc)
   {
      case SQLITE_ROW: return (true);
      case SQLITE_DONE: return (false);
      default: throw(SQLiteException(rc));         
   };
}

/****************************************************************/
sqlite_int64 SQLite_Column_Integer(sqlite3_stmt* sqlStmt, int colIndex)
/****************************************************************/
{
   sqlite_int64 toReturn = sqlite3_column_int64(sqlStmt, colIndex);
   return(toReturn);
}

/****************************************************************/
double SQLite_Column_Float(sqlite3_stmt* sqlStmt, int colIndex)
/****************************************************************/
{
   double toReturn = sqlite3_column_double(sqlStmt, colIndex);
   return(toReturn);
}

/****************************************************************/
string SQLite_Column_Text(sqlite3_stmt* sqlStmt, int colIndex)
/****************************************************************/
{
   string toReturn((const char*)(sqlite3_column_text(sqlStmt, colIndex)));
   return(toReturn);
}

/****************************************************************/
int SQLite_NumColumns(sqlite3_stmt* sqlStmt)
/****************************************************************/
{
   int toReturn = sqlite3_column_count(sqlStmt);
   return(toReturn);
}

/****************************************************************/
void SQLite_Finalize(sqlite3_stmt* sqlStmt)
/****************************************************************/
{
   int rc = sqlite3_finalize(sqlStmt);
   if(SQLITE_OK != rc)
      throw(SQLiteException(rc));   
}

/****************************************************************/
void SQLite_CloseDB(sqlite3* db)
/****************************************************************/
{
   int rc = sqlite3_close(db);
   if(SQLITE_OK != rc)
      throw(SQLiteException(rc));
}
