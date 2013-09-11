Launchpad-Mame-Control
======================

TI Tiva C Launchpad firmware for Mame USB arcade controls adapter

======================

This project is intended to design firmware for the TI Tiva C Launchpad
development board to enable it to be used as a USB gamepad/mouse adapter
for arcade and other emulators.

======================

Version 0.0.1 - Initial Commit

Device shows up as two digital gamepads and a mouse.  Gamepad one has 12
buttons, gamepad two has 8 buttons, and the mouse has two buttons.

Pinout -

Port A Pins 0-7 - Gamepad One buttons 1-8

Port B Pins 0-7 - Gamepad Two buttons 1-8

Port C Pins 0-3 - Gamepad One buttons 9-12

Port C Pin 5    - Mouse Y Axis PHA

Port C Pin 6    - Mouse Y Axis PHB

Port D Pins 0-3 - Gamepad One D-Pad

Port D Pin 4    - USB D- (Onboard USB Device Port)

Port D Pin 5    - USB D+ (Onboard USB Device Port)

Port D Pin 6    - Mouse X Axis PHA

Port D Pin 7    - Mouse X Axis PHB

Port E Pins 0-1 - Mouse Buttons

Port E Pins 2-5 - Gamepad Two D-Pad

Port F Pin 4    - User SW1 (Onboard Button One)

Port F Pins 1-3 - Status LED (Onboard RGB LED)

User SW1 onboard is used to place the board in programming mode as
Port C Pins 0-3 are shared between the JTAG pins and Gamepad One
Buttons 9-12.  Port A Pins 0-1 are Serial RX/TX on the Launchpad board.
These pins are all accessible on an unpopulated pin header row between the
two large IC's on the board.  All other pins are accessible on the Launchpad
headers.  R9 and R10 must be removed from the Launchpad board as the connect
PD0/1 with PB6/7 for compatibility with older booster packs, but will cause
those problems for this project.

======================

Folder Structure
======================

If you would like to just flash your Launchpad using the LM Flash Programmer
Utility, the .bin file is located in the Debug folder of usb_dev_mame.

For those who wish to modify/build the package themselves, you will need
Code Creator Studio 5.4 and TivaWare 1.1 from the TI website.  

Replace usb-ids.h and usbhid.h in the \ti\TivaWare_C_Series-1.1\usblib with the
corresponding files in the usblib folder of this distribution.  Place
usbdhidmame.c and usbdhidmame.h from the usblib\device folder into the
\ti\TivaWare_C_Series-1.1\usblib\device folder.

Place the usb_dev_mame folder into the 
\ti\TivaWare_C_Series-1.1\examples\boards\ek-tm4c123gxl folder.

Open CCS and create a new project.  Import the driverlib and usblib projects from
the TivaWare package as well as the usb_dev_mame project.  Add filesystem links
to usbdhidmame.h and usbdhidmame.c in the device folder of the usblib project.
Rebuild first the driverlib and usblib projects, then you should be able to
successfully build the usb_dev_mame project.
