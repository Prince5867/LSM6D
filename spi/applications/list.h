/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-17     admin       the first version
 */
#ifndef APPLICATIONS_LIST_H_
#define APPLICATIONS_LIST_H_
typedef struct Node{
    int acce[3];
    struct Node * next;
}Node;
typedef struct Data{
    int data[3];
}Acc_Data;
typedef struct adc_Node{
    rt_uint32_t  data;
    struct adc_Node * next;
}adc_Node;

typedef struct List{

    Node* head;
    Node* tail;
    int size;
}List;
typedef struct adc_List{

    adc_Node* head;
    adc_Node* tail;
    int size;
}adc_List;
adc_List * create_adc_List();
adc_Node* create_adc_Node(rt_uint32_t data);
void Add_adc_data(adc_List * list,rt_uint32_t data);
void show_adc_data(adc_List* list);
void Free_adc_List(adc_List* list);
List * createList(void);
Node* createNode(Acc_Data * data);
void Add_data(List * list,Acc_Data * data);
void show_data(List* list);
void FreeList(List* list);
#endif /* APPLICATIONS_LIST_H_ */
