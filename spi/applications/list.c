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
#include<rtthread.h>
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
adc_List * create_adc_List(){

    adc_List * list =(adc_List* )rt_malloc(sizeof(adc_List));
    if(list==NULL)
        rt_kprintf("create list fail!");
    list->head=NULL;
    list->tail=NULL;
    list->size=0;
    return list;
}
Node* createNode(Acc_Data * data){

    Node* node =(Node*)rt_malloc(sizeof(Node));
    if(node==NULL)
        rt_kprintf("create node fail!");
    node->acce[0]=data->data[0];
    node->acce[1]=data->data[1];
    node->acce[2]=data->data[2];
    node->next=NULL;
    return node;
}

void Add_data(List * list,Acc_Data * data)
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
adc_Node* create_adc_Node(rt_uint32_t data)
{
    adc_Node* node =(adc_Node*)rt_malloc(sizeof(adc_Node));
    if(node==NULL)
        rt_kprintf("create node fail!");
    node->data=data;
    return node;
}
void Add_adc_data(adc_List * list,rt_uint32_t data)
{
    adc_Node * node =create_adc_Node(data);
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
void show_adc_data(adc_List* list)
{
    adc_Node * node_s=list->head;
    rt_kprintf("adc val start :\n");
    while(node_s!=NULL)
    {
        rt_kprintf("%d\n",node_s->data);
        node_s=node_s->next;
    }
}
void Free_adc_List(adc_List* list)
{
    rt_kprintf("free memory start:\n");
    adc_Node * node_s=list->head->next;
    while(list->size!=0)
    {
        rt_free(list->head);
        list->head=node_s;
        node_s=list->head->next;
        list->size--;
    }
}
