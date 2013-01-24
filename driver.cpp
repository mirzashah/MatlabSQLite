#include "SQLiteSimpleWrapper.h"
#include <iostream>
using namespace std;

int main()
{
   
   try
   {
      sqlite3* db = SQLite_OpenDB("test.s3db");
      cout<<"Database pointer = "<<db<<endl;
      
      string query = "select * from myTable";   
      cout<<"Query = "<<query<<endl;
      
      sqlite3_stmt* stmt = SQLite_Prepare(db, query);
      cout<<"Prepared statement pointer = "<<stmt<<endl;
      
      int numColumns = SQLite_NumColumns(stmt);
      cout<<"Number of columns = "<< numColumns<<endl;
    
      for(int c=0; c<numColumns; c++)
      {
      }
      
      
   }
   catch(SQLiteException e)
   {
      cout<<"Error: Code = "<<e.ErrorCode()<<", Msg = "<<e.Error()<<endl;
   }

   return(0);
}
