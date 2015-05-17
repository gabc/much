
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <curses.h>
#include <signal.h>

#define BUFSIZE 2048

char **buffer;
int start = 0;
int end;
int v_start;
int v_end;

void
finish(int sig)
{
        endwin();
        exit(sig);
}

void
repaint()
{
        int i;

        clear();
        for(i = v_start; i < v_end; i++) {
                printw("%s", buffer[i]);
        }
        refresh();
}

void
godown()
{
        if(v_end == end)
                return;
        v_end++;
        v_start++;
        repaint();
}

void
goup()
{
        if(v_start == start)
                return;
        v_end--;
        v_start--;
        repaint();
}
void
gostart()
{
        v_start = 0;
        v_end = LINES;
        repaint();
}

void
goend()
{
        v_start = end - LINES;
        v_end = end - 1;
        repaint();
}

void
init_buffer(void)
{
        buffer = calloc(BUFSIZE, sizeof(buffer));
        if(!buffer)
                err(1, "Can't allocate buffer\n");
}

int
read_file(FILE *fp)
{
        int i;
        ssize_t read;
        size_t len = 0;
        char *line = NULL;

        if(!fp)
                err(1, "Need a file\n");
for(i = 0; i < BUFSIZE; i++){
        if((read = getline(&line, &len, fp)) != -1){                   
                buffer[i] = line;
                line = NULL;
        } else
                break;
}
end = i;
        if(end < LINES){
                for(i = 0; i < end; i++)
                        printw("%s", buffer[i]);
                LINES = end;
        }

        return i;
}


int
main(int argc, char **argv)
{
        FILE *fp;
        FILE *in;

        if(argc == 2){
                fp = fopen(argv[1], "r");
                in = stdin;
        } else {
                fp = stdin;
                in = fopen("/dev/tty", "r");
        }
        if(!fp || !in)
                err(1, "Cannot open files");


        signal(SIGINT, finish);

        initscr();
        keypad(stdscr, TRUE);
        nonl();
        cbreak();
        noecho();

        init_buffer();
        read_file(fp);

        v_end = LINES;
        repaint();

        for(;;){
                int c = fgetc(in);
                switch(c){
                case 'j':
                        godown();
                        break;
                case 'k':
                        goup();
                        break;
                case '<':
                        gostart();
                        break;
                case '>':
                        goend();
                        break;
                case 'q':
                        finish(0);
                        break;
                }
        }
        finish(0);
        return 0;
}