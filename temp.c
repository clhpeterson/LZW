while (returned != EOF){
		num_children = TABLE[where_at]->num_children;
		index = binary_search(where_at, returned, 0, num_children-1, &to_insert);

		if (where_at != 0){
				TABLE[where_at]->num_appearances += 1;
		}

		
		if (to_insert){
				if (where_at == 0){
						fprintf(stderr, "%d:%d\n", num_bits, 1);
						fprintf(stderr, "%d:%d\n", 8, returned);

						putBits(num_bits, 1);
						putBits(8, returned);
				} else {
						fprintf(stderr, "%d:%d\n", num_bits, where_at);
						putBits(num_bits, where_at);
				}
		}


		if (!FULL){
				if (CURRENT_CODE == (1<<num_bits)){
						if (num_bits == MAXBITS){
								if (prune





												_)
						}
				}
		}
}







while (newC != 2){

		if (newC == 1){
				finalK = getBits(8);
				fprintf(stderr, "8:%d\n", finalk);
				printf("%c", finalK);
		} else if (newC != -1){
				if (C >= CURRENT_CODE){
						my_stack = push(finalK, my_stack);
						C = oldC;
						k_omega_k = TRUE;
				}

				prefix = TABLE[C]->prefix_code;
				
				while (prefix != 0){

						TABLE[C]->num_appearances += 1;
						my_stack = push(TABLE[C]->character, my_stack);
						C = prefix;
						prefix = TABLE[C]->prefix_code;
				}

				TABLE[C]->num_appearances += 1;
				finalK = TABLE[C]->character;
				printf("%c\n", finalK);

				while (my_stack->character != -1){
						int K = pop(&my_stack);
						printf("%c", K);
				}
		}

		if (oldC != 0 || newC == -1){
				if (!FULL){
						TABLE[CURRENT_CODE] = new_trie(finalK, oldC, TRUE);
						if (k_omega_k){
								TABLE[CURRENT_CODE]->num_appearances += 1;
								k_omega_k = FALSE;
						}

						int to_insert;
						int num_children = TABLE[oldC]->num_children;
						int index = binary_search(oldC, finalK, 0, num_children-1, &to_insert);
						insert(index, oldC, CURRENT_CODE);
						CURRENT_CODE += 1;

						if (CURRENT_CODE == 1<<(num_bits)){
								if (num_bits == MAXBITS){
										if (prune){
										} else {
												FULL = TRUE;
										}
								} else {
										num_bits += 1;
										new_table(num_bits);
								}
						}
				}
		}

		if (newC == 1){
				oldC = 0;
				newC = -1;
				continue;
		} else if (newC == -1){
				oldC = 0;
		} else {
				oldC = newC;
		}

		newC = getBits(num_bits);
		fprintf(stderr, "%d:%d\n", num_bits, newC);
		C = newC;
}










































































































































while (returned != EOF){
		// search
		//
		
		num_children = TABLE[where_at]->num_children;

		index = binary_search(where_at, returned, 0, num_children-1, &to_insert);

		if (to_insert){
				if (where_at == 0){
						putBits(num_bits, 1);
						putBits(8, returned);
				} else {
						putBits(num_bits, where_at);
				}
		}

		if (!FULL){
				if (CURRENT_CODE == (1<<num_bits)){
						if (num_bits == MAXBITS){
								if (prune){
										do_prune(initialize);
										num_bits = new_num_bits(CURRENT_CODE);
										where_at = 0;
										continue;
								} else {
										FULL = TRUE;
								}
						} else {
								num_bits += 1;
								new_table(num_bits);
						}
				}
		}

		if (where_at != 0){
				TABLE[where_at]->num_appearances += 1;
		}

		if (to_insert){
				if (!FULL){
						struct Trie* new_entry = new_trie(returned, where_at, TRUE);
						insert(index, where_at, CURRENT_CODE);
						TABLE[CURRENT_CODE] = new_entry;
						CURRENT_CODE += 1;
				}

				if (where_at != 0){
						where_at = 0;
						continue;
				}
		} else {
				where_at = (TABLE[where_at]->children)[index];
		}

		returned = getBits(8);
}



