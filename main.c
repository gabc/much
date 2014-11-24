#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

int
main(int argc, char *argv[])
{
	FILE *fp;
	FILE *usrin;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	int i = 0;

	if(argc == 1){
		fp = stdin;
	}else
		fp = fopen(argv[1], "r");

	if(fp == NULL){
		err(1, "cannot open %s", argv[1]);
		exit(EXIT_FAILURE);
	}

	usrin = fopen("/dev/tty", "r");
	if(usrin == NULL){
		err(1, "cannot open /dev/tty/");
	}

	while((read = getline(&line, &len, fp)) != -1){
		printf("%s", line);
		i++;
		if(i > 20){
			i = 0;
			fgetc(usrin);
		}
	}
	free(line);
	exit(EXIT_SUCCESS);
}
