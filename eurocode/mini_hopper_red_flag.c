#include "S3C2416.h"



s_red_flag_env red_flag_env;
u_red_flag_frame *p_red_flag_frame;


#define SEND_RED_FLAG_FRAME(ADDR, CMD, DATA) { \
	red_flag_payout_buf[0] = 0x05; \
	red_flag_payout_buf[1] = 0x10; \
	red_flag_payout_buf[2] = ADDR; \
	red_flag_payout_buf[3] = CMD; \
	red_flag_payout_buf[4] = DATA; \
	red_flag_payout_buf[5] = 0; \
	uint8_t i; \
	for (i = 0; i < RED_FLAG_PAYOUT_BUF_LEN - 1; i++){ \
		red_flag_payout_buf[5] += red_flag_payout_buf[i]; \
	} \
	for (i = 0; i < RED_FLAG_PAYOUT_BUF_LEN; i++){ \
		Uart3_sendchar (red_flag_payout_buf[i]); \
	} \
}


uint8_t recv_msg_ctr;
char red_flag_msg_buf[RED_FLAG_MSG_BUF_LEN];
char red_flag_payout_buf[RED_FLAG_PAYOUT_BUF_LEN];

void init_red_flag_recv_buf (void)
{
	recv_msg_ctr = 0;
}

int red_flag_request_remain_coin (uint8_t addr)
{
	init_red_flag_recv_buf ();
	if (addr == 0xFF){
		SEND_RED_FLAG_FRAME (0x00, REMAINING_COINS_REQUEST, 0);
		SEND_RED_FLAG_FRAME (0x01, REMAINING_COINS_REQUEST, 0);
		SEND_RED_FLAG_FRAME (0x02, REMAINING_COINS_REQUEST, 0);
	}else{
		SEND_RED_FLAG_FRAME (addr, REMAINING_COINS_REQUEST, 0);
	}
	return 0;
}
//
int red_flag_get_hopper_status (uint8_t addr)
{
//	PC_ALERT_MSG ("GET HOPPER STATUS");
	init_red_flag_recv_buf ();
	if (addr == 0xFF){
		SEND_RED_FLAG_FRAME (0x00, STATUS_REQUEST, 0);
		SEND_RED_FLAG_FRAME (0x01, STATUS_REQUEST, 0);
		SEND_RED_FLAG_FRAME (0x02, STATUS_REQUEST, 0);
	}else{
		SEND_RED_FLAG_FRAME (addr, STATUS_REQUEST, 0);
	}
	return 0;
}
//
int red_flag_empty_hopper (uint8_t addr)
{
//	PC_ALERT_MSG ("EMPTY HOPPER");
	init_red_flag_recv_buf ();
	if (addr == 0xFF){
		SEND_RED_FLAG_FRAME (0x00, EMPTY_HOPPER, 0);
		SEND_RED_FLAG_FRAME (0x01, EMPTY_HOPPER, 0);
		SEND_RED_FLAG_FRAME (0x02, EMPTY_HOPPER, 0);
	}else{
		SEND_RED_FLAG_FRAME (addr, EMPTY_HOPPER, 0);
	}
	return 0;
}
//
int red_flag_reset_hopper (uint8_t addr)
{
//	PC_ALERT_MSG ("RESET HOPPER");
	if (addr == 0xFF){
		SEND_RED_FLAG_FRAME (0x00, RESET_REQUEST, 0);
		SEND_RED_FLAG_FRAME (0x01, RESET_REQUEST, 0);
		SEND_RED_FLAG_FRAME (0x02, RESET_REQUEST, 0);
	}else{
		SEND_RED_FLAG_FRAME (addr, RESET_REQUEST, 0);
	}
	return 0;
}
//

#define DISP_RED_FLAG_FRAME() { \
		sprintf (str_buf, "%02x %02x %02x %02x %02x %02x --%02x", \
		p_frame->fill[0], \
		p_frame->fill[1], \
		p_frame->fill[2], \
		p_frame->fill[3], \
		p_frame->fill[4], \
		p_frame->fill[5], \
		checksum); \
		PC_ALERT_MSG (str_buf); \
}
int red_flag_hopper_res (u_red_flag_frame *p_frame)
{
	char str_buf[256];
	int i;
	uint8_t checksum = 0;
	for (i = 0; i < RED_FLAG_PAYOUT_BUF_LEN - 1; i++){
		//cy_print ("%02x ", p_frame->fill[i]);
		checksum += p_frame->fill[i];
	}
	//cy_println ("%02x--%02x", p_frame->fill[i], checksum);
	
	if (checksum == p_frame->data.checksum){
		if (p_frame->data.cmd == FINISH_MSG){
			para_set_value.data.hopper_unpayout_num[p_frame->data.addr] = p_frame->data.data;
			sprintf (str_buf, "Payout finished,hopper 0(%d-%d),hopper 1(%d-%d),hopper 2(%d-%d)",
				para_set_value.data.hopper_dispense_num[0], para_set_value.data.hopper_unpayout_num[0],
				para_set_value.data.hopper_dispense_num[1], para_set_value.data.hopper_unpayout_num[1],
				para_set_value.data.hopper_dispense_num[2], para_set_value.data.hopper_unpayout_num[2]
			);
			PC_ALERT_MSG (str_buf);
		}else if (p_frame->data.cmd == ONE_COIN_MSG){
		}else if (p_frame->data.cmd == STATUS_MSG){
			DISP_RED_FLAG_FRAME ();
		}else if (p_frame->data.cmd == ACK_MSG){
			sprintf (str_buf, "hopper %d had %d coin not payout",p_frame->data.addr, p_frame->data.data);
			PC_ALERT_MSG (str_buf);
		}else if (p_frame->data.cmd == BUSY_MSG){
		}
	}else{
		DISP_RED_FLAG_FRAME ();
	}
	return 0;
}

void red_flag_msg_process (void)
{
	uint8_t i;
	uint8_t recv_msg_ctr_tmp = recv_msg_ctr;
	
//	cy_println ("\n msg_received = %d", red_flag_env.msg_received);
//	red_flag_env.msg_received = 0;
//	for (i = 0; i < recv_msg_ctr; i++){
//		cy_print ("%02x ", red_flag_msg_buf[i]);
//	}
//	//cy_println ("--%d", recv_msg_ctr);
//	recv_msg_ctr = 0;
	
	red_flag_env.msg_received = 0;
	recv_msg_ctr = 0;
	i = 0;
	//cy_println ("--%d", recv_msg_ctr_tmp);
	while (i < recv_msg_ctr_tmp){
		p_red_flag_frame = (u_red_flag_frame*)&(red_flag_msg_buf[i]);
		red_flag_hopper_res (p_red_flag_frame);
		i += RED_FLAG_PAYOUT_BUF_LEN;
	}
//	
//	PC_ALERT_MSG ("Payout");
}

/*提供给串口中断服务程序，保存串口接收到的单个字符*/
void fill_red_flag_buf3 (char data)
{
	LED2_NOT;
	red_flag_msg_buf[recv_msg_ctr++] = data;
//		red_flag_env.msg_received = 1;
	if (recv_msg_ctr >= RED_FLAG_PAYOUT_BUF_LEN){
		red_flag_env.msg_received = 1;
	}
	//red_flag_env.msg_received_timeout = RED_FLAG_TIMEOUT;
	if (recv_msg_ctr >= RED_FLAG_MSG_BUF_LEN){
		recv_msg_ctr = 0;
	}
}

void red_flag_payout_live_msg (uint8_t addr, uint8_t payout_num)
{
	init_red_flag_recv_buf ();
	SEND_RED_FLAG_FRAME (addr, PAYOUT_REQUEST_LIVE_MSG, payout_num);
}



void red_flag_env_init (void)
{
	int i;
	S8 *p = (S8 *) &red_flag_env;
	for (i = 0; i < sizeof(s_red_flag_env); i++){
		*(p++) = 0;
	}
	recv_msg_ctr = 0;
	red_flag_env.p_process_msg_func = red_flag_msg_process;
	red_flag_env.p_fill_red_flag_buf = fill_red_flag_buf3;
	red_flag_env.p_red_flag_payout = red_flag_payout_live_msg;
	red_flag_env.p_get_hopper_status_func = red_flag_get_hopper_status;
	red_flag_env.p_empty_hopper_func = red_flag_empty_hopper;
	red_flag_env.p_reset_hopper_func = red_flag_reset_hopper;
}











