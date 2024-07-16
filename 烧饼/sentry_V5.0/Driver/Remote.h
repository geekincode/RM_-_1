#ifndef __REMOTE_H
#define __REMOTE_H

/*  Ö±½Ó¶ÁÈ¡rc_data  */

#define GetByteBit(x,n)  (FlagStatus)(x>>n)

/* ----------------------- RC Channel Definition---------------------------- */
#define RC_CH_VALUE_MIN ((uint16_t)364 )
#define RC_CH_VALUE_OFFSET ((uint16_t)0)
#define RC_CH_VALUE_MAX ((uint16_t)1684)
/* ----------------------- RC Switch Definition----------------------------- */
#define RC_SW_UP ((uint16_t)1)
#define RC_SW_MID ((uint16_t)3)
#define RC_SW_DOWN ((uint16_t)2)
/* ----------------------- PC Key Definition-------------------------------- */
#define KEY_PRESSED_OFFSET_W ((uint16_t)0x01<<0)
#define KEY_PRESSED_OFFSET_S ((uint16_t)0x01<<1)
#define KEY_PRESSED_OFFSET_A ((uint16_t)0x01<<2)
#define KEY_PRESSED_OFFSET_D ((uint16_t)0x01<<3)
#define KEY_PRESSED_OFFSET_Q ((uint16_t)0x01<<4)
#define KEY_PRESSED_OFFSET_E ((uint16_t)0x01<<5)
#define KEY_PRESSED_OFFSET_SHIFT ((uint16_t)0x01<<6)
#define KEY_PRESSED_OFFSET_CTRL ((uint16_t)0x01<<7)
#define RC_FRAME_LENGTH 18u
#define RC_DeltaRCValue 660.0

void Remote_StructInit();
typedef struct
{
 struct
 {
	 int16_t ch0;
	 int16_t ch1;
	 int16_t ch2;
	 int16_t ch3;
	 uint8_t s1;
	 uint8_t s2;
	 int16_t pulley_wheel;
 }rc;
 struct
 {
	 int16_t x;
	 int16_t y;
	 int16_t z;
	 uint8_t press_l;
	 uint8_t press_r;
 }mouse;
 struct
 {
	 uint16_t Key_W;
	 FlagStatus Key_S;
	 FlagStatus Key_A;
	 FlagStatus Key_D;
	 FlagStatus Key_Shift;
	 FlagStatus Key_Ctrl;
	 FlagStatus Key_Q;
	 FlagStatus Key_E;
	 FlagStatus Key_R;
	 FlagStatus Key_F;
	 FlagStatus Key_G;
	 FlagStatus Key_Z;
	 FlagStatus Key_X;
	 FlagStatus Key_C;
	 FlagStatus Key_V;
	 FlagStatus Key_B;
	 uint16_t Key_v;
 }key;
}RC_Data_Struct;

void Remote_StructInit();
void Remote_Init();
void Remote_RCRatio_Cal();

#endif

