#include <iostream>
#include <database/sqlite3.h>
#include <string.h>
#include <bits/stdc++.h>

/* Defining globals */

char dbname[]="resources/data.db";
sqlite3 *db;
char *zErrMsg = 0;
int rc;
char* sql;
sqlite3_stmt* stmt = 0;
const char* data = "Callback function called";
const char *pzTest;

using namespace std;

/* Prototypes of functions */
/* DB Prototypes */
int openDB();
int setupDB();
void closeDB();

/* Utils */
void clrscr();
void getch();

/* Program Sections */
int loginRegister();
void welcome();
void checkOption(int choice);

static int callback(void *NotUsed, int argc, char **argv, char **azColName);

/* Tasks */
void addStudent();
void listStudents();
void delStudent();
void listDStudents();

/* Main function */

int main(int argc, char* argv[])
{
    if(!setupDB())
	{
		closeDB();
 		return 0;
	}

	welcome();

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

/* Creating Tables */
int setupDB()
{
	if(!openDB()) return 0;

	/* Creating required Tables */
	cout<<"[INFO] Setting up Tables\n";

	sql = "CREATE TABLE IF NOT EXISTS students(" \
		"id INTEGER PRIMARY KEY AUTOINCREMENT," \
		"name TEXT NOT NULL," \
		"class INT NOT NULL," \
		"roll INT NOT NULL" \
		");";

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
   for(i = 0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   
   printf("\n");

}


/* Main Menu */
void welcome()
{
	clrscr();
	cout<<"1. Add Students"<<endl;
	cout<<"2. List Students"<<endl;
	cout<<"3. Delete Student"<<endl;
	cout<<"4. List in Descending Order"<<endl;
	cout<<"5. Exit"<<endl;
	cout<<"Enter Value:";
	int option;
	cin>>option;

	checkOption(option);
}

void checkOption(int choice)
{
	switch(choice) {
		case 1: 
			addStudent();
			break;
		case 2:
			listStudents();
			break;
		case 3:
			delStudent();
			break;
		case 4:
			listDStudents();
			break;
		case 5:
			break;
		default:
			cout<<"Invalid Option.. Try Again";
			welcome();
			break;
	}
}

/* Function to add Student in DB */
void addStudent()
{
	string temp;
	char* name;
	int rollno;
	int cl;
	clrscr();

	cout<<"Enter Name:";
	cin.ignore(20, '\n');
	getline(cin, temp, '\n');
	cout<<"Roll No: ";
	cin>>rollno;
	cout<<"Class: ";
	cin>>cl;

	name = new char[temp.length() + 1];
	strcpy(name, temp.c_str());

	ostringstream l_query;
	l_query <<"INSERT INTO students (name,class,roll) ";
	l_query << "VALUES (\""<<name<<"\"," <<cl<<"," << rollno<<");";

    rc = sqlite3_exec(db, l_query.str().c_str(), callback, 0, &zErrMsg);

    if( rc != SQLITE_OK) {
		cerr<<"[ERROR] Can't insert student in database: "<<zErrMsg<<endl;
		sqlite3_free(zErrMsg);
		// addStudent();
	} else {

		cout<<"[INFO] Data has been inserted successfully \n";
		getch();
		welcome();
	}

}

/* Function to list Students from DB */
void listStudents()
{

	sqlite3_stmt *statement;    

    char *query = "select * from students ORDER BY name ASC";

    if ( sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK ) 
    {
        int ctotal = sqlite3_column_count(statement);
        int res = 0;

        while ( 1 )         
        {
            res = sqlite3_step(statement);

            if ( res == SQLITE_ROW ) 
            {
                for ( int i = 0; i < ctotal; i++ ) 
                {
                	string c = (char*)sqlite3_column_name(statement, i);
                    string s = (char*)sqlite3_column_text(statement, i);
                    // print or format the output as you want 
                    cout <<c<<" : "<< s << endl;
                }
                cout << endl;
            }
            
            if ( res == SQLITE_DONE || res==SQLITE_ERROR)    
            {
                cout << "done " << endl;

                break;
            }    
        }
    }

    getch();
    welcome();

}

/* Function to delete Student in DB */
void delStudent()
{
	int id;
	cout<<"Id: ";
	cin>>id;

	ostringstream l_query;
	l_query <<"DELETE FROM students ";
	l_query << "WHERE id = "<<id<<";";
	rc = sqlite3_exec(db, l_query.str().c_str(), callback, 0, &zErrMsg);

    if( rc != SQLITE_OK) {
		cerr<<"[ERROR] Can't delete record from database: "<<zErrMsg<<endl;
		sqlite3_free(zErrMsg);
	} else {

		cout<<"[INFO] Data has been deleted successfully \n";
		getch();
		welcome();
	}

}

void listDStudents()
{
	sqlite3_stmt *statement;    

    char *query = "select * from students ORDER BY name DESC;";

    if ( sqlite3_prepare(db, query, -1, &statement, 0 ) == SQLITE_OK ) 
    {
        int ctotal = sqlite3_column_count(statement);
        int res = 0;

        while ( 1 )         
        {
            res = sqlite3_step(statement);

            if ( res == SQLITE_ROW ) 
            {
                for ( int i = 0; i < ctotal; i++ ) 
                {
                	string c = (char*)sqlite3_column_name(statement, i);
                    string s = (char*)sqlite3_column_text(statement, i);
                    // print or format the output as you want 
                    cout <<c<<" : "<< s << endl;
                }
                cout << endl;
            }
            
            if ( res == SQLITE_DONE || res==SQLITE_ERROR)    
            {
                cout << "done " << endl;

                break;
            }    
        }
    }

    getch();
    welcome();
}

/* Function to close Database Called before end of program */
void closeDB()
{
	cout<<"[INFO] Closing database \n";
	sqlite3_close(db);

}

/* Custom made function to clear screen of terminal */
void clrscr()
{
	cout << "\033[2J\033[1;1H";
}


void getch()
{
	char n;
	cin>>n;
}