/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 *
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

// Main program for exercise

#include <stdio.h>
#include <time.h>
#include <math.h>
#include "platform.h"

// Student includes
#include "Pixel.h"
#include "draw.h"
#include "gamelogic.h"
#include "main.h"

// #define enable_interrupts


/***************************************************************************************
Name: Erkka Lehtoranta
Name: Veikka Laaksoviita

Tick boxes that you have coded

Led-matrix driver		Game		    Assembler
	[X]					[X]					[]

Brief description: A game where you shoot a pixel alien out of the sky in the name
of liberty and freedom.

Usage:
	Buttons:	Move left	BTN2
				Move right	BTN0
				Shoot		BTN3
	UI:
		Your ship:	Blue ship on the bottom
		Alien:		Green dot on the top
		Ammo left:	Red dots on the right
		Alien HP:	Cyan dots on the left
	Playing:
		Win: Hit the alien enough, depleting its hitpoints.
		Lose: Miss the alien enough, running out of ammo.

*****************************************************************************************/


int main()
{
	init_platform();
#ifdef	enable_interrupts
	init_interrupts();
#endif

	reset_game();

	// Setup screen
	setup();

	Xil_ExceptionEnable();

	// Try to avoid writing any code in the main loop.
	while(1){}

	cleanup_platform();
	return 0;
}


uint8_t current_open_channel = 0;
//Timer interrupt handler for led matrix update. Frequency is 800 Hz
void TickHandler(void *CallBackRef){
	uint32_t StatusEvent;

	// exceptions must be disabled when updating screen
	Xil_ExceptionDisable();

	open_line(42); // Magic number that goes to default case (zero all)
	run(current_open_channel);
	open_line(current_open_channel);

	current_open_channel = current_open_channel == 7 ? 0 : current_open_channel + 1;

	draw_alien(&alien);
	draw_ship(&ship);
	draw_state(&ship);
	if (finished) {
		draw_end(finished);
	} else {
		draw_shots(current_shots);
	}

	/**** Template code ****/
	// clear timer interrupt status
	StatusEvent = XTtcPs_GetInterruptStatus((XTtcPs *)CallBackRef);
	XTtcPs_ClearInterruptStatus((XTtcPs *)CallBackRef, StatusEvent);
	// Enable exceptions
	Xil_ExceptionEnable();
	/* End of template */
}



//Timer interrupt for moving alien, shooting... Frequency is 10 Hz by default
void TickHandler1(void *CallBackRef) {

	//Don't remove this
	uint32_t StatusEvent;

	static uint8_t counter = 1;

	// Game state is checked at the start of every tick
	if (alien.hp <= 0) {
		finished = WIN;
	} else if (ship.ammo <= 0) {
		finished = LOSE;
	}

	if (!finished) {
		++counter;
		if (counter % ALIEN_PACE == 0) {
			move_alien(&alien);
			counter = 0;
		}

		for (uint8_t i = 0; i < CURRENT_SHOTS_MAX; ++i) {
			if (current_shots[i] == NULL) { continue; }

			if (current_shots[i]->y == 0) {
				detect_collision(&alien, current_shots[i]);
				--ship.ammo;
				destroy_shot(&current_shots[i], i);
			} else {
				current_shots[i]->y -= 1;
			}
		}
	}

	//******* Template code ********
	//clear timer interrupt status. DO NOT REMOVE
	StatusEvent = XTtcPs_GetInterruptStatus((XTtcPs *)CallBackRef);
	XTtcPs_ClearInterruptStatus((XTtcPs *)CallBackRef, StatusEvent);

}

//Interrupt handler for switches and buttons.
//Reading Status will tell which button or switch was used
//Bank information is useless in this exercise
void ButtonHandler(void *CallBackRef, u32 Bank, u32 Status){

	//If true, btn0 was used to trigger interrupt
	if (!finished) {
		if (Status & 0x05) {
			if (Status & 0x01) {
				move_ship(RIGHT); // Move right
			}
			if (Status & 0x04) {
				move_ship(LEFT); // Move left
			}
		}
		if (Status & 0x08) {
			shoot();
		}
	}

	if (Status & 0x10) {
		reset_game();
	}
}


