#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define TRUE 0
#define FALSE 1


// trie needs to have a root
struct Trie{
		int character;
		int num_children;
		struct Trie** children;
};

struct Trie* new_trie(int character);
char* add_substring(struct Trie** my_trie_pointer, char* starting_point);
int binary_search(struct Trie** children, int num_children, int key, int min_index, int max_index);
int midpoint(int min_index, int max_index);
void print_array(struct Trie** array, int length);
void print_trie(struct Trie *my_trie, char* so_far);

int main (int argc, char** argv){
		//first you want to test the functionality of the trie
		char* test = "ababababababa";
		struct Trie* root = new_trie(-2);
		char *next = add_substring(&root, test);
		while (*next != '\0'){
				printf("--end of block--\n");
				next = add_substring(&root, next);
				print_trie(root, "");
		}
		// testing binary search
		struct Trie* first = new_trie(0);
		struct Trie* second = new_trie(2);
		struct Trie* third = new_trie(4);
		struct Trie* my_root = new_trie(-1);
		struct Trie* array[3] = {first, second, third};
		my_root->num_children = 3;
		my_root->children = array;
		//printf("%d\n", binary_search(array, 3, -1, 0, 2));
		// seems to be working
		//
		// testing 
		return 0;
}

struct Trie* new_trie(int character){
		struct Trie* to_return = calloc(1, sizeof(struct Trie));
		to_return->character = character;
		to_return->num_children = 0;
		to_return->children = NULL;
		return to_return;
}


// pass a pointer to beginning of the string. make this recursive!
// you get the trie and a pointer to the next character. returns true if inserted, false otherwise
//
char* add_substring(struct Trie** my_trie_pointer, char* starting_point){
		//TODO account for reaching the end of the file
		struct Trie* my_trie = *my_trie_pointer;
		struct Trie** children = my_trie->children;
		int num_children = my_trie->num_children;
		if (num_children == 0){
				struct Trie** new_children = calloc(1, sizeof(struct Trie*));
				new_children[0] = new_trie((int) *starting_point);
				my_trie->children = new_children;
				my_trie->num_children = 1;
				return starting_point+1;
		}
		int index = binary_search(children, num_children, (int) *starting_point, 0, num_children-1);
		
		// this means that it's not in the array, so you need to add it
		if (index >= num_children){
				struct Trie* to_insert = new_trie((int) *starting_point);
				index -= num_children;
				struct Trie** new_children = calloc(num_children+1, sizeof(struct Trie*));
				memcpy(new_children, children, sizeof(struct Trie*)*(index));
				memcpy(new_children+index, &to_insert, sizeof(struct Trie*));
				memcpy(new_children+index+1, children+index, sizeof(struct Trie*)*(num_children-index));
				free(children);
				my_trie->children = new_children;
				my_trie->num_children = num_children+1;
				return starting_point+1;
		}
		// else we found it, we need to go to the next level
		else{
			return add_substring(&(children[index]), starting_point+1);
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
