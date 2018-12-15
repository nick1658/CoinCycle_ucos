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
		if (*(pre_value.coin[i].data.p_hopper_balance_cur) >= *(pre_value.coin[i].data.p_pre_count_set)){
			//cy_println ("1 ->%d->%d", i, *(pre_value.coin[i].data.p_hopper_balance_cur));
			*pre_value.coin[i].data.p_pre_count_full_flag = 1;
		}else{
			//cy_println ("2 ->%d->%d", i, *(pre_value.coin[i].data.p_hopper_balance_cur));
			*pre_value.coin[i].data.p_pre_count_full_flag = 0;
		}
		*pre_value.coin[i].data.p_coin_current_receive = 0;
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
	coin_env.motor_stuck = MOTOR_STUCK_TIME;//ˢ��ת�̶�תʱ��
	sys_env.stop_flag = 0;
	STORAGE_DIR_P();//��ת
	runstep =0; //�������������
	sys_env.sys_runing_time = 0;
	//cy_println("finish init coin_env");
}

void IR_detect_func(void)
{
	#define IR_SENSOR_FILTER 50
	static uint16_t ir_low_ctr, ir_high_ctr;
	
	if (COIN_DETECT == IR_DETECT_ON){
		ir_low_ctr++;
		ir_high_ctr = 0;
	}else{
		ir_low_ctr = 0;
		ir_high_ctr++;
	}
	if (ir_low_ctr >= IR_SENSOR_FILTER){
		coin_in_flag = 0;
	}
	if (ir_high_ctr >= IR_SENSOR_FILTER){
		coin_in_flag = 1;
	}
	coin_env.coin_speed_time++;
	if (coin_env.motor_stuck > 0){
		coin_env.motor_stuck--;
	}
////////////////////////////////////////////////////////////////////////////////////////////
	if((coin_in_flag == 1) && (coin_in_flag_old == 0)){//��������½��ؼ��
		sys_env.coin_speed = (20000 / coin_env.coin_speed_time) * 10;
		coin_env.coin_speed_time = 0;
		coin_env.motor_stuck = MOTOR_STUCK_TIME;//ˢ��ת�̶�תʱ��
		
		KICK_Q_SCAN(0);
		KICK_Q_SCAN(1);

		RECV_KICK_Q_SCAN(0, 0);
		RECV_KICK_Q_SCAN(0, 1);
		RECV_KICK_Q_SCAN(0, 2);
		RECV_KICK_Q_SCAN(0, 3);
		RECV_KICK_Q_SCAN(0, 4);
		RECV_KICK_Q_SCAN(1, 0);
		RECV_KICK_Q_SCAN(1, 1);
		RECV_KICK_Q_SCAN(1, 2);
		RECV_KICK_Q_SCAN(1, 3);
		RECV_KICK_Q_SCAN(1, 4);
		RECV_KICK_Q_SCAN(2, 0);
		RECV_KICK_Q_SCAN(2, 1);
		RECV_KICK_Q_SCAN(2, 2);
		RECV_KICK_Q_SCAN(2, 3);
		RECV_KICK_Q_SCAN(2, 4);
		
	}
	coin_in_flag_old = coin_in_flag;
}

void runfunction(void)   //������������
{
	if(blockflag == 0){//�±�
		SEND_ERROR(PRESSMLOCKED);
	}
	if (coin_env.motor_stuck == 0){//ת�̶�ת
		SEND_ERROR(MOTOR_STUCK_ERROR);
	}
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
//
void motor_test_poll (void)
{
	static uint16_t pan_test_status = 0;
	static uint16_t belt_test_status = 0;
	static uint16_t one_second_ctr = 0;
	static uint16_t pan_time_ctr;
	static uint16_t belt_time_ctr;
	static uint16_t pan_cycle_time_ctr;
	static uint16_t belt_cycle_time_ctr;
	if (coin_env.pan_test_flag == 1){
		if (pan_cycle_time_ctr > 0){
			switch (pan_test_status)
			{
				case 0:
					if (pan_time_ctr > para_set_value.data.pan_run_time){
						pan_time_ctr = 0;
						pan_test_status = 1;
					}else{
						STORAGE_MOTOR_STARTRUN();
					}
					break;
				case 1:
					if (pan_time_ctr > para_set_value.data.pan_stop_time){
						pan_time_ctr = 0;
						pan_test_status = 0;
						if (pan_cycle_time_ctr > 0){
							pan_cycle_time_ctr--;
						}
					}else{
						STORAGE_MOTOR_STOPRUN();
					}
					break;
				default:break;
			}
		}
	}else{
		pan_time_ctr = 0;
		pan_test_status = 0;
		pan_cycle_time_ctr = para_set_value.data.pan_test_cycle_time;
	}
	if (coin_env.belt_test_flag == 1){
		if (belt_cycle_time_ctr > 0){
			switch (belt_test_status)
			{
				case 0:
					if (belt_time_ctr > para_set_value.data.belt_run_time){
						belt_time_ctr = 0;
						belt_test_status = 1;
					}else{
						BELT_MOTOR_STARTRUN();
					}
					break;
				case 1:
					if (belt_time_ctr > para_set_value.data.belt_stop_time){
						belt_time_ctr = 0;
						belt_test_status = 0;
						if (belt_cycle_time_ctr > 0){
							belt_cycle_time_ctr--;
						}
					}else{
						BELT_MOTOR_STOPRUN();
					}
					break;
				default:
					break;
			}
		}
	}else{
		//cy_println ("belt stop");
		belt_time_ctr = 0;
		belt_test_status = 0;
		belt_cycle_time_ctr = para_set_value.data.belt_test_cycle_time;
	}
	if (one_second_ctr < 5){
		belt_time_ctr++;
		pan_time_ctr++;
		one_second_ctr++;
	}else{
		one_second_ctr = 0;
	}
}
//
