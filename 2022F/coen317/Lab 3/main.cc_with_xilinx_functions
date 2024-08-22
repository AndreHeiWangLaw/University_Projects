
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


    static XGpio GPIOInstance_Ptr;
    XTmrCtr TimerInstancePtr;
  
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
    //Step-3: AXI Timer Initialization and Setting
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    xStatus = XTmrCtr_Initialize(&TimerInstancePtr, XPAR_AXI_TIMER_0_DEVICE_ID);
    if(xStatus != XST_SUCCESS)
    {
      cout <<  "TIMER INIT FAILED" << endl;
      return 0;
    }

	
     //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     //Step-4: Setting the timer Reset Value
     //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
     // XTmrCtr_SetResetValue(XTmrCtr * InstancePtr, u8 TmrCtrNumber,
     // u32 ResetValue);

    XTmrCtr_SetResetValue(&TimerInstancePtr, 0, 0);

	
     //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     //Step-5: Setting the Timer Option
     //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    XTmrCtr_SetOptions(&TimerInstancePtr, XPAR_AXI_TIMER_0_DEVICE_ID, XTC_CAPTURE_MODE_OPTION);




    // perform a write to the GPIO output port

    int LEDs ;

   cout << "Enter the number to be sent to the LEDs" << endl;
   cin >> LEDs;

   // Determine how long it takes to perform a DiscreteWrite to the GPIO
   //start the timer

      XTmrCtr_Start(&TimerInstancePtr, 0);

    XGpio_DiscreteWrite(&GPIOInstance_Ptr, 1, (u8) LEDs);

    //stop the timer

   XTmrCtr_Stop(&TimerInstancePtr, 0);

    // read the value of the counter register and store it in count
    unsigned int count;
    count = XTmrCtr_GetValue(&TimerInstancePtr, 0);
    cout << " It required Count = " <<  count << " clock cycles to perform the DiscreteWrite " <<  endl;



    return 0;

}
