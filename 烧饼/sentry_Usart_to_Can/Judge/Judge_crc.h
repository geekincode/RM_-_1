#ifndef _JUDGE_CEC_H_
#define _JUDGE_CEC_H_

unsigned char get_crc8_check_sum(unsigned char *pchMessage,unsigned int dwLength,unsigned char ucCRC8); 
unsigned int verify_crc8_check_sum(unsigned char *pchMessage, unsigned int dwLength);
void append_crc8_check_sum(unsigned char *pchMessage, unsigned int dwLength);

uint16_t get_crc16_check_sum(uint8_t *pchMessage,uint32_t dwLength,uint16_t wCRC);
uint32_t verify_crc16_check_sum(uint8_t *pchMessage, uint32_t dwLength);
void append_crc16_check_sum(uint8_t * pchMessage,uint32_t dwLength); 


#endif