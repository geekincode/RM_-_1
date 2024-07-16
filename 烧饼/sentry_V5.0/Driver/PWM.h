#ifndef __PWM_H
#define __PWM_H

//初始化PWM
void PWM_Init(void);
//snail行程校准
void Snail_Init(void);
//snail发射
void Snail_Shoot(uint16_t Speed);

#endif
