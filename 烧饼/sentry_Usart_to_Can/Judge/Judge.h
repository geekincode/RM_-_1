#ifndef __JUDGE_H
#define __JUDGE_H


/* -------------------- 裁判系统解析结构体 -------------------- */

typedef enum
{ 
	game_status_ID 									=0x0001,   // 比赛状态
	game_result_ID 									=0x0002,   // 比赛结果
	game_robot_HP_ID 								=0x0003,   // 机器人血量
	ICRA_buff_debuff_zone_status_ID  				=0x0005,   // 人工智能挑战赛加成与惩罚区状态
	event_data_ID 									=0x0101,   // 场地事件
	supply_projectile_action_ID		  				=0x0102,   // 补给站动作标识
	referee_warning_ID						    	=0x0104,   // 裁判警告信息
	dart_remaining_time_ID 							=0x0105,   // 飞镖发射口倒计时
	game_robot_status_ID 							=0x0201,   // 比赛机器人状态
	power_heat_data_ID 								=0x0202,   // 实时功率热量数据
	game_robot_pos_ID 						  		=0x0203,   // 机器人位置
	buff_ID 										=0x0204,   // 机器人增益
	aerial_robot_energy_ID 							=0x0205,   // 空中机器人能量状态
	robot_hurt_ID									=0x0206,   // 伤害状态
	shoot_data_ID 								  	=0x0207,   // 实时射击信息
	bullet_remaining_ID 						  	=0x0208,   // 子弹剩余发射数
	rfid_status_ID 									=0x0209,   // 机器人 RFID 状态
	dart_client_cmd_ID 								=0x020A    // 飞镖机器人客户端指令数据
	
}DJI_Judge_CmdID_t;

/***比赛状态***/
typedef struct
{
	uint8_t game_type : 4;
	uint8_t game_progress : 4;
	uint16_t stage_remain_time;
	uint64_t SyncTimeStamp;
	
}game_status_t; 

/***比赛结果***/
typedef struct 
{
	uint8_t winner;
	
}game_result_t;

/***机器人血量***/
typedef struct 
{
	uint16_t red_1_robot_HP;
	uint16_t red_2_robot_HP;
	uint16_t red_3_robot_HP;
	uint16_t red_4_robot_HP;
	uint16_t red_5_robot_HP;
	uint16_t red_7_robot_HP;
	uint16_t red_outpost_HP;
	uint16_t red_base_HP;
	uint16_t blue_1_robot_HP;
	uint16_t blue_2_robot_HP;
	uint16_t blue_3_robot_HP;
	uint16_t blue_4_robot_HP;
	uint16_t blue_5_robot_HP;
	uint16_t blue_7_robot_HP;
	uint16_t blue_outpost_HP;
	uint16_t blue_base_HP;
	
}game_robot_HP_t;

/***飞镖***/
typedef struct   
{
	uint8_t dart_belong;
	uint16_t stage_remaining_time;
	
}dart_status_t;

/***人工智能挑战赛加成与惩罚区状态***/
typedef struct  
{
	uint8_t F1_zone_status:1;
	uint8_t F1_zone_buff_debuff_status:3;
	uint8_t F2_zone_status:1;
	uint8_t F2_zone_buff_debuff_status:3;
	uint8_t F3_zone_status:1;
	uint8_t F3_zone_buff_debuff_status:3;
	uint8_t F4_zone_status:1;
	uint8_t F4_zone_buff_debuff_status:3;
	uint8_t F5_zone_status:1;
	uint8_t F5_zone_buff_debuff_status:3;
	uint8_t F6_zone_status:1;
	uint8_t F6_zone_buff_debuff_status:3;
	uint16_t red1_bullet_left;
	uint16_t red2_bullet_left;
	uint16_t blue1_bullet_left;
	uint16_t blue2_bullet_left;
	
}ICRA_buff_debuff_zone_status_t;

/***场地事件***/
typedef struct  
{
	uint32_t event_type;
	
}event_data_t;

/***补给站动作标识***/
typedef struct   
{
	uint8_t supply_projectile_id;
	uint8_t supply_robot_id;
	uint8_t supply_projectile_step;
	uint8_t supply_projectile_num;
	
}supply_projectile_action_t;

/***裁判警告信息***/
typedef struct  
{
	uint8_t level;
	uint8_t foul_robot_id;
	
}referee_warning_t;

/***飞镖发射口倒计时***/
typedef struct  
{
	uint8_t dart_remaining_time;
	
}dart_remaining_time_t;

/***比赛机器人状态***/
typedef struct  
{
	uint8_t robot_id;
	uint8_t robot_level;
	uint16_t remain_HP;
	uint16_t max_HP;
	uint16_t shooter_id1_17mm_cooling_rate;
	uint16_t shooter_id1_17mm_cooling_limit;
	uint16_t shooter_id1_17mm_speed_limit;
	uint16_t shooter_id2_17mm_cooling_rate;
	uint16_t shooter_id2_17mm_cooling_limit;
	uint16_t shooter_id2_17mm_speed_limit;
	uint16_t shooter_id1_42mm_cooling_rate;
	uint16_t shooter_id1_42mm_cooling_limit;
	uint16_t shooter_id1_42mm_speed_limit;
	uint16_t chassis_power_limit;
	uint8_t mains_power_gimbal_output : 1;
	uint8_t mains_power_chassis_output : 1;
	uint8_t mains_power_shooter_output : 1;
	
}game_robot_status_t;

/***实时功率热量数据***/
typedef struct  
{
	uint16_t chassis_volt;
	uint16_t chassis_current;
	float chassis_power;
	uint16_t chassis_power_buffer;
	uint16_t shooter_id1_17mm_cooling_heat;
	uint16_t shooter_id2_17mm_cooling_heat;
	uint16_t shooter_id1_42mm_cooling_heat;
	
}power_heat_data_t;

/***机器人位置***/
typedef struct   
{
	float x;
	float y;
	float z;
	float yaw;
	
}game_robot_pos_t;

/***机器人增益***/
typedef struct  
{
	uint8_t power_rune_buff;
	
}buff_t;

/***空中机器人能量状态***/
typedef struct 
{
  uint8_t attack_time;
	
}aerial_robot_energy_t;

/***伤害状态***/
typedef struct
{
	uint8_t armor_id : 4;
	uint8_t hurt_type : 4;
	
}robot_hurt_t;

/***实时射击信息***/
typedef struct  
{
	uint8_t bullet_type;
	uint8_t shooter_id;
	uint8_t bullet_freq;
	float bullet_speed;
	
}shoot_data_t;

/***子弹剩余发射数***/
typedef struct 
{
	uint16_t bullet_remaining_num_17mm;
	uint16_t bullet_remaining_num_42mm;
	uint16_t coin_remaining_num;
	
}bullet_remaining_t;

/***机器人 RFID 状态***/
typedef struct 
{
	uint32_t rfid_status;
	
}rfid_status_t;

/***飞镖机器人客户端指令数据***/
typedef struct
{
	uint8_t dart_launch_opening_status;
	uint8_t dart_attack_target;
	uint16_t target_change_time;
	uint8_t first_dart_speed;
	uint8_t second_dart_speed;
	uint8_t third_dart_speed;
	uint8_t fourth_dart_speed;
	uint16_t last_dart_launch_time;
	uint16_t operate_launch_cmd_time;
	
}dart_client_cmd_t;


typedef struct
{
  uint8_t  sof;
  uint16_t data_length;
  uint8_t  seq;
  uint8_t  crc8;
	
}frame_head_t;

typedef struct
{
	frame_head_t frame_header;
	uint16_t cmd_id;
	uint16_t frame_tail;
	
}judge_get_data_t;

typedef struct
{
	judge_get_data_t Judge_Data;
	
	game_status_t game_status;
	game_result_t game_result;
	game_robot_HP_t game_robot_HP;
	ICRA_buff_debuff_zone_status_t ICRA_buff_debuff_zone_status;
	event_data_t event_data;
	supply_projectile_action_t supply_projectile_action;
	referee_warning_t referee_warning;
	dart_remaining_time_t dart_remaining_time;
	game_robot_status_t game_robot_status;
	power_heat_data_t power_heat_data;
	game_robot_pos_t game_robot_pos;
	buff_t buff;
	aerial_robot_energy_t aerial_robot_energy;
	robot_hurt_t robot_hurt;
	shoot_data_t shoot_data;
	bullet_remaining_t bullet_remaining;
	rfid_status_t rfid_status;
	dart_client_cmd_t dart_client_cmd;
	
}judge_t;//裁判系统数据结构体


//DMA接收原始数据
extern uint8_t judge_buf[2][220];
//裁判系统数据结构体
extern judge_t Judge;


void Judge_Init(void);
void Judge_SendByte(uint8_t Byte);
void Judge_SendString(char *string);
void Judge_SendArray(uint8_t *Array, uint16_t Length);
const judge_t * Get_JUDGE_Data(void);


#endif
