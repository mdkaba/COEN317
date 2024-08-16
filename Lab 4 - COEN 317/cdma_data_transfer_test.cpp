#include "xil_exception.h"
#include "xil_cache.h"
#include "xparameters.h"
#include <iostream>
#include "xtmrctr.h"

using namespace std;

int main()
{
	//-------------------------------------------------------Declare variables------------------------------------
	//declare the three pointers
	u32* cdma_ptr = (u32*) XPAR_AXI_CDMA_0_BASEADDR;
	u32* source_ptr = (u32*) XPAR_PS7_DDR_0_S_AXI_HP0_BASEADDR;
	u32* destination_ptr = (u32*) XPAR_PS7_DDR_0_S_AXI_HP2_BASEADDR;

	//declare the timer pointer
	XTmrCtr TimerInstancePtr;
	int xStatus;

	//--------------------------------------------------------Initialize the variables------------------------------------------------
	//initializing the contents of the source array within the maximum size
	for(int i =0; i <= 24; i++)
	{
		*(source_ptr + i) = i;
	}

	//initializing the contents of the destination array
	for(int i =0; i <= 24; i++)
	{
		*(destination_ptr + i) = -i;
	}

	//----------initialize the axi timer
	xStatus = XTmrCtr_Initialize(&TimerInstancePtr, XPAR_AXI_TIMER_0_DEVICE_ID);
	if(xStatus != XST_SUCCESS)
    {
	   cout <<  "TIMER INIT FAILED" << endl;
	   return 0;
	}

	//setting the timer reset value
	XTmrCtr_SetResetValue(&TimerInstancePtr, 0, 0);

	//setting the timer option
	XTmrCtr_SetOptions(&TimerInstancePtr, XPAR_AXI_TIMER_0_DEVICE_ID, XTC_CAPTURE_MODE_OPTION);


	//reset the cdma
	*(cdma_ptr) = 0x00000004;

	//configure the cdma
	*(cdma_ptr) = 0x00000020;

	//load address of source array
	*(cdma_ptr + 6) = 0x20000000;

	//load address of destination array
	*(cdma_ptr + 8) = 0x30000000;

	//flush the cash
	Xil_DCacheFlush();

	//number of bytes to transfer
	*(cdma_ptr + 10) = 0x64;

	//----------------------------------------------------main operations------------------------------------------
	//take in a value to start the process
	char dummy;
	cout << "Enter a key to begin" << endl;
	cin >> dummy;

	//start the timer
	XTmrCtr_Start(&TimerInstancePtr, 0);

	//begin transfer
	int status_reg_val = *(cdma_ptr + 1) & 2;
	while(status_reg_val == 0)
	{
		status_reg_val = *(cdma_ptr + 1) & 2; //isolate the idle bit
		if(status_reg_val == 2){ break; } //break when idle bit is 1
	}

	//transfer ends, stop timer and get the count
	XTmrCtr_Stop(&TimerInstancePtr, 0);
	unsigned int count = XTmrCtr_GetValue(&TimerInstancePtr, 0);

	bool are_equal = true;
	//compare the contents after the transfer
	for(int i=0 ; i <= 32; i++)
	{
		if(*(destination_ptr + i) != *(source_ptr + i))
		{
			are_equal = false;
			cout <<"The destination and source value are not the same : " << *(destination_ptr + i) << " " << *(source_ptr + i) << endl;
		}
		else {
			cout <<"The destination and source value are the same : " << *(destination_ptr + i) << " " << *(source_ptr + i) << endl;
		}
	}
	//output the results
	if (are_equal){ cout << "The values in the source and destination array are equal!" << endl;}
	else { cout << "The values in the source and destination array are not equal!" << endl;}
	cout << " It required " <<  count << " clock cycles to transfer the data " <<  endl;
	return 0;
}
