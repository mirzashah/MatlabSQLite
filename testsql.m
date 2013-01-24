function testsql()
    db = SQLiteDatabase('test.s3db');
    
    db.Prepare('select * from myTable');
    
    while(db.Step())
        v1 = db.GetColumnAsInteger(0);
        v2 = db.GetColumnAsString(1);
        fprintf(1, 'v1 = %i, v2 = %s\n', v1, v2); 
    end
    
    sqlStmt = sprintf('insert into myTable values(%i, ''coolnessfactor'')', 5);
    fprintf(1, '%s\n', sqlStmt);
    db.Prepare(sqlStmt);
    db.Step();
    db.Close();
end