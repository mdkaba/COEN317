#include "xil_exception.h"
#include "xil_cache.h"
#include "xparameters.h"
#include <iostream>
#include "xtmrctr.h"

using namespace std;

// Function to transfer data without CDMA
unsigned int transferWithoutCDMA(u32* source_ptr, u32* destination_ptr, int num_integers, XTmrCtr& TimerInstancePtr) {
    XTmrCtr_Start(&TimerInstancePtr, 0);
    for(int i = 0; i < num_integers; i++) {
        *(destination_ptr + i) = *(source_ptr + i);
    }
    XTmrCtr_Stop(&TimerInstancePtr, 0);
    return XTmrCtr_GetValue(&TimerInstancePtr, 0);
}

// Function to transfer data with CDMA
unsigned int transferWithCDMA(u32* cdma_ptr, u32* source_ptr, u32* destination_ptr, int num_bytes, XTmrCtr& TimerInstancePtr) {
    // Reset CDMA
    *(cdma_ptr) = 0x00000004;
    // Configure CDMA
    *(cdma_ptr) = 0x00000020;
    // Load source and destination addresses
    *(cdma_ptr + 6) = (u32)source_ptr;
    *(cdma_ptr + 8) = (u32)destination_ptr;
    // Flush cache
    Xil_DCacheFlush();
    // Set number of bytes to transfer
    *(cdma_ptr + 10) = num_bytes;

    XTmrCtr_Start(&TimerInstancePtr, 0);
    // Wait for transfer to complete
    while(!(*(cdma_ptr + 1) & 2)) {}
    XTmrCtr_Stop(&TimerInstancePtr, 0);
    return XTmrCtr_GetValue(&TimerInstancePtr, 0);
}

int main() {
    // Declare variables
    u32* source_ptr = (u32*)XPAR_PS7_DDR_0_S_AXI_HP0_BASEADDR;
    u32* destination_ptr = (u32*)XPAR_PS7_DDR_0_S_AXI_HP2_BASEADDR;
    u32* cdma_ptr = (u32*)XPAR_AXI_CDMA_0_BASEADDR;
    XTmrCtr TimerInstancePtr;
    int xStatus;

    // Initialize AXI Timer
    xStatus = XTmrCtr_Initialize(&TimerInstancePtr, XPAR_AXI_TIMER_0_DEVICE_ID);
    if(xStatus != XST_SUCCESS) {
        cout << "TIMER INIT FAILED" << endl;
        return 0;
    }
    XTmrCtr_SetResetValue(&TimerInstancePtr, 0, 0);
    XTmrCtr_SetOptions(&TimerInstancePtr, XPAR_AXI_TIMER_0_DEVICE_ID, XTC_CAPTURE_MODE_OPTION);

    // Array sizes to test
    int array_sizes[] = {1024, 4096, 8192, 16384, 32767, 1048576};
    int num_sizes = sizeof(array_sizes) / sizeof(array_sizes[0]);

    // Measure transfer time for each array size
    for(int i = 0; i < num_sizes; i++) {
        int num_integers = array_sizes[i];
        int num_bytes = num_integers * sizeof(u32);

        // Without CDMA
        unsigned int cycles_without_cdma = transferWithoutCDMA(source_ptr, destination_ptr, num_integers, TimerInstancePtr);

        // With CDMA
        unsigned int cycles_with_cdma = transferWithCDMA(cdma_ptr, source_ptr, destination_ptr, num_bytes, TimerInstancePtr);

        // Output results
        cout << "Array Size: " << num_bytes << " bytes" << endl;
        cout << "Time without CDMA: " << cycles_without_cdma << " clock cycles" << endl;
        cout << "Time with CDMA: " << cycles_with_cdma << " clock cycles" << endl;

        // Compute speedup
        float speedup = static_cast<float>(cycles_without_cdma) / cycles_with_cdma;
        cout << "Speedup: " << speedup << endl << endl;
    }

    return 0;
}