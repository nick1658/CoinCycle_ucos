#ifndef MINI_HOPPER_RED_FLAG_H
#define MINI_HOPPER_RED_FLAG_H

#define FRAME_SIZE 6

#define HEADER_BYTE 0x05
#define MASTER 0x01

//命令
#define PAYOUT_REQUEST_NO_MSG 0x10
#define STATUS_REQUEST 0x11
#define RESET_REQUEST 0x12
#define REMAINING_COINS_REQUEST 0x13
#define PAYOUT_REQUEST_LIVE_MSG 0x14
#define EMPTY_HOPPER 0x15

//响应说明
#define STATUS_MSG 0x04
#define STATUS_MSG_MOTOR_ERROR 0x01
#define STATUS_MSG_HOPPER_LOW 0x02
#define STATUS_MSG_PRISM_FAILED 0x08
#define STATUS_MSG_SHAFT_FAILED 0x10
#define STATUS_MSG_BUSY        0x20

#define ONE_COIN_MSG 0x07
#define FINISH_MSG 0x08

#define ACK_MSG 0xAA
#define BUSY_MSG 0xBB

#define RED_FLAG_MSG_BUF_LEN 60
#define RED_FLAG_PAYOUT_BUF_LEN 6
#define RED_FLAG_TIMEOUT 5000 //50ms

typedef struct
{
	uint8_t header;
	uint8_t dir;
	uint8_t addr;
	uint8_t cmd;
	uint8_t data;
	uint8_t checksum;
}s_red_flag_frame;

typedef union {
	U8 fill[RED_FLAG_PAYOUT_BUF_LEN];
	s_red_flag_frame data;
}u_red_flag_frame;


typedef struct
{
	uint16_t msg_received;
	uint16_t msg_received_timeout;
	//成员函数
	int (*p_get_hopper_status_func)(uint8_t data);
	int (*p_empty_hopper_func)(uint8_t data);
	int (*p_reset_hopper_func)(uint8_t data);
	void (*p_process_msg_func)(void);
	void (*p_fill_red_flag_buf)(char data);
	void (*p_red_flag_payout) (uint8_t addr, uint8_t payout_num);
}s_red_flag_env;


extern s_red_flag_env red_flag_env;

void red_flag_env_init (void);


#endif
