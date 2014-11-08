#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "/c/cs323/Hwk2/code.h"


#define TRUE 1
#define FALSE 0

int MAXBITS = 12;
int CURRENT_CODE = 0;


struct Trie{
		int character;
		int num_children;
		int* children;
		int num_appearances;
		int prefix_code;
};


void initialize_trie(struct Trie*** array_pointer);
struct Trie* new_trie(int character, int prefix_code);

int main(int argc, char** argv){
	// first we'll assume that we're initializing it

}



// you need code to recursively insert new codes into the table
int add_substring(int where_at, int character){
	struct Trie* entry = TABLE[where_at];
	int num_children = entry->num_children;
	if (num_children == 0){
		int* new_children = calloc(1, sizeof(int));
		new_children[0] = CURRENT_CODE;
		entry->children = new_children;
		entry->num_children = 1;
		struct Trie* to_insert = new_trie(character, where_at);
		TABLE[CURRENT_CODE] = to_insert;
		return -1;
	}
	int returned = binary_search(where_at, character, 0, num_children);
	if (returned >= num_children){
		returned -= num_children;
		int* new_children = calloc(num_children+1, sizeof(int));
		struct Trie* to_insert = new_trie(character, where_at);
		memcpy(new_children, children, )
	}


}

// binary search has an
// index: where in TABLE to look
// key: the next character
// 

int binary_search(int index, int key, int min_index, int max_index){
	struct Trie* entry = TABLE[index];
	int* array = entry->children;
	int num_children = entry->num_children;
	if (max_index < min_index){
		return min_index+num_children;
	} else {
		int mid_index = midpoint(min_index, max_index);
		int of_interest = TABLE[array[mid_index]]->character;
		if (of_interest > key){
			return binary_search(index, key, min_index, mid_index-1);
		} else if (of_interest < key){
			return binary_search(index, key, mid_index+1, max_index);
		} else{
			return mid_index;
		}
	}
}


void initialize_trie(struct Trie*** array_pointer){
	struct Trie** array = *array_pointer;
	int* children = calloc(256, sizeof(int));
	for (int i = 0; i < 256; i++){
		array[i+3] = new_trie(i, 0);
		children[i] = i;
	}
	struct Trie* first_entry = array[0];
	first_entry->children = children;
	first_entry->num_children = num_children;
}

struct Trie* new_trie(int character, int prefix_code){
	struct Trie* to_return = calloc(1, sizeof(struct Trie));
	to_return->character = 0;
	to_return->prefix_code = prefix_code;
	to_return->num_children = 0;
	to_return->children = NULL;
	to_return->num_appearances = 0;
	return to_return;
}