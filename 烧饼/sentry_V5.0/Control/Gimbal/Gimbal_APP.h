#ifndef __GIMBAL_APP_H
#define __GIMBAL_APP_H


//云台Yaw轴动作
void Gimbal_Yaw_Action(float Target);
//云台Pitch轴动作
void Gimbal_Pitch_Action(float Target);
//拨弹速度环动作
void Shoot_Speed_Action(float Target1,float Target2);

#endif
