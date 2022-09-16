#include "mylist.h"
List* list_init()
{
	List* header=(List *)malloc(sizeof(List));
	header->next = NULL;
	header->piror = NULL;
	return header;
}

int list_add(List* list, void* val)
{
	List* new_node = (List*)malloc(sizeof(List));
	new_node->value = val;
	if (list->next == NULL) {
		new_node->next = list->next;
		new_node->piror = list;
		list->next = new_node;
	}
	else{
		new_node->next = list->next;
		list->next->piror = new_node;
		new_node->piror = list;
		list->next = new_node;	
	}
	return 1;
}

int list_delete(List* list)
{
	List* find = list->next;
	if (find->next == NULL) {
		free(find);
		return 1;
	}
	if (list->next == NULL && list->piror == NULL)
		return 0;
	find->piror->next = find->next;
	find->next->piror = find->piror;
	free(find);
	return 1;
}

int list_length(List* list) {
	if (list->next == NULL)
		return 0;
	List* find=list->next;
	int i = 1;
	while (find->next != NULL) {
		i++;
		find = find->next;
	}
	return i;
}

int list_destroy(List* list)
{
	int k = list_length(list);
	for (int i = 0; i < k; i++) {
		list_delete(list);
	}
	list->next = NULL;
	return 1;
}
