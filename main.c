#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define MAX_COUNT_VAL 8

//Adam is free 8:45 - 11:30 and 2 - 4 and maybe after 5:30

//numblocks in blocks per line
//cache size  = line number

int main (int argc, char *argv[]) {
	
	//Argument Count Checking
	
	if (argc > 6 ){
        printf("Too many arguments supplied\n");
		return 1;
    }
	else if (argc < 6) {
		printf("Too few arguments supplied\n");
		return 1;
	}
	
	//File Intake Checking

	FILE *file_ptr;

	if ((file_ptr = fopen(argv[5], "r")) == NULL){
		printf("Unable to open file at path specified.");
		return 1;
	}

	//Args
	int blk_sz = atoi(argv[1]);		//Block size in bytes	
	int num_elm = atoi(argv[2]);	//Number of blocks in the cache
	int assoc = atoi(argv[3]);		//Associativity
	int lru_sel = atoi(argv[4]);	//0 - Random 	1 -LRU
	int data_file = atoi(argv[5]);	//Datafile name

	//Hit and miss variables
	long int hits = 0;
	long int misses = 0;
	bool hit_flag = 0;
	bool miss_flag = 0;
	bool miss_done_flag = 0;
	bool nonvalid_exists = 0;
	long int rand_tries = 0;
	long int max_count_index = 0;
	
	long int num_reads = 0;

	//Address broken out
	long int curr_addr;
	long int curr_index;
	long int curr_tag;
	long int curr_block_addr;

	//Offset/index length variables
	long int b_min;
	long int shift_temp;
	long int offset_bits;

	//Time initialization for random placememnt
	time_t tm;
	srand((unsigned) time(&tm));
	int temp_rand = 0;

	//Check to make sure associativity is a power of 2 and not 0
	if (((assoc & (assoc - 1)) != 0) || (assoc == 0)) {
		printf("Please enter an associativity that is > 0 and of the form 2^n.");
		return 1;
	} 
	//Check to make sure block size is a power of 2 and not 0
	if (((blk_sz & (blk_sz - 1)) != 0) || (blk_sz == 0)) {
		printf("Please enter a block size that is > 0 and of the form 2^n.");
		return 1;
	} 
	
	//Determine the number of bits needed to represent the offset based on the block size
	offset_bits = ceill(log(blk_sz) / log(2));	
	
	//Determine the number of bits needed to represent the index based on the number of elements and the block size
	b_min = ceill(log(num_elm) / log(2));
	if (assoc > 1) {
		b_min = b_min - ((int) (ceill(log(assoc) / log(2))));
	}
	
	//Generate a mask for the length of the index, shifted later for offset consideration
	shift_temp = 0;
	for (int i = 0; i < b_min; i++) {
		shift_temp = (shift_temp << 1) | 1;
	}
	
	//printf("shift temp = %d\n", shift_temp);
	
	//Generate arrays for tags, valid bit, and count variables
	
	/*
	long int *tag[(shift_temp + 1)]; 
	for (int x = 0; x < (shift_temp + 1); x++) 					///try removing 1
         tag[x] = (long int *)malloc(assoc * sizeof(long int));
	bool *valid[(shift_temp + 1)]; 
	for (int y = 0; y < (shift_temp + 1); y++) 
         valid[y] = (bool *)malloc(assoc * sizeof(bool));
	int *count[(shift_temp + 1)]; 
	for (int z = 0; z < (shift_temp + 1); z++) 
         count[z] = (int *)malloc(assoc * sizeof(int));
	*/
	
	int tag[assoc][((shift_temp + 1))];
	bool valid[assoc][((shift_temp + 1))];
	int count[assoc][((shift_temp + 1))];
	
	//int index[assoc][((shift_temp + 1))];
	
	//Shift the mask now that is has been used to generate the arrays
	shift_temp = shift_temp << offset_bits;

	//------------------------------------------------------------------------------//

	//For each element in the file...
	for (int i = 0; !(feof(file_ptr)); i++){
		
		fscanf(file_ptr, "%x", &curr_addr);						//Scan a value in from the file
		num_reads++;
		curr_tag = curr_addr >> (b_min + offset_bits);			//Generate the tag by shifting the size of the index (b_min) and the size of the offset (offset_bits)
		curr_index = (curr_addr & shift_temp) >> offset_bits;	//Generate the index by ANDing the value from the file with the mask, then shift it over by the offset
		curr_block_addr = curr_addr / blk_sz;					//determine the current block address based on the current address and the block size
		
		//printf("curr addr = %li, curr tag = %li, curr block = %li, curr index = %li\n", curr_addr, curr_tag, curr_block_addr, curr_index);	//DEBUG

		//For each element in the index/set...
		for (int j = 0; j < assoc; j++){
			if ((valid[j][curr_index - 1] == 1) && (tag[j][curr_index - 1] == curr_tag)) {					//If valid flag is set and the tags match...
				hits++;																						//Increase the hits...
				miss_flag = 0;																				//Clear the miss flag...
				if (lru_sel = 1) {																			//If LRU is selected instead of random...
					for (int m = 0; m < assoc; m++) {														//Loop through each of the values in the index/set
						if ((valid[m][curr_index - 1] == 1) && (tag[m][curr_index - 1] == curr_tag) &&(count[m][curr_index - 1] < MAX_COUNT_VAL)) {	//If the valid bit is set and the counter < some maximum value...
							count[m][curr_index - 1] = count[m][curr_index - 1] + 1;						//Increase the counter for that value
						}
					}
					count[j][curr_index - 1] = 1;						//If a hit, reset the counter for the element to 1
				}
				break;													//Break out of the for loop if a hit already happened
			}
			else {
				miss_flag = 1;											//Set the miss flag if valid not set or tags don't match
				//index[j][curr_index - 1] = curr_addr;
			}
		}
		if (miss_flag == 1) {											//If miss flag is set after for loop...
			misses++;													//Increase the misses...
			miss_flag = 0;												//Reset the miss flag...
			if (lru_sel == 0) {											//If random is selected...
				for (int n = 0; n < assoc; n++) {
					if (valid[n][curr_index - 1] == 0) {
						nonvalid_exists = 1;
					}
				}
				while (nonvalid_exists) {								//While a value has not been placed...
					temp_rand = rand() % assoc;							//Generate a random value less than the associativity (column number)
					if (valid[temp_rand][curr_index - 1] == 0){			//If the random value index is not valid 
						valid[temp_rand][curr_index - 1] = 1;			//Set the valid bit
						tag[temp_rand][curr_index - 1] = curr_tag;		//Add the tag
						nonvalid_exists = 0;							//Set a flag that the value has been placed somewhere
						break;											//Break out of the while loop
					}
				}
			}
			
			else {														//If LRU is selected...
				for (int k = 0; k < assoc; k++) {
					if (count[k][curr_index - 1] >= count[max_count_index][curr_index - 1]){	//Find the value with the maximum count
						max_count_index = k;													//Save that index
					}
				}
				valid[max_count_index][curr_index - 1] = 1;										//Set the valid flag at that index
				tag[max_count_index][curr_index - 1] = curr_tag;								//Set the tag at that index
				max_count_index = 0;															//Reset the index
			}
		}
	}
	
	/*
	if (num_elm == 16) {
		printf("Set 0 = %d, %d\n", index[0][0], index[0][1]);
		printf("Set 1 = %d, %d\n", index[0][2], index[0][3]);
		printf("Set 2 = %d, %d\n", index[0][4], index[0][5]);
		printf("Set 3 = %d, %d\n", index[0][6], index[0][7]);
		printf("Set 4 = %d, %d\n", index[0][8], index[0][9]);
		printf("Set 5 = %d, %d\n", index[0][10], index[0][11]);
		printf("Set 6 = %d, %d\n", index[0][12], index[0][13]);
		printf("Set 7 = %d, %d\n", index[0][14], index[0][15]);
	}
	*/
	
	//Print required outputs
	printf("Cache size: %dk\n", ((num_reads) / 1000));
	printf("Reads: %d\n", num_reads);
	printf("Hits: %d\n", hits);
	printf("Misses: %d\n", misses);
	printf("Hit Rate: %2.2f%%\n", (((float) hits / num_reads) * 100));
	printf("Miss Rate: %2.2f%%\n", (((float) misses / num_reads)* 100));
	
	return 0;
}