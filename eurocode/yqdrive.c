#include "s3c2416.h"

volatile uint32_t runtime = 0;   // // ִ�м��õļ�ʱ����
unsigned short int runstep = 0;   //  �������������
volatile uint32_t pulse_time = 0;   // //��������ʱ�߱ҵ�����õļ�ʱ����


static int coin_in_flag = 1;
static int coin_in_flag_old = 1;
void deviceinit(void)	//�����Ȱ�ͨ���ϵıҵ���ȥ
{
	int i = 0;

	processed_coin_info.total_coin_old = processed_coin_info.total_coin;
	for (i = 0; i < COIN_TYPE_NUM; i++){//Ԥ�ü���ģʽʱ����ĳ��Ӳ�ҵļ���ֵ�ﵽԤ��ֵ���Ϳ��������Ӳ�ҵļ���
		if (*(pre_value.country[COUNTRY_ID].coin[i].data.p_hopper_balance_cur) >= *(pre_value.country[COUNTRY_ID].coin[i].data.p_pre_count_set)){
			*pre_value.country[COUNTRY_ID].coin[i].data.p_pre_count_full_flag = 1;
		}else{
			*pre_value.country[COUNTRY_ID].coin[i].data.p_pre_count_full_flag = 0;
		}
	}
	disp_allcount();
	coin_env.full_stack_num = 0;
	ch0_count =0;
	coin_env.ad0_step = 0;
	coin_env.ad1_step = 0;
	coin_env.ad2_step = 0;
	sys_env.re_run_time = 0;
	coin_in_flag = 0;
	coin_in_flag_old = 0;
	ccstep = 0;


	processed_coin_info.coinnumber = 0;
	blockflag = ADBLOCKT;
	sys_env.stop_flag = 0;
	STORAGE_DIR_P();//��ת
	runstep =0; //�������������
	sys_env.sys_runing_time = 0;
	//cy_println("finish init coin_env");
}

void IR_detect_func(void)
{
	if (COIN_DETECT == IR_DETECT_ON){
		coin_in_flag = 0;
	}else{
		coin_in_flag = 1;
	}
////////////////////////////////////////////////////////////////////////////////////////////
	if((coin_in_flag == 1) && (coin_in_flag_old == 0)){//��������½��ؼ�⵽Ӳ��
		
		KICK_Q_SCAN(0);
		KICK_Q_SCAN(1);

		RECV_KICK_Q_SCAN(0, 0);
		RECV_KICK_Q_SCAN(0, 1);
		RECV_KICK_Q_SCAN(0, 2);
		RECV_KICK_Q_SCAN(0, 3);
		
		RECV_KICK_Q_SCAN(1, 0);
		RECV_KICK_Q_SCAN(1, 1);
		RECV_KICK_Q_SCAN(1, 2);
		RECV_KICK_Q_SCAN(1, 3);
		RECV_KICK_Q_SCAN(2, 0);
		RECV_KICK_Q_SCAN(2, 1);
		RECV_KICK_Q_SCAN(2, 2);
		RECV_KICK_Q_SCAN(2, 3);
		
		RECV_KICK_Q_SCAN(4, 0);
		RECV_KICK_Q_SCAN(4, 1);
		RECV_KICK_Q_SCAN(4, 2);
		RECV_KICK_Q_SCAN(4, 3);
		RECV_KICK_Q_SCAN(5, 0);
		RECV_KICK_Q_SCAN(5, 1);
		RECV_KICK_Q_SCAN(5, 2);
		RECV_KICK_Q_SCAN(5, 3);
		
	}
	coin_in_flag_old = coin_in_flag;
}

void runfunction(void)   //������������
{
	switch (runstep){
		case 0:
			STORAGE_MOTOR_STARTRUN();	//  ��ת�̵��
			BELT_MOTOR_STARTRUN();   //��������
			runstep = 1;
			break;
		case 1:
			break;
		case 20://Ԥ���������Զ�ͣ��
			STORAGE_MOTOR_STOPRUN();	//  ͣת�̵��
			runtime = 100; // 2��
			runstep = 100;
			break;
		case 40://�ֶ�ͣ��
			STORAGE_MOTOR_STOPRUN();	//  ͣת�̵��
			runtime = 100; // 2��
			runstep = 120;
			break;
		case 100:
			if (runtime == 0){
				prepic_num = JSJM;
				SEND_ERROR(COUNT_FINISHED);
			}
			break;
		case 120:
			if (runtime == 0){
				comscreen(Disp_Indexpic[JSJM],Number_IndexpicB);	 // back to the  picture before alert
				sys_env.workstep = 0;
			}
			break;
		default:break;
	}
}


