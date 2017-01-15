#include <stdio.h>
#include <stdlib.h>
#include <string.h>

	//	ncurses libs
#include <ncurses.h>
#include <menu.h>
#include <panel.h>

	//	mysql libs
#include <my_global.h>
#include <mysql.h>

struct conf {
	int port;
	char *host;
	char *usr;
	char *db;
	char *table;
	char *pass;
};
typedef struct conf Conf;

WINDOW *start_win,*config_win,*filename_win,*setmanual_win,*connection_win,*client_win,*cl_inp_win,*browser_win;

ITEM **config_menu_items;
MENU *config_menu;

ITEM **browser_menu_items;
MENU *browser_menu;

int panel_ctr=0;
PANEL **panels;

MYSQL *con;
MYSQL *realcon;

Conf *init_config();

/*		Function declarations	*/

void showrow(WINDOW *brow,char* arr[], int r,char* f[]);
int load_config(Conf *config,char *filename);
void display_config(Conf *c);
int connect_to_db(MYSQL *c, Conf *cnf);
void go_connect(MYSQL *con,Conf *c);
void load_config_file(Conf *c);
void set_config_manually(Conf *c);
void switch_to_browser(MYSQL *con,Conf *c);
char *replace_password(char *str);
WINDOW *create_win(int height,int width,int row,int column,char *visible);

int main() {

	Conf *conf_pntr=init_config();

	int i,ch;

	char *config_menu_array[]={
		"Load from file","Set manually","Connect",(char*)NULL,
	};
	int config_choices=(sizeof(config_menu_array) / sizeof(config_menu_array[0]));


	//	start ncurses
	initscr();
	cbreak();
	noecho();
	curs_set(0);

	//	create windows, attach them to panels and set visibility

	start_win=create_win(24,79,0,0,"on");
	config_win=create_win(13,40,7,7,"on");
	filename_win=create_win(7,30,12,10,"off");
	setmanual_win=create_win(15,50,5,5,"off");
	connection_win=create_win(12,40,8,8,"off");
	client_win=create_win(24,79,0,0,"off");
	cl_inp_win=create_win(10,45,7,7,"off");
	browser_win=create_win(20,77,3,1,"off");

	update_panels();
	doupdate();

	//	show config
	display_config(conf_pntr);

	//	attach menus to windows
	set_menu_win(config_menu,start_win);
	set_menu_sub(config_menu,derwin(start_win,3,60,1,1));


	//	set up config_menu
	config_menu_items = (ITEM **)calloc(config_choices + 1, sizeof(ITEM *));
	for(i = 0; i < config_choices; ++i) {
		config_menu_items[i] = new_item(config_menu_array[i],"");
	}

	config_menu_items[config_choices] = (ITEM *)NULL;
	config_menu = new_menu((ITEM **)config_menu_items);

	menu_opts_off(config_menu, O_NONCYCLIC);
	set_menu_mark(config_menu,"");
	set_menu_format(config_menu,1,3);

	post_menu(config_menu);

	wrefresh(start_win);


	//	loop for start_win
	while((ch=wgetch(start_win))!=KEY_F(2)) {

		switch (ch) {
			case KEY_UP:
				menu_driver(config_menu,REQ_PREV_ITEM);
				break;
			case KEY_LEFT:
				menu_driver(config_menu,REQ_PREV_ITEM);
				break;

			case KEY_DOWN:
				menu_driver(config_menu,REQ_NEXT_ITEM);
				break;
			case KEY_RIGHT:
				menu_driver(config_menu,REQ_NEXT_ITEM);
				break;
			case 10: {

					if (item_name(current_item(config_menu))=="Connect") {
						go_connect(con,conf_pntr);
					}
					if (item_name(current_item(config_menu))=="Set manually") {
						set_config_manually(conf_pntr);
					}
					if (item_name(current_item(config_menu))=="Load from file") {
						load_config_file(conf_pntr);
					}
				}
				break;
			default:
				
				break;
		}
	}


	//	cleaning up menus
	unpost_menu(config_menu);
	for (i=0;i<config_choices;i++) {
		free_item(config_menu_items[i]);
	}
	
	//	cleaning up memory
	free(conf_pntr->db);
	free(conf_pntr->host);
	free(conf_pntr->usr);
	free(conf_pntr->pass);
	free(conf_pntr);
	free(panels);

	echo();
	curs_set(1);
	endwin();
	printf("thank you for using rsdbc\nbye\n");

}
//	end of main()



//	FUNCTIONS

	#include "conffunc.h"	/*	FUNCTIONS FOR SETTING UP CONFIG

		*create_win()
		init_config()
		*replace_password()
		display_config()
		set_config_manually()
		load_config_file()
	*/
	
	#include "dbfunc.h"		/*	FUNCTIONS FOR MYSQL CONNECTION

	connect_to_db()
	go_connect()
	*/

	#include "browser.h"	/*	FUNCTIONS FOR BROWSING

	showrow()
	show_con_det()
	read_input()
	change_db()
	change_table()
	lets_browse()
	switch_to_browser()

*/


	#include "fileio.h"		/*	FILE I/O

	load_config()

	*/

