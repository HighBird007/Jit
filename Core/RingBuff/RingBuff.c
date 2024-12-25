#include "RingBuff.h"


//环形缓冲区结构体实例
RingBuffStruct ringBuff;

//初始化ringbuff
void ringBuffInit(void){
	ringBuff.size=RingBuffSize;
	ringBuff.readPoint=0;
	ringBuff.writePoint=0;
}
//返回是否为空
uint8_t ringBuffIsEmpty(void){
	if(ringBuff.readPoint == ringBuff.writePoint){
	return 1;
	}
	return 0;
}
//返回是否满
uint8_t ringBuffIsFull(void){
	if(ringBuff.readPoint == (ringBuff.writePoint+1)%RingBuffSize){
	quickSend("ringbuff is full\n");
	return 1;
	}
	return 0;
}
//不空不满为正常状态
uint8_t ringBuffIsNormal(void){
	if(ringBuffIsFull()==0&&ringBuffIsEmpty()==0)return 1;
	return 0;
}

//计算出环形缓冲区剩余的容量
uint16_t ringBuffGetSpace(void){
	uint16_t space = 0;;
	//写指针>=读指针位置  此时大小为 size - (write - read ) - 1 
	if(ringBuff.writePoint >= ringBuff.readPoint){
		
	space =  RingBuffSize-(ringBuff.writePoint-ringBuff.readPoint+1);
		
	}else 
	//写指针<读指针位置  此时大小为 read - write - 1
	if(ringBuff.writePoint < ringBuff.readPoint){
		
	space =  ringBuff.readPoint - ringBuff.writePoint -1;	
		
	}
	return space ;
}

//写入数据
void ringBuffWrite(uint8_t* data,int length){
	//剩余空间是否可以容纳目前的数据
	if(length > ringBuffGetSpace()){
 	quickSend("error : ringBuff cant hava more data\n");
		return ;
	}
	
	int tailPart = ringBuff.size - ringBuff.writePoint;

	//如果环形缓冲区尾部容量不够存放
	if(length > tailPart){
		//首先将数据存放到环形缓冲区尾部
	memcpy(&ringBuff.buff[ringBuff.writePoint],data,tailPart);
		//将数据放开头
	memcpy(ringBuff.buff,data+tailPart,length-tailPart);
	}else 
	//有一种情况 如果写指针在读指针前面  此时不用管 因为算出容量是足够的  只需要直接copy
	if(length <= tailPart){
		
	memcpy(&ringBuff.buff[ringBuff.writePoint],data,length);
		
	}
	//调整写指针位置
	ringBuff.writePoint = (ringBuff.writePoint + length) % ringBuff.size;
	//quickSend("ringbuff : write success !!!!!!!!!!\n");
}

//读取一个字节 返回的true == 1  false == 0  检验数据是否读取成功
uint8_t ringBuffReadByte(uint8_t* data){
	//为空直接退出
	if(ringBuffIsEmpty() == true )return false;
	
	//读取一个字节 并且移动 对应的读指针
	*data =  ringBuff.buff[ringBuff.readPoint];
	
	ringBuff.readPoint = (ringBuff.readPoint + 1)%ringBuff.size;
	
	return true;
}
//获取当前数据量
uint16_t ringBuffGetDataCount(void){
	
	return RingBuffSize - ringBuffGetSpace();
	
}

void ringBuffOffset(uint16_t num){
	uint8_t t;
	for(int i = 0 ; i < num ;i++){
		
		ringBuffReadByte(&t);
		
	}
}

