//IMPORTANT: FILE MUST BE COMPILED WITH -lm

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h> 
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h>


//Global constant
#define PIPE "fifo_file"

//Structures
typedef enum Sizes
{
	FIRST_NAME_SIZE = 20,
	LAST_NAME_SIZE = 20,
	LIFE_SIZE = 1,
	eMaxSize = 21
} eSizes;

typedef enum Offsets
{
	FIRST_NAME_OFFSET = 0, 
	LAST_NAME_OFFSET = 0,
	LIFE_OFFSET = 0,
	MaxOffset = 0
} eOffsets;

typedef struct Participant{
	char first_name[20];
	char last_name[20];
	int life;
} eParticipant;

//Global arrays
static char buffer[eMaxSize] = {0}; // For first and last name
static int buffer1[eMaxSize] = {0}; // For life

//Functions
void getFirstName(const int, int);
void getLastName (const int);
void getLife(const int);
struct Participant battle(struct Participant player_one, struct Participant player_two);
void FisherYates(int *f_array_pairs, int number_of_elements);
struct Participant *tournament(struct Participant *array_structs, int *positions, int size_array);

int main(int argc, char **argv){
	srand(time(0)); 
	pid_t pid = 0;
	int named_pipe_fd = 0;
	


	(void) mkfifo(PIPE, 0666);
	
	pid = fork();

	if (argc > 1){
		int check = open(argv[1], O_RDONLY);
		//check whether rhe correcct binary file is provided 
		if((roundf(lseek(check, 0, SEEK_END)/44.0f))==(lseek(check, 0, SEEK_END)/44.0f)){
			printf("\nCorrect binary file provided\n");
		}else{
			printf("\nPlease provide the correct binary file\n");
			exit(-1);
		}
		
		close(check);
		
		if(pid > 0){ //This is parent

			int counter = 0; //This is used to change the offset
			int number_of_participants;
			int add_in_array = 0;
			srand(time(0)); 

			int fd = open(argv[1], O_RDONLY); //read from file 
			number_of_participants = (lseek(fd, 0, SEEK_END)/44); //how many participants are int book
			

			//Container - array from structs
			struct Participant participants[number_of_participants];
			memset(participants, 0, number_of_participants*sizeof(struct Participant)); //set the size of the container

			//Arrays
			
			lseek(fd, 0, SEEK_SET); //Set pointer to the beginning of the file
			
			for(int i = 0; i < number_of_participants; i++){
				getFirstName(fd, counter);  //function for reading from file
				strncpy(participants[i].first_name, buffer, 20); //copy content from buffer to model's structure
				memset(buffer, 0, sizeof(buffer)); //clear the buffer
				
				getLastName(fd);  //function for reading from file
				strncpy(participants[i].last_name, buffer, 20);  //copy content from buffer1 to model's structure
				memset(buffer, 0, sizeof(buffer)); //clear the buffer
				
				getLife(fd);  //function for reading from file
				participants[i].life = buffer1[0]; //copy content from buffer1 to model's structure
				memset(buffer1, 0, sizeof(buffer1)); //clear the buffer
				
				counter+=44;
			}

			//write the array in FIFO file.
			named_pipe_fd = open(PIPE, O_WRONLY); //write in the FIFO file

			int fifo_wr = write(named_pipe_fd, participants, sizeof(participants)); //write in the FIFO file
			if(fifo_wr < 0) {
				printf("\nERROR: Parent didnt't successfully write in the FIFO file\n");
			return -1;
			}
		
			close(named_pipe_fd);
			wait(NULL);

		}else if(pid <= 0){ //This is child
			int fd_2 = open(argv[1], O_RDONLY); //read from file 
			int arr_size = (lseek(fd_2, 0, SEEK_END)/44); //how many participants are int book
			close(fd_2);
		    
		    int c_rounds = 1;
			
			struct Participant read_all_participants_array[arr_size];
			struct Participant *all_participants_array;

			int array_pairs[arr_size];
			memset(array_pairs, 0, arr_size*sizeof(int)); //set the size of the array	
			
			named_pipe_fd = open(PIPE, O_RDONLY);
			
			if(read(named_pipe_fd, read_all_participants_array, sizeof(read_all_participants_array)) >= 0){
			
				all_participants_array = read_all_participants_array;

				while(1){
					FisherYates(array_pairs, arr_size); //this array is used to define which two players will fight, the array is with shuffled numbers			
					printf("\nRound: %d\n", c_rounds);
					c_rounds++;

					// this is one round, the function takes as an argument the list of players, the order in which they are fighting and the number of the participants
					struct Participant *winners = tournament(all_participants_array, array_pairs, arr_size); 
				
					all_participants_array = winners; //the winners from the round are stored ina the array
				
					arr_size = arr_size/2; //only half of the participants continue to the next round
				
					if(arr_size == 1){ //check if there is a winner
						printf("\n\nPlayer %s %s won the tournament\n", winners[0].first_name, winners[0].last_name);
					break;
					}
				}
			}else{
				printf("Child could not read from file\n");
			}
			close(named_pipe_fd);
		}else{
			printf("\nError, proccess not created\n");
		}
	}else{
		printf("\nPlease, provide a file with participants\n");
	}
	return 0;
}



void getFirstName(const int fd, int offset_counter){
	lseek(fd, FIRST_NAME_OFFSET + offset_counter, SEEK_SET); 
	read(fd, buffer, sizeof(char)*FIRST_NAME_SIZE);
}

void getLastName(const int fd){
	lseek(fd, LAST_NAME_OFFSET, SEEK_CUR); 
	read(fd, buffer, sizeof(char)*LAST_NAME_SIZE);
}

void getLife(const int fd){
	lseek(fd, LIFE_OFFSET, SEEK_CUR); 
	read(fd, buffer1, sizeof(int)*LIFE_SIZE);
}


struct Participant battle(struct Participant player_one, struct Participant player_two){
	int first_fighter;

	first_fighter = rand() % (1 + 1 - 0) + 0; //define who will strike first

	if(first_fighter == 0){	//the first player strikes first
		
		while(1){
			
			player_two.life = player_two.life - (rand() % (20 + 1 - 1) + 1); //generate punch whitch takes life up to 20
			
			if(player_two.life <=0){
				//check if the second fighter has lost
				player_two.life = 0;

				printf("Player %s %s won with life %d", player_one.first_name, player_one.last_name, player_one.life);
				printf("   Player %s %s lost with life %d\n", player_two.first_name, player_two.last_name, player_two.life);

				player_one.life = 100; //the person who won the fight
				return player_one;
			}
			
			player_one.life = player_one.life - (rand() % (20 + 1 - 1) + 1); //generate punch whitch takes life up to 20
			if(player_one.life <= 0){
				//check if the first fighter has lost
				player_one.life = 0; //the person who lost the fight

				printf("Player %s %s lost with life %d", player_one.first_name, player_one.last_name, player_one.life);
				printf("   Player %s %s won with life %d\n", player_two.first_name, player_two.last_name, player_two.life);

				player_two.life = 100; //the person who won the fight
				return player_two;
			}
		}

	}else{ //the second player strikes first

		while(1){

			player_one.life = player_one.life - (rand() % (20 + 1 - 1) + 1); //generate punch whitch takes life up to 20
			//check if the first fighter has lost
			if(player_one.life <=0){
				
				player_one.life = 0; //the person who lost the fight

				printf("Player %s %s won with life %d", player_two.first_name, player_two.last_name, player_two.life);
				printf("   Player %s %s lost with life %d\n", player_one.first_name, player_one.last_name, player_one.life);
				
				player_two.life = 100; //the person who won the fight
				return player_two;
			}
			
			player_two.life = player_two.life - (rand() % (20 + 1 - 1) + 1); //generate punch whitch takes life up to 20
			//check if the second fighter has lost
			if(player_two.life <= 0){

				player_two.life = 0; //the person who lost the fight
				
				printf("Player %s %s lost with life %d", player_two.first_name, player_two.last_name, player_two.life);
				printf("   Player %s %s won with life %d\n", player_one.first_name, player_one.last_name, player_one.life);
				
				player_one.life = 100; //the person who won the fight
				return player_one;
			}
		}
	}
}


void FisherYates(int *f_array_pairs, int number_of_elements) { //implementation of Fisher
    int i, j, tmp; // create local variables to hold values for shuffle
	
	//fill the array with sequence numbers

	for(int count = 0; count < number_of_elements; count++){
		f_array_pairs[count] = count;
	}

    //shuffle tha array
    for (i = number_of_elements - 1; i >= 0; i--) { // for loop to shuffle
        j = rand() % (i + 1); //randomise j for shuffle with Fisher Yates
        tmp = f_array_pairs[j];
        f_array_pairs[j] = f_array_pairs[i];
        f_array_pairs[i] = tmp;
     }
}

struct Participant *tournament(struct Participant *array_structs, int *positions, int size_array){
	int c_arr = 0, f_counter = 0;
	struct Participant *winners = (struct Participant*) calloc((size_array/2), sizeof(struct Participant));
	
	while(f_counter < size_array){
		
		winners[c_arr] = battle(array_structs[positions[f_counter]], array_structs[positions[f_counter+1]]);
		printf("\n");
		f_counter = f_counter + 2;
		c_arr++;
	}
	return winners;
}