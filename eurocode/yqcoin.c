//////  本函数包括 计数  自学习

#include "s3c2416.h"

/////////////////////////////////////////////////
 uint16_t ccstep = 0;

int16_t coin_value0 =0;
int16_t coin_value1 =0;
int16_t coin_value2 =0;


 int16_t coin_maxvalue0 = AD0STDSET;
 int16_t coin_minvalue0 = AD0STDSET;
 int16_t coin_maxvalue1 = AD1STDSET;
 int16_t coin_minvalue1 = AD1STDSET;
 int16_t coin_maxvalue2 = AD2STDSET;
 int16_t coin_minvalue2 = AD2STDSET;
s_processed_coin_info processed_coin_info;

//4*9
uint16_t coine[COINCNUM][COIN_TYPE_NUM]=     // 由币种决定
{
{100,	50,	50,10,10,5,2,1,10,1000,	500},	//人民币 的面值 倍数  用于计算金额
{200,	100,50,20,10,5,2,1,0, 0, 	0},//欧元的 的面值 倍数
{100, 50,	25,10, 5,1,0,0,0, 0, 	0},//美元的 的面值 倍数	 实际面额的100倍。没有这种面额就直接置0
{200,	100,50,20,10,5,2,1,0, 0, 	0},//英镑的 的面值 倍数
};



void save_coin_number (void)
{
	write_para ();
}

void prepare_coin_cmp_value (void)
{
	if (ad0_min > 0){
		coin_env.cmp_use_index = 0;
	}else if ( ad1_min > 0){
		coin_env.cmp_use_index = 1;
	}else if (ad2_min > 0){
		coin_env.cmp_use_index = 2;
	}else{
		coin_env.cmp_use_index = 0;
	}
	coin_env.ad_index = coin_env.AD_min_index[coin_env.cmp_use_index];
#ifdef SAMPLE_METHOD_0
	coin_value0 = Detect_AD_Value_buf_p[coin_env.ad_index].AD0;
	coin_value1 = Detect_AD_Value_buf_p[coin_env.ad_index].AD1 - coin_value0;
	coin_value2 = Detect_AD_Value_buf_p[coin_env.ad_index].AD2 - Detect_AD_Value_buf_p[coin_env.ad_index].AD1;
#endif
#ifdef SAMPLE_METHOD_1
	coin_value0 = Detect_AD_Value_buf_p[coin_env.ad_index].AD0;
	coin_value1 = Detect_AD_Value_buf_p[coin_env.ad_index].AD1;
	coin_value2 = Detect_AD_Value_buf_p[coin_env.ad_index].AD2;
#endif
	coin_env.AD_min_index[0] = 0;
	coin_env.AD_min_index[1] = 0;
	coin_env.AD_min_index[2] = 0;
}

int16_t is_good_coin (void)
{
	//鉴伪这里时间上还可以优化一下
	int16_t i;
	for (i = 0; i < COIN_TYPE_NUM; i++){
		if   ( (( coin_value0 >= coin_cmp_value[i].compare_min0) && ( coin_value0 <= coin_cmp_value[i].compare_max0))
			&& (( coin_value1 >= coin_cmp_value[i].compare_min1) && ( coin_value1 <= coin_cmp_value[i].compare_max1))
			&& (( coin_value2 >= coin_cmp_value[i].compare_min2) && ( coin_value2 <= coin_cmp_value[i].compare_max2)))
		{
			if (sys_env.save_good_data){
				GOOD_value_buf[good_value_index].AD0 = coin_value0;
				GOOD_value_buf[good_value_index].AD1 = coin_value1;
				GOOD_value_buf[good_value_index].AD2 = coin_value2;
				GOOD_value_buf[good_value_index].use_index = coin_env.cmp_use_index;
				GOOD_value_buf[good_value_index].ad_index = coin_env.ad_index;
				good_value_index++;
				if (good_value_index >= GOOD_BUF_LENGTH)
					good_value_index = 0;
			}
			return i;
		}
	}
	if (sys_env.save_ng_data){
		NG_value_buf[ng_value_index].AD0 = coin_value0;
		NG_value_buf[ng_value_index].AD1 = coin_value1;
		NG_value_buf[ng_value_index].AD2 = coin_value2;
		NG_value_buf[ng_value_index].use_index = coin_env.cmp_use_index;
		NG_value_buf[ng_value_index].ad_index = coin_env.ad_index;
		ng_value_index++;
		if (ng_value_index >= NG_BUF_LENGTH)
			ng_value_index = 0;
	}
	return -1;
}

void coin_recv1_out_func(void)
{
	//cy_println ("kick 1 out");
	EMKICK1(STARTRUN);
}
void coin_recv2_out_func(void)
{
	//cy_println ("kick 2 out");
	EMKICK2(STARTRUN);
}
void coin_recv3_out_func(void)
{
	//cy_println ("kick 3 out");
	EMKICK3(STARTRUN);
}
void coin_recv1_in_func(void)
{
	//cy_println ("kick 1 in");
	EMKICK1(STOPRUN);
}
void coin_recv2_in_func(void)
{
	//cy_println ("kick 2 in");
	EMKICK2(STOPRUN);
}
void coin_recv3_in_func(void)
{
	//cy_println ("kick 3 in");
	EMKICK3(STOPRUN);
}
	
void coin_null_func(void)
{
}


void cy_precoincount(void)
{
	int16_t good_coin = -1;
	if ( (ch0_pre_count != ch0_count) ){	//mean there is a coin come
		ch0_pre_count = ch0_count;
		processed_coin_info.coinnumber++;
		prepare_coin_cmp_value ();
		good_coin = is_good_coin ();
		sys_env.stop_time = STOP_TIME;//无币停机时间10秒
		if ((good_coin < 0) || (coin_env.inhibit_coin[good_coin] == 0) || (coin_env.cycle_box_full == 1)){ //假币或者不接受此类硬币，剔除出来
			if (coin_env.kick_Q[coin_env.kick_Q_index] == 0){
				coin_env.kick_Q[coin_env.kick_Q_index] = para_set_value.data.kick_start_delay_t0;
				coin_env.kick_Q_index++;
				coin_env.kick_Q_index %= KICK_Q_LEN;
			}else{//剔除工位1队列追尾错误
				SEND_ERROR(KICK1COINERROR);
				dbg ("kick1 error alertflag = %d %s, %d", KICK1COINERROR,  __FILE__, __LINE__);
			}
		}else{//真币
			(*(pre_value.country[COUNTRY_ID].coin[good_coin].data.p_count_cur))++;
			(*(pre_value.country[COUNTRY_ID].coin[good_coin].data.p_coin_current_receive))++;
			processed_coin_info.total_money += pre_value.country[coinchoose].coin[good_coin].data.money;
			processed_coin_info.total_good++;
			if (*pre_value.country[COUNTRY_ID].coin[good_coin].data.p_pre_count_full_flag == 0){//
				(*(pre_value.country[COUNTRY_ID].coin[good_coin].data.p_hopper_balance_cur))++;
				if( *(pre_value.country[COUNTRY_ID].coin[good_coin].data.p_hopper_balance_cur) >= *(pre_value.country[COUNTRY_ID].coin[good_coin].data.p_pre_count_set)){// 当前的币种  数量 达到其预置值
					*pre_value.country[COUNTRY_ID].coin[good_coin].data.p_pre_count_full_flag = 1; //此类硬币预置数到，做个标记
				}
				if (coin_env.recv_kick_Q[good_coin][coin_env.recv_kick_Q_index[good_coin]] == 0){
					coin_env.recv_kick_Q[good_coin][coin_env.recv_kick_Q_index[good_coin]] = para_set_value.data.kick_start_delay_t[good_coin];
					coin_env.recv_kick_Q_index[good_coin]++;
					coin_env.recv_kick_Q_index[good_coin] %= RECV_KICK_Q_LEN;
				}else{//收币队列追尾错误
					SEND_ERROR(KICK2COINERROR);
					dbg ("recv kick coin %d error alertflag = %d %s, %d", good_coin, KICK2COINERROR,  __FILE__, __LINE__);
				}
			}
		}
		processed_coin_info.total_coin++;
		sys_env.coin_over = 1;
	}
}




// 根据AD值 计数并取 AD 最大值最小值
uint32_t coinlearnnumber = 0;   //count the coin number all proceed
void cy_coinlearn(void)
{
	if ( (ch0_pre_count != ch0_count) ){	//mean there is a coin come
		ch0_pre_count = ch0_count;
		processed_coin_info.coinnumber++;
		coinlearnnumber++;
		prepare_coin_cmp_value ();
		if( ( coin_value0 > coin_maxvalue0)){     //0
			coin_maxvalue0 = coin_value0;
		}
		if( ( coin_value0 < coin_minvalue0)){
			coin_minvalue0 = coin_value0;
		}
		if( ( coin_value1 > coin_maxvalue1)) {  //  1
			coin_maxvalue1 = coin_value1;
		}
		if( ( coin_value1 < coin_minvalue1)){
			coin_minvalue1 = coin_value1;
		}
		if( ( coin_value2 > coin_maxvalue2)) {  //  2
			coin_maxvalue2 = coin_value2;
		}
		if( ( coin_value2 < coin_minvalue2)){
			coin_minvalue2 = coin_value2;
		}
		sys_env.coin_over = 1;
	}
}
/*************************
**************************/
void detect_read(void)
{
	if(COIN_DETECT == IR_DETECT_ON)    // 红外对射传感器
		dgus_tf1word(ADDR_DETCET1,1);	//被遮挡
	else
		dgus_tf1word(ADDR_DETCET1,0);	//未被遮挡
}

#define TIPS_SIZE 9
const char *coin_tips [] = {"一元", "五角", "五角", "大一角", "一角", "一角", "五分", "两分", "一分", "纪念币10元", "纪念币5元"};

 uint16_t prepic_prenum =0;      // 用于记录 报错前的界面
void alertfuc(uint16_t errorflag) //报错
{
	char str_buf[256];
	dbg ("alert flag is %d", errorflag);
	switch(errorflag)
	{
		case COINNGKICKERROR:
			ALERT_MSG ("提示", "漏踢错误，请注意检查！");
			break;
		case DETECTERROR:
			ALERT_MSG ("提示", "第二个剔除工位电眼计数没有计数！");
			break;
		case KICKCOINFULL:
			ALERT_MSG ("提示", "第二个剔除工位计数不准确或有漏踢！");
			break;
		case KICK1COINERROR:
			ALERT_MSG ("提示", "第一个剔除工位剔除错误！");
			break;
		case KICK2COINERROR:
			ALERT_MSG ("提示", "第二个剔除工位剔除错误！");
			break;
		case PRESSMLOCKED:
			ALERT_MSG ("提示", "轨道堵币！请检查轨道或传感器。再次启动前请先清零！");
			break;
		case ADSTDEEROR:
			ALERT_MSG ("提示", "传感器异常，请检查传感器下面是否卡有硬币或者调整基准值，然后重试！");
			break;
		case COUNT_FINISHED:
			switch (coin_env.full_stack_num){
				case 1:
					if (coin_env.full_coin_stack[0] < TIPS_SIZE){
						sprintf (str_buf, "请更换%s的纸筒。", coin_tips[coin_env.full_coin_stack[0]]);
					}else{
						sprintf (str_buf, "数组越界: %d", coin_env.full_coin_stack[0]);
					}
					break;
				case 2:
					if ((coin_env.full_coin_stack[0] < TIPS_SIZE) &&
						(coin_env.full_coin_stack[1] < TIPS_SIZE)){
						sprintf (str_buf, "请更换%s和%s的纸筒。", 	coin_tips[coin_env.full_coin_stack[0]],
																	coin_tips[coin_env.full_coin_stack[1]]);
					}else{
						sprintf (str_buf, "数组越界: %d, %d", coin_env.full_coin_stack[0], coin_env.full_coin_stack[1]);
					}
					break;
				case 3:
					if ((coin_env.full_coin_stack[0] < TIPS_SIZE) &&
						(coin_env.full_coin_stack[1] < TIPS_SIZE) &&
						(coin_env.full_coin_stack[2] < TIPS_SIZE)){
						sprintf (str_buf, "请更换%s、%s和%s的纸筒。", 	coin_tips[coin_env.full_coin_stack[0]],
																		coin_tips[coin_env.full_coin_stack[1]],
																		coin_tips[coin_env.full_coin_stack[2]]);
					}else{
						sprintf (str_buf, "数组越界: %d, %d, %d", coin_env.full_coin_stack[0], coin_env.full_coin_stack[1], coin_env.full_coin_stack[2]);
					}
					break;
				case 4:
					if ((coin_env.full_coin_stack[0] < TIPS_SIZE) &&
						(coin_env.full_coin_stack[1] < TIPS_SIZE) &&
						(coin_env.full_coin_stack[2] < TIPS_SIZE) &&
						(coin_env.full_coin_stack[3] < TIPS_SIZE)){
						sprintf (str_buf, "请更换%s、%s、%s和%s的纸筒。", coin_tips[coin_env.full_coin_stack[0]],
																		coin_tips[coin_env.full_coin_stack[1]],
																		coin_tips[coin_env.full_coin_stack[2]],
																		coin_tips[coin_env.full_coin_stack[3]]);
					}else{
						sprintf (str_buf, "数组越界: %d, %d, %d, %d", coin_env.full_coin_stack[0], coin_env.full_coin_stack[1], coin_env.full_coin_stack[2], coin_env.full_coin_stack[3]);
					}
					break;
				default:sprintf (str_buf, "ERROR: 1001");break;
			}
			ALERT_MSG ("提示", str_buf);
			break;
		case COMPLETE_UPDATE:
			ALERT_MSG ("提示", "程序更新完成，请断电重新启动");
			break;
		default:
			ALERT_MSG ("提示", "异常，请断电重新启动");
			break;
	}
	return;
}



