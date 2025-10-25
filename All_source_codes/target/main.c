//main.c
#include <stdint.h>
#include <stdio.h> 

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

// Add this prototype for the function in syscalls.c
void ITM_Init(void);

int main(void)
{
    ITM_Init(); // Initialize the ITM hardware
    while(1){
    /* Loop forever */
    printf("Hello World\n"); 
    }

    for(;;);
}

