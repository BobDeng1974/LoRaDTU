#include "LoRa.h"
#include "String.h"
#include "stdlib.h"
#include "LoRa_usart.h"
#include "RoutingTable.h"

 void LoRaSendByte(uint8_t data);
 void LoRaSendBytes(uint8_t length,uint8_t* p);
 _Bool CRC_Check(uint8_t* p,uint8_t count);
uint8_t CRC8_Table(uint8_t* p,uint8_t count);
LoRaAddress localhost;
LoRaAddress defaultHost;


void LoRaInit(void){
    
    localhost.Address_H = 0x02;
    localhost.Address_L = 0x02;
    localhost.Channel = 0x03;
    defaultHost.Address_H = 0x03;
    defaultHost.Address_L = 0x03;
    defaultHost.Channel = 0x00;

    
    
    
    
    #ifdef ROUTING_MODE
    RoutingTableInit();
    if(localhost.Address_H ==0x02 && localhost.Address_L == 0x02){
        routing.add(771,1,771);
    }else if(localhost.Address_H ==0x03 && localhost.Address_L == 0x03){
        routing.add(514,1,514);
    }
    
    #endif /* ROUTING_MODE */
}





/**
 *  在定向模式下，发送数据前需要发送接收方地址、通道共3字节
 *  这三个字节属于数据链路层头部，不需要插入转义字符
 *  @Deprecated
 *
 */ 
void LoRaAddressConfig(uint16_t address,uint8_t channel){
    Usart_SendByte( LoRaUSART, (0xff00&address)>>8);
    Usart_SendByte( LoRaUSART, (0xff&address));
    Usart_SendByte( LoRaUSART, channel);

}



/**
 *  配置 LoRa 定点发送模式下的接收方地址 本质是发送三字节地址通道数据
 *  @param address_H 地址高八位
 *  @param address_L 地址低八位
 *  @param channel    通道号 0-31
 */ 
static void _LoRaAddressConfig(uint8_t address_H,uint8_t address_L,uint8_t channel){
    Usart_SendByte( LoRaUSART, address_H);
    Usart_SendByte( LoRaUSART, address_L);
    Usart_SendByte( LoRaUSART, channel);
    

}


/**
 *  向 LoRa 发送一个字节数组
 *  @param p 字节数组指针
 *  @param length 字节数组的长度
 *
 */ 
 void LoRaSendBytes(uint8_t length,uint8_t* p){
    for( ; length > 0 ; length-- ){
        switch(*p){
        case 0x01:
        case 0x04:
        case 0x1B:
            Usart_SendByte( LoRaUSART, 0x1B);

        default:
            Usart_SendByte( LoRaUSART, *p);

            break;
        }
        p++;
    }
}




/**
 *  发送数据
 *  传一个 DataPacket* 过来源地址、源信道、目的地址、目的信道
 *  规定上层传下来的数据中前六位要写好
 *
 */ 
_Bool LoRaSendData(DataPacket* packet){
    if (packet == NULL){
        return 0;
    }
    packet->crc = calculateCRC(packet);
    //发送数据之前先发送下一跳地址、信道
    #ifndef ROUTING_MODE        //没有定义 ROOTING_MODE 证明是终端设备
    if(packet->destination.Address_H == localhost.Address_H && packet->destination.Address_L == localhost.Address_L){
        //如果是本集群内，直接转发
        _LoRaAddressConfig(packet->destination.Address_H,packet->destination.Address_L,packet->destination.Channel);
    }else{
        //不是本集群内部的，发送给路由
        _LoRaAddressConfig(localhost.Address_H,localhost.Address_L,0x00);
    }
    #else
    //路由模式下每次转发都要寻找下一跳地址
    if(packet->destination.Address_H == localhost.Address_H && packet->destination.Address_L == localhost.Address_L){
        //如果是本集群内就直接发送
        _LoRaAddressConfig(packet->destination.Address_H,packet->destination.Address_L,packet->destination.Channel);
    }else{
        static uint16_t nextjmpaddress;
        nextjmpaddress = routing.select((packet->destination.Address_H << 8) + packet->destination.Address_L);
        _LoRaAddressConfig((nextjmpaddress&0xff00)>>8,(nextjmpaddress&0xff),0x00);
    }
    
    #endif /* ROUTING_MODE */
    
    
    
    
    
    //发送帧起始符
    Usart_SendByte( LoRaUSART, 0x01);
    LoRaSendByte(packet->count);
    //发送源地址、源信道、目的地址、目的信道
    LoRaSendByte(packet->source.Address_H);
    LoRaSendByte(packet->source.Address_L);
    LoRaSendByte(packet->source.Channel);
    LoRaSendByte(packet->destination.Address_H);
    LoRaSendByte(packet->destination.Address_L);
    LoRaSendByte(packet->destination.Channel);
    
    //发送真正要交付到目的主机的数据
    LoRaSendBytes(packet->dataBytes.length,packet->dataBytes.data);
    
    //发送CRC码
    LoRaSendByte(packet->crc);
    //发送帧结束符
    Usart_SendByte( LoRaUSART, 0x04);
    return 1;
}




/**
 *  直接发送数据
 *  传一个 DataPacket* 过来源地址、源信道、目的地址、目的信道
 *  规定上层传下来的数据中前六位要写好
 *
 */ 
_Bool _LoRaSendData(DataPacket* packet){
    if (packet == NULL || packet->dataBytes.data ==NULL || packet->dataBytes.length ==0){
        return 0;
    }
    packet->crc = calculateCRC(packet);
    _LoRaAddressConfig(packet->destination.Address_H,packet->destination.Address_L,packet->destination.Channel);
    
    //发送帧起始符
    Usart_SendByte( LoRaUSART, 0x01);
    LoRaSendByte(0x10);
    //发送源地址、源信道、目的地址、目的信道
    LoRaSendByte(packet->source.Address_H);
    LoRaSendByte(packet->source.Address_L);
    LoRaSendByte(packet->source.Channel);
    LoRaSendByte(packet->destination.Address_H);
    LoRaSendByte(packet->destination.Address_L);
    LoRaSendByte(packet->destination.Channel);
    
    //发送真正要交付到目的主机的数据
    LoRaSendBytes(packet->dataBytes.length,packet->dataBytes.data);
    
    //发送CRC码
    LoRaSendByte(packet->crc);
    //发送帧结束符
    Usart_SendByte( LoRaUSART, 0x04);
    
    destroyPacket(packet);
    
    return 1;
}




/**
 *  向 LoRa 发送一个字节
 *  @param data 一字节数据
 *  
 *
 */ 
 void LoRaSendByte(uint8_t data){
    switch(data){
        case 0x01:
        case 0x04:
        case 0x1B:
            Usart_SendByte( LoRaUSART, 0x1B);

        
        default:
            Usart_SendByte( LoRaUSART, data);

            break;
    }
}
   





   
        








