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
 * helloworld.c: simple test application
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

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "platform.h"
#include "xil_printf.h"
#include "xgpio.h"
#include "xgpiops.h"



int main()
{
	init_platform();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//GPIO PS: BTN3 and LED4
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	XGpioPs_Config* p_btn3_led4_config;
	XGpioPs btn3, led4;
	s32 result_btn3, result_led4;

	//BTN3 and LED4: LookupConfig
	p_btn3_led4_config = XGpioPs_LookupConfig(XPAR_PS7_GPIO_0_DEVICE_ID);
	if (p_btn3_led4_config == NULL) {
		xil_printf("LookupConfig BTN3 and LED4 failed\r\n");
		while (1);
	}

	//BTN3: CfgInitialize
	result_btn3 = XGpioPs_CfgInitialize(&btn3, p_btn3_led4_config, p_btn3_led4_config->BaseAddr);
	if (result_btn3 != XST_SUCCESS) {								//it ALWAYS returns XST_SUCCESS, so no point in checking this(???)
		xil_printf("CfgInitilize BTN3 failed %d\r\n", result_btn3);
		while (1);
	}

	//LED4: CfgInitialize
	result_led4 = XGpioPs_CfgInitialize(&led4, p_btn3_led4_config, p_btn3_led4_config->BaseAddr);
	if (result_led4 != XST_SUCCESS) {								//it ALWAYS returns XST_SUCCESS, so no point in checking this(???)
		xil_printf("CfgInitilize LED4 failed %d\r\n", result_led4);
		while (1);
	}

	XGpioPs_SetDirectionPin(&btn3, 54, 0); //Set GPIO PS in pin 54 (BTN3) as input
	XGpioPs_SetDirectionPin(&led4, 7, 1); //Set GPIO PS in pin 7 (LED4) as output


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//AXI GPIO: sws_4bits (SW0, SW1, SW2, SW3) and leds_4bits (LD0, LD1, LD2, LD3)
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	XGpio_Config* p_sws_config;
	XGpio_Config* p_leds_config;
	XGpio sws;
	XGpio leds;
	int result_sws;
	int result_leds;
	bool exit = false;


	//sws_4bits: LookupConfig and CfgInitialize
	p_sws_config = XGpio_LookupConfig(XPAR_SWITCHES_DEVICE_ID);
	if (p_sws_config == NULL) {
		xil_printf("LookupConfig switches failed\r\n");
		while (1);
	}
	result_sws = XGpio_CfgInitialize(&sws, p_sws_config, p_sws_config->BaseAddress);
	if (result_sws != XST_SUCCESS) {
		xil_printf("CfgInitilize switches failed %d\r\n", result_sws);
		while (1);
	}

	//leds_4bits: LookupConfig and CfgInitialize
	p_leds_config = XGpio_LookupConfig(XPAR_LEDS_DEVICE_ID);
	if (p_leds_config == NULL) {
		xil_printf("LookupConfig LEDs failed\r\n");
		while (1);
	}
	result_leds = XGpio_CfgInitialize(&leds, p_leds_config, p_leds_config->BaseAddress);
	if (result_leds != XST_SUCCESS) {
		xil_printf("CfgInitilize LEDs failed %d\r\n", result_leds);
		while (1);
	}

	XGpio_SetDataDirection(&sws, 1, 0xFFFFFFFF);  //Set sws_4bits (AXI GPIO) as inputs
	XGpio_SetDataDirection(&leds, 1, 0x00000000); //Set leds_4bits (AXI GPIO) as outputs


	while (!exit){
		u32 button = XGpioPs_ReadPin(&btn3, 54); //Read BTN3 value
		XGpioPs_WritePin(&led4, 7, button);

		u32 switches = XGpio_DiscreteRead(&sws, 1); //Read switches value
		XGpio_DiscreteWrite(&leds, 1, switches&0x000F);

		if (switches == 0x000F){
			sleep(2);
			XGpio_DiscreteWrite(&leds, 1, 0000);
			exit = true;
		}
	}

	cleanup_platform();

}
