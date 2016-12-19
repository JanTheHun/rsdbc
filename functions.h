#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void showrow(WINDOW *browser,char* arr[],int r,char* f[]) {
	int x;
	for (x=0;x<r;x++) {
		wmove(browser,(2*x+1),1);
		wprintw(browser,"%s:",f[x]);
		wmove(browser,(2*x+2),1);
		wclrtoeol(browser);
		wattron(browser,A_BOLD);
		wprintw(browser,"\t%s",arr[x]);
		wattroff(browser,A_BOLD);

		box(browser,0,0);
	}
	wrefresh(browser);
};

void connect_to_db(MYSQL *con,const char *host,const char *usr,const char *pass ,const char *db,const int port,const char *table,WINDOW *startwin) {

		startwin=newwin(15,40,5,5);
		box(startwin,0,0);
		int r=0;
		mvwprintw(startwin,++r,1,"host:%s",host);
		mvwprintw(startwin,++r,1,"port:%d",port);
		mvwprintw(startwin,++r,1,"db:%s",db);
		mvwprintw(startwin,++r,1,"table:%s",table);
		mvwprintw(startwin,++r,1,"usr:%s",usr);

		char *masked_pass=malloc(strlen(pass)-1);
		memset(masked_pass,'*',strlen(pass));
		masked_pass[strlen(masked_pass)-2]='\0';

		mvwprintw(startwin,++r,1,"pass:%s",masked_pass);

		wgetch(startwin);

	if (con==NULL) {
		mvwprintw(startwin,++r,5,"mysql_init() failed!");
		wgetch(startwin);
		endwin();
		exit(1);
	}
	if (mysql_real_connect(con, host, usr, pass , db, port, NULL, 0) == NULL) {
		mvwprintw(startwin,++r,1,"%s", mysql_error(con));
		mysql_close(con);
		wgetch(startwin);
		endwin();
		exit(1);
	} else {
		mvwprintw(startwin,++r,1,"succesfully connected to database!");
		mvwprintw(startwin,++r,1,"version:%s",mysql_get_client_info());
	}
}
#endif
