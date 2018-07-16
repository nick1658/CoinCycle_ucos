//////  ���������� ����  ��ѧϰ

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
uint16_t coine[COINCNUM][COIN_TYPE_NUM]=     // �ɱ��־���
{
{100,	50,	50,10,10,5,2,1,10,1000,	500},	//����� ����ֵ ����  ���ڼ�����
{200,	100,50,20,10,5,2,1,0, 0, 	0},//ŷԪ�� ����ֵ ����
{100, 	50,	25,10, 5,1,0,0,0, 0, 	0},//��Ԫ�� ����ֵ ����	 ʵ������100����û����������ֱ����0
{200,	100,50,20,10,5,2,1,0, 0, 	0},//Ӣ���� ����ֵ ����
};

void update_coin_number (void)
{
	para_set_value.data.m_1yuan = *(pre_value.country[COUNTRY_ID].coin[0].data.p_pre_count_cur);
	para_set_value.data.m_5jiao = (*(pre_value.country[COUNTRY_ID].coin[1].data.p_pre_count_cur));
	para_set_value.data.m_1jiao_big = 0;//(*(pre_value.country[COUNTRY_ID].coin[3].data.p_pre_count_cur));
	para_set_value.data.m_1jiao = (*(pre_value.country[COUNTRY_ID].coin[4].data.p_pre_count_cur));
	para_set_value.data.m_5fen =  0;//*(pre_value.country[COUNTRY_ID].coin[6].data.p_pre_count_cur);
	para_set_value.data.m_2fen =  0;//*(pre_value.country[COUNTRY_ID].coin[7].data.p_pre_count_cur);
	para_set_value.data.m_1fen =  0;//*(pre_value.country[COUNTRY_ID].coin[8].data.p_pre_count_cur);
	para_set_value.data.m_10yuan = 0;//*(pre_value.country[COUNTRY_ID].coin[9].data.p_pre_count_cur);
	para_set_value.data.m_5yuan = 0;//*(pre_value.country[COUNTRY_ID].coin[10].data.p_pre_count_cur);
}

void recover_coin_number (void)
{
	*(pre_value.country[COUNTRY_ID].coin[0].data.p_pre_count_cur)= para_set_value.data.m_1yuan;
	(*(pre_value.country[COUNTRY_ID].coin[1].data.p_pre_count_cur)) = para_set_value.data.m_5jiao;
	(*(pre_value.country[COUNTRY_ID].coin[3].data.p_pre_count_cur)) = 0;//para_set_value.data.m_1jiao_big;
	(*(pre_value.country[COUNTRY_ID].coin[4].data.p_pre_count_cur)) = para_set_value.data.m_1jiao;
	*(pre_value.country[COUNTRY_ID].coin[6].data.p_pre_count_cur) = 0;//para_set_value.data.m_5fen;
	*(pre_value.country[COUNTRY_ID].coin[7].data.p_pre_count_cur) = 0;//para_set_value.data.m_2fen;
	*(pre_value.country[COUNTRY_ID].coin[8].data.p_pre_count_cur) = 0;//para_set_value.data.m_1fen;
	*(pre_value.country[COUNTRY_ID].coin[9].data.p_pre_count_cur) = 0;//para_set_value.data.m_10yuan;
	*(pre_value.country[COUNTRY_ID].coin[10].data.p_pre_count_cur) = 0;//para_set_value.data.m_5yuan;
}

void save_coin_number (void)
{
	//update_coin_number ();
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
	//��α����ʱ���ϻ������Ż�һ��
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
		if ((sys_env.stop_flag == 1)){
			sys_env.stop_flag = 4;
		}else if ((sys_env.stop_flag != 3) && (sys_env.stop_flag != 4)){//������ڷ�ת״̬
			sys_env.stop_flag = 0;
			sys_env.stop_time = STOP_TIME;//�ޱ�ͣ��ʱ��10��
		}
		if ((good_coin < 0) ||  ((para_set_value.data.coin_full_rej_pos == 1) &&
								 ((*(pre_value.country[COUNTRY_ID].coin[good_coin].data.p_pre_count_set) == 0) ||
								  (coin_env.inhibit_coin[good_coin] == 0) ||
								  (*pre_value.country[COUNTRY_ID].coin[good_coin].data.p_pre_count_full_flag == 1)))){ //�ٱ� ����ֵС��0
			if (coin_env.kick_Q[coin_env.kick_Q_index] == 0){
				coin_env.kick_Q[coin_env.kick_Q_index] = para_set_value.data.kick_start_delay_t0;
				coin_env.kick_Q_index++;
				coin_env.kick_Q_index %= KICK_Q_LEN;
				coin_env.coin_Q[coin_env.coin_Q_remain] = COIN_NG_FLAG;//
			}else{//�޳���λ1����׷β����
				SEND_ERROR(KICK1COINERROR);
				dbg ("kick1 error alertflag = %d %s, %d", KICK1COINERROR,  __FILE__, __LINE__);
			}
		}else {//���
			if (*(pre_value.country[COUNTRY_ID].coin[good_coin].data.p_pre_count_set) == 9999){//ֻʹ����ֹ���
				*(pre_value.country[COUNTRY_ID].coin[good_coin].data.p_pre_count_cur) += 1;
				processed_coin_info.total_money += pre_value.country[coinchoose].coin[good_coin].data.money;
				processed_coin_info.total_good++;
				//coin_env.coin_Q[coin_env.coin_Q_remain] = COIN_GOOD_FLAG;//������޳���λ�޳�
				if (coin_env.recv_kick_Q[good_coin][coin_env.recv_kick_Q_index[good_coin]] == 0){
					coin_env.recv_kick_Q[good_coin][coin_env.recv_kick_Q_index[good_coin]] = para_set_value.data.kick_start_delay_t[good_coin]*10;
					coin_env.recv_kick_Q_index[good_coin]++;
					coin_env.recv_kick_Q_index[good_coin] %= RECV_KICK_Q_LEN;
				}else{//�ձҶ���׷β����
					SEND_ERROR(KICK2COINERROR);
					dbg ("recv kick coin %d error alertflag = %d %s, %d", good_coin, KICK2COINERROR,  __FILE__, __LINE__);
				}
			}else if((*(pre_value.country[COUNTRY_ID].coin[good_coin].data.p_pre_count_set) == 0) ||
					  (*pre_value.country[COUNTRY_ID].coin[good_coin].data.p_pre_count_full_flag == 1)){//�����ܴ���Ӳ�һ���Ԥ�����ѵ�
				coin_env.coin_Q[coin_env.coin_Q_remain] = COIN_FULL_FLAG;//������޳���λ�޳�
			}else{//Ԥ�ü���
				*(pre_value.country[COUNTRY_ID].coin[good_coin].data.p_pre_count_cur) += 1;
				processed_coin_info.total_money += pre_value.country[coinchoose].coin[good_coin].data.money;
				processed_coin_info.total_good++;
				coin_env.coin_Q[coin_env.coin_Q_remain] = COIN_GOOD_FLAG;//
				if( *(pre_value.country[COUNTRY_ID].coin[good_coin].data.p_pre_count_cur) >= *(pre_value.country[COUNTRY_ID].coin[good_coin].data.p_pre_count_set)){// ��ǰ�ı���  ���� �ﵽ��Ԥ��ֵ
					*pre_value.country[COUNTRY_ID].coin[good_coin].data.p_pre_count_full_flag = 1; //����Ӳ��Ԥ���������������
					*(pre_value.country[COUNTRY_ID].coin[good_coin].data.p_coinval) += 1;//��װ����
					coin_env.full_coin_stack[coin_env.full_stack_num] = good_coin;
					coin_env.full_stack_num++;
					if (coin_env.full_stack_num >= para_set_value.data.pre_count_stop_n){
						runstep = 20;   //��ʼͣ��
					}
				}
			}
			coin_env.coin_Q_remain++;//��α�������ͺ��⴫����֮���и�Ӳ��ѭ�����У����Ϊ16����ʾ֮�������Լ�16��Ӳ��
			coin_env.coin_Q_remain %= COIN_Q_LEN;
		}
		processed_coin_info.total_coin++;
		sys_env.coin_over = 1;
		if (sys_env.sys_runing_time == 0){
			sys_env.sys_runing_time_total = 0;
			sys_env.sys_runing_time = 1;
		}else{
			sys_env.sys_runing_time_total += (sys_env.sys_runing_time - 1);
			sys_env.sys_runing_time = 1;
		}
	}
}




// ����ADֵ ������ȡ AD ���ֵ��Сֵ
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
	if(COIN_DETECT == IR_DETECT_ON)    // ������䴫����
		dgus_tf1word(ADDR_DETCET1,1);	//���ڵ�
	else
		dgus_tf1word(ADDR_DETCET1,0);	//δ���ڵ�
}

#define TIPS_SIZE 9
const char *coin_tips [] = {"һԪ", "���", "���", "��һ��", "һ��", "һ��", "���", "����", "һ��", "�����10Ԫ", "�����5Ԫ"};

 uint16_t prepic_prenum =0;      // ���ڼ�¼ ����ǰ�Ľ���
void alertfuc(uint16_t errorflag) //����
{
	char str_buf[256];
	dbg ("alert flag is %d", errorflag);
	switch(errorflag)
	{
		case COINNGKICKERROR:
			ALERT_MSG ("��ʾ", "©�ߴ�����ע���飡");
			break;
		case DETECTERROR:
			ALERT_MSG ("��ʾ", "�ڶ����޳���λ���ۼ���û�м�����");
			break;
		case KICKCOINFULL:
			ALERT_MSG ("��ʾ", "�ڶ����޳���λ������׼ȷ����©�ߣ�");
			break;
		case KICK1COINERROR:
			ALERT_MSG ("��ʾ", "��һ���޳���λ�޳�����");
			break;
		case KICK2COINERROR:
			ALERT_MSG ("��ʾ", "�ڶ����޳���λ�޳�����");
			break;
		case PRESSMLOCKED:
			ALERT_MSG ("��ʾ", "����±ң��������򴫸������ٴ�����ǰ�������㣡");
			break;
		case ADSTDEEROR:
			ALERT_MSG ("��ʾ", "�������쳣�����鴫���������Ƿ���Ӳ�һ��ߵ�����׼ֵ��Ȼ�����ԣ�");
			break;
		case COUNT_FINISHED:
			switch (coin_env.full_stack_num){
				case 1:
					if (coin_env.full_coin_stack[0] < TIPS_SIZE){
						sprintf (str_buf, "�����%s��ֽͲ��", coin_tips[coin_env.full_coin_stack[0]]);
					}else{
						sprintf (str_buf, "����Խ��: %d", coin_env.full_coin_stack[0]);
					}
					break;
				case 2:
					if ((coin_env.full_coin_stack[0] < TIPS_SIZE) &&
						(coin_env.full_coin_stack[1] < TIPS_SIZE)){
						sprintf (str_buf, "�����%s��%s��ֽͲ��", 	coin_tips[coin_env.full_coin_stack[0]],
																	coin_tips[coin_env.full_coin_stack[1]]);
					}else{
						sprintf (str_buf, "����Խ��: %d, %d", coin_env.full_coin_stack[0], coin_env.full_coin_stack[1]);
					}
					break;
				case 3:
					if ((coin_env.full_coin_stack[0] < TIPS_SIZE) &&
						(coin_env.full_coin_stack[1] < TIPS_SIZE) &&
						(coin_env.full_coin_stack[2] < TIPS_SIZE)){
						sprintf (str_buf, "�����%s��%s��%s��ֽͲ��", 	coin_tips[coin_env.full_coin_stack[0]],
																		coin_tips[coin_env.full_coin_stack[1]],
																		coin_tips[coin_env.full_coin_stack[2]]);
					}else{
						sprintf (str_buf, "����Խ��: %d, %d, %d", coin_env.full_coin_stack[0], coin_env.full_coin_stack[1], coin_env.full_coin_stack[2]);
					}
					break;
				case 4:
					if ((coin_env.full_coin_stack[0] < TIPS_SIZE) &&
						(coin_env.full_coin_stack[1] < TIPS_SIZE) &&
						(coin_env.full_coin_stack[2] < TIPS_SIZE) &&
						(coin_env.full_coin_stack[3] < TIPS_SIZE)){
						sprintf (str_buf, "�����%s��%s��%s��%s��ֽͲ��", coin_tips[coin_env.full_coin_stack[0]],
																		coin_tips[coin_env.full_coin_stack[1]],
																		coin_tips[coin_env.full_coin_stack[2]],
																		coin_tips[coin_env.full_coin_stack[3]]);
					}else{
						sprintf (str_buf, "����Խ��: %d, %d, %d, %d", coin_env.full_coin_stack[0], coin_env.full_coin_stack[1], coin_env.full_coin_stack[2], coin_env.full_coin_stack[3]);
					}
					break;
				default:sprintf (str_buf, "ERROR: 1001");break;
			}
			ALERT_MSG ("��ʾ", str_buf);
			break;
		case COMPLETE_UPDATE:
			ALERT_MSG ("��ʾ", "���������ɣ���ϵ���������");
			break;
		default:
			ALERT_MSG ("��ʾ", "�쳣����ϵ���������");
			break;
	}
	return;
}


