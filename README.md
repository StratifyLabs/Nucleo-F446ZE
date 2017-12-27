# Nucleo-F446ZE BSP

This is the Stratify OS Board Support Package (BSP) for running Stratify OS on the STM32 Nucleo-F446ZE microcontroller development board.

## Work to Do

This is the first BSP for any STM32F device. As such there is much work to do in porting drivers over the the platform. Currently, all the necessary
drivers needed to install the bootloader as well as run applications are working. The following is a (non-exhaustive) list of the drivers that need
to be integrated:

Minimum Required for bootloader and Apps:

- ~~USB~~
- ~~Flash~~
- ~~GPIO~~
- ~~Timer~~
- ~~Watchdog Timer~~

Basic:

- UART (partial)
- SPI
- I2C
- ADC
- DAC
- RTC

Advanced:

- SDIO
- I2S (SAI and SPI)
- DCMI
- CAN
- SPDIF
- QUADSPI
- FMC
- CRC


