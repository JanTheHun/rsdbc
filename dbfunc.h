#ifndef DBFUNC_H
#define DBFUNC_H

/*	FUNCTONS FOR MYSQL
	connect_to_db()
	go_connect()
*/


int connect_to_db(MYSQL *con,Conf *cnf) {

	//	turn on cursor and echo
	curs_set(1);
	echo();

	//	switch to connection_win
	werase(connection_win);
	box(connection_win,0,0);
	show_panel(panels[4]);
	update_panels();
	doupdate();

	int r=0;

	mvwprintw(connection_win,++r,1,"host:\t\t%s",cnf->host);
	mvwprintw(connection_win,++r,1,"port:\t\t%d",cnf->port);
	mvwprintw(connection_win,++r,1,"user:\t\t%s",cnf->usr);
	mvwprintw(connection_win,++r,1,"password:\t%s",replace_password(cnf->pass));
	r++;
	mvwprintw(connection_win,++r,1,"database:\t%s",cnf->db);
	mvwprintw(connection_win,++r,1,"table:\t\t%s",cnf->table);

	con=mysql_init(NULL);


	if (con==NULL) {
		mvwprintw(connection_win,++r,5,"mysql_init() failed!");

		wgetch(connection_win);
		//	turn off cursor and echo
		curs_set(0);
		noecho();

		//	switch back to start_win
		hide_panel(panels[4]);
		update_panels();
		doupdate();
		return (1);

	} else {

		if (mysql_real_connect(con, cnf->host, cnf->usr, cnf->pass , cnf->db, cnf->port, NULL, 0) == NULL) {
			mvwprintw(connection_win,++r,1,"%s", mysql_error(con));
			mysql_close(con);
			wgetch(connection_win);

			//	turn off cursor and echo
			curs_set(0);
			noecho();

			//	switch back to start_win
			hide_panel(panels[4]);
			update_panels();
			doupdate();

			return (1);
		} else {

			switch_to_browser(con,cnf);

			//	turn off cursor and echo
			curs_set(0);
			noecho();

			//	switch back to start_win
			hide_panel(panels[4]);
			update_panels();
			doupdate();

			return (1);
		}

	}
}



void go_connect(MYSQL *con, Conf *conf_pntr) {
	connect_to_db(con,conf_pntr);
}

#endif
