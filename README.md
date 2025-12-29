# STM32F401 Blackpill - LDR ADC with DMA and Timer  
*Assignment Submission*

---

## 1. Project Objective

The aim of this project is to design and implement a *real-time light sensing system* using a *Light Dependent Resistor (LDR)* interfaced with an *STM32F401 Blackpill* microcontroller. The system should:

1. Trigger ADC readings periodically at *100 Hz* using a hardware timer.  
2. Transfer ADC results using *DMA* to reduce CPU load.  
3. Display ADC readings via *Serial UART* for monitoring light intensity.  
4. Demonstrate understanding of STM32 peripherals: *ADC, DMA, Timer, and UART*.  

---

## 2. Hardware Components

- STM32F401 Blackpill board  
- LDR (Light Dependent Resistor)  
- Resistors: 33 kΩ, 66 kΩ, 470 Ω, 220 Ω  
- CP2102 USB-to-UART module (for serial communication)  
- Breadboard and jumper wires  

---

## 3. Circuit Diagram

Voltage divider for LDR:
---
      ┌───────────────┐
      │  TIM2 Timer   │
      │  100 Hz ISR   │
      └───────┬───────┘
              │
              ▼
      ┌───────────────┐
      │ ADC Conversion│
      │ via DMA       │
      └───────┬───────┘
              │
              ▼
      ┌───────────────┐
      │ DMA Complete  │
      │ Callback sets │
      │ dmaComplete   │
      └───────┬───────┘
              │
              ▼
      ┌───────────────┐
      │  Main Loop    │
      │ Print ADC via │
      │ SerialUART    │
      └───────────────┘
---
## 6. Usage Instructions

1. Open LDR_DMA_Timer.ino in *Arduino IDE*.  
2. Select *STM32F401 Blackpill* as the board.  
3. Connect *LDR + series resistor* to *PA0*.  
4. Connect *PA2 (TX)* and *PA3 (RX)* to the *CP2102 module*.  
5. Upload the code.  
6. Open *Serial Monitor* at *115200 baud*.  
7. Observe ADC readings change with light intensity.

---

## 7. Observations & Results

- ADC value changes with ambient light.  
- Using a *33 kΩ series resistor*, ADC swing is limited (~low-resolution for bright light).  
- Lower series resistors (e.g., ~10 kΩ) provide better dynamic range.  
- DMA ensures *non-blocking, efficient ADC read, and the timer ensures **consistent 100 Hz sampling*.  

---

## 8. Conclusion

The project successfully demonstrates:

- Real-time ADC reading of an LDR using *DMA*.  
- Periodic sampling using a *hardware timer at 100 Hz*.  
- Efficient communication of results via *Serial UART*.  

It validates understanding of *STM32 peripherals* and the practical implementation of *embedded real-time systems*.
---
