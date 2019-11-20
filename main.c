#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

int main (int argc, char *argv[]) {
	
	if (argc > 6 ){
        printf("Too many arguments supplied\n");
		return 1;
    }
	else if (argc < 6) {
		printf("Too few arguments supplied\n");
		return 1;
	}

	//Args
	int blk_sz = atoi(argv[1]);		//Block size in bytes	
	int num_lns = atoi(argv[2]);	//Number of blocks in the cache
	int assoc = atoi(argv[3]);		//Associativity
	int lru_sel = atoi(argv[4]);	//0 - Random 	1 -LRU
	int data_file = atoi(argv[5]);	//Datafile name

	long int hits = 0;
	long int misses = 0;

	long int curr_addr;
	long int curr_index;
	long int curr_tag;
	long int curr_block_addr;

	int b_min;
	long int shift_temp;
	
	FILE *file_ptr;
	
	//------------------------------------------------------------------------------//

	b_min = ceill(log(num_lns) / log(2));		// find the number of bits needed

	shift_temp = 0;
	for (int i = 0; i < b_min; i++) {
		shift_temp = (shift_temp << 1) | 1;
	}

	long int tag[assoc][((shift_temp + 1) / assoc)];
	bool valid[assoc][(num_lns / assoc)];

	if ((file_ptr = fopen(argv[5], "rt")) == NULL){
		printf("Unable to open file at path specified.");
		return 1;
	}
	else {
		//Main Program after arg and file check

		for (int i = 0; i < num_lns; i++){
			
			fscanf(file_ptr, "%x", &curr_addr);
			curr_tag = curr_addr >> b_min;
			curr_index = curr_addr & shift_temp;
			curr_block_addr = curr_addr / blk_sz;
			
			//printf("curr addr = %li, curr tag = %li, curr block = %li, curr index = %li\n", curr_addr, curr_tag, curr_block_addr, curr_index);

			if (tag[assoc - 1][curr_index - 1] == curr_tag) {
				if (valid[assoc - 1][curr_index - 1] == 1) {
					hits++;
				}
				else {
					misses++;
				}
			}
			else {
				misses++;
				// what to do if find a tag but it is not valid?
				valid[assoc - 1][curr_index - 1] = 1;
				tag[assoc - 1][curr_index - 1] = curr_tag;
			}
		}
	}	
	
	printf("Cache size: %dk\n", ((blk_sz * num_lns) / 1000));
	printf("Reads: %d\n", num_lns);
	printf("Hits: %d\n", hits);
	printf("Misses: %d\n", misses);
	printf("Hit Rate: %2.2f%%\n", (((float) hits / num_lns) * 100));
	printf("Miss Rate: %2.2f%%\n", (((float) misses / num_lns)* 100));
	return 0;
}