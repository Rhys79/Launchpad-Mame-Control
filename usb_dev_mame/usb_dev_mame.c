//*****************************************************************************
//
// usb_dev_mame.c - Main routines for a Mame control device.
//
// Copyright (c) 2011-2013 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 1.0 of the EK-LM4F232 Firmware Package.
//
//*****************************************************************************

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/qei.h"
#include "driverlib/interrupt.h"
#include "usblib/usblib.h"
#include "usblib/usbhid.h"
#include "usblib/device/usbdevice.h"
#include "usblib/device/usbdhid.h"
#include "usblib/device/usbdhidmame.h"
#include "usb_mame_structs.h"
#include "Mame_pins.h"

//*****************************************************************************
//
//! \addtogroup example_list
//! <h1>USB HID Mame Control Device (usb_dev_mame)</h1>
//!
//! This example application turns the evaluation board into a USB volume control
//! supporting the Human Interface Device class.  When the left button is
//! pressed the volume is increased, when the right button is pressed the volume is decreased
//!
//! The device implemented by this application also supports USB remote wakeup
//! allowing it to request the host to reactivate a suspended bus.  If the bus
//! is suspended (as indicated on the application display), pressing the
//! push button will request a remote wakeup assuming the host has not
//! specifically disabled such requests.
//!
//
//*****************************************************************************

//*****************************************************************************
//
// The system tick timer period.
//
//*****************************************************************************
#define SYSTICKS_PER_SECOND     1000  // 1ms systick rate

//*****************************************************************************
//
// The number of checks for switch debouncing.
//
//*****************************************************************************
#define MAX_CHECKS 5

//*****************************************************************************
//
// Pre-scalar for mouse input.
//
//*****************************************************************************
#define MOUSE_SCALAR 2

//*****************************************************************************
//
// This global indicates whether or not we are connected to a USB host.
//
//*****************************************************************************
volatile bool g_bConnected = false;

//*****************************************************************************
//
// This global indicates whether or not the USB bus is currently in the suspend
// state.
//
//*****************************************************************************
volatile bool g_bSuspended = false;

//*****************************************************************************
//
// Global system tick counter holds elapsed time since the application started
// expressed in 100ths of a second.
//
//*****************************************************************************
volatile uint32_t g_ui32SysTickCount;

//*****************************************************************************
//
// Global button arrays and ticks hold button data from each loop for debouncing
//
//*****************************************************************************
volatile signed char g_ui8Pad1[3];
volatile signed char g_ui8Pad2[2];
volatile signed char g_ui8Mouse[3];
volatile signed char g_ui8Pad1_1State[MAX_CHECKS];
volatile signed char g_ui8Pad1_2State[MAX_CHECKS];
volatile signed char g_ui8Pad1_3State[MAX_CHECKS];
volatile signed char g_ui8Pad2_1State[MAX_CHECKS];
volatile signed char g_ui8Pad2_2State[MAX_CHECKS];
volatile signed char g_ui8Mouse_State[MAX_CHECKS];
volatile signed char g_ui8IndexDebounce;
volatile signed char g_ui8Pad1_Debounced[3];
volatile signed char g_ui8Pad2_Debounced[2];
volatile signed char g_ui8Mouse_Debounced[3];

//*****************************************************************************
//
// Global variable indicating if the board is in programing or GPIO mode.
//
//*****************************************************************************
volatile bool g_bProgramMode;

//*****************************************************************************
//
// This enumeration holds the various states that the device can be in during
// normal operation.
//
//*****************************************************************************
volatile enum
{
    //
    // Unconfigured.
    //
    STATE_UNCONFIGURED,

    //
    // No keys to send and not waiting on data.
    //
    STATE_IDLE,

    //
    // Waiting on data to be sent out.
    //
    STATE_SENDING
}
g_eCustomHidState = STATE_UNCONFIGURED;

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

//*****************************************************************************
//
// Handles asynchronous events from the HID driver.
//
// \param pvCBData is the event callback pointer provided during
// USBDHIDCustomHidInit().  This is a pointer to our device structure
// (&g_sCustomHidDevice).
// \param ui32Event identifies the event we are being called back for.
// \param ui32MsgData is an event-specific value.
// \param pvMsgData is an event-specific pointer.
//
// This function is called by the HID driver to inform the application
// of particular asynchronous events related to operation of the HID
// device.
//
// \return Returns 0 in all cases.
//
//*****************************************************************************
uint32_t
CustomHidHandler(void *pvCBData, uint32_t ui32Event, uint32_t ui32MsgData,
                void *pvMsgData)
{
    switch (ui32Event)
    {
        //
        // The host has connected to us and configured the device.
        //
        case USB_EVENT_CONNECTED:
        {
            g_bConnected = true;
            g_bSuspended = false;
            break;
        }

        //
        // The host has disconnected from us.
        //
        case USB_EVENT_DISCONNECTED:
        {
            g_bConnected = false;
            break;
        }

        //
        // We receive this event every time the host acknowledges transmission
        // of a report. It is used here purely as a way of determining whether
        // the host is still talking to us or not.
        //
        case USB_EVENT_TX_COMPLETE:
        {
            //
            // Enter the idle state since we finished sending something.
            //
            g_eCustomHidState = STATE_IDLE;
            break;
        }

        //
        // This event indicates that the host has suspended the USB bus.
        //
        case USB_EVENT_SUSPEND:
        {
            g_bSuspended = true;
            break;
        }

        //
        // This event signals that the host has resumed signalling on the bus.
        //
        case USB_EVENT_RESUME:
        {
            g_bSuspended = false;
            break;
        }


        //
        // We ignore all other events.
        //
        default:
        {
            break;
        }
    }

    return(0);
}

//*****************************************************************************
//
// Send Data if necessary
//
//*****************************************************************************
void SendHIDReport(char ReportNum, signed char ReportData[])
{
	g_eCustomHidState = STATE_SENDING;
	USBDHIDCustomHidStateChange((void *)&g_sCustomHidDevice,ReportNum,ReportData);
	while(g_eCustomHidState != STATE_IDLE)
	{
	}
}

//*****************************************************************************
//
// Store switch states in buffers
//
//*****************************************************************************
void
StoreSwitches(void)
{
	g_ui8Pad1_1State[g_ui8IndexDebounce] =~GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	g_ui8Pad1_2State[g_ui8IndexDebounce] =~GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|
			GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
	g_ui8Pad1_3State[g_ui8IndexDebounce] =~GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	g_ui8Pad2_1State[g_ui8IndexDebounce] =~GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5)>>2;
	g_ui8Pad2_2State[g_ui8IndexDebounce] =~GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|
			GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
	g_ui8Mouse_State[g_ui8IndexDebounce] =~GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0|GPIO_PIN_1);
	++g_ui8IndexDebounce;
	if(g_ui8IndexDebounce >= MAX_CHECKS)
	{
		g_ui8IndexDebounce = 0;
	}
}

//*****************************************************************************
//
// Debounce switches
//
//*****************************************************************************
void
DebounceSwitches(void)
{
	unsigned char i,j;
	j=0xff;
	for (i=0; i<MAX_CHECKS; i++)
	{
		j=j & g_ui8Pad1_1State[i];
	}
	g_ui8Pad1_Debounced[0]=j;

	j=0xff;
	for (i=0; i<MAX_CHECKS; i++)
	{
		j=j & g_ui8Pad1_2State[i];
	}
	g_ui8Pad1_Debounced[1]=j;

	j=0xff;
	for (i=0; i<MAX_CHECKS; i++)
	{
		j=j & g_ui8Pad1_3State[i];
	}
	g_ui8Pad1_Debounced[2]=j;

	j=0xff;
	for (i=0; i<MAX_CHECKS; i++)
	{
		j=j & g_ui8Pad2_1State[i];
	}
	g_ui8Pad2_Debounced[0]=j;

	j=0xff;
	for (i=0; i<MAX_CHECKS; i++)
	{
		j=j & g_ui8Pad2_2State[i];
	}
	g_ui8Pad2_Debounced[1]=j;

	j=0xff;
	for (i=0; i<MAX_CHECKS; i++)
	{
		j=j & g_ui8Mouse_State[i];
	}
	g_ui8Mouse_Debounced[0]=j;
}

//*****************************************************************************
//
// Check buttons
//
//*****************************************************************************
void
CustomHidChangeHandler(void)
{
	// Local arrays to pass to USB
	//
	signed char Pad1[3];
	signed char Pad2[2];
	signed char Mouse[3];

	// If the bus is suspended then resume it.
	//
	if(g_bSuspended)
	{
		USBDHIDCustomHidRemoteWakeupRequest((void *)&g_sCustomHidDevice);
	}

	//Get debounced switch states
	//
	DebounceSwitches();

	// Get mouse position data
	//
	g_ui8Mouse_Debounced[1] = QEIPositionGet(QEI0_BASE)-127 * MOUSE_SCALAR;
	QEIPositionSet(QEI0_BASE, 127);
	g_ui8Mouse_Debounced[2] = QEIPositionGet(QEI1_BASE)-127 * MOUSE_SCALAR;
	QEIPositionSet(QEI1_BASE, 127);

	bool Equals = true;
	signed char i;
	for (i=0; i<3; i++)		//Check for switch state change
	{
		if (g_ui8Pad1[i] != g_ui8Pad1_Debounced[i])
		{
			Equals = false;
		}
	}

	if (!Equals)			//Send report if state has changed
	{
		Pad1[0] = g_ui8Pad1_Debounced[0];
		Pad1[1] = g_ui8Pad1_Debounced[1];
		Pad1[2] = g_ui8Pad1_Debounced[2];
		SendHIDReport(1,Pad1);
	}
	g_ui8Pad1[0] = g_ui8Pad1_Debounced[0];		//Update old states
	g_ui8Pad1[1] = g_ui8Pad1_Debounced[1];
	g_ui8Pad1[2] = g_ui8Pad1_Debounced[2];

	Equals = true;
	for (i=0; i<2; i++)
	{
		if (g_ui8Pad2[i] != g_ui8Pad2_Debounced[i])
		{
			Equals = false;
		}
	}

	if (!Equals)
	{
		Pad2[0] = g_ui8Pad2_Debounced[0];
		Pad2[1] = g_ui8Pad2_Debounced[1];
		SendHIDReport(2,Pad2);
	}
	g_ui8Pad2[0] = g_ui8Pad2_Debounced[0];
	g_ui8Pad2[1] = g_ui8Pad2_Debounced[1];

	Equals = true;
	for (i=0; i<3; i++)
	{
		if (g_ui8Mouse[i] != g_ui8Mouse_Debounced[i])
		{
			Equals = false;
		}
	}

	if (!Equals)
	{
		Mouse[0]=g_ui8Mouse_Debounced[0];
		Mouse[1]=g_ui8Mouse_Debounced[1];
		Mouse[2]=g_ui8Mouse_Debounced[2];
		SendHIDReport(3,Mouse);
	}
	g_ui8Mouse[0] = g_ui8Mouse_Debounced[0];
	g_ui8Mouse[1] = g_ui8Mouse_Debounced[1];
	g_ui8Mouse[2] = g_ui8Mouse_Debounced[2];
}

//*****************************************************************************
//
// This is the interrupt handler for the SysTick interrupt.  It is used to
// update our local tick count and place the board in programming mode when appropriate
//
//*****************************************************************************
void
SysTickIntHandler(void)
{
	g_ui32SysTickCount++;
	StoreSwitches();

    //
    // If the left button has been pressed, and was previously not pressed,
    // start the process of changing the behavior of the JTAG pins.
    //
    if(!ROM_GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4))
    {
            //
            // Change PC0-3 into hardware (i.e. JTAG) pins.
            //
            HWREG(GPIO_PORTC_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
            HWREG(GPIO_PORTC_BASE + GPIO_O_CR) = 0x01;
            HWREG(GPIO_PORTC_BASE + GPIO_O_AFSEL) |= 0x01;
            HWREG(GPIO_PORTC_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
            HWREG(GPIO_PORTC_BASE + GPIO_O_CR) = 0x02;
            HWREG(GPIO_PORTC_BASE + GPIO_O_AFSEL) |= 0x02;
            HWREG(GPIO_PORTC_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
            HWREG(GPIO_PORTC_BASE + GPIO_O_CR) = 0x04;
            HWREG(GPIO_PORTC_BASE + GPIO_O_AFSEL) |= 0x04;
            HWREG(GPIO_PORTC_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
            HWREG(GPIO_PORTC_BASE + GPIO_O_CR) = 0x08;
            HWREG(GPIO_PORTC_BASE + GPIO_O_AFSEL) |= 0x08;
            HWREG(GPIO_PORTC_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
            HWREG(GPIO_PORTC_BASE + GPIO_O_CR) = 0x00;
            HWREG(GPIO_PORTC_BASE + GPIO_O_LOCK) = 0;

            //
            // Change the LED to BLUE to indicate that the pins are in JTAG mode.
            //
            ROM_GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 4);

            //
            // Mark device in programming mode
            //
            g_bProgramMode = true;
    }
}

//*****************************************************************************
//
// This is the main loop that runs the application.
//
//*****************************************************************************
int
main(void)
{
    bool bLastSuspend;

    //
    // Enable lazy stacking for interrupt handlers.  This allows floating-point
    // instructions to be used within interrupt handlers, but at the expense of
    // extra stack usage.
    //
    ROM_FPULazyStackingEnable();

    //
    // Set the clocking to run from the PLL at 50MHz.
    //
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);

	// Initialize the inputs
    PortFunctionInit();

    // Set the system tick to control how often the buttons are polled.
	ROM_SysTickEnable();
	ROM_SysTickIntEnable();
	ROM_SysTickPeriodSet(ROM_SysCtlClockGet() / SYSTICKS_PER_SECOND);

	// Set initial LED Status to RED to indicate not connected
	ROM_GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 2);

    //
    // Not configured initially.
    //
    g_bConnected = false;
    g_bSuspended = false;
    bLastSuspend = false;
    g_bProgramMode = false;
    g_ui8Pad1_Debounced[0] = 0x00;
    g_ui8Pad1_Debounced[1] = 0x00;
    g_ui8Pad1_Debounced[2] = 0x00;
    g_ui8Pad2_Debounced[0] = 0x00;
    g_ui8Pad2_Debounced[1] = 0x00;
    g_ui8Mouse_Debounced[0] = 0x00;
    g_ui8Mouse_Debounced[1] = 0x00;
    g_ui8Mouse_Debounced[2] = 0x00;
    g_ui8Pad1[0] = 0x00;
    g_ui8Pad1[1] = 0x00;
    g_ui8Pad1[2] = 0x00;
    g_ui8Pad2[0] = 0x00;
    g_ui8Pad2[1] = 0x00;
    g_ui8Mouse[0] = 0x00;
    g_ui8Mouse[1] = 0x00;
    g_ui8Mouse[2] = 0x00;

    //
    // Initialize the USB stack for device mode. (must use force on the Tiva launchpad since it doesn't have detection pins connected)
    //
    USBStackModeSet(0, eUSBModeForceDevice, 0);

    //
    // Pass our device information to the USB HID device class driver,
    // initialize the USB
    // controller and connect the device to the bus.
    //
    USBDHIDCustomHidInit(0, &g_sCustomHidDevice);

    //DISable peripheral and int before configuration
   	QEIDisable(QEI0_BASE);
   	QEIDisable(QEI1_BASE);
   	QEIIntDisable(QEI0_BASE,QEI_INTERROR | QEI_INTDIR | QEI_INTTIMER | QEI_INTINDEX);
   	QEIIntDisable(QEI1_BASE,QEI_INTERROR | QEI_INTDIR | QEI_INTTIMER | QEI_INTINDEX);

   	// Configure quadrature encoder, use a top limit of 256
   	QEIConfigure(QEI0_BASE, (QEI_CONFIG_CAPTURE_A_B  | QEI_CONFIG_NO_RESET 	| QEI_CONFIG_QUADRATURE | QEI_CONFIG_NO_SWAP), 255);
   	QEIConfigure(QEI1_BASE, (QEI_CONFIG_CAPTURE_A_B  | QEI_CONFIG_NO_RESET 	| QEI_CONFIG_QUADRATURE | QEI_CONFIG_NO_SWAP), 255);

   	// Enable the quadrature encoder.
   	QEIEnable(QEI0_BASE);
   	QEIEnable(QEI1_BASE);

   	//Set position to a middle value so we can see if things are working
   	QEIPositionSet(QEI0_BASE, 127);
   	QEIPositionSet(QEI1_BASE, 127);

    IntMasterEnable();

    //
    // The main loop starts here.  We begin by waiting for a host connection
    // then drop into the main volume handling section.  If the host
    // disconnects, we return to the top and wait for a new connection.
    //
    while(1)
    {
        //
        // Wait here until USB device is connected to a host.
        //
        while(!g_bConnected)
        {
        	//Set the onboard LED to red when not connected
        	ROM_GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 2);
        }

        //
        // Update the status to green when connected.
        ROM_GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 8);
        //
        // Enter the idle state.
        //
        g_eCustomHidState = STATE_IDLE;

        //
        // Assume that the bus is not currently suspended if we have just been
        // configured.
        //
        bLastSuspend = false;

        //
        // Keep checking the volume buttons for as
        // long as we are connected to the host. This is a simple example
        //
        while(g_bConnected)
        {
            //
			// Has the suspend state changed since last time we checked?
			//
			if(bLastSuspend != g_bSuspended)
			{
				//
				// Yes - the state changed so update the display.
				//
				bLastSuspend = g_bSuspended;
				if(bLastSuspend)
				{
					//Set the onboard LED to red when not connected
					ROM_GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 2);
				}
				else
				{
				    // Update the status to green when connected.
					ROM_GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 8);
				}
			}

		    if(g_ui32SysTickCount>=8  && !g_bProgramMode)
		    {
		    	//Reset systick counter
		    	g_ui32SysTickCount = 0;

				//Check inputs and act accordingly
		    	CustomHidChangeHandler();
		    }


        }
    }
}
