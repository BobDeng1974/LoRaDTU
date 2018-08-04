#ifndef  ROUTINGTABLE_H
#define  ROUTINGTABLE_H

#include "LinkedList.h"
#include <stdint.h>


#ifdef ROOTING_MODE

typedef struct _routMessage {
    uint16_t DestinationAddress;    //目的地址
    uint16_t nextJmpAddress;        //下一跳地址
    uint8_t  hops;                  //跳数
}routMessage;


typedef struct _Rout {
    routMessage defaultRoot;
    void (*add)(uint16_t DestinationAddress,uint8_t hops,uint16_t nextJmpAddress);
    void (*deleteRoutMessage)(uint16_t DestinationAddress);
    uint16_t (*select)(uint16_t DestinationAddress);
}Rout;


extern LinkedList* routingTable;
extern routMessage defaultRout;
extern Rout routing;

void RoutingTableInit(void);
void add(uint16_t DestinationAddress,uint8_t hops,uint16_t nextJmpAddress);
void deleteRoutMessage(uint16_t DestinationAddress);
uint16_t select(uint16_t DestinationAddress);
#endif /* ROOTING_MODE */
#endif /* ROUTINGTABLE_H */

