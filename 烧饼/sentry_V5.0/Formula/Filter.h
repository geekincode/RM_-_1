#ifndef __FILTER_H
#define __FILTER_H

//一阶低通滤波结构体
typedef struct 
{
	float Last_num;	//上次采样数据
	
	float KL;		//滤波参数
	
	float OutPut;	//滤波输出
	
}LowPass_Struct;


//一阶低通滤波公式
float Low_Pass_Filter(LowPass_Struct *filter,float num);
//一阶低通滤波初始化
void Low_Pass_Filter_Init(void);
//一阶低通滤波清零
void Low_Pass_Filter_Clear(LowPass_Struct *filter);

#endif
