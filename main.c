#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

int main (int argc, char *argv[]) {
	
	if (argc > 6 ){
        printf("Too many arguments supplied\n");
		return 1;
    }
	else if (argc < 6) {
		printf("Too few arguments supplied\n");
		return 1;
	}

	FILE *file_ptr;

	if ((file_ptr = fopen(argv[5], "rt")) == NULL){
		printf("Unable to open file at path specified.");
		return 1;
	}

	//Args
	int blk_sz = atoi(argv[1]);		//Block size in bytes	
	int num_elm = atoi(argv[2]);	//Number of blocks in the cache
	int assoc = atoi(argv[3]);		//Associativity
	int lru_sel = atoi(argv[4]);	//0 - Random 	1 -LRU
	int data_file = atoi(argv[5]);	//Datafile name

	long int hits = 0;
	long int misses = 0;
	bool hit_flag = 0;
	bool miss_flag = 0;
	bool miss_done_flag = 0;

	long int curr_addr;
	long int curr_index;
	long int curr_tag;
	long int curr_block_addr;

	long int b_min;
	long int shift_temp;

	time_t tm;
	srand((unsigned) time(&tm));
	int temp_rand = 0;

	if ((assoc & (assoc - 1)) != 0) {
		printf("Please enter an associativity that is 2^n.");
		return 1;
	} 

	b_min = ceill(log(num_elm) / log(2));		// find the number of bits needed for all elements
	if (assoc > 1) {
		b_min = b_min - ((int) (ceill(log(assoc) / log(2))));
	}

	shift_temp = 0;
	for (int i = 0; i < b_min; i++) {
		shift_temp = (shift_temp << 1) | 1;
	}

	//printf("shift temp = %li\n", shift_temp);

	long int tag[assoc][((shift_temp + 1))];
	bool valid[assoc][((shift_temp + 1))];
	int count[assoc][((shift_temp + 1))];

	//------------------------------------------------------------------------------//

	for (int i = 0; i < num_elm; i++){
		
		fscanf(file_ptr, "%x", &curr_addr);
		curr_tag = curr_addr >> b_min;
		curr_index = curr_addr & shift_temp;
		curr_block_addr = curr_addr / blk_sz;
		
		//printf("curr addr = %li, curr tag = %li, curr block = %li, curr index = %li\n", curr_addr, curr_tag, curr_block_addr, curr_index);

		for (int j = 0; j < assoc; j++){
			if ((valid[j][curr_index - 1] == 1) && (tag[j][curr_index - 1] == curr_tag)) {
				hit_flag = 1;
				break;
			}
			else {
				miss_flag = 1;
				for (int k = 0; k < assoc; k++){
					if (valid[j][curr_index - 1] == 0) {
						valid[j][curr_index - 1] = 1;
						tag[j][curr_index - 1] = curr_tag;
						miss_done_flag = 1;
						break;
					}
				}
				if (miss_done_flag == 1) {
					miss_done_flag = 0;
					break;
				}
				else {		//if tag not found in line then randomly pick one to replace
					temp_rand = rand() % assoc;
					valid[temp_rand][curr_index - 1] = 1;
					tag[temp_rand][curr_index - 1] = curr_tag;
				}
			}
		}
		if (miss_flag == 1) {
			misses++;
			miss_flag = 0;
		}
		if (hit_flag == 1) {
			hits++;
			hit_flag = 0;
		}
		
	}
	
	printf("Cache size: %dk\n", ((blk_sz * num_elm) / 1000));
	printf("Reads: %d\n", num_elm);
	printf("Hits: %d\n", hits);
	printf("Misses: %d\n", misses);
	printf("Hit Rate: %2.2f%%\n", (((float) hits / num_elm) * 100));
	printf("Miss Rate: %2.2f%%\n", (((float) misses / num_elm)* 100));
	return 0;
}