//////////////////////////////////////////////////////////////////////////////
//
//  自定义链表
//  my list
//
//  2022/9/5
//


#pragma once

#include<stdlib.h>

typedef struct m_list{
	void* value;
	struct m_list* next;
	struct m_list* piror;
}List;

List* list_init();
int list_add(List* list, void* value);
int list_delete(List* list);
int list_length(List* list);
int list_destroy(List* list);



