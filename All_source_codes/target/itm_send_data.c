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

