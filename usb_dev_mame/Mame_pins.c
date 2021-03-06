//*****************************************************************************
// Copyright (c) 2013 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
//   Redistribution and use in source and binary forms, with or without
//   modification, are permitted provided that the following conditions
//   are met:
// 
//   Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.
// 
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the  
//   distribution.
// 
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// This file was automatically generated by the Tiva C Series PinMux Utility
// Version: 1.0.2
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "Mame_pins.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom_map.h"
#include "driverlib/gpio.h"

//*****************************************************************************
void
PortFunctionInit(void)
{
    //
    // Enable Peripheral Clocks 
    //
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI0);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI1);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //
    // Enable port PA1 for GPIOInput
    //
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_1);
    MAP_GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //
    // Enable port PA2 for GPIOInput
    //
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_2);
    MAP_GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //
    // Enable port PA4 for GPIOInput
    //
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_4);
    MAP_GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //
    // Enable port PA7 for GPIOInput
    //
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_7);
    MAP_GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_7, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //
    // Enable port PA6 for GPIOInput
    //
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_6);
    MAP_GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_6, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //
    // Enable port PA5 for GPIOInput
    //
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_5);
    MAP_GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_5, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //
    // Enable port PA0 for GPIOInput
    //
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_0);
    MAP_GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //
    // Enable port PA3 for GPIOInput
    //
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_3);
    MAP_GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //
    // Enable port PB5 for GPIOInput
    //
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_5);
    MAP_GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_5, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //
    // Enable port PB2 for GPIOInput
    //
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_2);
    MAP_GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //
    // Enable port PB7 for GPIOInput
    //
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_7);
    MAP_GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_7, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //
    // Enable port PB4 for GPIOInput
    //
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_4);
    MAP_GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //
    // Enable port PB3 for GPIOInput
    //
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_3);
    MAP_GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //
    // Enable port PB1 for GPIOInput
    //
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_1);
    MAP_GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //
    // Enable port PB6 for GPIOInput
    //
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_6);
    MAP_GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_6, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //
    // Enable port PB0 for GPIOInput
    //
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_0);
    MAP_GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //
    // Enable port PC1 for GPIOInput
    //

    //
    //First open the lock and select the bits we want to modify in the GPIO commit register.
    //
    HWREG(GPIO_PORTC_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTC_BASE + GPIO_O_CR) = 0x2;

    //
    //Now modify the configuration of the pins that we unlocked.
    //
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_1);
    MAP_GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //
    // Enable port PC3 for GPIOInput
    //

    //
    //First open the lock and select the bits we want to modify in the GPIO commit register.
    //
    HWREG(GPIO_PORTC_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTC_BASE + GPIO_O_CR) = 0x8;

    //
    //Now modify the configuration of the pins that we unlocked.
    //
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_3);
    MAP_GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //
    // Enable port PC0 for GPIOInput
    //

    //
    //First open the lock and select the bits we want to modify in the GPIO commit register.
    //
    HWREG(GPIO_PORTC_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTC_BASE + GPIO_O_CR) = 0x1;

    //
    //Now modify the configuration of the pins that we unlocked.
    //
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_0);
    MAP_GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //
    // Enable port PC2 for GPIOInput
    //

    //
    //First open the lock and select the bits we want to modify in the GPIO commit register.
    //
    HWREG(GPIO_PORTC_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTC_BASE + GPIO_O_CR) = 0x4;

    //
    //Now modify the configuration of the pins that we unlocked.
    //
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_2);
    MAP_GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //
    // Enable port PD3 for GPIOInput
    //
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_3);
    MAP_GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //
    // Enable port PD1 for GPIOInput
    //
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_1);
    MAP_GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //
    // Enable port PD2 for GPIOInput
    //
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_2);
    MAP_GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //
    // Enable port PD0 for GPIOInput
    //
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_0);
    MAP_GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //
    // Enable port PE1 for GPIOInput
    //
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_1);
    MAP_GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //
    // Enable port PE2 for GPIOInput
    //
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_2);
    MAP_GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //
    // Enable port PE5 for GPIOInput
    //
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_5);
    MAP_GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_5, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //
    // Enable port PE3 for GPIOInput
    //
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_3);
    MAP_GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //
    // Enable port PE0 for GPIOInput
    //
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_0);
    MAP_GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    //
    // Enable port PE4 for GPIOInput
    //
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_4);
    MAP_GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // Enable Left Button
    MAP_GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_4);
    MAP_GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // Enable Output Status Light
    MAP_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    //
    // Enable port PD6 for QEI0 PHA0
    //
    MAP_GPIOPinConfigure(GPIO_PD6_PHA0);
    MAP_GPIOPinTypeQEI(GPIO_PORTD_BASE, GPIO_PIN_6);

    //
    // Enable port PD7 for QEI0 PHB0
    // First open the lock and select the bits we want to modify in the GPIO commit register.
    //
    HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTD_BASE + GPIO_O_CR) = 0x80;

    //
    // Now modify the configuration of the pins that we unlocked.
    //
    MAP_GPIOPinConfigure(GPIO_PD7_PHB0);
    MAP_GPIOPinTypeQEI(GPIO_PORTD_BASE, GPIO_PIN_7);

    //
    // Enable port PC5 for QEI1 PHA1
    //
    MAP_GPIOPinConfigure(GPIO_PC5_PHA1);
    MAP_GPIOPinTypeQEI(GPIO_PORTC_BASE, GPIO_PIN_5);

    //
    // Enable port PC6 for QEI1 PHB1
    //
    MAP_GPIOPinConfigure(GPIO_PC6_PHB1);
    MAP_GPIOPinTypeQEI(GPIO_PORTC_BASE, GPIO_PIN_6);

    //
    // Enable port PD5 for USB0 USB0DP
    //
    MAP_GPIOPinTypeUSBAnalog(GPIO_PORTD_BASE, GPIO_PIN_5);

    //
    // Enable port PD4 for USB0 USB0DM
    //
    MAP_GPIOPinTypeUSBAnalog(GPIO_PORTD_BASE, GPIO_PIN_4);
}
