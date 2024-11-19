#ifndef RingBuff_h
#define RingBuff_h
#include "usart.h"
#include "dma.h"
#define RingBuffSize 500
//当前环形缓冲区的状态
typedef enum 
{
    Normal,
    Empty,		    
    Full,			
    HalfFull,		
}RingBufferState;

typedef struct{
	volatile uint16_t readPoint;
	volatile uint16_t writePoint;
	uint8_t buff[RingBuffSize];
	uint16_t size;
}RingBuffStruct;

extern RingBuffStruct ringBuff;
//初始化
void ringBuffInit(void);

//当前状态
uint8_t ringBuffIsEmpty(void);

uint8_t ringBuffIsFull(void);

uint8_t ringBuffIsNormal(void);

//获取当前还有多少容量
uint16_t ringBuffGetSpace(void);

//写入数据
void ringBuffWrite(uint8_t* data,int length);

//读取一个字节数据
uint8_t ringBuffReadByte(uint8_t* data);

//获取当前数据量
uint16_t ringBuffGetDataCount(void);

void ringBuffOffset(uint16_t num);
#endif
