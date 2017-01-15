#ifndef CONFFUNC_H
#define CONFFUNC_H

/*	FUNCTIONS FOR SETTING UP CONFIG

	*create_win()
	init_config()
	*replace_password()
	display_config()
	set_config_manually()
	load_config_file()
*/


WINDOW *create_win(int height,int width,int row,int column,char *visible) {
	WINDOW *w;

	//	create window and frame, turn on keypad
	w=newwin(height,width,row,column);
	keypad(w,TRUE);
	box(w,0,0);

	//	attach window to panels array
	panels=realloc(panels,(panel_ctr+1)*sizeof(PANEL));
	panels[panel_ctr]=new_panel(w);

	if (strcmp(visible,"on")==0) {
		show_panel(panels[panel_ctr]);

	} else {
		hide_panel(panels[panel_ctr]);

	}
	panel_ctr++;
	return w;
}

Conf *init_config() {

	Conf *tempconf=(Conf *)malloc(sizeof(Conf));
	tempconf->db=malloc(255*sizeof(char));
	strcpy(tempconf->db,"");
	tempconf->usr=malloc(255*sizeof(char));
	strcpy(tempconf->usr,"");
	tempconf->host=malloc(255*sizeof(char));
	strcpy(tempconf->host,"");
	tempconf->pass=malloc(255*sizeof(char));
	strcpy(tempconf->pass,"");
	tempconf->table=malloc(255*sizeof(char));
	strcpy(tempconf->table,"");
	tempconf->port=0;
	return tempconf;

}

char *replace_password(char *str) {

	return ("***");

}

void display_config(Conf *cnf) {
	werase(config_win);

	const char *NS="<not set>";

	mvwprintw(config_win,2,3,"host:\t%s",( strcmp(cnf->host,"")==0 ? NS : cnf->host ) );
	if (cnf->port==0) {
		mvwprintw(config_win,3,3,"port:\t%s",NS);
	} else {
		mvwprintw(config_win,3,3,"port:\t%d",cnf->port);
	}
	mvwprintw(config_win,4,3,"user:\t%s",( strcmp(cnf->usr,"")==0 ? NS : cnf->usr ));
	mvwprintw(config_win,5,3,"pass:\t%s",( strcmp(cnf->pass,"")==0 ? NS : replace_password(cnf->pass) ));
//	mvwprintw(config_win,5,3,"pass:\t%s",( strcmp(cnf->pass,"")==0 ? NS : cnf->pass ));
	mvwprintw(config_win,7,3,"db:\t\t%s",( strcmp(cnf->db,"")==0 ? NS : cnf->db ));
	mvwprintw(config_win,8,3,"table:\t%s",( strcmp(cnf->table,"")==0 ? NS : cnf->table ));
	box(config_win,0,0);
	update_panels();
	doupdate();
}

void set_config_manually(Conf *c) {


	char *inp;
	int port;

	//	turn on cursor and echo
	curs_set(1);
	echo();

	//	switch to setmanual_win
	werase(setmanual_win);
	box(setmanual_win,0,0);
	show_panel(panels[3]);
	update_panels();
	doupdate();


	mvwprintw(setmanual_win,2,2,"hostname:");
	mvwprintw(setmanual_win,3,2,"port:");
	mvwprintw(setmanual_win,4,2,"user:");
	mvwprintw(setmanual_win,5,2,"password:");
	mvwprintw(setmanual_win,7,2,"database:");
	mvwprintw(setmanual_win,8,2,"table:");


	//	hostname
	inp=malloc(255*(sizeof(char)));
		strcpy(inp,"");
	wmove(setmanual_win,2,12);
	wscanw(setmanual_win,"%s",inp);
	if (strlen(inp)==0) {
		strcpy(inp,"127.0.0.1");
	}
	c->host=realloc(c->host,1+strlen(inp));
	strcpy(c->host,inp);
	free(inp);

	//	port
	inp=malloc(5*(sizeof(char)));
		strcpy(inp,"");
	wmove(setmanual_win,3,12);
	wscanw(setmanual_win,"%5s",inp);
	if (strlen(inp)==0) {
		strcpy(inp,"3306");
	}
	char *r;
	port=strtol(inp,&r,10);
	c->port=port;



	//	user
	inp=malloc(255*(sizeof(char)));
		strcpy(inp,"");
	wmove(setmanual_win,4,12);
	wscanw(setmanual_win,"%s",inp);
	if (strlen(inp)==0) {
		strcpy(inp,"root");
	}
	c->usr=realloc(c->usr,1+strlen(inp));
	strcpy(c->usr,inp);
	free(inp);

	//	password
	char *ps=malloc(255*(sizeof(char)));
		strcpy(ps,"");
	wmove(setmanual_win,5,12);
	int ch,cntr=0;
	cbreak();
	noecho();
	while ((ch=wgetch(setmanual_win))!='\n') {
		switch (ch) {
			case KEY_BACKSPACE:
				if (cntr>0) {
					cntr--;
					mvwaddch(setmanual_win,5,12+cntr,' ');
					wmove(setmanual_win,5,12+cntr);
					ps[cntr]='\0';
				}
				break;
			default:
				mvwaddch(setmanual_win,5,12+cntr,'*');
				ps[cntr++]=ch;
				break;
		}
	}
	ps[cntr]='\0';
	echo();
	c->pass=realloc(c->pass,1+strlen(ps));
	strcpy(c->pass,ps);
	free(ps);

	//	database
	inp=malloc(255*(sizeof(char)));
		strcpy(inp,"");
	wmove(setmanual_win,7,12);
	wscanw(setmanual_win,"%s",inp);
	if (strlen(inp)==0) {
		strcpy(inp,"");
	}
	c->db=realloc(c->db,1+strlen(inp));
	strcpy(c->db,inp);
	free(inp);

	//	table
	inp=malloc(255*(sizeof(char)));
		strcpy(inp,"");
	wmove(setmanual_win,8,12);
	wscanw(setmanual_win,"%s",inp);
	if (strlen(inp)==0) {
		strcpy(inp,"");
	}
	c->table=realloc(c->table,1+strlen(inp));
	strcpy(c->table,inp);
	free(inp);

	//	show new stuff
	display_config(c);

	//	turn off cursor and echo
	curs_set(0);
	noecho();

	//	switch back to start_win
	hide_panel(panels[3]);
	update_panels();
	doupdate();

}

void load_config_file(Conf *c) {

		char *filename=malloc(200);

		//	turn on cursor and echo
		curs_set(1);
		echo();

		//	switch to filename_win
		werase(filename_win);
		box(filename_win,0,0);
		show_panel(panels[2]);
		update_panels();
		doupdate();

		mvwprintw(filename_win,2,2,"name of connection file");
		wmove(filename_win,3,5);
		wscanw(filename_win,"%s",filename);



		//	checking config file
		int file_loaded=load_config(c,filename);

		if (file_loaded==0) {
			display_config(c);
		} else {
			//item_opts_off(config_menu_items[0],O_SELECTABLE);
			mvwprintw(filename_win,5,3,"couldn't read %s",filename);
			wgetch(filename_win);	
		}

		//	turn off cursor and echo
		curs_set(0);
		noecho();

		//	switch back to start_win
		hide_panel(panels[2]);
		update_panels();
		doupdate();

		free(filename);

		return;
}

#endif
