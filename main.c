#include <stdio.h>
#incude <stdlib.h>

int main (int argc, char *argv[]) {
	
	//Args
	int blk_sz = *argv[1];		//Block size in bytes
	int num_lns = *argv[2];		//Number of blocks in the cache
	int assoc = *argv[3];		//Associativity
	int hit_tm = *argv[4];		//Hit time
	int miss_tm = *argv[5];		//Miss time
	int lru_sel = *argv[6];		//LRU
	int data_file = *argv[7];	//Datafile name
	
	//num_blk		//number of blocks = associativity * number of sets
	
	FILE *file_ptr;
	
    if( argc = 8 ) {
        printf("The proper number of arguments have been supplied...\n");
		
		if ((file_ptr = fopen(argv[7], "r") == NULL){
			printf("Unable to open file at path specified.";
			return 1;
		}
		
		
		
		
		
		
		
	}
    else if (argc > 8 ){
        printf("Too many arguments supplied\n");
		return 1;
    }
	else {
		printf("Too few arguments supplied\n");s
		return 1;
	}
	
	printf("Cache size: %dk", 
	
	return 0;
}