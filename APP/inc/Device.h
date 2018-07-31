#ifndef __DEVICE_H
#define __DEVICE_H

typedef struct{
	short uid; 
	const char* name;
	void* pvalue;
	_Bool update_event;
	short (*Device_update)(void *this);
	/* ...add more func  */	
} Device_node;
Device_node* device(void *self,short uid,const char *name,void *pvalue,short (*Device_update)(void *this));
#endif /* __DEVICE_H */
