#include "RoutingTable.h"
#include "LoRa.h"
#include "stdlib.h"
#ifdef ROOTING_MODE


LinkedList* routingTable;
routMessage defaultRout;
Rout routing;
static routMessage* search(LinkedList* list,routMessage* rout);

/**
 *  ��ʼ��һ��·�ɱ�
 *  ������·�ɱ�����һ������ΪĬ����һ����ַ
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
 *  ���һ��·����Ϣ��·�ɱ�
 *  ���·�ɱ�������һ��Ŀ�͸���
 *  ���û�о�����һ���ڴ�
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
 *  ɾ��һ����Ϣ
 *  @param DestinationAddress Ŀ�ĵ�ַΪ DestinationAddress ����Ϣ����ɾ��
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
 *  7.����ָ��ֵ�Ľڵ�
 *  @param list Ҫ����������
 *  @param nodeData Ҫ�Ƚϵ�����ָ��
 *  @return ����ָ��
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
























