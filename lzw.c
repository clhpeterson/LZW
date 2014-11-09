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

struct Trie* new_trie(int character, int prefix_code);
void initialize_table();
int add_substring(int where_at, int character, int num_bits);
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
				TABLE[0] = new_trie(-1, -1);
				CURRENT_CODE = 3;
			}

			putBits(5, MAXBITS);
			putBits(1, initialize);
			putBits(1, prune);

			int where_at = 0;
			int caught = 0;
			int returned = getBits(8);

			




			while(returned != EOF){
				if (!FULL){
					if (CURRENT_CODE >= 1<<num_bits){
						if(num_bits == MAXBITS){
							FULL = TRUE;
							//fprintf(stderr, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
						} else {
							num_bits += 1;
							new_table(num_bits);
						}
						
					}
				}
				
				//fprintf(stderr, "%d\n", CURRENT_CODE);
				// add_substring returns NULL if it inserts it, or else returns the entry where the next character was found
				// with the -e flag, you need to check if add_substring returns null 
				caught = add_substring(where_at, returned, num_bits);
				// caught being -1 means that something could have been inserted
				// and the code was emitted. if where_at is 0, then it was a new code. else it wasn't
				if (caught == -1){
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
				returned = getBits(8);
			}
			if (where_at != 0){
				where_at = add_substring(where_at, returned, num_bits);
				if (where_at == -1){
					if (!FULL){
						CURRENT_CODE += 1;
					}
				}
			}
			if (CURRENT_CODE >= 1<< num_bits && !FULL){
				num_bits += 1;
			}
			//fprintf(stderr, "%d 2 %d\n", CURRENT_CODE, num_bits);
			putBits(num_bits, 2);
			flushBits();
			//putBits(1, 1);
			//putBits(num_bits, 2);
			//printf("2\n");
			//printf("%d\n", CURRENT_CODE);
			//int last_char = get_prefix(267);
			//printf("\n");
			//printf("%c\n", last_char);
			free_table();
			return 0;
		}


		else{
			MAXBITS = getBits(5);
			initialize = getBits(1);
			prune = getBits(1);
			int num_bits;
			//printf("MAXBITS: %d\n-e: %d\n-p: %d\n", MAXBITS, initialize, prune);
			if (initialize){
				num_bits = 9;
				TABLE = calloc(1<<num_bits, sizeof(struct Trie*));
				initialize_table();
				CURRENT_CODE = 256+3;
			} else {
				num_bits = 2;
				TABLE = calloc(1<<num_bits, sizeof(struct Trie*));
				TABLE[0] = new_trie(-1, -1);
				CURRENT_CODE = 3;
				TABLE[1] = new_trie(-1, 0);
			}
			int newC = getBits(num_bits);
			int oldC = 0;
			int C = newC;
			int prefix;
			int finalK;

			struct Stack* my_stack = initialize_stack();


			int one_more = FALSE;
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
				fprintf(stderr, "%d\n", C);
				if (C == 1){
					finalK = getBits(8);
					printf("%c", finalK);
					if (FULL == TRUE){
						//printf("aaaaaaaaaaaaaaaa\n");
					}
					//fprintf(stderr, "character is %d %d\n", finalK, oldC);
				}
				if (C >= CURRENT_CODE){
					my_stack = push(finalK, my_stack);
					C = oldC;
				}
				prefix = TABLE[C]->prefix_code;
				//fprintf(stderr, "---------------\n");
				while (prefix != 0){
					//fprintf(stderr, "prefix is %d\n", C);
					//fprintf(stderr, "%d\n", C);
					my_stack = push(TABLE[C]->character, my_stack);
					C = prefix;
					prefix = TABLE[C]->prefix_code;
				}
				if (C != 1){
					//fprintf(stderr, "\n%d %d %d\n", newC, C, oldC);
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
					if (!FULL || one_more){
						TABLE[CURRENT_CODE] = new_trie(finalK, oldC);
						int to_insert;
						int num_children = TABLE[oldC]->num_children;
						int index = binary_search(oldC, finalK, 0, num_children-1, &to_insert);
						insert(index, oldC, CURRENT_CODE);
						//fprintf(stderr, "here: %d\n", CURRENT_CODE);
						CURRENT_CODE += 1;
						one_more = FALSE;
					}
				}
				if (CURRENT_CODE == (1<<num_bits)-1){
					if (num_bits == MAXBITS){
						FULL = TRUE;
						one_more = TRUE;
						//printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
					} else {
						num_bits += 1;
						new_table(num_bits);
					}
					//fprintf(stderr, "new num bits is: %d\n", num_bits);
				}
				if (C == 1 || one_more){
					//fprintf(stderr, "%d %d\n", CURRENT_CODE, finalK);
					oldC = 0;
					if (!FULL){
						TABLE[CURRENT_CODE] = new_trie(finalK, oldC);
						int to_insert;
						int num_children = TABLE[oldC]->num_children;
						int index = binary_search(oldC, finalK, 0, num_children-1, &to_insert);
						insert(index, oldC, CURRENT_CODE);
						CURRENT_CODE += 1;
						one_more = FALSE;
					}
				}
				if (C != 1){
					oldC = newC;
				}
				if (CURRENT_CODE == (1<<num_bits)){
					if(num_bits == MAXBITS){
						FULL = TRUE;
						one_more = TRUE;
						//printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
					} else {
						num_bits += 1;
						new_table(num_bits);
					}
					//fprintf(stderr, "new num bits is: %d\n", num_bits);
				}
				newC = getBits(num_bits);
				C = newC;
			}




			
			//table_stderr();
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
struct Trie* new_trie(int character, int prefix_code){
		struct Trie* to_return = calloc(1, sizeof(struct Trie));
		to_return->character = character;
		to_return->prefix_code = prefix_code;
		to_return->num_appearances = 0;
		to_return->children = NULL;
		return to_return;
}


void initialize_table(){
		int* children = calloc(256, sizeof(int));
		for (int i = 0; i < 256; i++){
				TABLE[i+3] = new_trie(i, 0);
				children[i] = i;
		}
		TABLE[0] = new_trie(-1, -1);
		TABLE[0]->children = children;
		TABLE[0]->num_children = 256;
}

// pass a pointer to beginning of the string. make this recursive!
// you get the trie and a pointer to the next character. returns true if inserted, false otherwise
// you need to modify this so that you instead return 
// returns a pointer to subtrie. if it was inserted, 
int add_substring(int where_at, int character, int num_bits){
		struct Trie* entry = TABLE[where_at];
		int num_children = entry->num_children;
		int* children = entry->children;
		int to_insert;
		int index = binary_search(where_at, character, 0, num_children-1, &to_insert);
		if (to_insert){
			// this means that you need to add it
			if (!FULL){
				struct Trie* to_insert = new_trie(character, where_at);
				insert(index, where_at, CURRENT_CODE);
				TABLE[CURRENT_CODE] = to_insert;
			}
			
			
			if (where_at == 0){
				//printf("1\n%d\n", character);
				fprintf(stderr, "%d 1\n%d\n", CURRENT_CODE, character);
				putBits(num_bits, 1);
				putBits(8, character);
			} else {
				//printf("%d\n", where_at);
				fprintf(stderr, "%d %d\n", CURRENT_CODE, where_at);
				putBits(num_bits, where_at);
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
		int* children = entry->children;
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
	for (int i = 3; i < CURRENT_CODE; i++){
		struct Trie* of_interest = TABLE[i];
		fprintf(stderr, "%d, %d, %d\n", i, of_interest->prefix_code, of_interest->character);
	}
}