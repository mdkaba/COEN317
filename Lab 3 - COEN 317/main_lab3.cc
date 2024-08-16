// Mamadou Kaba (27070179)

#include "stdbool.h"
#include "xparameters.h"
#include "xil_types.h"
#include "xgpio.h"
#include "xil_io.h"
#include "xil_exception.h"
#include "xtmrctr.h"
#include <iostream>
using namespace std;

int main()

{
	//modified from the code completed in the prelab

    static XGpio GPIOInstance_Ptr;
    u32* timerPtr = (u32*) XPAR_TMRCTR_0_BASEADDR; //creating a u32 variable that points to the address

    int xStatus;

    cout << "####  Counter Application Starts ####" << endl;


    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Step-1: AXI GPIO Initialization
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    xStatus = XGpio_Initialize(&GPIOInstance_Ptr, XPAR_AXI_GPIO_FOR_OUTPUT_DEVICE_ID);
    if(xStatus != XST_SUCCESS)
    {
	    cout << "GPIO A Initialization FAILED" << endl;
	    return 1;
    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Step-2: AXI GPIO Set the Direction
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //XGpio_SetDataDirection(XGpio *InstancePtr, unsigned Channel, u32 DirectionMask);
    //we use only channel 1, and 0 is the the parameter for output

    XGpio_SetDataDirection(&GPIOInstance_Ptr, 1, 0);

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Step-3: PMW Timer Initialization and Setting
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    *timerPtr = 0x206; //setting the control/status of reg0's PWM's timer to enable pulse width, enabling generate mode and, enable count down
    *(timerPtr + 4) = 0x206; //setting the second timeer's control/status

     //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     //Step-4: Setting the Timer Option
     //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    //the following are variables to control the timer
    float cycleTime; //variable for the duty cycle
    float periodTimer; //variable for the period

    cout << "Enter duty cycle time: " << endl; cin >> cycleTime;
    cout << "Enter period (s): " << endl; cin >> periodTimer;

    //----------perform the PWM--------------------------

    while(true){
//50000000Hz  (given in lab)
   *(timerPtr + 1) = (periodTimer * 50000000) -2;  // put results in loads
    *(timerPtr + 5) = (periodTimer*(cycleTime/100)*50000000)-2; //putting percentage of duty in the timer 2's load

    //change controls to enable load the values from above into the timer
    *timerPtr = 0x226;
    *(timerPtr + 4) = 0x226;

    //change control to stop load and enable the counter
    *timerPtr = 0x286;
    *(timerPtr + 4) = 0x286;
    }
    return 0;
}
