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

typedef struct List{

    Node* head;
    Node* tail;
    int size;
}List;
List * createList(void);
Node* createNode(struct rt_sensor_data *data);
void Add_data(List * list,struct rt_sensor_data *data);
void show_data(List* list);
void FreeList(List* list);
#endif /* APPLICATIONS_LIST_H_ */
