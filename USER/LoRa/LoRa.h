#ifndef __LORA_H__
#define __LORA_H__


#define LoRaUSART USART2


 
#include "packet.h"
extern LoRaAddress localhost;
extern LoRaAddress defaultHost;

 void LoRaSendByte(uint8_t data);
 void LoRaSendBytes(uint8_t length,uint8_t* p);

_Bool LoRaSendData(DataPacket* packet);


void LoRaInit(void);
uint8_t calculateCRC(DataPacket* packet);
#endif /* __LORA_H__ */

