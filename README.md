# COEN317
Microprocessor-Based Systems


[Lab 1: Introduction to the ZYNQ ZC702 “Hello World”](https://github.com/mdkaba/COEN317/tree/main/Lab%201%20-%20COEN%20317)

**Goals:**
- Familiarize with the Xilinx ZC702 development board and its integrated dual-core ARM Cortex A9 processor.
- Gain experience in using the Xilinx software tools (PlanAhead, XPS, and SDK) for programming the board.
- Develop a simple "Hello World" application to run on the ZC702 board, showcasing the basic setup and execution.

[Lab 2: AND Array Implemented in Programmable Logic](https://github.com/mdkaba/COEN317/tree/main/Lab%202%20-%20COEN%20317)

**Goals:**
- Introduce the use of programmable logic within the XC7020 chip by implementing a two-input, 8-bit wide AND gate.
- Understand how to interface the processor with the programmable logic through AXI GPIO.
- Display the results of the AND gate operation on the board's LEDs and explore hardware-software integration.

[Lab 3: Using the AXI Hardware Timer](https://github.com/mdkaba/COEN317/tree/main/Lab%203%20-%20COEN%20317)

**Goals:**
- Learn to use the LogiCORE IP AXI Timer as a hardware counter with multiple operating modes including basic timing, capture mode, and pulse-width modulation (PWM).
- Implement the AXI Timer in various configurations to perform tasks like measuring execution time, counting events, and generating PWM signals.
- Explore the practical use of hardware timers in embedded systems, including handling input capture and output generation.

[Lab 4: Interrupts with the AXI Timer (Part 1 & Part 2)](https://github.com/mdkaba/COEN317/tree/main/Lab%204%20-%20COEN%20317)

**Goals:**
- Introduce the interrupt structure of the ARM processor using the AXI timer.
- Configure the AXI timer to generate interrupts and handle these interrupts with a user-defined C++ function.
- Design a system that responds to external events by temporarily halting execution and servicing interrupts.
- Implement additional functionality in the system by extending the initial interrupt setup to include more complex operations.
