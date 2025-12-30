#Code Explanation
*STM32F401 Blackpill – ADC Triggered by Timer with DMA and UART Output*

## Overview
This project demonstrates how to use Timer, ADC, DMA, and UART together on an STM32F401 Blackpill using the Arduino IDE.
A 100 Hz hardware timer periodically triggers an ADC conversion. The ADC result is automatically transferred to memory using DMA, and once the DMA transfer is complete, the sampled value is sent to a serial terminal via UART.
This approach minimizes CPU usage and follows best practices for real-time embedded systems.

---

## Peripheral Usage
| Peripheral | Purpose |
|--------------|------------|
| TIM2 | Generates a 100 Hz periodic interrupt |
| ADC1 | Reads analog voltage from LDR (PA0) |
| DMA2 | Transfers ADC data to RAM |
| USART2 | Sends ADC data to Serial Monitor | 

---

## UART Configuration
HardwareSerial SerialUART(PA3, PA2);   // RX, TX (USART2)
•	USART2 is used for serial communication
•	Baud rate: 115200
•	ADC values are transmitted to the serial terminal

---

## Timer Configuration (100 Hz)
MyTimer = new HardwareTimer(TIM2);
MyTimer->setOverflow(100, HERTZ_FORMAT);
MyTimer->attachInterrupt(onTimer);
MyTimer->resume();
•	Timer overflow occurs every 10 ms (100 Hz)
•	On overflow, a callback function is executed
•	The timer is responsible for periodic ADC triggering

---

## Timer Interrupt Callback
void onTimer() {
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adcBuffer, ADC_BUF_LEN);
}
•	This function is executed on every timer overflow
•	Starts ADC conversion with DMA
•	No CPU polling or manual ADC read is required

---

## ADC Configuration
hadc1.Instance = ADC1;
hadc1.Init.Resolution = ADC_RESOLUTION_12B;
hadc1.Init.ScanConvMode = DISABLE;
hadc1.Init.ContinuousConvMode = DISABLE;
hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
hadc1.Init.NbrOfConversion = 1;
•	ADC resolution: 12-bit (0–4095)
•	Single-channel conversion
•	ADC channel used: PA0 (ADC_CHANNEL_0) connected to LDR
•	Conversion is triggered by software (timer callback)

---

## DMA Configuration
hdma_adc1.Instance = DMA2_Stream0;
hdma_adc1.Init.Channel = DMA_CHANNEL_0;
hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
hdma_adc1.Init.Mode = DMA_NORMAL;
•	DMA transfers ADC data directly from ADC data register to RAM
•	Eliminates CPU involvement during data transfer
•	Stores result in adcBuffer[]

---

## ADC to DMA Data Flow
When this line is executed:
HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adcBuffer, ADC_BUF_LEN);
The following happens automatically in hardware:
LDR Voltage (PA0)
   ↓
ADC Conversion
   ↓
ADC Data Register
   ↓ (DMA request)
DMA Controller
   ↓
adcBuffer[0]
•	No manual ADC read
•	No CPU intervention
•	True hardware DMA transfer

---

## DMA Completion Callback
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
  if (hadc->Instance == ADC1) {
    dmaComplete = true;
  }
}
Called automatically when DMA transfer finishes
Sets a flag to indicate data is ready
Confirms successful ADC → DMA transfer

---

## Main Loop Processing
void loop() {
  if (dmaComplete) {
    dmaComplete = false;

    SerialUART.print("LDR ADC Value: ");
    SerialUART.println(adcBuffer[0]);
  }
}
•	Continuously checks DMA completion flag
•	Reads ADC value from memory
•	Sends result to serial terminal via UART

---

 ## Output
•	Serial monitor displays real-time ADC values
•	Output updates at 100 Hz
•	ADC values change according to light intensity on LDR
Example:
LDR ADC Value: 1240
LDR ADC Value: 2310
LDR ADC Value: 389

