#include "RoutingTable.h"
#include "LoRa.h"
#include "stdlib.h"
#ifdef ROOTING_MODE


LinkedList* routingTable;
routMessage defaultRout;
Rout routing;
static routMessage* search(LinkedList* list,routMessage* rout);

/**
 *  初始化一个路由表
 *  并设置路由表的最后一条数据为默认下一跳地址
 *
 *
 */ 
void RoutingTableInit(void){
    routingTable = newLinkedList("routing");
    defaultRout.DestinationAddress = (defaultHost.Address_H << 8)+defaultHost.Address_L;
    defaultRout.hops = 0;
    defaultRout.nextJmpAddress = defaultRout.DestinationAddress;
    routing.add = add;
    routing.deleteRoutMessage = deleteRoutMessage;
    routing.select = select;
    routing.defaultRoot = defaultRout;
}


/**
 *  添加一条路由信息到路由表
 *  如果路由表里有这一条目就更新
 *  如果没有就申请一段内存
 *  
 */ 
void add(uint16_t DestinationAddress,uint8_t hops,uint16_t nextJmpAddress){
    static routMessage root;
    root.DestinationAddress = DestinationAddress;
    routMessage* temp = search(routingTable,&root);
    if( temp == NULL ){
        temp = (routMessage*)malloc(sizeof(routMessage)); 
        temp->DestinationAddress = DestinationAddress;
    }
    temp->hops = hops;
    temp->nextJmpAddress = nextJmpAddress;
    
}

uint16_t select(uint16_t DestinationAddress){
    Node* node ;
    routMessage* temp;
    node = routingTable->first->next;
    while(node->next != NULL){
        temp = node->nodeData;
        if(temp->DestinationAddress == DestinationAddress){
            return temp->nextJmpAddress;
        }
        node = node->next;
    }
    
    return defaultRout.nextJmpAddress;
}



/**
 *  删除一栏信息
 *  @param DestinationAddress 目的地址为 DestinationAddress 的信息将被删除
 *
 *
 */ 
void deleteRoutMessage(uint16_t DestinationAddress){
    static Node* node ;
    static routMessage* temp;
    node = routingTable->first->next;
    while(node->next != NULL){
        temp = node->nodeData;
        if(temp->DestinationAddress == DestinationAddress){
            routingTable->deleteByNode(routingTable,node);
            return;
        }
        node = node->next;
    }
}



/**
 *  7.查找指定值的节点
 *  @param list 要操作的链表
 *  @param nodeData 要比较的数据指针
 *  @return 数据指针
 */ 
static routMessage* search(LinkedList* list,routMessage* rout){
    Node* node = list->first->next;
    routMessage* temp;
    while(node->next != NULL){
        
        temp = node->nodeData;
        if(temp->DestinationAddress == rout->DestinationAddress){
            return temp;
        }
        node = node->next;
    }
    return NULL;
    
}



#endif /* ROOTING_MODE */
























