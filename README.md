# STM32F401 ADC + DMA + UART (Arduino IDE)

## Objective
The objective of this assignment is to implement an embedded system on STM32 Blackpill where:
1. A 100Hz timer triggers ADC conversion
2. ADC data is transferred using DMA
3. Data is transmitted via UART
4. Output is displayed on Serial Monitor

## Hardware Used
- STM32F401CC Blackpill
- ST-Link V2 (Programming/debugging)
- CP2102 USB to UART module
- 10kΩ potentiometer
- Jumper wires 

## Software
- Arduino IDE
- STM32 Boards Package (STMicroelectronics)

## Pin Configuration
###ADC
| Function | STM32 Pin |
|--------|----|
| ADC Input | PA0 |
### UART
| Function | STM32 Pin |
|--------|----|
| UART TX | PA3 |
| UART RX | PA2 |

## Hardware Connections
### 10 kilo ohm potiometer
| Potentiometer Pin | STM32 Pin |
|--------|----|
| One end | 3.3V|
| Middle wire | PA0 |
| Other end | GND |

### CP2102 Module
| Function | STM32 Pin |
|--------|----|
| TXD | PA3 |
| RXD | PA2 |
| GND | GND |
| VCC | Not Connected |

### ST-LINK V2
| Function | STM32 Pin |
|--------|----|
| SWCLK| SWCLK |
| SWDIO | SWDIO|
| 3.3V | 3.3V |
| GND | GND |

## How It Works
- Timer2 generates interrupts at 100Hz
- Each interrupt triggers ADC read
- DMA behavior is simulated using interrupt-driven buffer
- ADC value is sent via UART

## Output
ADC values (0–4095) are displayed on Arduino Serial Monitor at 115200 baud.

## Notes
- Potentiometer must be powered with 3.3V
- pinMode(PA0, ANALOG) is required
- CP2102 VCC should not be connected





