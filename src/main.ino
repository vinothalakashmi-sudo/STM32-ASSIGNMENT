#include <Arduino.h>

/* ================= UART ================= */
HardwareSerial SerialUART(PA3, PA2);   // RX, TX (USART2)

/* ================= TIMER ================= */
HardwareTimer *MyTimer;

/* ================= ADC + DMA ================= */
#define ADC_BUF_LEN 1
uint16_t adcBuffer[ADC_BUF_LEN];
volatile bool dmaComplete = false;

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

/* ================= DMA IRQ HANDLER ================= */
extern "C" void DMA2_Stream0_IRQHandler(void) {
  HAL_DMA_IRQHandler(&hdma_adc1);
}

/* ================= ADC DMA COMPLETE CALLBACK ================= */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
  if (hadc->Instance == ADC1) {
    dmaComplete = true;
  }
}

/* ================= TIMER CALLBACK (100 Hz) ================= */
void onTimer() {
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adcBuffer, ADC_BUF_LEN);
}

/* ================= ADC + DMA INITIALIZATION ================= */
void adcDmaInit() {

  __HAL_RCC_ADC1_CLK_ENABLE();
  __HAL_RCC_DMA2_CLK_ENABLE();

  hdma_adc1.Instance = DMA2_Stream0;
  hdma_adc1.Init.Channel = DMA_CHANNEL_0;
  hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
  hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
  hdma_adc1.Init.Mode = DMA_NORMAL;
  hdma_adc1.Init.Priority = DMA_PRIORITY_LOW;

  HAL_DMA_Init(&hdma_adc1);

  hadc1.Instance = ADC1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;

  HAL_ADC_Init(&hadc1);
  __HAL_LINKDMA(&hadc1, DMA_Handle, hdma_adc1);

  ADC_ChannelConfTypeDef sConfig = {0};
  sConfig.Channel = ADC_CHANNEL_0;   // PA0 -> LDR
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_144CYCLES;

  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

  NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}

/* ================= SETUP ================= */
void setup() {
  SerialUART.begin(115200);
  delay(1000);

  SerialUART.println("STM32F401 ADC + DMA + TIMER (100 Hz) - LDR Example");
  SerialUART.println("Connect LDR + 10k resistor to PA0 (Voltage Divider)");

  adcDmaInit();

  MyTimer = new HardwareTimer(TIM2);
  MyTimer->setOverflow(100, HERTZ_FORMAT);   // 100 Hz
  MyTimer->attachInterrupt(onTimer);
  MyTimer->resume();
}

/* ================= LOOP ================= */
void loop() {
  if (dmaComplete) {
    dmaComplete = false;

    SerialUART.print("LDR ADC Value: ");
    SerialUART.println(adcBuffer[0]);
  }
}
