/**
 *	@���ߣ���˼Դ
 *	@���ڣ�2017��12��22��22:54:41
 *	@���ܣ��ṩһ��ͳһ������ӿ�
 *	@�汾��1.0
 *	@�޸ģ�no
 *	@�ӿڣ�Device_node 									����Ľӿ���
 *	@�ӿڣ�Device_node* device()							������������ʼ������
 *		@����˵����	void *self							ָ�����豾��Ķ�����
 *		@����˵����	short uid							����Ψһ���ID���������ظ����������
 *		@����˵����	const char *name					�������֣�����ͬ���������������������Ա���� update ����Ϊ����ǿ�У������
 *		@����˵����	void *pvalue						����ֵָ�룬��ʼ������������Ϊ�������
 *		@����˵����	short (*Device_update)(void *this)	������º���ָ��
 *			@����˵����short return��					ͳһ��Ƶķ���ֵ�� 	ERROR == 0 == �������� || ����ʧ��
 *																		  	SUCCESS == 1 == ���³ɹ�
 *																			����ֵ == ������� ԭ����ӦΪ�����Ʊ�־λ
 *			@����˵����void *this						���������ָ��
 *			
 *	@ע�����	update_event ����Ϊ�Ƿ���������¼�������
 *				pvalue	��Ϊ�������ʵ����Чֵ��ָ�룬
 *						���ڲ�ͬ�������в�ͬ�ķ���ֵ���� 
 *						�����ķ���ֵ������ ���顢��ֵ���ṹ��
 *						�������ϲ�����ֱ�ӽ�����
 */

#include "Device.h"
#include "common.h"
//typedef struct{
//	short uid; 
//	char* name;
//	void* pvalue;
//	_Bool update_event;
//	short (*Device_update)(void *this,void *pvalue);
//	/* ...add more func  */	
//} Device_node;



Device_node* device(void *self,short uid,const char *name,void *pvalue,short (*Device_update)(void *this)){
	if(self == null)return null;
	if(name == null ||*name == '\0')return null;
	if(pvalue == null)return null;
	if(Device_update == null)return null;
	Device_node * this = self;
	this->Device_update = Device_update;
	this->name = name;
	this->pvalue = pvalue;
	this->uid = uid;
	this->update_event = false;
	return this;
}

