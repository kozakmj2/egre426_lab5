#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) {
	
	//Args
	int blk_sz = *argv[1];		//Block size in bytes
	int num_lns = *argv[2];		//Number of blocks in the cache
	int assoc = *argv[3];		//Associativity
	int lru_sel = *argv[4];		//0 - Random 	1 -LRU
	int data_file = *argv[5];	//Datafile name
	
	//num_blk		//number of blocks = associativity * number of sets
	
	FILE *file_ptr;
	
    if( argc = 6 ) {
        printf("The proper number of arguments have been supplied...\n");
		
		if ((file_ptr = fopen(argv[7], "r") == NULL){
			printf("Unable to open file at path specified.";
			return 1;
		}
		
		
		
		
		
		
		
	}
    else if (argc > 6 ){
        printf("Too many arguments supplied\n");
		return 1;
    }
	else {
		printf("Too few arguments supplied\n");s
		return 1;
	}
	
	printf("Cache size: %dk\n", blk_sz * num_lns);
	printf("Reads: %dk\n", blk_sz * num_lns);
	printf("Hits: %dk\n", blk_sz * num_lns);
	printf("Misses: %dk\n", blk_sz * num_lns);
	printf("Hit Rate: %2.2f\n", ((hits / reads) * 100));
	printf("Miss Rate: %2.2f\n", ((misses / reads)* 100));
	return 0;
}