/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-27     admin       the first version
 */
/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-10-17     admin       the first version
 */
#include <sensor.h>
#include "list.h"
List * createList(){

    List * list =(List* )rt_malloc(sizeof(List));
    if(list==NULL)
        rt_kprintf("create list fail!");
    list->head=NULL;
    list->tail=NULL;
    list->size=0;
    return list;
}

Node* createNode(struct rt_sensor_data *data){

    Node* node =(Node*)rt_malloc(sizeof(Node));
    if(node==NULL)
        rt_kprintf("create node fail!");
    node->acce[0]=data->data.acce.x;
    node->acce[1]=data->data.acce.y;
    node->acce[2]=data->data.acce.z;
    node->next=NULL;
    return node;
}

void Add_data(List * list,struct rt_sensor_data *data)
{
    Node * node =createNode(data);
    if(node==NULL)
        return;
    if(list->size==0)
    {
        list->head=node;

    }
    else
    {
        list->tail->next=node;

    }
    list->size++;
    list->tail=node;
}

void show_data(List* list)
{
    Node * node_s=list->head;
    rt_kprintf("unloaded data:\n");
    while(node_s!=NULL)
    {
        rt_kprintf("%d,%d,%d\n",node_s->acce[0],node_s->acce[1],node_s->acce[2]);
        node_s=node_s->next;
    }
}
void FreeList(List* list)
{
    rt_kprintf("free memory start:\n");
    Node * node_s=list->head->next;
    while(list->size!=0)
    {
        rt_free(list->head);
        list->head=node_s;
        node_s=list->head->next;
        list->size--;
    }
}
