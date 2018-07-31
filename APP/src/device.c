/**
 *	@作者：杨思源
 *	@日期：2017年12月22日22:54:41
 *	@功能：提供一个统一的外设接口
 *	@版本：1.0
 *	@修改：no
 *	@接口：Device_node 									外设的接口类
 *	@接口：Device_node* device()							外设外设对象初始化函数
 *		@参数说明：	void *self							指向外设本身的对象句柄
 *		@参数说明：	short uid							外设唯一身份ID，不允许重复，保留设计
 *		@参数说明：	const char *name					外设名字，允许同类外设重名，在外设类成员方法 update 中作为对象非空校验依据
 *		@参数说明：	void *pvalue						外设值指针，初始化后不再允许作为左操作数
 *		@参数说明：	short (*Device_update)(void *this)	外设更新函数指针
 *			@参数说明：short return；					统一设计的返回值， 	ERROR == 0 == 参数错误 || 更新失败
 *																		  	SUCCESS == 1 == 更新成功
 *																			其他值 == 保留设计 原则上应为二进制标志位
 *			@参数说明：void *this						外设对象句柄指针
 *			
 *	@注意事项：	update_event 是作为是否产生更新事件的依据
 *				pvalue	作为外设更新实际有效值的指针，
 *						由于不同的外设有不同的返回值类型 
 *						常见的返回值类型有 数组、数值、结构体
 *						因此设计上不允许直接解引用
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

