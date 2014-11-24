#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

int
main(int argc, char *argv[])
{
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	int i = 0;

	if(argc < 2){
		err(1, "need to specify a file\n");
		exit(EXIT_FAILURE);
	}
	fp = fopen(argv[1], "r");
	if(fp == NULL){
		err(1, "cannot open %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	while((read = getline(&line, &len, fp)) != -1){
		printf("%s", line);
		i++;
		if(i > 20){
			i = 0;
			getchar();
		}
	}
	free(line);
	exit(EXIT_SUCCESS);
}
