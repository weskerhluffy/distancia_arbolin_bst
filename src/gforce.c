#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define MAX_NODES 1048576 /** 2^20 */
#define INVALID_VALUE -1
 #define log_debug(formato, args...) 0
/*
#define log_debug printf
 */

static inline char *array_to_string(int *arreglo, int tam_arreglo, char *buffer) {
	int i;
	char *ap_buffer = NULL;
	int characteres_escritos = 0;

	memset(buffer, 0, 100);
	ap_buffer = buffer;

	for (i = 0; i < tam_arreglo; i++) {
		characteres_escritos += sprintf(ap_buffer + characteres_escritos, "%d",
				*(arreglo + i));
		if (i < tam_arreglo - 1) {
			*(ap_buffer + characteres_escritos++) = ',';
		}
	}
	return ap_buffer;
}

static inline void bst_create_recursive(int *bst_data, int node_index,
		int number, int *max_index_used) {
	int node_value = 0;
	int child_offset = 0;

	node_value = *(bst_data + node_index);
	if (node_value == INVALID_VALUE) {
		if (node_index > *max_index_used) {
			*max_index_used = node_index;
		}
		*(bst_data + node_index) = number;
	} else {
		if (number < node_value) {
			child_offset = 1;
		} else {
			child_offset = 2;
		}
		bst_create_recursive(bst_data, 2 * node_index + child_offset, number,
				max_index_used);
	}
}

static inline void bst_create(int *numbers, int num_numbers, int *bst_data,
		int *max_index_used) {
	for (int i = 0; i < num_numbers; i++) {
		bst_create_recursive(bst_data, 0, *(numbers + i), max_index_used);
	}
}

static void inline bst_path(int *bst_data, int *nodes_in_path, int node_index,
		int *num_nodes_in_path, int number) {
	int node_number = 0;
	int child_offset = 0;

	node_number = *(bst_data + node_index);

	if (node_number == INVALID_VALUE) {
		return;
	}

	*(nodes_in_path + (*num_nodes_in_path)++) = node_index;
	if (node_number != number) {

		if (number < node_number) {
			child_offset = 1;
		} else {
			child_offset = 2;
		}

		bst_path(bst_data, nodes_in_path, 2 * node_index + child_offset,
				num_nodes_in_path, number);
	}
}

static inline void bst_main() {
	int first_node = 0;
	int sec_node = 0;
	int num_nodes = 0;
	int num_nodes_power_of_2 = 0;
	int tree_height = 0;
	int max_index_used = 0;
	char buf[100] = { '\0' };
	int *numbers = NULL;
	int *bst_data = NULL;
	int path_num_1[21] = { 0 };
	int path_num_2[21] = { 0 };
	int num_nodes_in_path_1 = 0;
	int num_nodes_in_path_2 = 0;
	int num_nodes_min = 0;
	int num_common_nodes = 0;
	int distance = 0;

	scanf("%d", &first_node);
	scanf("%d", &sec_node);
	scanf("%d", &num_nodes);

	log_debug("values %d, %d, %d\n", first_node, sec_node, num_nodes);

	numbers = calloc(MAX_NODES, sizeof(int));
	assert(numbers);

	bst_data = calloc(MAX_NODES, sizeof(int));
	assert(bst_data);

	for (int i = 0; i < num_nodes; i++) {
		scanf("%d", numbers + i);
	}

	log_debug("the array is %s\n", array_to_string(numbers, num_nodes, buf));

	for (int i = 0; i < MAX_NODES; i++) {
		*(bst_data + i) = INVALID_VALUE;
	}

	bst_create(numbers, num_nodes, bst_data, &max_index_used);

	log_debug("bst is now %s\n", array_to_string(bst_data, num_nodes + 1, buf));
	log_debug("max index used %d\n", max_index_used);

	while ((num_nodes >> tree_height++))
		;

	tree_height--;

	log_debug("tree h %d\n", tree_height);

	num_nodes_power_of_2 = (1 << (tree_height - 1));

	log_debug("rounded to power of 2 number of numbers %d\n",
			num_nodes_power_of_2);

	bst_path(bst_data, path_num_1, 0, &num_nodes_in_path_1, first_node);
	bst_path(bst_data, path_num_2, 0, &num_nodes_in_path_2, sec_node);

	log_debug("bst path 1 %s\n",
			array_to_string(path_num_1, num_nodes_in_path_1, buf));
	log_debug("bst path 2 %s\n",
			array_to_string(path_num_2, num_nodes_in_path_2, buf));

	if (bst_data[*(path_num_1 + num_nodes_in_path_1 - 1)] != first_node
			|| bst_data[*(path_num_2 + num_nodes_in_path_2 - 1)] != sec_node) {
		printf("Not found\n");
		return;
	}

	if (num_nodes_in_path_1 < num_nodes_in_path_2) {
		num_nodes_min = num_nodes_in_path_1;
	} else {
		num_nodes_min = num_nodes_in_path_2;
	}

	for (num_common_nodes = 0; num_common_nodes < num_nodes_min;
			num_common_nodes++) {
		if (path_num_1[num_common_nodes] != path_num_2[num_common_nodes]) {
			break;
		}
	}

	distance = num_nodes_in_path_1 + num_nodes_in_path_2 - 2 * num_common_nodes;

	log_debug("distance %d\n", distance);

	printf("%d\n",distance);

	free(numbers);
}

int main() {
	bst_main();
	return 0;
}
