#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "/c/cs323/Hwk2/code.h"

#define TRUE 0
#define FALSE 1

int MAXBITS = 12;
int current_code = 0;

// trie needs to have a root
struct Trie{
		int character;
		int num_children;
		struct Trie** children;
		int num_appearances;
		int code;
};


struct Trie* new_trie(int character, int code);
struct Trie* add_substring(struct Trie* my_trie, int next_character);
int binary_search(struct Trie** children, int num_children, int key, int min_index, int max_index);
int midpoint(int min_index, int max_index);
void print_array(struct Trie** array, int length);
void print_trie(struct Trie *my_trie, char* so_far);
struct Trie* initialize_trie();
void free_trie(struct Trie* my_trie);

int main (int argc, char** argv){
		//first you want to test the functionality of the trie
		struct Trie* root = initialize_trie();
		int returned = getBits(8);
		struct Trie* where_at = root;
		current_code = 256+3;
		while(returned != EOF){
				where_at = add_substring(where_at, returned);
				if (where_at == NULL){
						where_at = root;
						current_code += 1;
continue;
				}
				
				returned = getBits(8);
		}
		where_at = add_substring(where_at, returned);
		
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

struct Trie* new_trie(int character, int code){
		struct Trie* to_return = calloc(1, sizeof(struct Trie));
		to_return->character = character;
		to_return->num_children = 0;
		to_return->code = code;
		to_return->children = NULL;
		return to_return;
}

struct Trie* initialize_trie(){
		struct Trie* to_return = calloc(1, sizeof(struct Trie));
		to_return->character = -1;
		struct Trie** children = calloc(256, sizeof(struct Trie*));
		for (int i = 0; i < 256; i++){
				children[i] = new_trie(i, i+3);
		}
		to_return->num_children = 256;
		to_return->children = children;
		return to_return;
}


// pass a pointer to beginning of the string. make this recursive!
// you get the trie and a pointer to the next character. returns true if inserted, false otherwise
// you need to modify this so that you instead return 
// returns a pointer to subtrie. if it was inserted, 
struct Trie* add_substring(struct Trie* my_trie, int next_character){
		struct Trie** children = my_trie->children;
		int num_children = my_trie->num_children;
		if (num_children == 0){
				struct Trie** new_children = calloc(1, sizeof(struct Trie*));
				new_children[0] = new_trie(next_character, current_code);
				my_trie->children = new_children;
				my_trie->num_children = 1;
				printf("%d\n", my_trie->code);
				return NULL;
		}
		int index = binary_search(children, num_children, next_character, 0, num_children-1);
		
		// this means that it's not in the array, so you need to add it
		if (index >= num_children){
				struct Trie* to_insert = new_trie(next_character, current_code);
				index -= num_children;
				struct Trie** new_children = calloc(num_children+1, sizeof(struct Trie*));
				memcpy(new_children, children, sizeof(struct Trie*)*(index));
				memcpy(new_children+index, &to_insert, sizeof(struct Trie*));
				memcpy(new_children+index+1, children+index, sizeof(struct Trie*)*(num_children-index));
				free(children);
				my_trie->children = new_children;
				my_trie->num_children = num_children+1;
				printf("%d\n", my_trie->code);
				return NULL;
		}
		// else we found it, we need to go to the next level
		else{
			return children[index];
		}
}

// this should take a bunch of children. if it finds it, then it returns the
// index where it is. if it does not find it, it returns the index i such that
// children[i] < key, children[i-1] > key
int binary_search(struct Trie** children, int num_children, int key, int min_index, int max_index){
	if (max_index < min_index){
			// this means that everything was checked but it was not found.
			// these two are going to differ by 1. 
			return min_index+num_children;
	}
	else{
			int mid_index = midpoint(min_index, max_index);
			int character_of_interest = children[mid_index]->character;
			if (character_of_interest > key){
					return binary_search(children, num_children, key, min_index, mid_index-1);
			}
			else if (character_of_interest < key){
					return binary_search(children, num_children, key, mid_index+1, max_index);
			}
			else {
					return mid_index;
			}
	}
}

int midpoint(int min_index, int max_index){
		return min_index+(max_index-min_index)/2;
}

void print_array(struct Trie** array, int length){
		for (int i = 0; i < length; i++){
				printf("%c ", array[i]->character);

		}
		printf("\n");

}


// now you want code to print every word in the trie recursively
//

void print_trie(struct Trie *my_trie, char* so_far){
		struct Trie** children = my_trie->children;
		int num_children = my_trie->num_children;
		printf("%s\n", so_far);
		if (num_children == 0){
				free(so_far);
				return;
		}
		int length = strlen(so_far);
				for(int i = 0; i < num_children; i++){
						char* new_string = malloc((length+1)*sizeof(char));
						memcpy(new_string, so_far, sizeof(char)*length);
						memcpy(new_string+length, &(children[i]->character), sizeof(char));
						print_trie(children[i], new_string);
				}
		return;
}

void free_trie(struct Trie* my_trie){
		// you need to free both and Trie and the array
		for (int i = 0; i < my_trie->num_children; i++){
				free_trie((my_trie->children)[i]);
		}
		if (my_trie->children != NULL){
				free(my_trie->children);
		}
		free(my_trie);	
		return;
}
