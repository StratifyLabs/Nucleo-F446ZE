# Nucleo-F446ZE BSP

This is the Stratify OS Board Support Package (BSP) for running Stratify OS on the STM32 Nucleo-F446ZE microcontroller development board.

## Work to Do

This is the first BSP for any STM32F device. As such there is much work to do in porting drivers over to the platform. Currently, all the necessary
drivers needed to install the bootloader as well as run applications are working. The following is a (non-exhaustive) list of the drivers that need
to be integrated:

Minimum Required for bootloader and Apps:

- ~~USB~~
- ~~Flash~~
- ~~GPIO~~
- ~~Timer~~
- ~~Watchdog Timer~~

Basic:

- ~~UART~~
- ~~SPI~~
- ~~I2C~~
- EINT (External Interrupts)
- ADC
- DAC
- RTC

Advanced:

- ~~I2S (SPI)~~
- SDIO
- I2S (SAI)
- DCMI
- CAN
- SPDIF
- QUADSPI
- FMC
- CRC


