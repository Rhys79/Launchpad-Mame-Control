//*****************************************************************************
//
// usbdhidcustomhid.c - USB HID CustomHid device class driver.
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
// Jeff Lawrence made up generic example based on TI mouse file.
// 8/28/2013 - Custom HID Mame panel - John Byers
//
//*****************************************************************************

#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/usb.h"
#include "usblib/usblib.h"
#include "usblib/usblibpriv.h"
#include "usblib/device/usbdevice.h"
#include "usblib/usbhid.h"
#include "usblib/device/usbdhid.h"
#include "usblib/device/usbdhidmame.h"

//*****************************************************************************
//
//! \addtogroup hid_customhid_device_class_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
// HID device configuration descriptor.
//
// It is vital that the configuration descriptor bConfigurationValue field
// (byte 6) is 1 for the first configuration and increments by 1 for each
// additional configuration defined here.  This relationship is assumed in the
// device stack for simplicity even though the USB 2.0 specification imposes
// no such restriction on the bConfigurationValue values.
//
// Note that this structure is deliberately located in RAM since we need to
// be able to patch some values in it based on client requirements.
//
//*****************************************************************************
uint8_t g_pui8CustomHidDescriptor[] =
{
    //
    // Configuration descriptor header.
    //
    9,                          // Size of the configuration descriptor.
    USB_DTYPE_CONFIGURATION,    // Type of this descriptor.
    USBShort(34),               // The total size of this full structure.
    1,                          // The number of interfaces in this
                                // configuration.
    1,                          // The unique value for this configuration.
    5,                          // The string identifier that describes this
                                // configuration.
    USB_CONF_ATTR_SELF_PWR,     // Bus Powered, Self Powered, remote wake up.
    250,                        // The maximum power in 2mA increments.
};

//*****************************************************************************
//
// The remainder of the configuration descriptor is stored in flash since we
// don't need to modify anything in it at runtime.
//
//*****************************************************************************
uint8_t g_pui8HIDInterface[HIDINTERFACE_SIZE] =
{
    //
    // HID Device Class Interface Descriptor.
    //
    9,                          // Size of the interface descriptor.
    USB_DTYPE_INTERFACE,        // Type of this descriptor.
    0,                          // The index for this interface.
    0,                          // The alternate setting for this interface.
    1,                          // The number of endpoints used by this
                                // interface.
    USB_CLASS_HID,              // The interface class
    USB_HID_SCLASS_NONE,        // The interface sub-class.
    USB_HID_PROTOCOL_NONE,     // The interface protocol for the sub-class
                                // specified above.
    4,                          // The string index for this interface.
};

const uint8_t g_pui8HIDInEndpoint[HIDINENDPOINT_SIZE] =
{
    //
    // Interrupt IN endpoint descriptor
    //
    7,                          // The size of the endpoint descriptor.
    USB_DTYPE_ENDPOINT,         // Descriptor type is an endpoint.
    USB_EP_DESC_IN | USBEPToIndex(USB_EP_1),
    USB_EP_ATTR_INT,            // Endpoint is an interrupt endpoint.
    USBShort(USBFIFOSizeToBytes(USB_FIFO_SZ_64)),
                                // The maximum packet size.
    1,                         // The polling interval for this endpoint.
};

//*****************************************************************************
//
// The report descriptor for the Mame class device. (Built off CustomHid example)
//
//*****************************************************************************
static const uint8_t g_pui8CustomHidReportDescriptor[] =
{
		UsagePage(USB_HID_GENERIC_DESKTOP),
		    Usage(USB_HID_GAMEPAD),
		    Collection(USB_HID_APPLICATION),
		        Collection(USB_HID_PHYSICAL),

		    	ReportID(1),
				//
				// The X and Y (Will appear as two thumb controls).
				//
				UsagePage(USB_HID_GENERIC_DESKTOP),
				Usage(USB_HID_X),
				Usage(USB_HID_Y),
				LogicalMinimum(-1),
				LogicalMaximum(1),
				ReportSize(2),
				ReportCount(2),
				Input(USB_HID_INPUT_DATA | USB_HID_INPUT_VARIABLE | USB_HID_INPUT_ABS | USB_HID_INPUT_NONULL),

				ReportCount(4),
				ReportSize(1),
				Input(USB_HID_INPUT_CONSTANT | USB_HID_INPUT_ARRAY | USB_HID_INPUT_ABS),

				//
				// 12 - 1 bit values for the first set of buttons.
				//
				UsagePage(USB_HID_BUTTONS),
				UsageMinimum(1),
				UsageMaximum(12),
				LogicalMinimum(0),
				LogicalMaximum(1),
				ReportSize(1),
				ReportCount(12),
				Input(USB_HID_INPUT_DATA | USB_HID_INPUT_VARIABLE | USB_HID_INPUT_ABS),

				//
				// 1 - 4 bit unused constant value to fill the second 8 bits.
				//
				ReportSize(4),
				ReportCount(1),
				Input(USB_HID_INPUT_CONSTANT | USB_HID_INPUT_ARRAY | USB_HID_INPUT_ABS),

		        EndCollection,
		    EndCollection,

		    UsagePage(USB_HID_GENERIC_DESKTOP),
		        Usage(USB_HID_GAMEPAD),
		        Collection(USB_HID_APPLICATION),
		            Collection(USB_HID_PHYSICAL),

		        	ReportID(2),
		    		//
		    		// The X, Y Z and Rx (Will appear as two thumb controls.
		    		//
		    		UsagePage(USB_HID_GENERIC_DESKTOP),
		    		Usage(USB_HID_X),
		    		Usage(USB_HID_Y),
		    		LogicalMinimum(-1),
		    		LogicalMaximum(1),
		    		ReportSize(2),
		    		ReportCount(2),
		    		Input(USB_HID_INPUT_DATA | USB_HID_INPUT_VARIABLE | USB_HID_INPUT_ABS | USB_HID_INPUT_NONULL),

		    		ReportCount(4),
		    		ReportSize(1),
		    		Input(USB_HID_INPUT_CONSTANT | USB_HID_INPUT_ARRAY | USB_HID_INPUT_ABS),

		    		//
		    		// 8 - 1 bit values for the first set of buttons.
		    		//
		    		UsagePage(USB_HID_BUTTONS),
		    		UsageMinimum(1),
		    		UsageMaximum(8),
		    		LogicalMinimum(0),
		    		LogicalMaximum(1),
		    		ReportSize(1),
		    		ReportCount(8),
		    		Input(USB_HID_INPUT_DATA | USB_HID_INPUT_VARIABLE | USB_HID_INPUT_ABS),

		            EndCollection,
		        EndCollection,

		        UsagePage(USB_HID_GENERIC_DESKTOP),
					Usage(USB_HID_MOUSE),
					Collection(USB_HID_APPLICATION),
						Usage(USB_HID_POINTER),
						Collection(USB_HID_PHYSICAL),

							ReportID(3),
							//
							// The buttons.
							//
							UsagePage(USB_HID_BUTTONS),
							UsageMinimum(1),
							UsageMaximum(2),
							LogicalMinimum(0),
							LogicalMaximum(1),

							//
							// 2 - 1 bit values for the buttons.
							//
							ReportSize(1),
							ReportCount(2),
							Input(USB_HID_INPUT_DATA | USB_HID_INPUT_VARIABLE | USB_HID_INPUT_ABS),

							//
							// 1 - 6 bit unused constant value to fill the 8 bits.
							//
							ReportSize(6),
							ReportCount(1),
							Input(USB_HID_INPUT_CONSTANT | USB_HID_INPUT_ARRAY | USB_HID_INPUT_ABS),

							//
							// The X and Y axis.
							//
							UsagePage(USB_HID_GENERIC_DESKTOP),
							Usage(USB_HID_X),
							Usage(USB_HID_Y),
							LogicalMinimum(-127),
							LogicalMaximum(127),

							//
							// 2 - 8 bit Values for x and y.
							//
							ReportSize(8),
							ReportCount(2),
							Input(USB_HID_INPUT_DATA | USB_HID_INPUT_VARIABLE | USB_HID_INPUT_RELATIVE),


						EndCollection,
					EndCollection,

};

//*****************************************************************************
//
// The HID descriptor for the customhid device.
//
//*****************************************************************************
static const tHIDDescriptor g_sCustomHidHIDDescriptor =
{
    9,                              // bLength
    USB_HID_DTYPE_HID,              // bDescriptorType
    0x111,                          // bcdHID (version 1.11 compliant)
    0,                              // bCountryCode (not localized)
    1,                              // bNumDescriptors
    {
        {
            USB_HID_DTYPE_REPORT,   // Report descriptor
            sizeof(g_pui8CustomHidReportDescriptor)
                                    // Size of report descriptor
        }
    }
};

//*****************************************************************************
//
// The HID configuration descriptor is defined as four or five sections
// depending upon the client's configuration choice.  These sections are:
//
// 1.  The 9 byte configuration descriptor (RAM).
// 2.  The interface descriptor (RAM).
// 3.  The HID report and physical descriptors (provided by the client)
//     (FLASH).
// 4.  The mandatory interrupt IN endpoint descriptor (FLASH).
// 5.  The optional interrupt OUT endpoint descriptor (FLASH).
//
//*****************************************************************************
const tConfigSection g_sHIDConfigSection =
{
    sizeof(g_pui8CustomHidDescriptor),
    g_pui8CustomHidDescriptor
};

const tConfigSection g_sHIDInterfaceSection =
{
    sizeof(g_pui8HIDInterface),
    g_pui8HIDInterface
};

const tConfigSection g_sHIDInEndpointSection =
{
    sizeof(g_pui8HIDInEndpoint),
    g_pui8HIDInEndpoint
};

//*****************************************************************************
//
// Place holder for the user's HID descriptor block.
//
//*****************************************************************************
tConfigSection g_sHIDDescriptorSection =
{
   sizeof(g_sCustomHidHIDDescriptor),
   (const uint8_t *)&g_sCustomHidHIDDescriptor
};

//*****************************************************************************
//
// This array lists all the sections that must be concatenated to make a
// single, complete HID configuration descriptor.
//
//*****************************************************************************
const tConfigSection *g_psHIDSections[] =
{
    &g_sHIDConfigSection,
    &g_sHIDInterfaceSection,
    &g_sHIDDescriptorSection,
    &g_sHIDInEndpointSection,
};

#define NUM_HID_SECTIONS        (sizeof(g_psHIDSections) /                    \
                                 sizeof(g_psHIDSections[0]))

//*****************************************************************************
//
// The header for the single configuration we support.  This is the root of
// the data structure that defines all the bits and pieces that are pulled
// together to generate the configuration descriptor.  Note that this must be
// in RAM since we need to include or exclude the final section based on
// client supplied initialization parameters.
//
//*****************************************************************************
tConfigHeader g_sHIDConfigHeader =
{
    NUM_HID_SECTIONS,
    g_psHIDSections
};

//*****************************************************************************
//
// Configuration Descriptor.
//
//*****************************************************************************
const tConfigHeader * const g_ppsHIDConfigDescriptors[] =
{
    &g_sHIDConfigHeader
};

//*****************************************************************************
//
// The HID class descriptor table.  For the customhid class, we have only a single
// report descriptor.
//
//*****************************************************************************
static const uint8_t * const g_pui8CustomHidClassDescriptors[] =
{
    g_pui8CustomHidReportDescriptor
};

//*****************************************************************************
//
// Forward references for customhid device callback functions.
//
//*****************************************************************************
static uint32_t HIDCustomHidRxHandler(void *pvCustomHidDevice, uint32_t ui32Event,
                                  uint32_t ui32MsgData, void *pvMsgData);
static uint32_t HIDCustomHidTxHandler(void *pvCustomHidDevice, uint32_t ui32Event,
                                  uint32_t ui32MsgData, void *pvMsgData);


//*****************************************************************************
//
// Main HID device class event handler function.
//
// \param pvCustomHidDevice is the event callback pointer provided during
// USBDHIDInit().  This is a pointer to our HID device structure
// (&g_sHIDCustomHidDevice).
// \param ui32Event identifies the event we are being called back for.
// \param ui32MsgData is an event-specific value.
// \param pvMsgData is an event-specific pointer.
//
// This function is called by the HID device class driver to inform the
// application of particular asynchronous events related to operation of the
// customhid HID device.
//
// \return Returns a value which is event-specific.
//
//*****************************************************************************
static uint32_t
HIDCustomHidRxHandler(void *pvCustomHidDevice, uint32_t ui32Event,
                  uint32_t ui32MsgData, void *pvMsgData)
{
    tHIDCustomHidInstance *psInst;
    tUSBDHIDCustomHidDevice *psCustomHidDevice;

    //
    // Make sure we did not get a NULL pointer.
    //
    ASSERT(pvCustomHidDevice);

    //
    // Get a pointer to our instance data
    //
    psCustomHidDevice = (tUSBDHIDCustomHidDevice *)pvCustomHidDevice;
    psInst = &psCustomHidDevice->sPrivateData;

    //
    // Which event were we sent?
    //
    switch(ui32Event)
    {
        //
        // The host has connected to us and configured the device.
        //
        case USB_EVENT_CONNECTED:
        {
            psInst->ui8USBConfigured = true;

            //
            // Pass the information on to the client.
            //
            psCustomHidDevice->pfnCallback(psCustomHidDevice->pvCBData,
                                       USB_EVENT_CONNECTED, 0, (void *)0);

            break;
        }

        //
        // The host has disconnected from us.
        //
        case USB_EVENT_DISCONNECTED:
        {
            psInst->ui8USBConfigured = false;

            //
            // Pass the information on to the client.
            //
            psCustomHidDevice->pfnCallback(psCustomHidDevice->pvCBData,
                                       USB_EVENT_DISCONNECTED, 0, (void *)0);

            break;
        }

        //
        // The host is polling us for a particular report and the HID driver
        // is asking for the latest version to transmit.
        //
        case USBD_HID_EVENT_IDLE_TIMEOUT:
        case USBD_HID_EVENT_GET_REPORT:
        {
            //
            // We only support a single input report so we don't need to check
            // the ui32MsgValue parameter in this case.  Set the report pointer
            // in *pvMsgData and return the length of the report in bytes.
            //
            *(uint8_t **)pvMsgData = psInst->pui8Report;
            return(8);
        }

        //
        // The device class driver has completed sending a report to the
        // host in response to a Get_Report request.
        //
        case USBD_HID_EVENT_REPORT_SENT:
        {
            //
            // We have nothing to do here.
            //
            break;
        }

        //
        // This event is sent in response to a host Set_Report request.  The
        // customhid device has no output reports so we return a NULL pointer and
        // zero length to cause this request to be stalled.
        //
        case USBD_HID_EVENT_GET_REPORT_BUFFER:
        {
            //
            // We are being asked for a report that does not exist for
            // this device.  Return 0 to indicate that we are not providing
            // a buffer.
            //
            return(0);
        }

        //
        // The host is asking us to set either boot or report protocol (not
        // that it makes any difference to this particular customhid).
        //
        case USBD_HID_EVENT_SET_PROTOCOL:
        {
            psInst->ui8Protocol = ui32MsgData;
            break;
        }

        //
        // The host is asking us to tell it which protocol we are currently
        // using, boot or request.
        //
        case USBD_HID_EVENT_GET_PROTOCOL:
        {
            return(psInst->ui8Protocol);
        }

        //
        // Pass ERROR, SUSPEND and RESUME to the client unchanged.
        //
        case USB_EVENT_ERROR:
        case USB_EVENT_SUSPEND:
        case USB_EVENT_RESUME:
        {
            return(psCustomHidDevice->pfnCallback(psCustomHidDevice->pvCBData,
                                              ui32Event, ui32MsgData,
                                              pvMsgData));
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
// HID device class transmit channel event handler function.
//
// \param pvCustomHidDevice is the event callback pointer provided during
// USBDHIDInit(). This is a pointer to our HID device structure
// (&g_sHIDCustomHidDevice).
// \param ui32Event identifies the event we are being called back for.
// \param ui32MsgData is an event-specific value.
// \param pvMsgData is an event-specific pointer.
//
// This function is called by the HID device class driver to inform the
// application of particular asynchronous events related to report
// transmissions made using the interrupt IN endpoint.
//
// \return Returns a value which is event-specific.
//
//*****************************************************************************
static uint32_t
HIDCustomHidTxHandler(void *pvCustomHidDevice, uint32_t ui32Event,
                  uint32_t ui32MsgData, void *pvMsgData)
{
    tHIDCustomHidInstance *psInst;
    tUSBDHIDCustomHidDevice *psCustomHidDevice;

    //
    // Make sure we did not get a NULL pointer.
    //
    ASSERT(pvCustomHidDevice);

    //
    // Get a pointer to our instance data
    //
    psCustomHidDevice = (tUSBDHIDCustomHidDevice *)pvCustomHidDevice;
    psInst = &psCustomHidDevice->sPrivateData;

    //
    // Which event were we sent?
    //
    switch (ui32Event)
    {
        //
        // A report transmitted via the interrupt IN endpoint was acknowledged
        // by the host.
        //
        case USB_EVENT_TX_COMPLETE:
        {
            //
            // Our last transmission is complete.
            //
            psInst->iCustomHidState = eHIDCustomHidStateIdle;

            //
            // Pass the event on to the client.
            //
            psCustomHidDevice->pfnCallback(psCustomHidDevice->pvCBData,
                                       USB_EVENT_TX_COMPLETE, ui32MsgData,
                                       (void *)0);

            break;
        }

        //
        // We ignore all other events related to transmission of reports via
        // the interrupt IN endpoint.
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
//! Initializes HID customhid device operation for a given USB controller.
//!
//! \param ui32Index is the index of the USB controller which is to be
//! initialized for HID customhid device operation.
//! \param psCustomHidDevice points to a structure containing parameters
//! customizing the operation of the HID customhid device.
//!
//! An application wishing to offer a USB HID customhid interface to a USB host
//! must call this function to initialize the USB controller and attach the
//! customhid device to the USB bus.  This function performs all required USB
//! initialization.
//!
//! On successful completion, this function will return the \e psCustomHidDevice
//! pointer passed to it.  This must be passed on all future calls to the HID
//! customhid device driver.
//!
//! When a host connects and configures the device, the application callback
//! will receive \b USB_EVENT_CONNECTED after which calls can be made to
//! USBDHIDCustomHidStateChange() to report pointer movement and button presses
//! to the host.
//!
//! \note The application must not make any calls to the lower level USB device
//! interfaces if interacting with USB via the USB HID customhid device API.
//! Doing so will cause unpredictable (though almost certainly unpleasant)
//! behavior.
//!
//! \return Returns NULL on failure or the psCustomHidDevice pointer on success.
//
//*****************************************************************************
void *
USBDHIDCustomHidInit(uint32_t ui32Index, tUSBDHIDCustomHidDevice *psCustomHidDevice)
{
    void *pvRetcode;
    tUSBDHIDDevice *psHIDDevice;
    tConfigDescriptor *pConfigDesc;

    //
    // Check parameter validity.
    //
    ASSERT(psCustomHidDevice);
    ASSERT(psCustomHidDevice->ppui8StringDescriptors);
    ASSERT(psCustomHidDevice->pfnCallback);

    //
    // Get a pointer to the HID device data.
    //
    psHIDDevice = &psCustomHidDevice->sPrivateData.sHIDDevice;

    pConfigDesc = (tConfigDescriptor *)g_pui8CustomHidDescriptor;
    pConfigDesc->bmAttributes = psCustomHidDevice->ui8PwrAttributes;
    pConfigDesc->bMaxPower =  (uint8_t)(psCustomHidDevice->ui16MaxPowermA / 2);

    //
    // Call the common initialization routine.
    //
    pvRetcode = USBDHIDCustomHidCompositeInit(ui32Index, psCustomHidDevice, 0);

    //
    // If we initialized the HID layer successfully, pass our device pointer
    // back as the return code, otherwise return NULL to indicate an error.
    //
    if(pvRetcode)
    {
        //
        // Initialize the lower layer HID driver and pass it the various
        // structures and descriptors necessary to declare that we are a
        // keyboard.
        //
        pvRetcode = USBDHIDInit(ui32Index, psHIDDevice);

        return((void *)psCustomHidDevice);
    }
    else
    {
        return((void *)0);
    }
}

//*****************************************************************************
//
//! Initializes HID customhid device operation for a given USB controller.
//!
//! \param ui32Index is the index of the USB controller which is to be
//! initialized for HID customhid device operation.
//! \param psCustomHidDevice points to a structure containing parameters
//! customizing the operation of the HID customhid device.
//! \param psCompEntry is the composite device entry to initialize when
//! creating a composite device.
//!
//! This call is very similar to USBDHIDCustomHidInit() except that it is used for
//! initializing an instance of the HID customhid device for use in a composite
//! device.  If this HID customhid is part of a composite device, then the
//! \e psCompEntry should point to the composite device entry to initialize.
//! This is part of the array that is passed to the USBDCompositeInit()
//! function.
//!
//! \return Returns zero on failure or a non-zero instance value that should be
//! used with the remaining USB HID CustomHid APIs.
//
//*****************************************************************************
void *
USBDHIDCustomHidCompositeInit(uint32_t ui32Index,
                          tUSBDHIDCustomHidDevice *psCustomHidDevice,
                          tCompositeEntry *psCompEntry)
{
    tHIDCustomHidInstance *psInst;
    tUSBDHIDDevice *psHIDDevice;


    //
    // Check parameter validity.
    //
    ASSERT(psCustomHidDevice);
    ASSERT(psCustomHidDevice->ppui8StringDescriptors);
    ASSERT(psCustomHidDevice->pfnCallback);

    //
    // Get a pointer to our instance data
    //
    psInst = &psCustomHidDevice->sPrivateData;

    //
    // Get a pointer to the HID device data.
    //
    psHIDDevice = &psCustomHidDevice->sPrivateData.sHIDDevice;

    //
    // Initialize the various fields in our instance structure.
    //
    psInst->ui8USBConfigured = 0;
    psInst->ui8Protocol = USB_HID_PROTOCOL_REPORT;
    psInst->sReportIdle.ui8Duration4mS = 0;
    psInst->sReportIdle.ui8ReportID = 0;
    psInst->sReportIdle.ui32TimeSinceReportmS = 0;
    psInst->sReportIdle.ui16TimeTillNextmS = 0;
    psInst->iCustomHidState = eHIDCustomHidStateUnconfigured;

    //
    // Initialize the HID device class instance structure based on input from
    // the caller.
    //
    psHIDDevice->ui16PID = psCustomHidDevice->ui16PID;
    psHIDDevice->ui16VID = psCustomHidDevice->ui16VID;
    psHIDDevice->ui16MaxPowermA = psCustomHidDevice->ui16MaxPowermA;
    psHIDDevice->ui8PwrAttributes = psCustomHidDevice->ui8PwrAttributes;
    psHIDDevice->ui8Subclass = USB_HID_SCLASS_NONE;
    psHIDDevice->ui8Protocol = USB_HID_PROTOCOL_NONE;
    psHIDDevice->ui8NumInputReports = 1;
    psHIDDevice->psReportIdle = &psInst->sReportIdle;
    psHIDDevice->pfnRxCallback = HIDCustomHidRxHandler;
    psHIDDevice->pvRxCBData = (void *)psCustomHidDevice;
    psHIDDevice->pfnTxCallback = HIDCustomHidTxHandler;
    psHIDDevice->pvTxCBData = (void *)psCustomHidDevice;
    psHIDDevice->bUseOutEndpoint = false;
    psHIDDevice->psHIDDescriptor = &g_sCustomHidHIDDescriptor;
    psHIDDevice->ppui8ClassDescriptors = g_pui8CustomHidClassDescriptors;
    psHIDDevice->ppui8StringDescriptors =
                                    psCustomHidDevice->ppui8StringDescriptors;
    psHIDDevice->ui32NumStringDescriptors =
                                    psCustomHidDevice->ui32NumStringDescriptors;
    psHIDDevice->ppsConfigDescriptor = g_ppsHIDConfigDescriptors;

    //
    // Initialize the lower layer HID driver and pass it the various structures
    // and descriptors necessary to declare that we are a keyboard.
    //
    return(USBDHIDCompositeInit(ui32Index, psHIDDevice, psCompEntry));
}

//*****************************************************************************
//
//! Shuts down the HID customhid device.
//!
//! \param pvCustomHidDevice is the pointer to the device instance structure.
//!
//! This function terminates HID customhid operation for the instance supplied
//! and removes the device from the USB bus.  Following this call, the
//! \e pvCustomHidDevice instance may not me used in any other call to the HID
//! customhid device other than USBDHIDCustomHidInit().
//!
//! \return None.
//
//*****************************************************************************
void
USBDHIDCustomHidTerm(void *pvCustomHidDevice)
{
    tUSBDHIDCustomHidDevice *psCustomHidDevice;
    tUSBDHIDDevice *psHIDDevice;

    ASSERT(pvCustomHidDevice);

    //
    // Get a pointer to the device.
    //
    psCustomHidDevice = (tUSBDHIDCustomHidDevice *)pvCustomHidDevice;

    //
    // Get a pointer to the HID device data.
    //
    psHIDDevice = &psCustomHidDevice->sPrivateData.sHIDDevice;

    //
    // Mark our device as no longer configured.
    //
    psCustomHidDevice->sPrivateData.ui8USBConfigured = 0;

    //
    // Terminate the low level HID driver.
    //
    USBDHIDTerm(psHIDDevice);
}

//*****************************************************************************
//
//! Sets the client-specific pointer parameter for the customhid callback.
//!
//! \param pvCustomHidDevice is the pointer to the customhid device instance structure.
//! \param pvCBData is the pointer that client wishes to be provided on
//! each event sent to the customhid callback function.
//!
//! The client uses this function to change the callback pointer passed in
//! the first parameter on all callbacks to the \e pfnCallback function
//! passed on USBDHIDCustomHidInit().
//!
//! If a client wants to make runtime changes in the callback pointer, it must
//! ensure that the pvCustomHidDevice structure passed to USBDHIDCustomHidInit()
//! resides in RAM.  If this structure is in flash, callback data changes are
//! not possible.
//!
//! \return Returns the previous callback pointer that was set for this
//! instance.
//
//*****************************************************************************
void *
USBDHIDCustomHidSetCBData(void *pvCustomHidDevice, void *pvCBData)
{
    void *pvOldCBData;
    tUSBDHIDCustomHidDevice *psCustomHid;

    //
    // Check for a NULL pointer in the device parameter.
    //
    ASSERT(pvCustomHidDevice);

    //
    // Get a pointer to our customhid device.
    //
    psCustomHid = (tUSBDHIDCustomHidDevice *)pvCustomHidDevice;

    //
    // Save the old callback pointer and replace it with the new value.
    //
    pvOldCBData = psCustomHid->pvCBData;
    psCustomHid->pvCBData = pvCBData;

    //
    // Pass the old callback pointer back to the caller.
    //
    return(pvOldCBData);
}

//*****************************************************************************
//
// Reports a customhid state change, pointer movement or button press
//
//*****************************************************************************
uint32_t
USBDHIDCustomHidStateChange(void *pvCustomHidDevice, uint8_t ReportID, signed char HIDData[])
{
    uint32_t ui32Retcode, ui32Count;
    uint8_t i;
    tHIDCustomHidInstance *psInst;
    tUSBDHIDCustomHidDevice *psCustomHidDevice;
    tUSBDHIDDevice *psHIDDevice;

    //
    // Get a pointer to the device.
    //
    psCustomHidDevice = (tUSBDHIDCustomHidDevice *)pvCustomHidDevice;

    //
    // Get a pointer to the HID device data.
    //
    psHIDDevice = &psCustomHidDevice->sPrivateData.sHIDDevice;

    //
    // Get a pointer to our instance data
    //
    psInst = &psCustomHidDevice->sPrivateData;

	if (ReportID > 0)
	{
		psInst->pui8Report[0] = ReportID; //ReportID

		for(i = 1; i < CUSTOMHID_REPORT_SIZE; i++)
		{
			psInst->pui8Report[i] = HIDData[i-1];; //Data is offset by 1 due to the ReportID
		}
	}
	else
	{
		for(i = 0; i < CUSTOMHID_REPORT_SIZE; i++)
		{
			psInst->pui8Report[i] = HIDData[i];; //No ReportID so no offset
		}
	}

    //
    // If we are not configured, return an error here before trying to send
    // anything.
    //
    if(!psInst->ui8USBConfigured)
    {
        return(CUSTOMHID_ERR_NOT_CONFIGURED);
    }

    //
    // Only send a report if the transmitter is currently free.
    //
    if(USBDHIDTxPacketAvailable((void *)psHIDDevice))
    {
        //
        // Send the report to the host.
        //
        psInst->iCustomHidState = eHIDCustomHidStateSend;
        ui32Count = USBDHIDReportWrite((void *)psHIDDevice,
                                       psInst->pui8Report, CUSTOMHID_REPORT_SIZE,
                                       true);

        //
        // Did we schedule a packet for transmission correctly?
        //
        if(!ui32Count)
        {
            //
            // No - report the error to the caller.
            //
            ui32Retcode = CUSTOMHID_ERR_TX_ERROR;
        }
        else
        {
            ui32Retcode = CUSTOMHID_SUCCESS;
        }
    }
    else
    {
        ui32Retcode = CUSTOMHID_ERR_TX_ERROR;
    }
    //
    // Return the relevant error code to the caller.
    //
    return(ui32Retcode);
}

//*****************************************************************************
//
//! Reports the device power status (bus- or self-powered) to the USB library.
//!
//! \param pvCustomHidDevice is the pointer to the customhid device instance structure.
//! \param ui8Power indicates the current power status, either \b
//! USB_STATUS_SELF_PWR or \b USB_STATUS_BUS_PWR.
//!
//! Applications which support switching between bus- or self-powered
//! operation should call this function whenever the power source changes
//! to indicate the current power status to the USB library.  This information
//! is required by the USB library to allow correct responses to be provided
//! when the host requests status from the device.
//!
//! \return None.
//
//*****************************************************************************
void
USBDHIDCustomHidPowerStatusSet(void *pvCustomHidDevice, uint8_t ui8Power)
{
    tUSBDHIDCustomHidDevice *psCustomHidDevice;
    tUSBDHIDDevice *psHIDDevice;

    ASSERT(pvCustomHidDevice);

    //
    // Get the keyboard device pointer.
    //
    psCustomHidDevice = (tUSBDHIDCustomHidDevice *)pvCustomHidDevice;

    //
    // Get a pointer to the HID device data.
    //
    psHIDDevice = &psCustomHidDevice->sPrivateData.sHIDDevice;

    //
    // Pass the request through to the lower layer.
    //
    USBDHIDPowerStatusSet((void *)psHIDDevice, ui8Power);
}

//*****************************************************************************
//
//! Requests a remote wake up to resume communication when in suspended state.
//!
//! \param pvCustomHidDevice is the pointer to the customhid device instance structure.
//!
//! When the bus is suspended, an application which supports remote wake up
//! (advertised to the host via the configuration descriptor) may call this
//! function to initiate remote wake up signaling to the host.  If the remote
//! wake up feature has not been disabled by the host, this will cause the bus
//! to resume operation within 20mS.  If the host has disabled remote wake up,
//! \b false will be returned to indicate that the wake up request was not
//! successful.
//!
//! \return Returns \b true if the remote wake up is not disabled and the
//! signaling was started or \b false if remote wake up is disabled or if
//! signaling is currently ongoing following a previous call to this function.
//
//*****************************************************************************
bool
USBDHIDCustomHidRemoteWakeupRequest(void *pvCustomHidDevice)
{
    tUSBDHIDCustomHidDevice *psCustomHidDevice;
    tUSBDHIDDevice *psHIDDevice;

    ASSERT(pvCustomHidDevice);

    //
    // Get the keyboard device pointer.
    //
    psCustomHidDevice = (tUSBDHIDCustomHidDevice *)pvCustomHidDevice;

    //
    // Get a pointer to the HID device data.
    //
    psHIDDevice = &psCustomHidDevice->sPrivateData.sHIDDevice;

    //
    // Pass the request through to the lower layer.
    //
    return(USBDHIDRemoteWakeupRequest((void *)&psHIDDevice));
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
