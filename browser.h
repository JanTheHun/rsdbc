#ifndef BROWSER_H
#define BROWSER_H

/*	FUNCTIONS FOR BROWSING

	showrow()
	show_con_det()
	read_input()
	change_db()
	change_table()
	lets_browse()
	switch_to_browser()

*/

void showrow(WINDOW *browser,char* arr[],int r,char* f[]) {
	int x;
	for (x=0;x<r;x++) {
		wmove(browser,(2*x+5),1);
		wprintw(browser,"%s:",f[x]);
		wmove(browser,(2*x+6),1);
		wclrtoeol(browser);
		wattron(browser,A_BOLD);
		wprintw(browser,"\t%s",arr[x]);
		wattroff(browser,A_BOLD);

		box(browser,0,0);
	}
	wrefresh(browser);
};

void show_con_det(char*usr,char *host,int port,char *db,char *tbl) {
	mvwprintw(client_win,1,1,"CONNECTED %s@%s:%d",usr,host,port);
	wmove(client_win,2,1);
	wclrtoeol(client_win);
	box(client_win,0,0);
	mvwprintw(client_win,2,1,"DB:");
	wattron(client_win,A_BOLD);
	mvwprintw(client_win,2,4,(strcmp(db,"")==0 ? "<not set>" : db));
	wattroff(client_win,A_BOLD);
	mvwprintw(client_win,2,(16+strlen(db)),"table:");
	wattron(client_win,A_BOLD);
	mvwprintw(client_win,2,(23+strlen(db)),(strcmp(tbl,"")==0 ? "<not set>" : tbl));
	wattroff(client_win,A_BOLD);
	wrefresh(client_win);
}

void read_input(char *out,char *question) {

	char *i=malloc(255*(sizeof(char)));

	//	show cl_inp_win
	show_panel(panels[6]);
	curs_set(1);
	echo();

	werase(cl_inp_win);
	box(cl_inp_win,0,0);
	mvwprintw(cl_inp_win,2,2,"%s:",question);
	mvwscanw(cl_inp_win,2,12,"%s",i);
	strcpy(out,i);

	//	hide cl_inp_win
	hide_panel(panels[6]);
	curs_set(0);
	noecho();

	free(i);
}

void change_db(MYSQL *con, char *db,char *tbl) {

	char *q_str=malloc(255*(sizeof(char)));
	char *inp=malloc(255*(sizeof(char)));

	read_input(inp,"database");

	strcpy(q_str,"use ");
	strcat(q_str,inp);

	if (mysql_query(con,q_str)) {
		wmove(cl_inp_win,5,1);
		wclrtoeol(cl_inp_win);
		box(cl_inp_win,0,0);
		mvwprintw(cl_inp_win,5,4,"no such database");
		mvwprintw(cl_inp_win,7,4,"(hit any key)");
	} else {
		strcpy(db,inp);
		strcpy(tbl,"");
		wmove(cl_inp_win,7,1);
		wclrtoeol(cl_inp_win);
		box(cl_inp_win,0,0);
		mvwprintw(cl_inp_win,5,4,"changed to %s",db);
		mvwprintw(cl_inp_win,7,4,"(hit any key)");
	}

	free(inp);
	free(q_str);

	wgetch(cl_inp_win);
}
void change_table(MYSQL *con, char *tbl) {

	char *q_str=malloc(255*(sizeof(char)));
	char *inp=malloc(255*(sizeof(char)));
	read_input(inp,"table");

	strcpy(q_str,"show tables like \'");
	strcat(q_str,inp);
	strcat(q_str,"\'");


	if (mysql_query(con,q_str)) {
		wmove(cl_inp_win,5,1);
		wclrtoeol(cl_inp_win);
		box(cl_inp_win,0,0);
		mvwprintw(cl_inp_win,5,4,"DB error");
		mvwprintw(cl_inp_win,7,4,"(hit any key)");
	} else {

		MYSQL_RES *result = mysql_store_result(con);
		int rows=mysql_num_rows(result);

		if (rows>0) {
			strcpy(tbl,inp);
			wmove(cl_inp_win,5,1);
			wclrtoeol(cl_inp_win);
			box(cl_inp_win,0,0);
			mvwprintw(cl_inp_win,5,4,"changed to table ");
			wattron(cl_inp_win,A_BOLD);
			mvwprintw(cl_inp_win,5,21,tbl);
			wattroff(cl_inp_win,A_BOLD);
			mvwprintw(cl_inp_win,7,4,"(hit any key)");
		} else {
			wmove(cl_inp_win,5,1);
			wclrtoeol(cl_inp_win);
			box(cl_inp_win,0,0);
			mvwprintw(cl_inp_win,5,4,"no table found");
			mvwprintw(cl_inp_win,7,4,"(hit any key)");
		}
	}
	free(inp);
	free(q_str);
	wgetch(cl_inp_win);
}

void lets_browse (MYSQL *con,char *db,char *tbl) {

	//	turn off cursor and echo
	curs_set(0);
	noecho();
	//	switch to browser_win
	werase(browser_win);
	box(browser_win,0,0);
	show_panel(panels[7]);
	update_panels();
	doupdate();

	char *q_str=malloc(255*(sizeof(char)));

	int i;

	sprintf(q_str,"select * from %s",tbl);

	if (mysql_query(con,q_str)) {
		wmove(cl_inp_win,5,1);
		wclrtoeol(cl_inp_win);
		box(cl_inp_win,0,0);
		mvwprintw(browser_win,5,4,"DB error");
		mvwprintw(browser_win,7,4,"(hit any key)");
	} else {
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


		showrow(browser_win,resarr[rowtoshow],number_of_fields,fieldnames);

		int key;
		bool noexit=TRUE;

		//	loop for browser_win
		while (noexit) {
			key=wgetch(browser_win);

			switch(key) {
				case KEY_LEFT:
					if (rowtoshow>0) {
						rowtoshow--;
						showrow(browser_win,resarr[rowtoshow],number_of_fields,fieldnames);
					}
					continue;
				case KEY_RIGHT:
					if (rowtoshow<(rows-1)) {
						rowtoshow++;
						showrow(browser_win,resarr[rowtoshow],number_of_fields,fieldnames);
					}
					continue;
				case 27:
					noexit=FALSE;
					break;
				default:
					continue;
			}
		}

		mysql_free_result(result);
	}

	//	switch back to start_win
	hide_panel(panels[7]);
	update_panels();
	doupdate();

	//	free up memory
	free(q_str);

}

void switch_to_browser(MYSQL *realcon,Conf *cnf) {


	//	turn off cursor and echo
	curs_set(0);
	noecho();

	//	switch to client_win
	werase(client_win);
	box(client_win,0,0);
	show_panel(panels[5]);
	update_panels();
	doupdate();

	//	CLIENT WINDOW HERE

	int i,ch;

	char *query_str=malloc(255*(sizeof(char)));
	char *cur_db=malloc(255*(sizeof(char)));
	char *cur_tbl=malloc(255*(sizeof(char)));
	strcpy(cur_tbl,cnf->table);

	if (strcmp(cur_tbl,"")==0) {
		strcpy(cur_tbl,"");
	}

	strcpy(query_str,"select database()");

	if ( mysql_query(realcon,query_str) ) {
	} else {
		MYSQL_RES *res=mysql_store_result(realcon);
		MYSQL_ROW row=mysql_fetch_row(res);

		if (row[0]==NULL) {
			strcpy(cur_db,"");
		} else {
			strcpy(cur_db,row[0]);
		}
	}
	show_con_det(cnf->usr,cnf->host,cnf->port,cur_db,cur_tbl);

	//	set up browser_menu
	char *browser_menu_array[]={
		"Select DB","Select table","Browse table",(char*)NULL,
	};
	int browser_choices=(sizeof(browser_menu_array) / sizeof(browser_menu_array[0]));
	set_menu_win(browser_menu,client_win);
	set_menu_sub(browser_menu,derwin(client_win,3,60,4,1));
	browser_menu_items = (ITEM **)calloc(browser_choices + 1, sizeof(ITEM *));
	for(i = 0; i < browser_choices; ++i) {
		browser_menu_items[i] = new_item(browser_menu_array[i],"");
	}
	browser_menu_items[browser_choices] = (ITEM *)NULL;
	browser_menu = new_menu((ITEM **)browser_menu_items);
	menu_opts_off(browser_menu, O_NONCYCLIC);
	set_menu_mark(browser_menu,"");
	set_menu_format(browser_menu,1,3);

	post_menu(browser_menu);
	wrefresh(client_win);

	//	loop for client_win
	while((ch=wgetch(client_win))!=KEY_F(2)) {

		switch (ch) {
			case KEY_UP:
				menu_driver(browser_menu,REQ_PREV_ITEM);
				break;
			case KEY_LEFT:
				menu_driver(browser_menu,REQ_PREV_ITEM);
				break;

			case KEY_DOWN:
				menu_driver(browser_menu,REQ_NEXT_ITEM);
				break;
			case KEY_RIGHT:
				menu_driver(browser_menu,REQ_NEXT_ITEM);
				break;
			case 10: {

					if (item_name(current_item(browser_menu))=="Select DB") {
						change_db(realcon,cur_db,cur_tbl);
						show_con_det(cnf->usr,cnf->host,cnf->port,cur_db,cur_tbl);
					}
					if (item_name(current_item(browser_menu))=="Select table") {
						change_table(realcon,cur_tbl);
						show_con_det(cnf->usr,cnf->host,cnf->port,cur_db,cur_tbl);
					}
					if (item_name(current_item(browser_menu))=="Browse table") {
						lets_browse(realcon,cur_db,cur_tbl);
					}
				}
				break;
			default:
				
				break;
		}
	}


	//	closing connection to database
	mysql_close(con);

	//	freeing up memory
	free(query_str);
	free(cur_db);
	free(cur_tbl);

	//	turn off cursor and echo
	curs_set(0);
	noecho();

	//	switch back to start_win
	hide_panel(panels[5]);
	update_panels();
	doupdate();
	//	CLIENT WINDOW END
}

#endif
