#ifndef __USART_H
#define __USART_H

//��ʼ������1,6
void Serial_Init(void);

//�����ֽ�
void Serial_6_SendByte(uint8_t Byte);
//��������
void Serial_6_SendArray(uint8_t *Array, uint16_t Length);

//�����ֽ�
void Serial_1_SendByte(uint8_t Byte);
//��������
void Serial_1_SendArray(uint8_t *Array, uint16_t Length);
//��װsprintfʵ�ֶമ��ʹ��printf
void Serial_1_Printf(char *format, ...);

#endif
