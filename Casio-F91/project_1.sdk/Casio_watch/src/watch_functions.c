/*
 * watch_functions.c
 *
 *  Created on: Nov 11, 2021
 *      Author: gurk
 */


#include <stdio.h>
#include "xparameters.h"
#include "xgpio.h"
#include "xtmrctr.h"
#include "xscugic.h"
#include "xil_exception.h"
#include "xil_printf.h"
#include "xtime_l.h"
#include <unistd.h>
#include "watch_functions.h"

XGpio LEDs; // Not used as an actual Gpio driver

int settingTime = 0;
int hour10 = 2, hour1 = 3, min10 = 5, min1 = 9, sec10 = 4, sec1 = 0;
int alarmHour10, alarmHour1, alarmMinute10, alarmMinute1;
int sWMin10 = 0, sWMin1 = 0, sWSec10 = 0, sWSec1 = 0, sWMs10 = 0, sWMs1 = 0;

char *colon[9];
char *number0[9];
char *number1[9];
char *number2[9];
char *number3[9];
char *number4[9];
char *number5[9];
char *number6[9];
char *number7[9];
char *number8[9];
char *number9[9];

void setAsciiNumbers(){
	colon[0] = "   ";
	colon[1] = "   ";
	colon[2] = "   ";
	colon[3] = " O ";
	colon[4] = "   ";
	colon[5] = " O ";
	colon[6] = "   ";
	colon[7] = "   ";
	colon[8] = "   ";

	//char *number0[9];

	number0[0]= "OOOOOOOOO";
	number0[1] ="O       O";
	number0[2] ="O       O";
	number0[3] ="O       O";
	number0[4] ="O       O";
	number0[5] ="O       O";
	number0[6] ="O       O";
	number0[7] ="O       O";
	number0[8] ="OOOOOOOOO";

	//char *number1[9];

	number1[0]= "        O";
	number1[1]= "        O";
	number1[2]= "        O";
	number1[3]= "        O";
	number1[4]= "        O";
	number1[5]= "        O";
	number1[6]= "        O";
	number1[7]= "        O";
	number1[8]= "        O";

	//char *number2[9];

	number2[0]= "OOOOOOOOO";
	number2[1]= "        O";
	number2[2]= "        O";
	number2[3]= "        O";
	number2[4]= "OOOOOOOOO";
	number2[5]= "O        ";
	number2[6]= "O        ";
	number2[7]= "O        ";
	number2[8]= "OOOOOOOOO";

	//char *number3[9];

	number3[0]= "OOOOOOOOO";
	number3[1]= "        O";
	number3[2]= "        O";
	number3[3]= "        O";
	number3[4]= "OOOOOOOOO";
	number3[5]= "        O";
	number3[6]= "        O";
	number3[7]= "        O";
	number3[8]= "OOOOOOOOO";

	//char *number4[9];

	number4[0]=  "O       O";
	number4[1]= "O       O";
	number4[2]= "O       O";
	number4[3]= "O       O";
	number4[4]= "OOOOOOOOO";
	number4[5]= "        O";
	number4[6]= "        O";
	number4[7]= "        O";
	number4[8]= "        O";

	//char *number5[9];

	number5[0]= "OOOOOOOOO";
	number5[1]= "O        ";
	number5[2]= "O        ";
	number5[3]= "O        ";
	number5[4]= "OOOOOOOOO";
	number5[5]= "        O";
	number5[6]= "        O";
	number5[7]= "        O";
	number5[8]= "OOOOOOOOO";

	//char *number6[9];

	number6[0]= "OOOOOOOOO";
	number6[1]= "O        ";
	number6[2]= "O        ";
	number6[3]= "O        ";
	number6[4]= "OOOOOOOOO";
	number6[5]= "O       O";
	number6[6]= "O       O";
	number6[7]= "O       O";
	number6[8]= "OOOOOOOOO";

	//char *number7[9];

	number7[0]= "OOOOOOOOO";
	number7[1]= "        O";
	number7[2]= "        O";
	number7[3]= "        O";
	number7[4]= "        O";
	number7[5]= "        O";
	number7[6]= "        O";
	number7[7]= "        O";
	number7[8]= "        O";

	//char *number8[9];

	number8[0]= "OOOOOOOOO";
	number8[1]= "O       O";
	number8[2]= "O       O";
	number8[3]= "O       O";
	number8[4]= "OOOOOOOOO";
	number8[5]= "O       O";
	number8[6]= "O       O";
	number8[7]= "O       O";
	number8[8]= "OOOOOOOOO";

	//char *number9[9];

	number9[0]= "OOOOOOOOO";
	number9[1]= "O       O";
	number9[2]= "O       O";
	number9[3]= "O       O";
	number9[4]= "OOOOOOOOO";
	number9[5]= "        O";
	number9[6]= "        O";
	number9[7]= "        O";
	number9[8]= "OOOOOOOOO";
}

void setTime(XGpio BTNInst){
	int mod = 6000000;
	int test = 1;
	settingTime = 1;
	displayNumber(sec1, LEDs);
	xil_printf("Setting the time...\n\r");
	int btn_value = 0;
	enum SettingStates{
		seconds,
		hour,
		minute
	};
	enum SettingStates settingStates;
	settingStates = seconds;
	while(1){
		mod = 6000000;
		btn_value = XGpio_DiscreteRead(&BTNInst, 1);
		if (btn_value == 1){
			switch(settingStates){
			case seconds:
				while(1){
					btn_value = XGpio_DiscreteRead(&BTNInst, 1);
					if (btn_value != 1){
						break;
					}
					test++;
					if ((test % mod) == 0){
						sec1 = displayNumber(sec1+=2, LEDs);
						if (mod > 500000){
							mod /= 1.5;
						}
					}
				}
				test = 1;
				sec1 = displayNumber(++sec1, LEDs);
				break;
			case minute:
				while(1){
					btn_value = XGpio_DiscreteRead(&BTNInst, 1);
					if (btn_value != 1){
						break;
					}
					test++;
					if ((test % mod) == 0){
						min1+=2;
						sec1 = displayNumber(sec1, LEDs);
						if (mod > 500000){
							mod /= 1.5;
						}
					}
				}
				min1++;
				test = 1;
				sec1 = displayNumber(sec1, LEDs);
				break;
			case hour:
				while(1){
					btn_value = XGpio_DiscreteRead(&BTNInst, 1);
					if (btn_value != 1){
						break;
					}
					test++;
					if ((test % mod) == 0){
						hour1+=2;
						sec1 = displayNumber(sec1, LEDs);
						if (mod > 500000){
							mod /= 1.5;
						}
					}
				}
				hour1++;
				test = 1;
				sec1 = displayNumber(sec1, LEDs);
				break;
		}
		}
		if (btn_value == 4){
			while(1){
				btn_value = XGpio_DiscreteRead(&BTNInst, 1);
				if (btn_value != 4){
					break;
				}
			}
			settingTime = 0; // done
			return;
		}
		else if (btn_value == 8){
			while(1){
				btn_value = XGpio_DiscreteRead(&BTNInst, 1);
				if (btn_value != 8){
					break;
				}
			}
			if (settingStates == seconds){
			sec1 = displayNumber(sec1, LEDs);
			xil_printf("Setting hours\n\r");
			settingStates = hour;
			}
			else if(settingStates == hour){
				xil_printf("Setting minutes\n\r");
				settingStates = minute;
			}
			else{
				settingTime = 0;
				xil_printf("Completed!\n\r");
				return;
			}
		}
}
}

int startStopWatch(XGpio BTNInst){
	sWMin10 = 0, sWMin1 = 0, sWSec10 = 0, sWSec1 = 0, sWMs10 = 0, sWMs1 = 0;
	displayStopWatch(0);
}



void setAlarm(XGpio BTNInst){
	displayAlarm();
	xil_printf("Setting the alarm...\n\r");
	int btn_value = 0;
	enum AlarmStates{
		notSet,
		hour,
		minute
	};
	enum AlarmStates alarmState;
	alarmState = notSet;
	while(1){
		btn_value = XGpio_DiscreteRead(&BTNInst, 1);
		if (btn_value == 4){
			while(1){
				btn_value = XGpio_DiscreteRead(&BTNInst, 1);
				if (btn_value != 4){
					break;
				}
			}
			return;
		}
		else if (btn_value == 8){
			while(1){
				btn_value = XGpio_DiscreteRead(&BTNInst, 1);
				if (btn_value != 8){
					break;
				}
			}
			if (alarmState == notSet){
				displayAlarm();
			xil_printf("Setting hours\n\r");
				alarmState = hour;
			}
			else if(alarmState == hour){
				xil_printf("Setting minutes\n\r");
				alarmState = minute;
			}
			else{
				xil_printf("Completed!\n\r");
				return;
			}
		}
		else if (btn_value == 1){
			while(1){
				btn_value = XGpio_DiscreteRead(&BTNInst, 1);
				if(btn_value != 1){
					break;
				}
			}
			switch (alarmState){
				case hour:
					alarmHour1++;
					if (alarmHour10 == 2 && alarmHour1 == 4){
						alarmHour1 = 1;
						alarmHour10 = 0;
					}
					if(alarmHour1 > 9){
						alarmHour1 = 0;
						alarmHour10++;
					}
					displayAlarm();
					break;
				case minute:
					if (alarmHour10 == 2 && alarmHour1 == 4){
						alarmHour1 = 0;
						alarmHour10 = 0;
					}
					alarmMinute1++;
					if(alarmMinute1 > 9){
						alarmMinute1 = 0;
						alarmMinute10++;
						if (alarmMinute10 == 6){
							alarmMinute10 = 0;
							alarmHour1++;
							if(alarmHour1 > 9){
								alarmHour1 = 0;
								alarmHour10++;
							}
						}
					}
					displayAlarm();
					break;
				default:
					break;

		}
	}
}
	xil_printf("Done\n\r");
}

int displayNumber(int sec1, XGpio LEDInst){
	xil_printf("\e[1;1H\e[2J");

	if (sec1 > 10){
		sec10 += sec1/10;
		sec1 = sec1 % 10;
	}
	else if (sec1 > 9) {
		sec10++;
		sec1 = 0;
		}
	if (sec10 > 5) {
		min1++;
		sec10 = 0;
		}
	if (min1 > 9) {
		min10++;
		min1 = 0;
	}
	if (min10 > 5) {
		hour1++;
		min10 = 0;
	}
	if (hour1 > 9) {
		hour10++;
		hour1 = 0;
	}
	if (hour10 > 1 && hour1 > 3) {
		hour1 = 0;
		hour10 = 0;
	}

	if (hour10 == alarmHour10 && hour1 == alarmHour1 && min10 == alarmMinute10 && min1 == alarmMinute1 && !(settingTime)){
		//blink leds if alarm is on
		XGpio_DiscreteWrite(&LEDInst, 1, 15);
	}


	for (int i = 0; i < 9; i++) {

	switch(hour10) {
		case 0:
			xil_printf("%s", number0[i]);
			break;
		case 1:
			xil_printf("%s", number1[i]);
			break;
		case 2:
			xil_printf("%s", number2[i]);
			break;
		}

			xil_printf(" ");

		switch (hour1) {
			case 0:
				xil_printf("%s", number0[i]);
				break;
		case 1:
				xil_printf("%s", number1[i]);
				break;
		case 2:
				xil_printf("%s", number2[i]);
				break;
		case 3:
				xil_printf("%s", number3[i]);
				break;
		case 4:
			xil_printf("%s", number4[i]);
			break;
		case 5:
			xil_printf("%s", number5[i]);
			break;
		case 6:
			xil_printf("%s", number6[i]);
			break;
		case 7:
			xil_printf("%s", number7[i]);
			break;
		case 8:
			xil_printf("%s", number8[i]);
			break;
		case 9:
			xil_printf("%s", number9[i]);
			break;
		}

		xil_printf(colon[i]);

		switch (min10) {
		case 0:
			xil_printf("%s", number0[i]);
			break;
		case 1:
			xil_printf("%s", number1[i]);
			break;
		case 2:
			xil_printf("%s", number2[i]);
			break;
		case 3:
			xil_printf("%s", number3[i]);
			break;
		case 4:
			xil_printf("%s", number4[i]);
			break;
		case 5:
			xil_printf("%s", number5[i]);
			break;
		case 6:
			xil_printf("%s", number6[i]);
			break;
		}

		xil_printf(" ");

		switch (min1) {
		case 0:
			xil_printf("%s", number0[i]);
			break;
		case 1:
			xil_printf("%s", number1[i]);
			break;
		case 2:
			xil_printf("%s", number2[i]);
			break;
		case 3:
			xil_printf("%s", number3[i]);
			break;
		case 4:
			xil_printf("%s", number4[i]);
			break;
		case 5:
			xil_printf("%s", number5[i]);
			break;
		case 6:
			xil_printf("%s", number6[i]);
			break;
		case 7:
			xil_printf("%s", number7[i]);
			break;
		case 8:
			xil_printf("%s", number8[i]);
			break;
		case 9:
			xil_printf("%s", number9[i]);
			break;
		}

		xil_printf(colon[i]);

		switch (sec10) {
		case 0:
			xil_printf("%s", number0[i]);
			break;
		case 1:
			xil_printf("%s", number1[i]);
			break;
		case 2:
			xil_printf("%s", number2[i]);
			break;
		case 3:
			xil_printf("%s", number3[i]);
			break;
		case 4:
			xil_printf("%s", number4[i]);
			break;
		case 5:
			xil_printf("%s", number5[i]);
			break;
		case 6:
			xil_printf("%s", number6[i]);
			break;
		}

		xil_printf(" ");

		switch (sec1) {
		case 0:
			xil_printf("%s", number0[i]);
			break;
		case 1:
			xil_printf("%s", number1[i]);
			break;
		case 2:
			xil_printf("%s", number2[i]);
			break;
		case 3:
			xil_printf("%s", number3[i]);
			break;
		case 4:
			xil_printf("%s", number4[i]);
			break;
		case 5:
			xil_printf("%s", number5[i]);
			break;
		case 6:
			xil_printf("%s", number6[i]);
			break;
		case 7:
			xil_printf("%s", number7[i]);
			break;
		case 8:
			xil_printf("%s", number8[i]);
			break;
		case 9:
			xil_printf("%s", number9[i]);
			break;
		}

		xil_printf("\n\r");

	}
	//XGpio_DiscreteWrite(&LEDInst, 1, 0);
	//xil_printf("\n\n\n\n\n\n\n\n\n\n\n\n");
	return sec1;
}

void displayAlarm(){
	xil_printf("\e[1;1H\e[2J");

	for (int i = 0; i < 9; i++) {

	switch(alarmHour10) {
		case 0:
			xil_printf("%s", number0[i]);
			break;
		case 1:
			xil_printf("%s", number1[i]);
			break;
		case 2:
			xil_printf("%s", number2[i]);
			break;
		}

			xil_printf(" ");

		switch (alarmHour1) {
			case 0:
				xil_printf("%s", number0[i]);
				break;
		case 1:
				xil_printf("%s", number1[i]);
				break;
		case 2:
				xil_printf("%s", number2[i]);
				break;
		case 3:
				xil_printf("%s", number3[i]);
				break;
		case 4:
			xil_printf("%s", number4[i]);
			break;
		case 5:
			xil_printf("%s", number5[i]);
			break;
		case 6:
			xil_printf("%s", number6[i]);
			break;
		case 7:
			xil_printf("%s", number7[i]);
			break;
		case 8:
			xil_printf("%s", number8[i]);
			break;
		case 9:
			xil_printf("%s", number9[i]);
			break;
		}

		xil_printf(colon[i]);

		switch (alarmMinute10) {
		case 0:
			xil_printf("%s", number0[i]);
			break;
		case 1:
			xil_printf("%s", number1[i]);
			break;
		case 2:
			xil_printf("%s", number2[i]);
			break;
		case 3:
			xil_printf("%s", number3[i]);
			break;
		case 4:
			xil_printf("%s", number4[i]);
			break;
		case 5:
			xil_printf("%s", number5[i]);
			break;
		case 6:
			xil_printf("%s", number6[i]);
			break;
		}

		xil_printf(" ");

		switch (alarmMinute1) {
		case 0:
			xil_printf("%s", number0[i]);
			break;
		case 1:
			xil_printf("%s", number1[i]);
			break;
		case 2:
			xil_printf("%s", number2[i]);
			break;
		case 3:
			xil_printf("%s", number3[i]);
			break;
		case 4:
			xil_printf("%s", number4[i]);
			break;
		case 5:
			xil_printf("%s", number5[i]);
			break;
		case 6:
			xil_printf("%s", number6[i]);
			break;
		case 7:
			xil_printf("%s", number7[i]);
			break;
		case 8:
			xil_printf("%s", number8[i]);
			break;
		case 9:
			xil_printf("%s", number9[i]);
			break;
		}



		xil_printf("\n\r");

	}
}

int displayStopWatch(int sWMs10){
	xil_printf("\e[1;1H\e[2J");


	if (sWMs1 > 9) {
		sWMs10++;
		sWMs1 = 0;
		}
	if (sWMs10 > 9) {
		sWSec1++;
		sWMs10 = 0;
		}
	if (sWSec1 > 9) {
		sWSec10++;
		sWSec1 = 0;
	}
	if (sWSec10 > 5) {
		sWMin1++;
		sWSec10 = 0;
	}
	if (sWMin1 > 9) {
		sWMin10++;
		sWMin1 = 0;
	}
	if (sWMin10 > 5 && sWMin1 > 9) {
		sWMin1 = 0;
		sWMin10 = 0;
	}

	for (int i = 0; i < 9; i++) {

	switch(sWMin10) {
		case 0:
			xil_printf("%s", number0[i]);
			break;
		case 1:
			xil_printf("%s", number1[i]);
			break;
		case 2:
			xil_printf("%s", number2[i]);
			break;
		case 3:
			xil_printf("%s", number3[i]);
			break;
		case 4:
			xil_printf("%s", number4[i]);
			break;
		case 5:
			xil_printf("%s", number5[i]);
			break;
		case 6:
			xil_printf("%s", number6[i]);
			break;
		}

			xil_printf(" ");

		switch (sWMin1) {
			case 0:
			xil_printf("%s", number0[i]);
			break;
		case 1:
			xil_printf("%s", number1[i]);
			break;
		case 2:
			xil_printf("%s", number2[i]);
			break;
		case 3:
			xil_printf("%s", number3[i]);
			break;
		case 4:
			xil_printf("%s", number4[i]);
			break;
		case 5:
			xil_printf("%s", number5[i]);
			break;
		case 6:
			xil_printf("%s", number6[i]);
			break;
		case 7:
			xil_printf("%s", number7[i]);
			break;
		case 8:
			xil_printf("%s", number8[i]);
			break;
		case 9:
			xil_printf("%s", number9[i]);
			break;
		}

		xil_printf(colon[i]);

		switch (sWSec10) {
		case 0:
			xil_printf("%s", number0[i]);
			break;
		case 1:
			xil_printf("%s", number1[i]);
			break;
		case 2:
			xil_printf("%s", number2[i]);
			break;
		case 3:
			xil_printf("%s", number3[i]);
			break;
		case 4:
			xil_printf("%s", number4[i]);
			break;
		case 5:
			xil_printf("%s", number5[i]);
			break;
		case 6:
			xil_printf("%s", number6[i]);
			break;
		}

		xil_printf(" ");

		switch (sWSec1) {
		case 0:
			xil_printf("%s", number0[i]);
			break;
		case 1:
			xil_printf("%s", number1[i]);
			break;
		case 2:
			xil_printf("%s", number2[i]);
			break;
		case 3:
			xil_printf("%s", number3[i]);
			break;
		case 4:
			xil_printf("%s", number4[i]);
			break;
		case 5:
			xil_printf("%s", number5[i]);
			break;
		case 6:
			xil_printf("%s", number6[i]);
			break;
		case 7:
			xil_printf("%s", number7[i]);
			break;
		case 8:
			xil_printf("%s", number8[i]);
			break;
		case 9:
			xil_printf("%s", number9[i]);
			break;
		}

		xil_printf(colon[i]);

		switch (sWMs10) {
		case 0:
			xil_printf("%s", number0[i]);
			break;
		case 1:
			xil_printf("%s", number1[i]);
			break;
		case 2:
			xil_printf("%s", number2[i]);
			break;
		case 3:
			xil_printf("%s", number3[i]);
			break;
		case 4:
			xil_printf("%s", number4[i]);
			break;
		case 5:
			xil_printf("%s", number5[i]);
			break;
		case 6:
			xil_printf("%s", number6[i]);
			break;
		case 7:
			xil_printf("%s", number7[i]);
			break;
		case 8:
			xil_printf("%s", number8[i]);
			break;
		case 9:
			xil_printf("%s", number9[i]);
			break;
		}

		xil_printf(" ");

		switch (sWMs1) {
		case 0:
			xil_printf("%s", number0[i]);
			break;
		case 1:
			xil_printf("%s", number1[i]);
			break;
		case 2:
			xil_printf("%s", number2[i]);
			break;
		case 3:
			xil_printf("%s", number3[i]);
			break;
		case 4:
			xil_printf("%s", number4[i]);
			break;
		case 5:
			xil_printf("%s", number5[i]);
			break;
		case 6:
			xil_printf("%s", number6[i]);
			break;
		case 7:
			xil_printf("%s", number7[i]);
			break;
		case 8:
			xil_printf("%s", number8[i]);
			break;
		case 9:
			xil_printf("%s", number9[i]);
			break;
		}

		xil_printf("\n\r");

	}

	return sWMs10;
}
