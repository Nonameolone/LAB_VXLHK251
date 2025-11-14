#include "software_timer.h"

unsigned char timer1_flag = 0;
unsigned char timer2_flag = 0;
unsigned char timer3_flag = 0;

static int timer1_counter = 0;
static int timer2_counter = 0;
static int timer3_counter = 0;

static int timer1_mul = 0;
static int timer2_mul = 0;
static int timer3_mul = 0;

void setTimer1(int duration){
	timer1_mul = duration / TIMER_CYCLE_1;
	timer1_counter = timer1_mul;
	timer1_flag = 0;
}

void setTimer2(int duration){
	timer2_mul = duration / TIMER_CYCLE_2;
	timer2_counter = timer2_mul;
	timer2_flag = 0;
}

void setTimer3(int duration){
	timer3_mul = duration / TIMER_CYCLE_3;
	timer3_counter = timer3_mul;
	timer3_flag = 0;
}


void timer1Run(void){
	 if(timer1_counter > 0){
		 timer1_counter--;
		 if(timer1_counter <= 0){
			timer1_flag = 1;
		 }
	 }
}

void timer2Run(void){
	 if(timer2_counter > 0){
		 timer2_counter--;
		 if(timer2_counter <= 0){
			timer2_flag = 1;
		 }
	 }
}

void timer3Run(void){
	 if(timer3_counter > 0){
		 timer3_counter--;
		 if(timer3_counter <= 0){
			timer3_flag = 1;
		 }
	 }
}


void ToggleLedRed(void){
	HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
}

void ToggleLedYellow(void){
	HAL_GPIO_TogglePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin);
}

void ToggleLedGreen(void){
	HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
}

void ToggleLedBlue(void){
	HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
}

void ToggleLedOrange(void){
	HAL_GPIO_TogglePin(LED_ORANGE_GPIO_Port, LED_ORANGE_Pin);
}

