//syscalls.c
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <stdint.h> // This include defines int8_t, uint8_t, uint32_t etc.

/* Defines for ITM */
#define DEMCR_TRCENA         (1U << 24)
#define DEMCR                *((volatile uint32_t*) 0xE000EDFCU)

#define ITM_STIMULUS_PORT0   *((volatile uint32_t*) 0xE0000000)
#define ITM_TRACE_ENABLE     *((volatile uint32_t*) 0xE0000E00)
#define ITM_TRACE_EN_PORT0   (1U << 0)


/**
  * @brief  Initializes the ITM hardware for tracing.
  * @brief  This function should be called ONCE from your main() function.
  * @param  None
  * @retval None
  */
void ITM_Init(void)
{
    DEMCR |= DEMCR_TRCENA;          // Enable TRCENA
    ITM_TRACE_ENABLE |= ITM_TRACE_EN_PORT0; // Enable ITM stimulus port 0
}

/**
  * @brief  Transmits a single character over ITM stimulus port 0.
  * @param  ch: The character to transmit
  * @retval None
  */
void ITM_SendChar(uint8_t ch)
{
    // Wait until ITM TxFIFO is ready (bit 0 is 1)
    while (!(ITM_STIMULUS_PORT0 & 1));

    // Write to ITM stimulus port 0
    // Note: The port is 32-bit, but we only write a byte.
    ITM_STIMULUS_PORT0 = (uint32_t)ch;
}


/* Variables */
extern int __io_putchar(int ch) __attribute__((weak));
extern int __io_getchar(void) __attribute__((weak));


char *__env[1] = { 0 };
char **environ = __env;


/* Functions */
void initialise_monitor_handles()
{
}

int _getpid(void)
{
  return 1;
}

int _kill(int pid, int sig)
{
  (void)pid;
  (void)sig;
  errno = EINVAL;
  return -1;
}

void _exit (int status)
{
  _kill(status, -1);
  while (1) {}    /* Make sure we hang here */
}

__attribute__((weak)) int _read(int file, char *ptr, int len)
{
  (void)file;
  int DataIdx;

  for (DataIdx = 0; DataIdx < len; DataIdx++)
  {
    *ptr++ = __io_getchar();
  }

  return len;
}

__attribute__((weak)) int _write(int file, char *ptr, int len)
{
  (void)file; // Suppress unused parameter warning
  int DataIdx;

  // ******************************************************************
  // ** This is the function that routes printf to ITM_SendChar.
  // **
  // ** REMINDER: You must call ITM_Init() once in your main.c
  // ** before using printf() for the first time.
  // ******************************************************************

  for (DataIdx = 0; DataIdx < len; DataIdx++)
  {
    // __io_putchar(*ptr++); // Default behavior
    ITM_SendChar(*ptr++); // Redirect to ITM
  }
  return len;
}

int _close(int file)
{
  (void)file;
  return -1;
}


int _fstat(int file, struct stat *st)
{
  (void)file;
  st->st_mode = S_IFCHR; // Corrected: removed extra 'st->'
  return 0;
}

int _isatty(int file)
{
  (void)file;
  return 1;
}

int _lseek(int file, int ptr, int dir)
{
  (void)file;
  (void)ptr;
  (void)dir;
  return 0;
}

int _open(char *path, int flags, ...)
{
  (void)path;
  (void)flags;
  /* Pretend like we always fail */
  return -1;
}

int _wait(int *status)
{
  (void)status;
  errno = ECHILD;
  return -1;
}

int _unlink(char *name)
{
  (void)name;
  errno = ENOENT;
  return -1;
}

int _times(struct tms *buf)
{
  (void)buf;
  return -1;
}

int _stat(char *file, struct stat *st)
{
  (void)file;
  st->st_mode = S_IFCHR;
  return 0;
}

int _link(char *old, char *new)
{
  (void)old;
  (void)new;
  errno = EMLINK;
  return -1;
}

int _fork(void)
{
  errno = EAGAIN;
  return -1;
}

int _execve(char *name, char **argv, char **env)
{
  (void)name;
  (void)argv;
  (void)env;
  errno = ENOMEM;
  return -1;
}
