#include "S3C2416.h"


s_cctalk_env cctalk_env;

char cctalk_send_buf[64];
int16_t cctalk_res_len;

void cctalk_env_init (void)
{
	int i;
	S8 *p = (S8 *) &cctalk_env;
	for (i = 0; i < sizeof(s_cctalk_env); i++){
		*(p++) = 0;
	}

}



void check_res_flag (void)
{
}
uint8_t cctalk_calc_checksum (char * buf, uint8_t len)
{
	int16_t i;
	int16_t sum = 0;
	for (i = 0; i < len; i++){
		sum += buf[i];
	}
	sum %= 256;
//	if (sum == 0){
//		sum = 256;
//	}
	return (uint8_t)(256 - sum);//自动截断
}
int cctalk_send (uint8_t res_addr, uint8_t * data_buf, int16_t data_len)
{
	int16_t i;
	int8_t sum = 0;
	uint16_t total_len;
	//fill data--------------------------------------------
	cctalk_send_buf[0] = res_addr;
	cctalk_send_buf[1] = data_len;
	cctalk_send_buf[2] = CCTALK_ADDR;
	cctalk_send_buf[3] = 0x00;
	for (i = 0; i < data_len; i++){
		cctalk_send_buf[i + 4] = data_buf[i];
	}
	total_len = data_len + 5;
	sum = cctalk_calc_checksum (cctalk_send_buf, total_len - 1);
	cctalk_send_buf[total_len - 1] = sum;
	
	for (i = 0; i < total_len; i++){
		Uart0_sendchar (cctalk_send_buf[i]);
	}
	return 0;
}



int cctalk_simple_poll_respond (char *recv_buf)
{
	//uint8_t data_buf_tmp[10];
	cctalk_send (recv_buf[2], 0, 0);
	return 0;
}
int res_equipment_id  (char *recv_buf)
{
	uint8_t data_buf_tmp[13];
	///////////////////////////////////////////////////////////////////
	data_buf_tmp[0] = 67;
	data_buf_tmp[1] = 111;
	data_buf_tmp[2] = 105;
	data_buf_tmp[3] = 110;
	data_buf_tmp[4] = 32;
	data_buf_tmp[5] = 65;
	data_buf_tmp[6] = 99;
	data_buf_tmp[7] = 99;
	data_buf_tmp[8] = 101;
	data_buf_tmp[9] = 112;
	data_buf_tmp[10] = 116;
	data_buf_tmp[11] = 111;
	data_buf_tmp[12] = 114;
	///////////////////////////////////////////////////////////////////
	cctalk_send (recv_buf[2], data_buf_tmp, sizeof(data_buf_tmp));
	return 0;
}
int res_modify_master_inhibit_status (char *recv_buf)
{
	return cctalk_simple_poll_respond(recv_buf);
}
int res_serial_number  (char *recv_buf)
{
	uint8_t data_buf_tmp[3];
	///////////////////////////////////////////////////////////////////
	data_buf_tmp[0] = 1;
	data_buf_tmp[1] = 2;
	data_buf_tmp[2] = 3;
	///////////////////////////////////////////////////////////////////
	cctalk_send (recv_buf[2], data_buf_tmp, sizeof(data_buf_tmp));
	return 0;
}
int res_polling_priotrity  (char *recv_buf)
{
	uint8_t data_buf_tmp[2];
	///////////////////////////////////////////////////////////////////
	data_buf_tmp[0] = 2;
	data_buf_tmp[1] = 20;
	///////////////////////////////////////////////////////////////////
	cctalk_send (recv_buf[2], data_buf_tmp, sizeof(data_buf_tmp));
	return 0;
}

int res_manufacturer_id  (char *recv_buf)
{
	uint8_t data_buf_tmp[2];
	///////////////////////////////////////////////////////////////////
	data_buf_tmp[0] = 'C';
	data_buf_tmp[1] = 'Y';
	///////////////////////////////////////////////////////////////////
	cctalk_send (recv_buf[2], data_buf_tmp, sizeof(data_buf_tmp));
	return 0;
}

int res_product_number  (char *recv_buf)
{
	uint8_t data_buf_tmp[6];
	///////////////////////////////////////////////////////////////////
	data_buf_tmp[0] = 'C';
	data_buf_tmp[1] = 'Y';
	data_buf_tmp[2] = '-';
	data_buf_tmp[3] = '8';
	data_buf_tmp[4] = '0';
	data_buf_tmp[5] = '3';
	///////////////////////////////////////////////////////////////////
	cctalk_send (recv_buf[2], data_buf_tmp, sizeof(data_buf_tmp));
	return 0;
}
int res_sortware_version  (char *recv_buf)
{
	uint8_t data_buf_tmp[12];
	///////////////////////////////////////////////////////////////////
	data_buf_tmp[0] = 'C';
	data_buf_tmp[1] = 'Y';
	data_buf_tmp[2] = 'B';
	data_buf_tmp[3] = 'C';
	data_buf_tmp[4] = 'R';
	data_buf_tmp[5] = '-';
	data_buf_tmp[6] = '0';
	data_buf_tmp[7] = '1';
	data_buf_tmp[8] = '0';
	data_buf_tmp[9] = '0';
	data_buf_tmp[10] = '0';
	data_buf_tmp[11] = '1';
	///////////////////////////////////////////////////////////////////
	cctalk_send (recv_buf[2], data_buf_tmp, sizeof(data_buf_tmp));
	return 0;
}

int res_read_buffered (char *recv_buf)
{
	static int8_t event = 0;
	uint8_t data_buf_tmp[11];
	event = (event + 5) % 255;
	///////////////////////////////////////////////////////////////////
	data_buf_tmp[0] = event;
	data_buf_tmp[1] = 1;
	data_buf_tmp[2] = 1;
	data_buf_tmp[3] = 2;
	data_buf_tmp[4] = 2;
	data_buf_tmp[5] = 3;
	data_buf_tmp[6] = 3;
	data_buf_tmp[7] = 4;
	data_buf_tmp[8] = 4;
	data_buf_tmp[9] = 5;
	data_buf_tmp[10] = 5;
	///////////////////////////////////////////////////////////////////
	cctalk_send (recv_buf[2], data_buf_tmp, sizeof(data_buf_tmp));
	return 0;
}

int res_modify_inhibit_status (char *recv_buf)
{
	cctalk_env.coin_inhibit_status = recv_buf[5] << 8 | recv_buf[4];
	
	coin_env.inhibit_coin[0] = cctalk_env.coin_inhibit_status & 0x0001;//1元
	coin_env.inhibit_coin[2] = cctalk_env.coin_inhibit_status & 0x0002;//5角
	coin_env.inhibit_coin[4] = cctalk_env.coin_inhibit_status & 0x0004;//1角
	
	if (cctalk_env.coin_inhibit_status != 0){
		coin_start ();
	}else{
		sys_env.re_run_time = 1;//持续运行
	}
	return cctalk_simple_poll_respond (recv_buf);
}
//
int res_Perform_self_check (char *recv_buf)
{
	uint8_t data_buf_tmp[2];
	///////////////////////////////////////////////////////////////////
	data_buf_tmp[0] = 0;
	data_buf_tmp[1] = 0;
	///////////////////////////////////////////////////////////////////
	cctalk_send (recv_buf[2], data_buf_tmp, sizeof(data_buf_tmp));
	return 0;
}
//
int res_modify_hopper_balance (char *recv_buf)
{
	uint8_t hopper_index = recv_buf[4];
	if (hopper_index > 0){
		hopper_index--;
	}
	if (hopper_index < HOPPER_NUM){
		cctalk_env.hopper_balance[hopper_index] =  recv_buf[5] +  recv_buf[6] * 256;
	}
	para_set_value.data.m_1yuan = cctalk_env.hopper_balance[0];
	para_set_value.data.m_5jiao = cctalk_env.hopper_balance[1];
	para_set_value.data.m_1jiao = cctalk_env.hopper_balance[2];
	check_res_flag ();
	return cctalk_simple_poll_respond (recv_buf);
}
int res_request_hopper_balance (char *recv_buf)
{
	uint8_t hopper_index;
	uint8_t data_buf_tmp[8];
	/////////////////////////////////////
	hopper_index = recv_buf[4];	
	if (hopper_index > 0){
		hopper_index--;
	}
	switch (hopper_index)
	{
		case 0://hopper 1 1元
			data_buf_tmp[0] = 'C';//
			data_buf_tmp[1] = 'N';//
			data_buf_tmp[2] = '1';//
			data_buf_tmp[3] = '0';//
			data_buf_tmp[4] = '0';//
			data_buf_tmp[5] = 'B';//
			data_buf_tmp[6] = (para_set_value.data.m_1yuan & 0xff);//
			data_buf_tmp[7] = (para_set_value.data.m_1yuan >> 8 & 0xff);//
			break;
		case 1://hopper 2 0.5元
			data_buf_tmp[0] = 'C';//
			data_buf_tmp[1] = 'N';//
			data_buf_tmp[2] = '0';//
			data_buf_tmp[3] = '5';//
			data_buf_tmp[4] = '0';//
			data_buf_tmp[5] = 'B';//
			data_buf_tmp[6] = (para_set_value.data.m_5jiao & 0xff);//
			data_buf_tmp[7] = (para_set_value.data.m_5jiao >> 8 & 0xff);//
			break;
		case 2://hopper 3 0.1元
			data_buf_tmp[0] = 'C';//
			data_buf_tmp[1] = 'N';//
			data_buf_tmp[2] = '0';//
			data_buf_tmp[3] = '1';//
			data_buf_tmp[4] = '0';//
			data_buf_tmp[5] = 'B';//
			data_buf_tmp[6] = (para_set_value.data.m_1jiao & 0xff);//
			data_buf_tmp[7] = (para_set_value.data.m_1jiao >> 8 & 0xff);//
			break;
		default:
			data_buf_tmp[0] = 'C';//
			data_buf_tmp[1] = 'N';//
			data_buf_tmp[2] = '0';//
			data_buf_tmp[3] = '0';//
			data_buf_tmp[4] = '0';//
			data_buf_tmp[5] = 'B';//
			data_buf_tmp[6] = 0;//
			data_buf_tmp[7] = 0;//
			break;
	}
	///////////////////////////////////////////////////////////////////////////////////
	cctalk_send (recv_buf[2], data_buf_tmp, sizeof(data_buf_tmp));
	return 0;
}
//

void set_active_resister (uint8_t reg_l, uint8_t reg_h)
{
	 cctalk_env.act_resister_l |= reg_l;
	 cctalk_env.act_resister_h |= reg_h;
}
void reset_active_resister (uint8_t reg_l, uint8_t reg_h)
{
	 cctalk_env.act_resister_l &= (~reg_l);
	 cctalk_env.act_resister_h &= (~reg_h);
}

int res_request_active_resister (char *recv_buf)
{
	uint8_t data_buf_tmp[2];
	///////////////////////////////////////////////////////////////////
	data_buf_tmp[0] = cctalk_env.act_resister_l;//低字节
	data_buf_tmp[1] =  cctalk_env.act_resister_h;//高字节
	///////////////////////////////////////////////////////////////////
	cctalk_send (recv_buf[2], data_buf_tmp, sizeof(data_buf_tmp));
	return 0;
}
//

void update_hopper_status (void)
{
	uint8_t i;
	for (i = 0; i < HOPPER_NUM; i++){
		if (cctalk_env.hopper_balance[i] < 10){
			cctalk_env.hopper_status[i] = HOPPER_STATUS_LOW;
		}else if (cctalk_env.hopper_balance[i] > 300){
			cctalk_env.hopper_status[i] = HOPPER_STATUS_HIGH;
		}else{
			cctalk_env.hopper_status[i] = 0;
		}
	}
}
int res_request_hopper_status (char *recv_buf)
{
	uint8_t hopper_index_tmp = recv_buf[4];
	uint8_t data_buf_tmp[1];
	///////////////////////////////////////////////////////////////////
	if (hopper_index_tmp > 2){
		hopper_index_tmp -= 3;
	}
	if (hopper_index_tmp < HOPPER_NUM){
		update_hopper_status ();
		data_buf_tmp[0] = cctalk_env.hopper_status[hopper_index_tmp];//低字节
	}else{
		data_buf_tmp[0] = 0;
	}
	///////////////////////////////////////////////////////////////////
	cctalk_send (recv_buf[2], data_buf_tmp, sizeof(data_buf_tmp));
	return 0;
}
//
int res_request_opto_status (char *recv_buf)
{
	uint8_t data_buf_tmp[2];
	///////////////////////////////////////////////////////////////////
	data_buf_tmp[0] = 0;//低字节
	data_buf_tmp[1] = 0;//高字节
	///////////////////////////////////////////////////////////////////
	cctalk_send (recv_buf[2], data_buf_tmp, sizeof(data_buf_tmp));
	return 0;
}
//
int res_request_coin_id (char *recv_buf)
{
	uint8_t hopper_index;
	uint8_t data_buf_tmp[6];
	/////////////////////////////////////
	hopper_index = recv_buf[4];	
	if (hopper_index > 0){
		hopper_index--;
	}
	switch (hopper_index)
	{
		case 0://hopper 1 1元
			data_buf_tmp[0] = 'C';//
			data_buf_tmp[1] = 'N';//
			data_buf_tmp[2] = '1';//
			data_buf_tmp[3] = '0';//
			data_buf_tmp[4] = '0';//
			data_buf_tmp[5] = 'B';//
			break;
		case 1://hopper 2 0.5元
			data_buf_tmp[0] = 'C';//
			data_buf_tmp[1] = 'N';//
			data_buf_tmp[2] = '0';//
			data_buf_tmp[3] = '5';//
			data_buf_tmp[4] = '0';//
			data_buf_tmp[5] = 'B';//
			break;
		case 2://hopper 3 0.1元
			data_buf_tmp[0] = 'C';//
			data_buf_tmp[1] = 'N';//
			data_buf_tmp[2] = '0';//
			data_buf_tmp[3] = '1';//
			data_buf_tmp[4] = '0';//
			data_buf_tmp[5] = 'B';//
			break;
		default:
			data_buf_tmp[0] = 'C';//
			data_buf_tmp[1] = 'N';//
			data_buf_tmp[2] = '0';//
			data_buf_tmp[3] = '0';//
			data_buf_tmp[4] = '0';//
			data_buf_tmp[5] = 'B';//
			break;
	}
	///////////////////////////////////////////////////////////////////////////////////
	cctalk_send (recv_buf[2], data_buf_tmp, sizeof(data_buf_tmp));
	return 0;
}
//
int res_request_error_status (char *recv_buf)
{
	uint8_t data_buf_tmp[2];
	///////////////////////////////////////////////////////////////////
	data_buf_tmp[0] = 0;//低字节
	data_buf_tmp[1] = 0;//高字节
	///////////////////////////////////////////////////////////////////
	cctalk_send (recv_buf[2], data_buf_tmp, sizeof(data_buf_tmp));
	return 0;
}
//
int res_request_inhibit_status (char *recv_buf)
{
	uint8_t data_buf_tmp[2];
	///////////////////////////////////////////////////////////////////
	data_buf_tmp[0] = cctalk_env.coin_inhibit_status & 0xFF;//低字节
	data_buf_tmp[1] = (cctalk_env.coin_inhibit_status >> 8) & 0xFF;//高字节
	///////////////////////////////////////////////////////////////////
	cctalk_send (recv_buf[2], data_buf_tmp, sizeof(data_buf_tmp));
	return 0;
}
//
int res_request_coins_in_by_type (char *recv_buf)
{
	int i;
	uint8_t data_buf_tmp[32];//
	///////////////////////////////////////////////////////////////////
	sys_env.re_run_time = 1;//持续运行
	///////////////////////////////////////////////////////////////////
	cctalk_env.hopper_balance[0] = para_set_value.data.m_1yuan;
	cctalk_env.hopper_balance[1] = para_set_value.data.m_5jiao;
	cctalk_env.hopper_balance[2] = para_set_value.data.m_1jiao;
	for (i = 0; i < 16; i++){
		if (i < HOPPER_NUM){
			data_buf_tmp[0 + 2*i] = cctalk_env.hopper_balance[i] & 0xFF;//低字节
			data_buf_tmp[1 + 2*i] = (cctalk_env.hopper_balance[i] >> 8) & 0xFF;//高字节
		}else{
			data_buf_tmp[0 + 2*i] = 0;//低字节
			data_buf_tmp[1 + 2*i] = 0;//高字节
		}
	}
	///////////////////////////////////////////////////////////////////
	cctalk_send (recv_buf[2], data_buf_tmp, sizeof(data_buf_tmp));
	return 0;
}
//
int res_request_money_in (char *recv_buf)
{
	uint32_t money_in_tmp = 0;
	uint8_t data_buf_tmp[4];
	money_in_tmp = para_set_value.data.m_1yuan * 100 + 
								 para_set_value.data.m_5jiao * 50 + 
								 para_set_value.data.m_1jiao * 10;
	///////////////////////////////////////////////////////////////////
	data_buf_tmp[0] = money_in_tmp & 0xFF;//
	data_buf_tmp[1] = (money_in_tmp >> 8) & 0xFF;//
	data_buf_tmp[2] = (money_in_tmp >> 16) & 0xFF;//
	data_buf_tmp[3] = (money_in_tmp >> 24) & 0xFF;//
	///////////////////////////////////////////////////////////////////
	cctalk_send (recv_buf[2], data_buf_tmp, sizeof(data_buf_tmp));
	return 0;
}
//
int res_clear_money_counters (char *recv_buf)
{
	int i;
	for (i = 0; i < HOPPER_NUM; i++){
		cctalk_env.hopper_balance[i] = 0;
	}
	para_set_value.data.m_1yuan = cctalk_env.hopper_balance[0];
	para_set_value.data.m_5jiao = cctalk_env.hopper_balance[1];
	para_set_value.data.m_1jiao = cctalk_env.hopper_balance[2];
	check_res_flag ();
	return cctalk_simple_poll_respond (recv_buf);
}
//
int res_pay_money_out (char *recv_buf)
{
	cctalk_env.payed_money_out = recv_buf[4] | 
													 recv_buf[5] << 8 |
													 recv_buf[6] << 16 |
													 recv_buf[7] << 24;
	//找零钱
	cctalk_env.dispense_event_ctr++;//找零事件加1
	check_res_flag ();
	return cctalk_simple_poll_respond (recv_buf);
}
//
int res_purge_hopper(char *recv_buf)
{
	uint8_t hopper_index;
	uint8_t coin_num;
	hopper_index = recv_buf[4];//0xFF 表示清空所有Hopper，否则代表Hopper序号
	coin_num = recv_buf[5];//0x0 代表清空hopper_index对应的Hopper，否则代表出的硬币数，最多255
	//清空操作
	check_res_flag ();
	return cctalk_simple_poll_respond (recv_buf);
}
//
int res_verify_money_out(char *recv_buf)
{
	uint8_t data_buf_tmp[9];
	///////////////////////////////////////////////////////////////////
	data_buf_tmp[0] = cctalk_env.dispense_event_ctr;
	data_buf_tmp[1] = cctalk_env.payed_money_out & 0xFF;//
	data_buf_tmp[2] = (cctalk_env.payed_money_out >> 8) & 0xFF;//
	data_buf_tmp[3] = (cctalk_env.payed_money_out >> 16) & 0xFF;//
	data_buf_tmp[4] = (cctalk_env.payed_money_out >> 24) & 0xFF;//
	data_buf_tmp[5] = cctalk_env.unpayed_money_out & 0xFF;//
	data_buf_tmp[6] = (cctalk_env.unpayed_money_out >> 8) & 0xFF;//
	data_buf_tmp[7] = (cctalk_env.unpayed_money_out >> 16) & 0xFF;//
	data_buf_tmp[8] = (cctalk_env.unpayed_money_out >> 24) & 0xFF;//
	///////////////////////////////////////////////////////////////////
	cctalk_send (recv_buf[2], data_buf_tmp, sizeof(data_buf_tmp));
	return 0;
}
//
int res_request_hopper_pattern(char *recv_buf)
{
	#define MAX_HOPPER_NUM 6
	int res_flag;
	uint16_t data_len, i, hopper_index_tmp, hopper_num_tmp;
	uint8_t hopper_tmp[MAX_HOPPER_NUM][2];//最多支持六个Hopper
	
	//按数量找零操作
	data_len = recv_buf[1];
	if (data_len <= MAX_HOPPER_NUM * 2){
		memcpy (&hopper_tmp[0][0], &recv_buf[4], data_len);
		for (i = 0; i < HOPPER_NUM; i++){
			para_set_value.data.hopper_num[i] = 0;
		}
		for (i = 0; i < data_len / 2; i++){
			hopper_index_tmp = hopper_tmp[i][0];
			hopper_num_tmp = hopper_tmp[i][1];
			if (hopper_index_tmp > 0){
				hopper_index_tmp--;
			}
			if (hopper_index_tmp < HOPPER_NUM){
				para_set_value.data.hopper_num[hopper_index_tmp] = hopper_num_tmp;
			}
		}
		check_res_flag ();
		res_flag = cctalk_simple_poll_respond (recv_buf);
		coin_dispense ();
		cctalk_env.hopper_balance[0] = para_set_value.data.m_1yuan;
		cctalk_env.hopper_balance[1] = para_set_value.data.m_5jiao;
		cctalk_env.hopper_balance[2] = para_set_value.data.m_1jiao;
		for (i = 0; i < HOPPER_NUM; i++){
			if (cctalk_env.hopper_balance[i] < 10){
				cctalk_env.hopper_status[i] = HOPPER_STATUS_LOW;
			}else if (cctalk_env.hopper_balance[i] > 300){
				cctalk_env.hopper_status[i] = HOPPER_STATUS_HIGH;
			}else{
				cctalk_env.hopper_status[i] = 0;
			}
		}
		cctalk_env.dispense_event_ctr++;//找零事件加1
	}
	return res_flag;
}
//

int cctalk_protocol (char *buf, uint32_t len)
{
	if (buf[len - 1] != cctalk_calc_checksum (buf, len - 1)){
		return -1;
	}
	if (buf[0] != CCTALK_ADDR){
		return -1;
	}
	switch (buf[3]){
		case CCTALK_METHOD_REQUEST_EQUIPMENT_CATEGORY_ID:
			res_equipment_id (buf);
			break;
		case CCTALK_METHOD_MODIFY_MASTER_INHIBIT_STATUS:
			res_modify_master_inhibit_status (buf);
			break;
		case CCTALK_METHOD_REQUEST_SERIAL_NUMBER:
			res_serial_number (buf);
			break;
		case CCTALK_METHOD_REQUEST_POLLING_PRIOTRITY:
			res_polling_priotrity (buf);
			break;
		case CCTALK_METHOD_REQUEST_MANUFACTURER_ID:
			res_manufacturer_id (buf);
			break;
		case CCTALK_METHOD_REQUEST_PRODUCT_CODE:
			res_product_number (buf);
			break;
		case CCTALK_METHOD_READ_BUFFERED_CREDIT_OR_ERROR_CODES:
			res_read_buffered (buf);
			break;
		case CCTALK_METHOD_SIMPLE_POLL://轮询
			cctalk_simple_poll_respond (buf);
			break;
		case CCTALK_METHOD_REQUEST_ACTIVITY_REGISTER://查看实时状态
			res_request_active_resister (buf);
			break;
		case CCTALK_METHOD_RESET_DEVICE://复位
			cctalk_simple_poll_respond (buf);
			break;
		case CCTALK_METHOD_PERFORM_SELF_CHECK://自检
			res_Perform_self_check (buf);
			break;
		case CCTALK_METHOD_MODIFY_HOPPER_BALANCE://设置当前硬币数量
			res_modify_hopper_balance (buf);
			break;
		case CCTALK_METHOD_MODIFY_INHIBIT_STATUS://只打开第N种硬币
			res_modify_inhibit_status (buf);
			break;
		case CCTALK_METHOD_REQUEST_INHIBIT_STATUS://查询设定的硬币接受种类
			res_request_inhibit_status (buf);
			break;
		case CCTALK_METHOD_REQUEST_MONEY_IN://询问已接受到的硬币
			res_request_money_in (buf);
			break;
		case CCTALK_METHOD_CLEAR_MONEY_COUNTERS://清除BCR接受硬币总金额
			res_clear_money_counters (buf);
			break;
		case CCTALK_METHOD_PAY_MONEY_OUT://按金额找零
			res_pay_money_out (buf);
			break;
		case CCTALK_METHOD_VERIFY_MONEY_OUT://核实找出去的硬币金额
			res_verify_money_out (buf);
			break;
		case CCTALK_METHOD_PURGE_HOPPER://清空指定找零器里面一定数量的硬币
			res_purge_hopper (buf);
			break;
		case CCTALK_METHOD_REQUEST_COIN_ID://查询16个通道里面分别有哪些版本的硬币
			res_request_coin_id (buf);
			break;
		case CCTALK_METHOD_REQUEST_ERROR_STATUS://查询错误标志
			res_request_error_status (buf);
			break;
		case CCTALK_METHOD_REQUEST_HOPPER_BALANCE://查询找零器里面硬币数量
			res_request_hopper_balance (buf);
			break;
		case CCTALK_METHOD_REQUEST_HOPPER_PATTERN://指定面额硬币的数量找零
			res_request_hopper_pattern (buf);
			break;
		case CCTALK_METHOD_REQUEST_SOFTWARE_REVISION://询问软件固件版本号
			res_sortware_version (buf);
			break;
		case CCTALK_METHOD_REQUEST_COIN_IN_BY_TYPE://询问各币种分别收到多少枚
			res_request_coins_in_by_type (buf);
			break;
		case CCTALK_METHOD_REQUEST_HOPPER_HIGH_LOW_LEVEL_SENSOR_STATUS://询问找零箱高低位传感器状态
			res_request_hopper_status (buf);
			break;
		case CCTALK_METHOD_READ_OPTO_STATES://读取传感器状态
			res_request_opto_status (buf);
			break;
		default:break;
	}
	return 0;
}
//










