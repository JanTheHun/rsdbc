#include <stdio.h>
#include <string.h>
#include <my_global.h>
#include <mysql.h>
#include <ncurses.h>


struct conf {
	int port;
	char *host;
	char *usr;
	char *db;
	char *table;
	char *pass;
} config;
typedef struct conf Conf;

#include "functions.h"
#include "fileio.h"

WINDOW *startwin,*browser;


//************	Function declarations

void showrow(WINDOW *brow,char* arr[], int r,char* f[]);
void connect_to_db(MYSQL *c, const char *host,const char *usr,const char *pass ,const char *db,const int port,const char *table,WINDOW *startwin);
void load_config(Conf *c);

//************


//************	main() loop
int main(int argc, char **argv) {

	Conf *c_pntr=&config;

	load_config(c_pntr);

	int i,x;

	initscr();
	noecho();

	if (argv[1]) {
		config.db=argv[1];
	}

	if (argc>2) {
		config.table=argv[2];
	}

	//	initialize query string
	char kvery[1024];
	sprintf(kvery,"SELECT * FROM %s",config.table);

	MYSQL *con;
	con=mysql_init(NULL);
	connect_to_db(con,config.host,config.usr,config.pass,config.db,config.port,config.table,startwin);

	if (mysql_query(con,kvery)) {
		endwin();
		printf("query failed!\n");
		exit(1);
	}


	//	querying and saving data
	mysql_query(con,kvery);
	MYSQL_RES *result = mysql_store_result(con);
	MYSQL_ROW row;
	MYSQL_FIELD *field;  

	int number_of_fields = mysql_num_fields(result);
	int rows=mysql_num_rows(result);

	char *fieldnames[20];
	char* resarr[rows][number_of_fields];

	int field_cnt=0,row_cnt=0;
	int rowtoshow=0;

	//	retrieving header
	while (field = mysql_fetch_field(result)) {
		fieldnames[field_cnt]=field->name;
		field_cnt++;
	}

	//	iterating through result and save it to resarr
	while ((row = mysql_fetch_row(result))) { 
		for(i = 0; i < number_of_fields; i++) {
			resarr[row_cnt][i]=malloc(strlen(row[i])+1);
			strcpy(resarr[row_cnt][i],row[i]);
		}
		row_cnt++;
	}

	// here comes browser
	browser=newwin(20,60,3,3);
	box(browser,0,0);
	keypad(browser,TRUE);
	noecho();

	showrow(browser,resarr[rowtoshow],number_of_fields,fieldnames);

	int key;
	bool noexit=TRUE;

	while (noexit) {
		key=wgetch(browser);

		switch(key) {
			case KEY_LEFT:
				if (rowtoshow>0) {
					rowtoshow--;
					showrow(browser,resarr[rowtoshow],number_of_fields,fieldnames);
				}
				continue;
			case KEY_RIGHT:
				if (rowtoshow<(rows-1)) {
					rowtoshow++;
					showrow(browser,resarr[rowtoshow],number_of_fields,fieldnames);
				}
				continue;
			case 27:
				noexit=FALSE;
				break;
			default:
				continue;
		}
	}

	//	closing connection to database
	mysql_free_result(result);
	mysql_close(con);
	//	end of everything
	endwin();

	return(0);
}

