/**
 * OTOS - Open Tec Operating System
 * Copyright (c) 2021 Sebastian Oberschwendtner, sebastian.oberschwendtner@gmail.com
 *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
/**
 ******************************************************************************
 * @file    arm_m4_nofpu.c
 * @author  SO
 * @version v1.0.0
 * @date    09-March-2021
 * @brief   Defines the assembly code for:
 *          - ARM Cortex M4
 *          - No FPU enabled
 ******************************************************************************
 */

// ****** Includes ******

// Only include functions when not unit testing!
#ifndef UNIT_TEST

// ****** Functions ******

/**
 * @brief This function is called by the kernel to give the control
 * to a specific thread, which is defined by the given stack address.
 * It saves the kernel context an restores the context of the thread which
 * is executed next.
 * @param ThreadStack Pointer with the current stack address of next task.
 * @return Returns the new stack pointer of the executed thread when it yields.
 * The address of this pointer is used to catch stack overflows.
 * @details: Handler Mode, Stack: msp
 */
unsigned int* __otos_switch(unsigned int* ThreadStack)
{
    // Declare return type
    unsigned int* NewThreadStack;

    // Hand over control to the task belonging to the stack address
    __asm__ volatile(
        // Save kernel context
        "push   {R1-R12,LR}     \n\t" // Push registers on msp stack
        "mrs    R1, PSR         \n\t"
        "push   {R1}            \n\t" // Push PSR on msp stack
        // Restore thread context
        "ldmia  R0!,{R4-R11}    \n\t" // Load R4-R11 from task stack (psp)
        "ldmia  R0!,{R1}        \n\t"
        "mov    LR, R1          \n\t" // Load LR from task stack (psp)
        "msr    PSP, R0         \n\t" // Restore the psp stack pointer
        "bx     LR                  "
        // Jump to user task, LR should contain a valid exception return behavior
        : "=r" (NewThreadStack) // Define the output item
        : "r" (ThreadStack) // Define the input item
    );
    
    // Return the current task stack pointer, when resuming kernel operation
    return NewThreadStack;
};

/**
 * @brief Yield execution of the current thread and give the control
 * back to the kernel. Calls the SVC interrupt. When the thread gets the 
 * control back from the kernel, the thread execution resumes where the
 * yield was called from.
 * @details Thread Mode, Stack: psp
 */
void __otos_yield(void)
{
    __asm__ volatile(
        "nop    \n\t"
        "svc 0  \n\t" // Call the SVC interrupt
        "nop    \n\t"
        "bx lr      " // Resume operation when the SVC call is finished
    );
};

/**
 * @brief SVC Interrupt handler. This interrupt stores the context of the
 * calling thread and restores the context of the kernel.
 * @details interrupt-handler, Stack: msp
 */
void SVC_Handler(void)
{
    // Give control back to the kernel
    __asm__ volatile(
        // Save thread context
        "mrs    R0, PSP         \n\t"   // Save the current stack pointer (psp)
        "stmdb  R0!, {R4-R11,LR}\n\t"   // Save LR, and registers onto psp stack
        // R0 now contains the end of the occupied stack
        // Restore kernel context
        "pop    {R1}            \n\t"
        "msr    PSR_NZCVQ, R1   \n\t" // Write PSR with bitmask with value from msp stack
        "pop    {R1-R12}        \n\t" // Pop the registers from the msp stack
        // Restore LR and set program counter to resume kernel operation
        // bit[0] os the loaded value has to be 1 to stay in thumb mode!
        "pop    {PC}" 
    );
};
  
/**
 * @brief Initializes the kernel. The kernel starts out in thread mode
 * with privileged operation. It uses the SVC interrupt to switch to
 * handler mode with privileged operation. For the switch the thread
 * stack is used as temporary memory.
 * @param ThreadStack Beginning of thread stack as temporary memory.
 * @details Thread Mode -> Handler Mode, Stack: msp
 */
void __otos_init_kernel(unsigned int* ThreadStack)
{
    // Initialize the CONTROL
    __asm__ volatile(
        "push   {R1-R12, LR}    \n\t" //Save Registers and LR in msp
        "mrs    R1, PSR         \n\t"
        "push   {R1}            \n\t" // Save PSR to msp stack
        "msr    psp, R0         \t\n" // Set the psp to temporary memory
        "mov    R0, #0b11       \n\t"
        "msr    CONTROL, R0     \n\t" // Set CONTROL for Thread mode
        "isb                        " // Has to follow the write to CONTROL
        : // No outputs
        : "r" (ThreadStack) // Input item
    );

    // Now call yield to resume execution in handler mode
    __otos_yield();
};

#endif