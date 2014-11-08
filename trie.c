#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0



struct Trie* new_trie(int character, int prefix_code){
		struct Trie* to_return = calloc(sizeof(struct Trie));
		to_return->character = character;
		to_return->prefix_code = prefix_code;
		to_return->num_appearances = 0;
		to_return->children = NULL;
		return to_return;
}

void initialize(struct Trie*** array_pointer){
		struct Trie** array = *array_pointer;
		int* children = calloc(256, sizeof(int));
		for (int i = 0; i < 256; i++){
				array[i+3] = new_trie(i, 0);
				children[i] = i;
		}
		array_pointer[0] = children;
		array_pointer->num_children = 256;
}

int binary_search(struct Trie*** array_pointer, int index, int key, int min_index, int max_index){
		struct Trie** array = *array_pointer;
		struct Trie* entry = array[index];
		int num_children = entry->num_children;
		int* children = entry->children;
		if (max_index < min_index){
				return min_index+max_children;
		} else {
				int mid_index = midpoint(min index, max_index);
				int of_interest = array[children[mid_index]]->character;
				if (of_interest > key){
						return binary_search(array_pointer, index, key, min_index, mid_index-1);
				} else if{
						return binary_search(array_pointer, index, key, mid_index+1, max_index);
				} else {
						return mid_index;
				}
		}
}

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
		int index = binary_search(where_at, character, 0, num_children);
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
		}
		else{
			return children[index];
		}
}
