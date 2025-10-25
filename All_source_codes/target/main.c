//main.c
#include <stdint.h>
#include <stdio.h> // <-- Make sure this is included

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

// Add this prototype for the function in syscalls.c
void ITM_Init(void);

int main(void)
{
    /* --- ADD THIS LINE --- */
    ITM_Init(); // Initialize the ITM hardware
    while(1){
    /* Loop forever */
    printf("HelloWorld\n"); // <-- Added \n to flush the buffer
    }

    for(;;);
}
