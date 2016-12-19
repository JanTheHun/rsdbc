#ifndef FILEIO_H
#define FILEIO_H

void load_config(Conf *config) {

	void trim_str(char *val,char *out) {
		int z;
		for (z=0;z<(strlen(val)-2);z++) {
			out[z]=val[z+1];
		}
		out[z]='\0';
	}

	FILE *file;

	int ch,x;
	int line_nr=0;
	int chnr=0;

	int this_line_length=0;
	int full_length=0;

	char *line=malloc(sizeof (char));
	char **attrs=malloc(sizeof (char*));

	file=fopen("rsdbc.conf","r");

	if (file) {
		while( ( ch = fgetc(file) ) != EOF )	{
				if (ch==10) {
					line[chnr]='\0';
					if (chnr>0) {
						if (line[0]!='#') {
								this_line_length=strlen(line)+1;
								full_length+=this_line_length;
								attrs=realloc(attrs,full_length);
								attrs[line_nr]=malloc(this_line_length);
								strcpy(attrs[line_nr],line);
								line_nr++;		
						}
						chnr=0;
					}
				} else {
					line=realloc(line,(sizeof(char)*chnr)+1);
					line[chnr]=ch;
					chnr++;
				}
		}
		fclose(file);
	}

	for (x=0;x<line_nr;x++) {

		char *temp1=malloc(strlen(attrs[x])+1);
		char *temp2=malloc(strlen(attrs[x])+1);

		strcpy(temp1,attrs[x]);
		strcpy(temp2,attrs[x]);

		char *fi=strtok(temp1,"=");
		char *rest=strchr(temp2,'=');
		char *val=malloc(strlen(rest));
		strncpy(val,rest+1,strlen(rest));
		//	and here we finally have the value after '=' in 'val'

		char firstchar=val[0];
		char lastchar=val[strlen(val)-1];
		char *out=malloc(strlen(val)-1);
		int y;

		if (strcmp(fi,"port")==0) {
			config->port=strtol(val,&rest,10);
		}

		if (strcmp(fi,"host")==0) {
			if (firstchar=='"' && lastchar=='"') {
				trim_str(val,out);
				config->host=out;
			}
		}

		if (strcmp(fi,"db")==0) {
			if (firstchar=='"' && lastchar=='"') {
				trim_str(val,out);
				config->db=out;
			}
		}

		if (strcmp(fi,"usr")==0) {
			if (firstchar=='"' && lastchar=='"') {
				trim_str(val,out);
				config->usr=out;
			}
		}

		if (strcmp(fi,"table")==0) {
			if (firstchar=='"' && lastchar=='"') {
				trim_str(val,out);
				config->table=out;
			}
		}
		if (strcmp(fi,"pass")==0) {
			if (firstchar=='"' && lastchar=='"') {
				trim_str(val,out);
				config->pass=out;
			}
		}
	}
}

#endif
