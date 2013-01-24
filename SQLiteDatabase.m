% ************************************************
% SQLiteDatabase
% ************************************************
% This class provides a binding to the SQLite
% database API. The class implements a facade
% pattern in that it provides a more limited 
% interface than the full API in exchange for
% ease of use. 
% Date: 06/02/2011
% ************************************************

%Usage
%1. Create an object: myDB = SQLiteDatabase('someDB.s3db');
%2. Prepare a query: myDB.Prepare('select * from myTable');
%3. Step and extract values:
%    while(myDB.Step())
%       val1 = myDB.GetColumnAsDouble(1);
%       val2 = myDB.GetColumnAsInteger(2);
%       valn = myDB.GetColumnAsText(n);
%    end
%4. Go back to 2 if desired.
%5. Close when done: myDB.Close() or automatically allow garbage collector
%to invoke destructor which will automatically close

classdef SQLiteDatabase < handle
    properties
        databaseFilename      = '';
        databaseHandle        = 0;
        currentPreparedStatement = 0;
    end
    
    methods(Access = public)
        %Constructor, takes SQLite filename and automatically opens
        %database
        function obj = SQLiteDatabase(dbFilename)
            obj.databaseFilename = dbFilename;            
            obj.Open();
        end
        
        %Destructor 
        function delete(obj)            
            obj.Close();
        end
        
        %Prepares an SQLite statement, removing current prepared statement and
        %automatically finalizing it. Returns 1 if successful, else
        %returns -1
        function rc = Prepare(obj, sqlExp)
            if(obj.databaseHandle > 0)
                obj.Finalize();
                obj.currentPreparedStatement = MatlabSQLite('PREPARE', obj.databaseHandle, sqlExp);
                rc = obj.currentPreparedStatement > 0;
            else
                fprintf(1, 'SQLiteDatabase::Prepare => Error: No database handle.\n');
                rc = -1;
            end
        end
        
        %Steps to the next line in the result set for a query or executes
        %last prepared command (e.g. insert,delete,drop,create). Returns 1
        %if new row, 0 if last/row (for queries) or if command executing
        %correctly. Returns -1 in event of failure.
        function isOk = Step(obj)
            isOk = MatlabSQLite('STEP', obj.currentPreparedStatement);
        end

        %Returns the column of the current row in the current result set
        %as a float
        function val = GetColumnAsFloat(obj, index)
            val = MatlabSQLite('GETCOLUMNFLOAT', obj.currentPreparedStatement, index);            
        end        
        
        %Returns the column of the current row in the current result set
        %as an integer
        function val = GetColumnAsInteger(obj, index)
            val = MatlabSQLite('GETCOLUMNINT', obj.currentPreparedStatement, index);            
        end
        
        %Returns the column of the current row in the current result set
        %as an string
        function val = GetColumnAsString(obj, index)
            val = MatlabSQLite('GETCOLUMNSTRING', obj.currentPreparedStatement, index);            
        end
        
        %Returns the number of columns in the current result set
        function numCols = GetNumberColumns(obj)
            numCols = MatlabSQLite('GETNUMCOLUMNS', obj.currentPreparedStatement);            
        end
        
        %Closes the database
        function Close(obj)
            obj.Finalize(); %Finalize any existing prepared statement if necessary
            if(obj.databaseHandle > 0)
                MatlabSQLite('CLOSE', obj.databaseHandle);
            end
            obj.databaseHandle = 0;
        end
    end
        
    methods(Access = private)
        %Opens the SQLite database
        function Open(obj)
            obj.databaseHandle = MatlabSQLite('OPEN', obj.databaseFilename);
        end
    
        %Finalizes the current prepared statement 
        function Finalize(obj)
            if(obj.currentPreparedStatement > 0)
                MatlabSQLite('FINALIZE', obj.currentPreparedStatement);                
            end
            obj.currentPreparedStatement = 0;
        end
    end
    
end