#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "/c/cs323/Hwk2/code.h"

#define TRUE 1
#define FALSE 0

int MAXBITS = 12;
int CURRENT_CODE = 0;
int FULL = FALSE;

// the table in going to be a global variable

// trie needs to have a root
struct Trie{
		int character;
		int* children;
		int num_children;
		int prefix_code;
		int num_appearances;
};


struct Stack{
	int character;
	struct Stack* last;
};


struct Trie** TABLE;

struct Trie* new_trie(int character, int prefix_code, int used);
void initialize_table();
int add_substring(int where_at, int character, int num_bits, int index);
int binary_search(int index, int key, int min_index, int max_index, int* to_insert);
int midpoint(int min_index, int max_index);
//int get_prefix(int where_at);
void insert(int int_index, int table_index, int value);
struct Stack* initialize_stack();
struct Stack* push(int character, struct Stack* current);
int pop(struct Stack** current);
void free_table();
void new_table(int new_bits);
void free_stack(struct Stack* my_stack);
void table_stderr();
int* new_indices(int* numer_remaining, int initialize);
void free_single_trie(int index);
void pruned_table(int* where_look, int how_many);
struct Trie* create_new_entry(int* where_look, int index);
void do_prune(int initialize);
int new_num_bits(int how_many);

// TODO
// modify to put out and read MAXBITS:


int main (int argc, char** argv){
		//first you want to test the functionality of the trie
		// let's do the command line processing
		int encode;
		int prune = FALSE;
		int initialize = TRUE;
		int length_first_arg = strlen(argv[0]);
		// TODO what if you have another file called decode symlinked to encode?
		if (length_first_arg < 6){
			fprintf(stderr, "invalid first parameter\n");
		}
		if (strcmp(argv[0]+length_first_arg-6,  "decode") == 0){
				if (argc > 1){
						fprintf(stderr, "you shouldn't have arguments after decode\n");
						return 0;
				}
				encode = FALSE;
		}

		else{
				if (strcmp(argv[0]+length_first_arg-6, "encode") != 0){
						fprintf(stderr, "no encode or decode paramter!\n");
						return 0;
				}
				encode = TRUE;
				int i = 1;
				while (i < argc){
						char *of_interest = argv[i];
						if (strcmp(of_interest, "-m") == 0){
								i += 1;
								if (i == argc){
										fprintf(stderr, "you need a number after -m\n");
										return 0;
								}
								of_interest = argv[i];
								char *pointer;
								int caught = (int) strtol(of_interest, &pointer, 10);
								if (pointer == of_interest){
										fprintf(stderr, "you need a number after -m\n");
										return 0;
								}
								if (caught <= 0){
									fprintf(stderr, "you need a positive number after -m\n");
								}
								if (caught > 8 && caught <= 20){
										MAXBITS = caught;
								}
								else{
										MAXBITS = 12;
								}
								i += 1;
								continue;
						}
						else if (strcmp(of_interest, "-e") == 0){
								initialize = FALSE;
								i += 1;
								continue;
						}
						else if (strcmp(of_interest, "-p") == 0){
								prune = TRUE;
								i += 1;
								continue;
						}
						else {
								fprintf(stderr, "invalid argument\n");
								return 0;
						}
				}
		}
		
		if (encode){
			int num_bits;
			if (initialize){
				num_bits = 9;
				TABLE = calloc(1<<num_bits, sizeof(struct Trie*));
				CURRENT_CODE = 256+3;
				initialize_table();				
			} else {
				num_bits = 2;
				TABLE = calloc(1<<num_bits, sizeof(struct Trie*));
				TABLE[0] = new_trie(-1, -1, FALSE);
				CURRENT_CODE = 3;
			}

			putBits(5, MAXBITS);
			putBits(1, initialize);
			putBits(1, prune);
			//printf("%d:%d:%d:", MAXBITS, prune, !initialize);

			int where_at = 0;
			int num_children;
			int index = 0;
			int to_insert = TRUE;
			int returned = getBits(8);

			



			//printf("current code: %d\n", CURRENT_CODE);
			while(returned != EOF){
				// add substring returns the index where returned is in the children of TABLE[where_at]

				// step 1: read
				// done

				// step 2: puts
				// need to know index

				//printf( "\nreturned: %d\n", returned);

				num_children = TABLE[where_at]->num_children;
				TABLE[where_at]->num_appearances += 1;
				//index is where to insert in TABLE[where_at]
				if (where_at == 0){
					//printf("CURRENT CODE is: %d\n", CURRENT_CODE);	
				}
				
				index = binary_search(where_at, returned, 0, num_children-1, &to_insert);

				//printf("where_at: %d\n", where_at);
				
				if (to_insert){
					//fprintf(stderr, "%d\n", index);
					if (where_at == 0){
						//printf("1\n%d\n", character);
						//fprintf(stderr, "%d 1\n%d\n", CURRENT_CODE, character);
						// fprintf(stderr, "%d:%d\n", num_bits, 1);
						// fprintf(stderr, "%d:%d\n", 8, returned);
						
						
						putBits(num_bits, 1);
						putBits(8, returned);
						

					} else {
						//printf("%d\n", where_at);
						//fprintf(stderr, "%d %d\n", CURRENT_CODE, where_at);
						// fprintf(stderr, "%d:%d\n", num_bits, where_at);
						
						putBits(num_bits, where_at);
					}
				}


				if (to_insert && where_at == 0){
					//printf("CURRENT CODE is now: %d\n", CURRENT_CODE);
				}


				if (!FULL){
				//printf("current code: %d\n", CURRENT_CODE);
					if (CURRENT_CODE == (1<<num_bits) && to_insert){
						if (num_bits == MAXBITS){
							//fprintf(stderr, "%d\n", where_at);
							if (prune){
								table_stderr();
								//return 0;
								//putBits(num_bits, returned);
								do_prune(initialize);
								//fprintf(stderr, "got here\n");
								num_bits = new_num_bits(CURRENT_CODE);
								//fprintf(stderr, "%d %d\n", CURRENT_CODE, num_bits);
								to_insert = FALSE;
								where_at = 0;
								//returned = getBits(8);
								continue;
								//table_stderr();
							} else {
								FULL = TRUE;
							}
							//table_stderr();
							//do_prune(initialize);
							//return 0;
							//table_stderr();
							//return 0;
							//fprintf(stderr, "------------------------------------\n");
							//table_stderr();
							//num_bits = new_num_bits(CURRENT_CODE);
							//table_stderr();
							//fprintf(stderr, "num new bits is %d\n", num_bits);
							//table_stderr();
						} else {
							num_bits += 1;
							new_table(num_bits);
						}
						/*
						if(num_bits == MAXBITS){
							FULL = TRUE;
							//fprintf(stderr, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
						} else {
							num_bits += 1;
							new_table(num_bits);
						}*/
						
					}
				}
					//printf("num bits: %d\n", num_bits);

				//printf("CURRENT CODE after checking is: %d\n\n", CURRENT_CODE);

				

				
				//fprintf(stderr, "%d\n", CURRENT_CODE);
				// add_substring returns NULL if it inserts it, or else returns the entry where the next character was found
				// with the -e flag, you need to check if add_substring returns null 
				//caught = add_substring(where_at, returned, num_bits);
				// caught being -1 means that something could have been inserted
				// and the code was emitted. if where_at is 0, then it was a new code. else it wasn't

				if (to_insert){
					if (!FULL){
						struct Trie* new_entry = new_trie(returned, where_at, TRUE);
						//fprintf(stderr, "CURRENT CODE is: %d\n", CURRENT_CODE);
						insert(index, where_at, CURRENT_CODE);
						TABLE[CURRENT_CODE] = new_entry;
						CURRENT_CODE += 1;
					}
					if (where_at != 0){
						where_at = 0;
						continue;
					}
					//printf("CURRENT CODE was: %d\n", CURRENT_CODE);
				} else {
					//fprintf(stderr, "where at: %d %d\n", where_at, index);
					where_at = (TABLE[where_at]->children)[index];
				}

				/*
				if (to_insert){
					if (!FULL){
						CURRENT_CODE += 1;
					}
					if (where_at != 0){
						where_at = 0;
						continue;
					}
				} else {
					where_at = caught;
				}
				*/
				returned = getBits(8);
			}
			//printf("finished loop\n");
			TABLE[where_at]->num_appearances += 1;
			if (where_at != 0){
				num_children = TABLE[where_at]->num_children;
				//printf("where at: %d\n", where_at);
				index = binary_search(where_at, returned, 0, num_children-1, &to_insert);
				//where_at = add_substring(where_at, returned, num_bits);
				if (to_insert){
					/*
					if (where_at == 0){
						//printf("1\n%d\n", character);
						//fprintf(stderr, "%d 1\n%d\n", CURRENT_CODE, character);
						printf("%d:%d\n", num_bits, 1);
						printf("%d:%d\n", 8, returned);
						//putBits(num_bits, 1);
						//putBits(8, character);
					} else {
						*/
						//printf("%d\n", where_at);
						//fprintf(stderr, "%d %d\n", CURRENT_CODE, where_at);
						// fprintf(stderr, "%d:%d\n", num_bits, where_at);
						
						putBits(num_bits, where_at);
					
					//CURRENT_CODE += 1;
				}
			}
			if (!FULL && (CURRENT_CODE+1 == 1<< num_bits) && to_insert){
				num_bits += 1;
			}
			//fprintf(stderr, "%d 2 %d\n", CURRENT_CODE, num_bits);
			
			// fprintf(stderr, "%d:%d\n", num_bits, 2);

			
			putBits(num_bits, 2);
			flushBits();
			


			//putBits(1, 1);
			//putBits(num_bits, 2);
			//printf("2\n");
			//printf("%d\n", CURRENT_CODE);
			//int last_char = get_prefix(267);
			//printf("\n");
			//printf("%c\n", last_char);
			table_stderr();
			free_table();
			return 0;
		}


		else{
			MAXBITS = getBits(5);
			initialize = getBits(1);
			prune = getBits(1);
			int num_bits;
			int one_more;
			//printf("MAXBITS: %d\n-e: %d\n-p: %d\n", MAXBITS, initialize, prune);
			if (initialize){
				num_bits = 9;
				TABLE = calloc(1<<num_bits, sizeof(struct Trie*));
				initialize_table();
				CURRENT_CODE = 256+3;
				one_more = FALSE;
			} else {
				num_bits = 2;
				TABLE = calloc(1<<num_bits, sizeof(struct Trie*));
				TABLE[0] = new_trie(-1, -1, FALSE);
				CURRENT_CODE = 3;
				TABLE[1] = new_trie(-1, 0, FALSE);
				one_more = TRUE;
			}
			int newC = getBits(num_bits);
			int oldC = 0;
			int C = newC;
			int prefix;
			int finalK;

			struct Stack* my_stack = initialize_stack();

			one_more = FALSE;

			//int one_more = FALSE;
			int k_omega_k = FALSE;
			FULL = FALSE;
			int just_written = FALSE;
			//printf("sldkfljdkf\n");
			//printf("maxbits: %d initialize: %d numbits: %d\n", MAXBITS, initialize, num_bits);
			while (newC != 2){

				/*
				if (C == 1){
					finalK = getBits(8);
				}
				if (C >= CURRENT_CODE){
					my_stack = push(finalK, my_stack);
					C = oldC;
				}
				prefix = TABLE[C]->prefix_code;
				while (prefix != 0){
					my_stack = push(TABLE[C])
				}
*/


				// if CURRENT_CODE = (1<<num_bits)-1, then you know there's only one entry left in the 
				// table. you need to be able to 

				


				//fprintf(stderr, "%d %d %d\n", CURRENT_CODE, C, num_bits);
				//fprintf(stderr, "%d\n", C);



				//fprintf(stderr, "%d:%d\n", num_bits, C);



				if (C == 1){
					finalK = getBits(8);
					//fprintf(stderr, "%d:%d\n", 8, finalK);
					printf("%c", finalK);
					//fprintf(stderr, "character is %d %d\n", finalK, oldC);
				}
				if (C >= CURRENT_CODE){
					my_stack = push(finalK, my_stack);
					C = oldC;
					k_omega_k = TRUE;
					//fprintf(stderr, "k omega k %d %d\n", oldC, CURRENT_CODE);
				}
				prefix = TABLE[C]->prefix_code;
				//fprintf(stderr, "---------------\n");
				while (prefix != 0){
					//fprintf(stderr, "prefix is %d\n", C);
					//fprintf(stderr, "%d\n", C);
					TABLE[C]->num_appearances += 1;
					my_stack = push(TABLE[C]->character, my_stack);
					C = prefix;
					prefix = TABLE[C]->prefix_code;
				}
				
				if (C != 1){
					//fprintf(stderr, "\n%d %d %d\n", newC, C, oldC);
					TABLE[C]->num_appearances += 1;
					finalK = TABLE[C]->character;
					
					printf("%c", finalK);
					//fprintf(stderr, "%c", finalK);
					while (my_stack->character != -1){
						int K = pop(&my_stack);
						//fprintf(stderr, "%c", K);
						printf("%c", K);
					}
					
				}
				if (oldC != 0){
					//printf("here\n");
					if (!FULL || one_more){
						TABLE[CURRENT_CODE] = new_trie(finalK, oldC, TRUE);
						if (k_omega_k){
							TABLE[CURRENT_CODE]->num_appearances += 1;
							k_omega_k = FALSE;
						}
						
						int to_insert;
						int num_children = TABLE[oldC]->num_children;
						int index = binary_search(oldC, finalK, 0, num_children-1, &to_insert);
						insert(index, oldC, CURRENT_CODE);
						//fprintf(stderr, "here: %d\n", CURRENT_CODE);
						if (one_more){
							//table_stderr();
							//do_prune(initialize);
							//table_stderr();
							//num_bits = new_num_bits(CURRENT_CODE);
							one_more = FALSE;
							//num_bits += 1;
							//FULL = TRUE;
							//oldC = 0;
							//continue;
						}
						CURRENT_CODE += 1;
						if (CURRENT_CODE == (1<<num_bits) && !FULL){
							if (num_bits == MAXBITS){
								if (prune){
									//table_stderr();
									//return 0;
									do_prune(initialize);
									num_bits = new_num_bits(CURRENT_CODE);
									oldC = 0;
									newC = getBits(num_bits);
									C = newC;
									continue;
								} else {
									FULL = TRUE;	
								}
								//one_more = TRUE;
								//fprintf(stderr, "hhhhhhhhhhhh%d %d\n", CURRENT_CODE, finalK);
								//printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
							} else {
								one_more = TRUE;
								num_bits += 1;
								new_table(num_bits);
							}
							//fprintf(stderr, "new num bits is: %d\n", num_bits);
						}
						
					}
					

				}
				if (C == 1){
					//fprintf(stderr, "%d %d\n", CURRENT_CODE, finalK);
					oldC = 0;
					//fprintf(stderr, "here we go\n");
					if (!FULL || one_more){
						TABLE[CURRENT_CODE] = new_trie(finalK, oldC, TRUE);
						int to_insert;
						int num_children = TABLE[oldC]->num_children;
						int index = binary_search(oldC, finalK, 0, num_children-1, &to_insert);
						insert(index, oldC, CURRENT_CODE);
						
						if (one_more){
							//table_stderr();
							//do_prune(initialize);
							//table_stderr();
							//num_bits = new_num_bits(CURRENT_CODE);
							one_more = FALSE;
							//num_bits += 1;
							//FULL = FALSE;
							//oldC = 0;
							//continue;
						}
						CURRENT_CODE += 1;
						if (CURRENT_CODE == (1<<num_bits) && !FULL){
							if(num_bits == MAXBITS){
								if (prune){
									table_stderr();
									//return 0;
									do_prune(initialize);
									num_bits = new_num_bits(CURRENT_CODE);
									oldC = 0;
									one_more = FALSE;
									newC = getBits(num_bits);
									C = newC;
									continue;
								} else {
									FULL = TRUE;
								}
								//fprintf(stderr, "ggggggggggggggg");
								//printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
							} else {
								one_more = TRUE;
								num_bits += 1;
								new_table(num_bits);
							}
							//fprintf(stderr, "new num bits is: %d\n", num_bits);
						}

						

					}


				}
				if (C != 1){
					oldC = newC;
				}
				if (one_more && C == 1){
					//fprintf(stderr, "one more is true %d\n", CURRENT_CODE);
					newC = getBits(num_bits-1);
				} else {
					newC = getBits(num_bits);
				}
				C = newC;
			}




			
			//table_stderr();
			table_stderr();
			free_table();
			free_stack(my_stack);
			return 0;
		}
		return 0;
}


struct Stack* initialize_stack(){
    struct Stack* to_return = calloc(1, sizeof(struct Stack));
	to_return->character = -1;
	to_return->last = NULL;
	return to_return;
}

struct Stack* push(int character, struct Stack* current){
	struct Stack* to_return = calloc(1, sizeof(struct Stack));
	to_return->character = character;
	to_return->last = current;
	return to_return;
}

int pop(struct Stack** current){
	struct Stack* entry = *current;
	int to_return = entry->character;
    *current = entry->last;
    free(entry);
	return to_return;
}

void free_stack(struct Stack* my_stack){
	while (my_stack != NULL){
		pop(&my_stack);
	}
}

// int_index is the index in children
// table_index is the index in TABLE
// value is the value to be inserted (code whose prefix is table_index)
void insert(int int_index, int table_index, int value){
	struct Trie* of_interest = TABLE[table_index];
	int num_children = of_interest->num_children;
	int* children = of_interest->children;
	int* new_children = calloc(num_children+1, sizeof(int));
	memcpy(new_children, children, sizeof(int)*int_index);
	memcpy(new_children+int_index, &value, sizeof(int));
	memcpy(new_children+int_index+1, children+int_index, sizeof(int)*(num_children-int_index));
	of_interest->children = new_children;
	free(children);
	of_interest->num_children = num_children+1;
}

// you want a recursive function to print out the prefix

// new_trie creates a new trie with character and prefix_code
struct Trie* new_trie(int character, int prefix_code, int used){
		struct Trie* to_return = calloc(1, sizeof(struct Trie));
		to_return->character = character;
		to_return->prefix_code = prefix_code;
		if (used == TRUE){
			to_return->num_appearances = 1;	
		} else {
			to_return->num_appearances = 0;
		}
		to_return->children = NULL;
		return to_return;
}


void initialize_table(){
		int* children = calloc(256, sizeof(int));
		for (int i = 0; i < 256; i++){
				TABLE[i+3] = new_trie(i, 0, FALSE);
				children[i] = i;
		}
		TABLE[0] = new_trie(-1, -1, FALSE);
		TABLE[0]->children = children;
		TABLE[0]->num_children = 256;
}

// pass a pointer to beginning of the string. make this recursive!
// you get the trie and a pointer to the next character. returns true if inserted, false otherwise
// you need to modify this so that you instead return 
// returns a pointer to subtrie. if it was inserted, 
int add_substring(int where_at, int character, int num_bits, int index){
		struct Trie* entry = TABLE[where_at];
		int num_children = entry->num_children;
		int* children = entry->children;
		entry->num_appearances += 1;
		int to_insert;
		
		if (to_insert){
			// this means that you need to add it
			if (!FULL){
				struct Trie* to_insert = new_trie(character, where_at, TRUE);
				insert(index, where_at, CURRENT_CODE);
				TABLE[CURRENT_CODE] = to_insert;
			}
			
			
			if (where_at == 0){
				//printf("1\n%d\n", character);
				//fprintf(stderr, "%d 1\n%d\n", CURRENT_CODE, character);
				printf("%d:%d\n", num_bits, 1);
				printf("%d:%d\n", 8, character);
				//putBits(num_bits, 1);
				//putBits(8, character);
			} else {
				//printf("%d\n", where_at);
				//fprintf(stderr, "%d %d\n", CURRENT_CODE, where_at);
				printf("%d:%d\n", num_bits, where_at);
				//putBits(num_bits, where_at);
			}

			return -1;
		}
		else{
			return children[index];
		}
}

// this should take a bunch of children. if it finds it, then it returns the
// index where it is. if it does not find it, it returns the index i such that
// children[i] < key, children[i-1] > key
int binary_search(int index, int key, int min_index, int max_index, int* to_insert){
		struct Trie* entry = TABLE[index];
		//entry->num_appearances += 1;
		int* children = entry->children;
		//entry->num_appearances += 1;
		//fprintf(stderr, "index is: %d\n", index);
		//fprintf(stderr, "index is: %d\n", index);
		/*
		if (index != 0){
		for(int i = 0; i < entry->num_children; i++){
			fprintf(stderr, "%d ", TABLE[children[i]]);
		}
	}
	*/
		//fprintf(stderr, "\n");
		if (max_index < min_index){
				*to_insert = TRUE;
				return min_index;
		} else {
				int mid_index = midpoint(min_index, max_index);
				int of_interest = TABLE[children[mid_index]]->character;
				if (of_interest > key){
						return binary_search(index, key, min_index, mid_index-1, to_insert);
				} else if (of_interest < key) {
						return binary_search(index, key, mid_index+1, max_index, to_insert);
				} else {
						*to_insert = FALSE;
						return mid_index;
				}
		}
}

int midpoint(int min_index, int max_index){
		return min_index+(max_index-min_index)/2;
}

void free_table(){
	for(int i = 0; i < CURRENT_CODE; i++){
		struct Trie* of_interest = TABLE[i];
		if (of_interest != NULL){
			free(of_interest->children);
		}
		free(of_interest);
	}
	free(TABLE);
}

void new_table(int new_bits){
	TABLE = realloc(TABLE, (1<<new_bits)*sizeof(struct Trie*));
}

void table_stderr(){
	fprintf(stderr, "i,Pref[i],Char[i],Used[i]\n");
	for (int i = 3; i < CURRENT_CODE; i++){
		struct Trie* of_interest = TABLE[i];
		fprintf(stderr, "%d,%d,%d,%d\n", i, of_interest->prefix_code, of_interest->character, of_interest->num_appearances);
		/*for (int j = 0; j < of_interest->num_children; j++){
			fprintf(stderr, "%d ", (of_interest->children)[j]);
		}*/
		//fprintf(stderr, "\n");
	}
	//fprintf(stderr, "that's it I guess\n");
}

//step 1: malloc new thing; go through table and check if num_appearances is good;
// initialization is a little bit trick. you will deal with it after you've dealt with
// the uninitialized entries


int* new_indices(int* number_remaining, int initialize){
	int* to_return = calloc(CURRENT_CODE, sizeof(int));
	to_return[0] = 0;
	struct Trie* of_interest;
	int counter;
	int where_start;
	if (initialize){
		where_start = 256+3;
		counter = 256+3;
	} else {
		where_start = 3;
		counter = 3;
	}
	for (int i = 3; i < where_start; i++){
		to_return[i] = i;
	}
	for (; where_start < CURRENT_CODE; where_start++){
		of_interest = TABLE[where_start];
		if (of_interest->num_appearances <= 1){
			free_single_trie(where_start);
			to_return[where_start] = -1;
		} else {
			to_return[where_start] = counter;
			counter += 1;
		}
	}
	*number_remaining = counter;
	//fprintf(stderr, "%d\n", counter);
	return to_return;
}

void free_single_trie(int index){
	struct Trie* of_interest = TABLE[index];
	if (of_interest != NULL){
		free(of_interest->children);
	}
	free(of_interest);
}

// step 2: go through again and make the new entries

void pruned_table(int* where_look, int how_many){
	struct Trie** to_return = calloc(1<<new_num_bits(how_many), sizeof(struct Trie*));
	to_return[0] = create_new_entry(where_look, 0);
	to_return[1] = TABLE[1];
	//fprintf(stderr, "aaaaaaaaa\n");
	for (int i = 3; i < CURRENT_CODE; i++){
		if (where_look[i] == -1){
			continue;
		}
		to_return[where_look[i]] = create_new_entry(where_look, i);
		//fprintf(stderr, "new index is %d\n", where_look[i]);
		//fprintf(stderr, "%d %d\n", where_look[i], so_far);
	}
	CURRENT_CODE = how_many;
	free(TABLE);
	TABLE = to_return;
	//fprintf(stderr, "found %d instances\n", so_far);
}

// you know it's there still
struct Trie* create_new_entry(int* where_look, int index){
	
	struct Trie* of_interest = TABLE[index];
	struct Trie* to_return = calloc(1, sizeof(struct Trie));
	//fprintf(stderr, "index is %d\nprefix is: %d\n", index, of_interest->prefix_code);
	if (index != 0){
		to_return->num_appearances = (of_interest->num_appearances)/2;
		to_return->prefix_code = where_look[of_interest->prefix_code];
		to_return->character = of_interest->character;
	} else {
		to_return->num_appearances = 0;
		to_return->prefix_code = 0;
		to_return->character = -1;
	}
	int num_children = of_interest->num_children;
	int* new_children = calloc(num_children, sizeof(int));
	int* children = of_interest->children;
	int num_new_children = 0;
	int potential_new;
	//fprintf(stderr, "current index is %d\n", index);
	for (int j = 0; j < num_children; j++){
		//fprintf(stderr, "%d ", children[j]);
		potential_new = where_look[children[j]];
		if (potential_new != -1){
			//fprintf(stderr, "yes: %d", potential_new);
			new_children[num_new_children] = potential_new;
			num_new_children += 1;
		}
		//fprintf(stderr, "\n");
	}
	//fprintf(stderr, "-----------------\n");
	//fprintf(stderr, "new array is\n");
	//fprintf(stderr, "new array 2 is\n");
	//new_children = realloc(new_children, sizeof(int)*num_new_children);

	int* replacement_children = calloc(num_new_children, sizeof(int));
	memcpy(replacement_children, new_children, num_new_children*sizeof(int));
	free(children);
	free(new_children);
	free(of_interest);
	//new_children = realloc(new_children, num_new_children*sizeof(int));
	to_return->children = replacement_children;
	to_return->num_children = num_new_children;
	return to_return;
}

void do_prune(int initialize){
	int num_remaining;
	int* lookup_table = new_indices(&num_remaining, initialize);
	pruned_table(lookup_table, num_remaining);
	free(lookup_table);
	//fprintf(stderr, "size of new table is: %d\nCURRENT CODE is: %d\n", num_remaining, CURRENT_CODE);
}

int new_num_bits(int how_many){
	int holder = how_many;
	int to_return = 0;
	while (holder != 0){
		holder /= 2;
		to_return += 1;
	}
	return to_return;
}
