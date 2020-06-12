#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "queue.h"
#include "utility.h"
#include "hostd.h"


#define MEMORY 1024


proc process;
node_t *queue;
node_t *dispatch_queue;
node_t *real_time, priority_1, priority_2, priority_3;
int dispatch_time = 0;
int index;


int main(int argc, char *argv[])
{


	resources resources;
	for (int i = 0; i < MEMORY; i++){
		resources.mbytes[i] = 0;
	}
	resources.scanners = 1;
	resources.printers = 2;
	resources.modems = 1;
	resources.cds = 2;

	FILE file = argv[1];
	node_t *cur_node;


	load_dispatch(file, queue);

	char *chars[256] = {0};
	chars[0] = "./process";


	cur_node = queue;
	while (cur_node != NULL){


		if ((cur_node -> p).arrival_time == dispatch_time){
			if ((cur_node -> p).priority == 0)
				push(real_time, cur_node -> p);
			if ((cur_node -> p).priority == 1)
				push(priority_1, cur_node -> p);
			if ((cur_node -> p).priority == 2)
				push(priority_2, cur_node -> p);
			if ((cur_node -> p).priority == 3)
					push(priority_3, cur_node -> p);

			dispatch_time = dispatch_time + 1;
		}

		while (cur_node != NULL){
			if (real_time != NULL){
				index = alloc_mem(res, (real_time -> p).mbytes);
				pid_t pid = fork();

				if (pid == 0)
					execvp (chars[0], chars);
				else
					waitpid(pid, 0, 0);
				free_mem(res, index, (real_time->p).mbytes);
			}
		}
		while(priority_1 != NULL){
			index.alloc_mem(res, priority_1->p).mbytes);
			pid_t pid = fork();
			pop(priority_1);

			push(priority_2, process);	

			
			if (pid == 0){
				signal(SIGINT, SIG_DFL);
				signal(SIGCONT, SIG_DFL);
				signal(SIGSTP, SIG_DFL);
				execvp(chars[0], chars);
			} else if (pid != 0){
				kill(pid,SIGINT);
         kill(pid,SIGCONT);
         kill(pid,SIGTSTP);
         waitpid(pid, 0, 0);
			}


			push(priority_2, priority_1->p);
			free_mem(res, index, (priority_1->p).mbytes);

			priority_1 = priority_1->next;
		}

		while(priority_2 != NULL){
			index.alloc_mem(res, priority_2->p).mbytes);
			pid_t pid = fork();
			pop(priority_2);

			push(priority_2, process);	


			if (pid == 0){
				signal(SIGINT, SIG_DFL);
				signal(SIGCONT, SIG_DFL);
				signal(SIGSTP, SIG_DFL);
				execvp(chars[0], chars);
			} else if (pid != 0){
				kill(pid,SIGINT);
         kill(pid,SIGCONT);
         kill(pid,SIGTSTP);
         waitpid(pid, 0, 0);
			}


			push(priority_3, priority_2->p);
			free_mem(res, index, (priority_2->p).mbytes);

			priority_2 = priority_2->next;
		}

		while(priority_3 != NULL){
			index.alloc_mem(res, priority_3->p).mbytes);
			pid_t pid = fork();
			pop(priority_3);

			push(priority_3, process);	


			if (pid == 0){
				signal(SIGINT, SIG_DFL);
				signal(SIGCONT, SIG_DFL);
				signal(SIGSTP, SIG_DFL);
				execvp(chars[0], chars);
			} else if (pid != 0){
				kill(pid,SIGINT);
         kill(pid,SIGCONT);
         kill(pid,SIGTSTP);
         waitpid(pid, 0, 0);
			}


			push(priority_3, priority_3->p);
			free_mem(res, index, (priority_3->p).mbytes);

			priority_3 = priority_3->next;
		}

	}

	cur_node = cur_node->next;
	real_time = real_time->next;


    return EXIT_SUCCESS;
}


void tokenize_char(char *input, char **tokens){

	int counter = 0;
	char *token = 0;
	char delimiter[1] = ",";
	char *new_line = strchr(input, "\n");

	while (new_line){
		*new_line = 0;
		new_line = strchr(input, "/n");
	}

	token = strtok(input, delimiter);
	while (token != NULL){
		tokens[counter] = token;
		token = strtok(NULL, delimiter);
		counter++;
	}
}
