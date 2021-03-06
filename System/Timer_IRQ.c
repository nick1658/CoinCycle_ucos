/**************************************************************/
/*The initial Timer4*/
/**************************************************************/

#include "S3C2416.h"
#include "Exception.h"


static void (*Timer4_Callback)(void);
static void (*Timer3_Callback)(void);
static void (*Timer2_Callback)(void);
static void (*Timer1_Callback)(void);
static void (*Timer0_Callback)(void); 

static volatile unsigned int timer_tick = 0;
static void timer_update (void)
{
	timer_tick++;
}

/*中断程序*/
volatile uint32_t tscount=0;
//20ms中断一次
void Timer4_IRQ(void)
{   
	if (sys_env.tty_online_ms > 1){sys_env.tty_online_ms--;}
	
	if (sys_env.stop_time > 0){sys_env.stop_time--;}
	if (sys_env.system_delay > 0){sys_env.system_delay--;}
	if(tscount >0){tscount--;}
	//if( blockflag> 0){blockflag--;}				
	if(adtime >0){adtime--;}
	if(runtime > 0){runtime--;}
	
	//SetWatchDog(); //看门狗喂狗
	rWTDAT=WatchDogTimer;//看门狗喂狗
	rWTCNT=WatchDogTimer;//看门狗喂狗
}
static void Timer4_Handler(void)
{	
	IRQ_ClearInt(INT_TIMER4);
	Timer4_Callback(); // 定时器4回调函数
}

void Timer4_Start(void)
{
	rTCON |= (0x1 << 20); // 定时器开启
}
void Timer4_Stop(void)
{
	rTCON &= ~(0x1 << 20); // 定时器停止
}

// 1us ~ 23ms			
//		if (para_set_value.data.hopper_output_timeout[X] > 0){ \
			para_set_value.data.hopper_output_timeout[X] = 20; \
		} \
//
void Timer4_Init(unsigned short us, void (*Callback)(void), unsigned short enable_int)
{
// 定时器4时钟频率为PCLK(66M)/(5+1)/4=2750KHZ
	if (us == 0) {
		return;
	}
	rTCFG0 &= ~(0xff << 8);	
	rTCFG0 |= (5 << 8);	
	rTCFG1 &= ~(0xf << 16);
	rTCFG1 |= (0x1 << 16);  // Timer4 4分频
	rTCNTB4 = ((unsigned int)us*2750)/1000;
	rTCON |= (0x1 << 21); // 更新计数值
	rTCON &= ~(0x1 << 21); // 清除
	rTCON |= (0x1 << 22); // 自动重装载
	
	if (Callback != 0) {
		Timer4_Callback = Callback;
		IRQ_Register(INT_TIMER4, Timer4_Handler);
		if (enable_int == 1){
			IRQ_EnableInt(INT_TIMER4);
		}
	}
}




static int16_t hopper_coin_in[HOPPER_NUM] = {0, 0, 0};
#define HOPPER_COUNTER(X) if (HOPPER##X##_CNT_IN == 0){ \
	if (hopper_coin_in[X] == 0){ \
		hopper_coin_in[X] = 1; \
		para_set_value.data.hopper_dispense_cnt[X]++; \
		para_set_value.data.hopper_dispense_num[X]--; \
		if (para_set_value.data.belt_runtime > 0){ \
			para_set_value.data.belt_runtime = BELT_RUN_TIME; \
		} \
	} \
}else{ \
		hopper_coin_in[X] = 0; \
}

extern void main_task(void);
void Timer3_IRQ(void)
{ 
	main_task ();
	
	HOPPER_COUNTER(0);
	HOPPER_COUNTER(1);
	HOPPER_COUNTER(2);
	
	if (coin_env.kick_keep_t0 > 0){
		coin_env.kick_keep_t0--;
		if (coin_env.kick_keep_t0 == 0){
			EMKICK0(STOPRUN);
		}
	}
	RECV_KICK_KEEP_SCAN(0);
	RECV_KICK_KEEP_SCAN(1);
	RECV_KICK_KEEP_SCAN(2);
	RECV_KICK_KEEP_SCAN(4);
	RECV_KICK_KEEP_SCAN(5);
	
	coin_cross_time++;
	if(pulse_time > 0){pulse_time--;}
}



static void Timer3_Handler(void)
{	
	IRQ_ClearInt(INT_TIMER3);
	Timer3_Callback(); // 定时器3回调函数
}
void Timer3_Start(void)
{
	rTCON |= (0x1 << 16); // 定时器开启
}
void Timer3_Stop(void)
{
	rTCON &= ~(0x1 << 16); // 定时器停止
	rTCNTB3 = 275;
	rTCON |= (0x1 << 17); // 更新计数值
	rTCON &= ~(0x1 << 17); // 清除
}
// 1us ~ 23ms
void Timer3_Init(unsigned short us, void (*Callback)(void), unsigned short enable_int)
{
// 定时器3时钟频率为PCLK(66M)/(5+1)/4=2750KHZ
	if (us == 0) {
		return;
	}
	rTCFG0 &= ~(0xff << 8);	
	rTCFG0 |= (5 << 8);	
	rTCFG1 &= ~(0xf << 12);
	rTCFG1 |= (0x1 << 12);  // Timer3 4分频
	rTCNTB3 = ((unsigned int)us*2750)/1000;
	rTCON |= (0x1 << 17); // 更新计数值
	rTCON &= ~(0x1 << 17); // 清除
	rTCON |= (0x1 << 19); // 自动重装载
	
	if (Callback != 0) {
		Timer3_Callback = Callback;
		IRQ_Register(INT_TIMER3, Timer3_Handler);
		if (enable_int == 1){
			IRQ_EnableInt(INT_TIMER3);
		}
	}
}



volatile uint32_t SystemTick;
uint32_t time_out = 0;

void Timer2_IRQ(void)
{
	SystemTick++; // 1ms/tick
	if (time_out)time_out++;
}

static void Timer2_Handler(void)
{	
	IRQ_ClearInt(INT_TIMER2);
	Timer2_Callback(); // 定时器2回调函数
}

void Timer2_Start(void)
{
	rTCON |= (0x1 << 12); // 定时器开启
}

void Timer2_Stop(void)
{
	rTCON &= ~(0x1 << 12); // 定时器停止
}

// 1us ~ 23ms
void Timer2_Init(unsigned short us, void (*Callback)(void), unsigned short enable_int)
{
// 定时器2时钟频率为PCLK(66M)/(5+1)/4=2750KHZ
	if (us == 0) {
		return;
	}
	rTCFG0 &= ~(0xff << 8);	
	rTCFG0 |= (5 << 8);	
	rTCFG1 &= ~(0xf << 8);
	rTCFG1 |= (0x1 << 8);  // Timer2 4分频
	rTCNTB2 = ((unsigned int)us*2750)/1000;
	rTCON |= (0x1 << 13); // 更新计数值
	rTCON &= ~(0x1 << 13); // 清除
	rTCON |= (0x1 << 15); // 自动重装载
	
	if (Callback != 0) {
		Timer2_Callback = Callback;
		IRQ_Register(INT_TIMER2, Timer2_Handler);
		if (enable_int == 1){
			IRQ_EnableInt(INT_TIMER2);
		}
	}
}



void Timer1_IRQ (void)
{
	IRQ_DisableInt(INT_TIMER1);
	cy_println ("\n--------------------------------------------------------------------------Timer1_IRQ");
}
static void Timer1_Handler(void)
{	
	IRQ_ClearInt(INT_TIMER1);
	Timer1_Callback(); // 定时器1回调函数
}
void Timer1_Start(void)
{
	rTCON |= (0x1 << 8); // 定时器开启
}
void Timer1_Stop(void)
{
	rTCON &= ~(0x1 << 8); // 定时器停止
}

// 1us ~ 23ms
void Timer1_Init(unsigned short us, void (*Callback)(void), unsigned short enable_int)
{
// 定时器1时钟频率为PCLK(66M)/(5+1)/4=2750KHZ
	if (us == 0) {
		return;
	}
	rTCFG0 &= ~(0xff);	
	rTCFG0 |= (5);	
	rTCFG1 &= ~(0xf << 4);
	rTCFG1 |= (0x1 << 4);  // Timer1 4分频
	rTCNTB1 = ((unsigned int)us*2750)/1000;
	rTCON |= (0x1 << 9); // 更新计数值
	rTCON &= ~(0x1 << 9); // 清除
	rTCON |= (0x1 << 11); // 自动重装载
	
	if (Callback != 0) {
		Timer1_Callback = Callback;
		IRQ_Register(INT_TIMER1, Timer1_Handler);
		if (enable_int == 1){
			IRQ_EnableInt(INT_TIMER1);
		}
	}
}



void Timer0_IRQ (void)
{
	static uint32_t T_count = 0;
	T_count++;
	
	if (T_count > 1000){
		T_count = 0;
		LED1_NOT;
	}
}

u32 is_system_ticks_handler = 0;

static void Timer0_Handler(void)//在SYS模式下执行
{	
	IRQ_ClearInt(INT_TIMER0);
	OSIntEnter ();
	Timer0_Callback(); // 定时器0回调函数
	is_system_ticks_handler = 1;
}

extern unsigned int Int_Offset;
void is_system_ticks (void)//在SVC模式下执行
{
	if (is_system_ticks_handler == 1){
		OSIntExit ();
	}else if(rSRCPND1 & (1 << INT_TIMER0)){
		IRQ_ClearInt(INT_TIMER0);
		OSIntEnter ();
		Timer0_Callback(); // 定时器0回调函数
		OSIntExit ();
	}
	is_system_ticks_handler = 0;
}

void Timer0_Start(void)
{
	rTCON |= (0x1 << 0); // 定时器开启
}
void Timer0_Stop(void)
{
	rTCON &= ~(0x1 << 0); // 定时器停止
}

// 1us ~ 23ms
void Timer0_Init(unsigned short us, void (*Callback)(void), unsigned short enable_int)
{
// 定时器0时钟频率为PCLK(66M)/(5+1)/4=2750KHZ
	if (us == 0) {
		return;
	}
	rTCFG0 &= ~(0xff);	
	rTCFG0 |= (5);	
	rTCFG1 &= ~(0xf);
	rTCFG1 |= (0x1);  // Timer0 4分频
	rTCNTB0 = ((unsigned int)us*2751)/1000;
	rTCON |= (0x1 << 1); // 更新计数值
	rTCON &= ~(0x1 << 1); // 清除
	rTCON |= (0x1 << 3); // 自动重装载
	
	if (Callback != 0) {
		Timer0_Callback = Callback;
		IRQ_Register(INT_TIMER0, Timer0_Handler);
		if (enable_int == 1){
			IRQ_EnableInt(INT_TIMER0);
		}
	}
}

void Init_OS_ticks (void)
{
	Timer0_Callback = OSTimeTick;
	Timer0_Init(1000, OSTimeTick, 1);//1ms
	Timer0_Start ();
}


void Timer_Init (void)
{
	Timer1_Callback = timer_update;
	Timer2_Callback = timer_update;
	Timer3_Callback = timer_update;
	Timer4_Callback = timer_update;
	
	//Timer1_Init(1000, OSTimeTick);//1ms
	Timer1_Init(23000, Timer1_IRQ, 0);//1ms
	Timer2_Init(1000, Timer2_IRQ, 1);//0.5ms
	Timer3_Init(100, Timer3_IRQ, 1);//0.1ms
	Timer4_Init(20000, Timer4_IRQ, 1);//20ms
	
	Timer4_Start ();
	Timer3_Start ();
	//Timer2_Start ();
	Timer1_Start ();
}


