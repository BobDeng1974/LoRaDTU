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
 *  �ڶ���ģʽ�£���������ǰ��Ҫ���ͽ��շ���ַ��ͨ����3�ֽ�
 *  �������ֽ�����������·��ͷ��������Ҫ����ת���ַ�
 *  @Deprecated
 *
 */ 
void LoRaAddressConfig(uint16_t address,uint8_t channel){
    Usart_SendByte( LoRaUSART, (0xff00&address)>>8);
    Usart_SendByte( LoRaUSART, (0xff&address));
    Usart_SendByte( LoRaUSART, channel);

}



/**
 *  ���� LoRa ���㷢��ģʽ�µĽ��շ���ַ �����Ƿ������ֽڵ�ַͨ������
 *  @param address_H ��ַ�߰�λ
 *  @param address_L ��ַ�Ͱ�λ
 *  @param channel    ͨ���� 0-31
 */ 
static void _LoRaAddressConfig(uint8_t address_H,uint8_t address_L,uint8_t channel){
    Usart_SendByte( LoRaUSART, address_H);
    Usart_SendByte( LoRaUSART, address_L);
    Usart_SendByte( LoRaUSART, channel);
    

}


/**
 *  �� LoRa ����һ���ֽ�����
 *  @param p �ֽ�����ָ��
 *  @param length �ֽ�����ĳ���
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
 *  ��������
 *  ��һ�� DataPacket* ����Դ��ַ��Դ�ŵ���Ŀ�ĵ�ַ��Ŀ���ŵ�
 *  �涨�ϲ㴫������������ǰ��λҪд��
 *
 */ 
_Bool LoRaSendData(DataPacket* packet){
    if (packet == NULL){
        return 0;
    }
    packet->crc = calculateCRC(packet);
    //��������֮ǰ�ȷ�����һ����ַ���ŵ�
    #ifndef ROUTING_MODE        //û�ж��� ROOTING_MODE ֤�����ն��豸
    if(packet->destination.Address_H == localhost.Address_H && packet->destination.Address_L == localhost.Address_L){
        //����Ǳ���Ⱥ�ڣ�ֱ��ת��
        _LoRaAddressConfig(packet->destination.Address_H,packet->destination.Address_L,packet->destination.Channel);
    }else{
        //���Ǳ���Ⱥ�ڲ��ģ����͸�·��
        _LoRaAddressConfig(localhost.Address_H,localhost.Address_L,0x00);
    }
    #else
    //·��ģʽ��ÿ��ת����ҪѰ����һ����ַ
    if(packet->destination.Address_H == localhost.Address_H && packet->destination.Address_L == localhost.Address_L){
        //����Ǳ���Ⱥ�ھ�ֱ�ӷ���
        _LoRaAddressConfig(packet->destination.Address_H,packet->destination.Address_L,packet->destination.Channel);
    }else{
        static uint16_t nextjmpaddress;
        nextjmpaddress = routing.select((packet->destination.Address_H << 8) + packet->destination.Address_L);
        _LoRaAddressConfig((nextjmpaddress&0xff00)>>8,(nextjmpaddress&0xff),0x00);
    }
    
    #endif /* ROUTING_MODE */
    
    
    
    
    
    //����֡��ʼ��
    Usart_SendByte( LoRaUSART, 0x01);
    LoRaSendByte(packet->count);
    //����Դ��ַ��Դ�ŵ���Ŀ�ĵ�ַ��Ŀ���ŵ�
    LoRaSendByte(packet->source.Address_H);
    LoRaSendByte(packet->source.Address_L);
    LoRaSendByte(packet->source.Channel);
    LoRaSendByte(packet->destination.Address_H);
    LoRaSendByte(packet->destination.Address_L);
    LoRaSendByte(packet->destination.Channel);
    
    //��������Ҫ������Ŀ������������
    LoRaSendBytes(packet->dataBytes.length,packet->dataBytes.data);
    
    //����CRC��
    LoRaSendByte(packet->crc);
    //����֡������
    Usart_SendByte( LoRaUSART, 0x04);
    return 1;
}




/**
 *  ֱ�ӷ�������
 *  ��һ�� DataPacket* ����Դ��ַ��Դ�ŵ���Ŀ�ĵ�ַ��Ŀ���ŵ�
 *  �涨�ϲ㴫������������ǰ��λҪд��
 *
 */ 
_Bool _LoRaSendData(DataPacket* packet){
    if (packet == NULL || packet->dataBytes.data ==NULL || packet->dataBytes.length ==0){
        return 0;
    }
    packet->crc = calculateCRC(packet);
    _LoRaAddressConfig(packet->destination.Address_H,packet->destination.Address_L,packet->destination.Channel);
    
    //����֡��ʼ��
    Usart_SendByte( LoRaUSART, 0x01);
    LoRaSendByte(0x10);
    //����Դ��ַ��Դ�ŵ���Ŀ�ĵ�ַ��Ŀ���ŵ�
    LoRaSendByte(packet->source.Address_H);
    LoRaSendByte(packet->source.Address_L);
    LoRaSendByte(packet->source.Channel);
    LoRaSendByte(packet->destination.Address_H);
    LoRaSendByte(packet->destination.Address_L);
    LoRaSendByte(packet->destination.Channel);
    
    //��������Ҫ������Ŀ������������
    LoRaSendBytes(packet->dataBytes.length,packet->dataBytes.data);
    
    //����CRC��
    LoRaSendByte(packet->crc);
    //����֡������
    Usart_SendByte( LoRaUSART, 0x04);
    
    destroyPacket(packet);
    
    return 1;
}




/**
 *  �� LoRa ����һ���ֽ�
 *  @param data һ�ֽ�����
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
   





   
        








