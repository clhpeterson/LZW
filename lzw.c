#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "/c/cs323/Hwk2/code.h"

#define TRUE 1
#define FALSE 0

int MAXBITS = 12;
int CURRENT_CODE = 0;

// the table in going to be a global variable

// trie needs to have a root
struct Trie{
		int character;
		int* children;
		int num_children;
		int prefix_code;
		int num_appearances;
};

struct Trie** TABLE;

struct Trie* new_trie(int character, int prefix_code);
void initialize_table();
int add_substring(int where_at, int character);
int binary_search(int index, int key, int min_index, int max_index);
int midpoint(int min_index, int max_index);

int main (int argc, char** argv){
		//first you want to test the functionality of the trie
		// let's do the command line processing
		if (argc == 1){
				fprintf(stderr, "too few arguments\n");
				return 0;
		}
		int encode;
		int delay;
		int prune = FALSE;
		int initialize = TRUE;
		if (strcmp(argv[1],  "decode") == 0){
				if (argc > 2){
						fprintf(stderr, "you shouldn't have arguments after decode\n");
						return 0;
				}
				encode = FALSE;
		}

		else{
				if (strcmp(argv[1], "encode") != 0){
						fprintf(stderr, "no encode or decode paramter!\n");
						return 0;
				}
				encode = TRUE;
				int i = 2;
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
			TABLE = calloc(512, sizeof(struct Trie*));

			
			CURRENT_CODE = 256+3;
			initialize_table();
			

			/*
			TABLE[0] = new_trie(-1, -1);
			CURRENT_CODE = 3;
			*/
			int where_at = 0;
			int caught = 0;
			int returned = getBits(8);

			while(returned != EOF){
				// add_substring returns NULL if it inserts it, or else returns the entry where the next character was found
				// with the -e flag, you need to check if add_substring returns null 
				caught = add_substring(where_at, returned);
				if (caught == -1){
					CURRENT_CODE += 1;
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
				where_at = add_substring(where_at, returned);
			}
			printf("2\n");
			return 0;
		}


		else{
				return 0;
		}

		
		// let's try this decode thing

		

		//print_trie(root, "");

		
		// testing binary search
		/*
		struct Trie* first = new_trie(0);
		struct Trie* second = new_trie(2);
		struct Trie* third = new_trie(4);
		struct Trie* my_root = new_trie(-1);
		struct Trie* array[3] = {first, second, third};
		my_root->num_children = 3;
		my_root->children = array;
		*/
		//printf("%d\n", binary_search(array, 3, -1, 0, 2));
		// seems to be working
		//
		// testing 
		return 0;
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
int add_substring(int where_at, int character){
		struct Trie* entry = TABLE[where_at];
		int num_children = entry->num_children;
		int* children = entry->children;
		if (num_children == 0){
				// this means that the entry has no children, so you've reached the end of the prefix and you need to create
				// a new code
				int* new_children = calloc(1, sizeof(int));
				new_children[0] = CURRENT_CODE;
				entry->children = new_children;
				entry->num_children = 1;
				struct Trie* new_entry = new_trie(character, where_at);
				TABLE[CURRENT_CODE] = new_entry;
				if (where_at == 0){
					// then you need to do 1 and ascii value
					printf("1\n%d\n", character);
				} else {
					printf("%d\n", where_at);
				}
				return -1;
		}
		int index = binary_search(where_at, character, 0, num_children-1);
		if (index >= num_children){
			// this means that you need to add it
			
			struct Trie* to_insert = new_trie(character, where_at);
			index -= num_children;
			int* new_children = calloc(num_children+1, sizeof(int));
			memcpy(new_children, children, sizeof(int)*index);
			memcpy(new_children+index, &CURRENT_CODE, sizeof(int));
			memcpy(new_children+index+1, children+index, sizeof(int)*(num_children-index));
			entry->children = new_children;
			free(children);
			entry->num_children = num_children+1;
			if (where_at == 0){
				printf("1\n%d\n", character);
			} else {
				printf("%d\n", where_at);
			}
			TABLE[CURRENT_CODE] = to_insert;
			return -1;
		}
		else{
			return children[index];
		}
}

// this should take a bunch of children. if it finds it, then it returns the
// index where it is. if it does not find it, it returns the index i such that
// children[i] < key, children[i-1] > key
int binary_search(int index, int key, int min_index, int max_index){
		struct Trie* entry = TABLE[index];
		int num_children = entry->num_children;
		int* children = entry->children;
		if (max_index < min_index){
				return min_index+num_children;
		} else {
				int mid_index = midpoint(min_index, max_index);
				int of_interest = TABLE[children[mid_index]]->character;
				if (of_interest > key){
						return binary_search(index, key, min_index, mid_index-1);
				} else if (of_interest < key) {
						return binary_search(index, key, mid_index+1, max_index);
				} else {
						return mid_index;
				}
		}
}

int midpoint(int min_index, int max_index){
		return min_index+(max_index-min_index)/2;
}
