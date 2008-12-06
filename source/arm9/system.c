/*---------------------------------------------------------------------------------

system.c -- System code

Copyright (C) 2005
Michael Noland (joat)
Jason Rogers (dovoto)
Dave Murphy (WinterMute)

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1.	The origin of this software must not be misrepresented; you
must not claim that you wrote the original software. If you use
this software in a product, an acknowledgment in the product
documentation would be appreciated but is not required.

2.	Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3.	This notice may not be removed or altered from any source
distribution.


---------------------------------------------------------------------------------*/

#include <nds/memory.h>
#include <nds/bios.h>
#include <nds/arm9/system.h>
#include <nds/fifocommon.h>
#include <nds/interrupts.h>
#include <nds/fifomessages.h>
#include <libnds_internal.h>

//todo document
//


//---------------------------------------------------------------------------------
// Handle system requests from the arm7
//---------------------------------------------------------------------------------
void powerValueHandler(u32 value, void* data){
	switch(value)
	{
	case PM_REQ_SLEEP:
		sleep();
		break;
	}
}

void systemMsgHandler(int bytes, void* user_data){
	
	FifoMessage message;

	SystemInputMsg *input;

	fifoGetDatamsg(FIFO_SYSTEM, bytes, (u8*)&message);

	switch (message.type)
	{
	case SYS_INPUT_MESSAGE:
		input = (SystemInputMsg*)&message;
		setTransferInputData(&(input->touch), input->keys);
		break;
	}
}

void sleep(void)
{
   unsigned long oldIE = REG_IE ;
 
   fifoSendValue32(FIFO_PM, PM_REQ_SLEEP);
  
   REG_IE = IRQ_VBLANK ;
	
   //wait two frames to give arm7 a chance
   swiWaitForVBlank();
   swiWaitForVBlank();
   
   //100ms
   swiDelay(419000);
		    
   REG_IE = oldIE ; 
}

void powerOn(PM_Bits bits)
{
   fifoSendValue32(FIFO_PM, PM_REQ_ON | bits);
}

void powerOff(PM_Bits bits)
{
   fifoSendValue32(FIFO_PM, PM_REQ_OFF | bits);
}

void ledBlink(PM_LedBlinkMode bm)
{
   fifoSendValue32(FIFO_PM, PM_REQ_LED | bm);
}
