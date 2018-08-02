#include "s3c2416.h"

volatile uint32_t runtime = 0;   // // 执行件用的计时变量
unsigned short int runstep = 0;   //  部件动作步骤号
volatile uint32_t pulse_time = 0;   // //部件测试时踢币电磁铁用的计时变量


static int coin_in_flag = 1;
static int coin_in_flag_old = 1;
void deviceinit(void)	//开机先把通道上的币挡下去
{
	int i = 0;

	processed_coin_info.total_coin_old = processed_coin_info.total_coin;
	for (i = 0; i < COIN_TYPE_NUM; i++){//预置计数模式时，当某种硬币的计数值达到预置值，就可以清零该硬币的计数
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
	STORAGE_DIR_P();//正转
	runstep =0; //正常工作步骤号
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
	if((coin_in_flag == 1) && (coin_in_flag_old == 0)){//对射电眼下降沿检测到硬币
		
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

void runfunction(void)   //部件动作函数
{
	switch (runstep){
		case 0:
			STORAGE_MOTOR_STARTRUN();	//  开转盘电机
			BELT_MOTOR_STARTRUN();   //斗送入电机
			runstep = 1;
			break;
		case 1:
			break;
		case 20://预置数到，自动停机
			STORAGE_MOTOR_STOPRUN();	//  停转盘电机
			runtime = 100; // 2秒
			runstep = 100;
			break;
		case 40://手动停机
			STORAGE_MOTOR_STOPRUN();	//  停转盘电机
			runtime = 100; // 2秒
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


