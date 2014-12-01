#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

FILE *fp;
FILE *usrin;
char *line = NULL;
size_t len = 0;
ssize_t read;

void
getl(int n)
{
	int i;
	for(i = 0; i < n; i++){
		if(getline(&line, &len, fp) == -1)
			return;
	}
}

void
cleanup(void)
{
	free(line);
	fclose(fp);
	fclose(usrin);
}

void
openfile(void)
{
	if(fp == NULL){
		err(1, "cannot open %s", argv[1]);
		exit(EXIT_FAILURE);
	}
	usrin = fopen("/dev/tty", "r");
	if(usrin == NULL)
		err(1, "cannot open /dev/tty/");
}

int
main(int argc, char *argv[])
{
	int i;

	if(argc == 1)
		fp = stdin;
	else
		fp = fopen(argv[1], "r");

	openfile();
	
	i = 0;
	while((read = getline(&line, &len, fp)) != -1){
		printw("%s", line);
		i++;
		if(i > 20){
			i = 0;
			fgetc(usrin);
		}
	}

	cleanup();
	exit(EXIT_SUCCESS);
}
