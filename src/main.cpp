#include <iostream>
#include <database/sqlite3.h>

/* Defining globals */

char dbname[]="resources/data.db";
sqlite3 *db;
char *zErrMsg = 0;
int rc;
char* sql;


using namespace std;

/* Prototypes of functions */

int openDB();
int setupDB();
void closeDB();
static int callback(void *NotUsed, int argc, char **argv, char **azColName);

/* Main function */

int main(int argc, char* argv[])
{
    if(!setupDB())
	{
		closeDB();
 		return 0;
	}



	closeDB();
    return 0;
}




/* Function to open database */
int openDB()
{

	rc = sqlite3_open(dbname, &db);

	if( rc ) {
		cerr<<"[ERROR] Error in opening database: "<<sqlite3_errmsg(db);
	return 0;
		
	}

	cout<<"[INFO] Database Opened Successfully\n";

	
	return 1;
}

int setupDB()
{
	if(!openDB()) return 0;

	/* Creating required Tables */
	cout<<"[INFO] Setting up Tables\n";

	sql = "CREATE TABLE IF NOT EXISTS users (" \
		"id INT PRIMARY KEY NOT NULL," \
		"name CHAR(20) NOT NULL," \
		"email char(50) NOT NULL," \
		"role char(50)";

	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	
	if( rc != SQLITE_OK) {
		cerr<<"[ERROR] Can't create table in database: "<<zErrMsg<<endl;
		sqlite3_free(zErrMsg);
		return 0;	
	} 
	cout<<"[INFO] Table users created successfully \n";
	return 1;
}

/* Callback for every db query */
static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{

   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;

}

/* Function to close Database Called before end of program */
void closeDB()
{
	cout<<"[INFO] Closing database \n";
	sqlite3_close(db);

}
