#ifndef __USART_H
#define __USART_H

//初始化串口1,6
void Serial_Init(void);

//发送字节
void Serial_6_SendByte(uint8_t Byte);
//发送数组
void Serial_6_SendArray(uint8_t *Array, uint16_t Length);

//发送字节
void Serial_1_SendByte(uint8_t Byte);
//发送数组
void Serial_1_SendArray(uint8_t *Array, uint16_t Length);
//封装sprintf实现多串口使用printf
void Serial_1_Printf(char *format, ...);

#endif
