//*****************************************************************************
//
// usbdhidmame.h - Public header file for the USB HID CustomHid device class
//                  driver
//
// Copyright (c) 2008-2013 Texas Instruments Incorporated.  All rights reserved.
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
// Jeff Lawrence made up generic example based on TI mouse file
//
//*****************************************************************************

#ifndef __USBDHIDCUSTOMHID_H__
#define __USBDHIDCUSTOMHID_H__

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
//! \addtogroup hid_customhid_device_class_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// PRIVATE
//
// The first few sections of this header are private defines that are used by
// the USB HID customhid code and are here only to help with the application
// allocating the correct amount of memory for the HID customhid device code.
//
//*****************************************************************************

//*****************************************************************************
//
// PRIVATE
//
// The size of the customhid input report sent to the host.
//
//*****************************************************************************
#define CUSTOMHID_REPORT_SIZE       4

//*****************************************************************************
//
// PRIVATE
//
// This enumeration holds the various states that the customhid can be in during
// normal operation.
//
//*****************************************************************************
typedef enum
{
    //
    // Unconfigured.
    //
    eHIDCustomHidStateUnconfigured,

    //
    // No keys to send and not waiting on data.
    //
    eHIDCustomHidStateIdle,

    //
    // Waiting on report data from the host.
    //
    eHIDCustomHidStateWaitData,

    //
    // Waiting on data to be sent out.
    //
    eHIDCustomHidStateSend
}
tCustomHidState;

//*****************************************************************************
//
// PRIVATE
//
// This structure provides the private instance data structure for the USB
// HID CustomHid device.  This structure forms the RAM workspace used by each
// instance of the customhid.
//
//*****************************************************************************
typedef struct
{
    //
    // The USB configuration number set by the host or 0 of the device is
    // currently unconfigured.
    //
    uint8_t ui8USBConfigured;

    //
    // The protocol requested by the host, USB_HID_PROTOCOL_BOOT or
    // USB_HID_PROTOCOL_REPORT.
    //
    uint8_t ui8Protocol;

    //
    // A buffer used to hold the last input report sent to the host.
    //
    uint8_t pui8Report[CUSTOMHID_REPORT_SIZE];

    //
    // The current state of the customhid interrupt IN endpoint.
    //
    volatile tCustomHidState iCustomHidState;

    //
    // The idle timeout control structure for our input report.  This is
    // required by the lower level HID driver.
    //
    tHIDReportIdle sReportIdle;

    //
    // This is needed for the lower level HID driver.
    //
    tUSBDHIDDevice sHIDDevice;
}
tHIDCustomHidInstance;

//*****************************************************************************
//
//! This structure is used by the application to define operating parameters
//! for the HID customhid device.
//
//*****************************************************************************
typedef struct
{
    //
    //! The vendor ID that this device is to present in the device descriptor.
    //
    const uint16_t ui16VID;

    //
    //! The product ID that this device is to present in the device descriptor.
    //
    const uint16_t ui16PID;

    //
    //! The maximum power consumption of the device, expressed in milliamps.
    //
    const uint16_t ui16MaxPowermA;

    //
    //! Indicates whether the device is self- or bus-powered and whether or not
    //! it supports remote wakeup.  Valid values are USB_CONF_ATTR_SELF_PWR or
    //! USB_CONF_ATTR_BUS_PWR, optionally ORed with USB_CONF_ATTR_RWAKE.
    //
    const uint8_t ui8PwrAttributes;

    //
    //! A pointer to the callback function which will be called to notify
    //! the application of events relating to the operation of the customhid.
    //
    const tUSBCallback pfnCallback;

    //
    //! A client-supplied pointer which will be sent as the first
    //! parameter in all calls made to the customhid callback, pfnCallback.
    //
    void *pvCBData;

    //
    //! A pointer to the string descriptor array for this device.  This array
    //! must contain the following string descriptor pointers in this order.
    //! Language descriptor, Manufacturer name string (language 1), Product
    //! name string (language 1), Serial number string (language 1),HID
    //! Interface description string (language 1), Configuration description
    //! string (language 1).
    //!
    //! If supporting more than 1 language, the descriptor block (except for
    //! string descriptor 0) must be repeated for each language defined in the
    //! language descriptor.
    //
    const uint8_t * const *ppui8StringDescriptors;

    //
    //! The number of descriptors provided in the ppStringDescriptors
    //! array.  This must be (1 + (5 * (num languages))).
    //
    const uint32_t ui32NumStringDescriptors;

    //
    //! The private instance data for this device.  This memory must
    //! remain accessible for as long as the customhid device is in use and must
    //! not be modified by any code outside the HID customhid driver.
    //
    tHIDCustomHidInstance sPrivateData;
}
tUSBDHIDCustomHidDevice;

//*****************************************************************************
//
//! This return code from USBDHIDCustomHidStateChange indicates success.
//
//*****************************************************************************
#define CUSTOMHID_SUCCESS           0

//*****************************************************************************
//
//! This return code from USBDHIDCustomHidStateChange indicates that an error was
//! reported while attempting to send a report to the host.  A client should
//! assume that the host has disconnected if this return code is seen.
//
//*****************************************************************************
#define CUSTOMHID_ERR_TX_ERROR      2

//*****************************************************************************
//
//! USBDHIDCustomHidStateChange returns this value if it is called before the
//! USB host has connected and configured the device.  All customhid state
//! information passed on the call will have been ignored.
//
//*****************************************************************************
#define CUSTOMHID_ERR_NOT_CONFIGURED \
                                4

//*****************************************************************************
//
// API Function Prototypes
//
//*****************************************************************************
extern void *USBDHIDCustomHidInit(uint32_t ui32Index,
                              tUSBDHIDCustomHidDevice *psCustomHidDevice);
extern void *USBDHIDCustomHidCompositeInit(uint32_t ui32Index,
                                     tUSBDHIDCustomHidDevice *psCustomHidDevice,
                                     tCompositeEntry *psCompEntry);
extern void USBDHIDCustomHidTerm(void *pvCustomHidDevice);
extern void *USBDHIDCustomHidSetCBData(void *pvCustomHidDevice, void *pvCBData);
extern uint32_t USBDHIDCustomHidStateChange(void *pvCustomHidDevice, uint8_t ReportID, signed char HIDData[]);
extern void USBDHIDCustomHidPowerStatusSet(void *pvCustomHidDevice,
                                       uint8_t ui8Power);
extern bool USBDHIDCustomHidRemoteWakeupRequest(void *pvCustomHidDevice);

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __USBDHIDCUSTOMHID_H__
