WM8940 audio codec library for STM32
======================================

## Useful links:
- Product page: https://www.cirrus.com/products/wm8940/
- Datasheet: https://statics.cirrus.com/pubs/proDatasheet/WM8940_v4.4.pdf

## How to use
Write your implementation to write/read register from the device. For example:
```C
// --- main.c ---

void MyI2C_Write(i2c_handle, i2c_address, register, value)
{
    // TODO
}

uint16_t MyI2C_Read(i2c_handle, i2c_address, register)
{
    // TODO
}
```
```C
// --- main.h ---

void MyI2C_Write(i2c_handle, i2c_address, register, value);
uint16_t MyI2C_Read(i2c_handle, i2c_address, register);
```

Edit the `wm8940.h` to call your implementation
```C
// --- wm8940.h ---

#include "main.h"    // The location of the function prototype for the implementation

#define WM8940_REG_WRITE(handle, reg, val) MyI2C_Write(handle, WM8940_I2C_ADDRESS, reg, val)
#define WM8940_REG_READ(handle, reg) MyI2C_Read(handle, WM8940_I2C_ADDRESS, reg)
```

Now you can initialize the audio codec, configure the Digital Audio Interface to transmit/receive audio data, and follow the Audio Signal Path from the datasheet.
```C
// --- main.c ---

#include "wm8940.h"

WM8940_t wm8940;

int main(void)
{
    // ...

    wm8940.comm_handle = &hi2c1;
    WM8940_Init(&wm8940);

    // STM32 as I2S Master
    WM8940_Set_Clock(&wm8940, 0, WM8940_BCLKDIV_1, WM8940_MCLKDIV_1, WM8940_CLKSEL_MCLK);
    WM8940_Set_AudioInterfaceFormat(&wm8940, WM8940_AUDIO_IFACE_FMT_I2S, WM8940_AUDIO_IFACE_WL_16BITS);
    WM8940_Set_SampleRate(&wm8940, WM8940_SAMPLERATE_8KHZ);

    // Audio bypass from MICN to Speaker (BTL) and Mono
    WM8940_Set_PGA_Input(&wm8940, WM8940_INPUT_MICN);
    WM8940_Set_PGA_Mute(&wm8940, 1);
    WM8940_Set_PGA_Enable(&wm8940, 1);
    WM8940_Set_Boost_Enable(&wm8940, 1);
    WM8940_Set_MicBias_Enable(&wm8940, 1);
    WM8940_Set_Speaker_Source(&wm8940, WM8940_OUTPUT_FROM_BYPASS);
    WM8940_Set_Speaker_Mute(&wm8940, 0);
    WM8940_Set_Mono_Source(&wm8940, WM8940_OUTPUT_FROM_BYPASS);
    WM8940_Set_Mono_Mute(&wm8940, 0);
    WM8940_Set_Output_Enable(&wm8940, WM8940_OUTPUT_SPK | WM8940_OUTPUT_MONO);

    // ...
}
```

## Implemented (based on device description):
- [X] Input signal path
    - [X] Microphone inputs
    - [X] Input PGA volume control
    - [X] Auxiliary input
    - [X] Input boost
    - [X] Microphone biasing
- [ ] Analogue to Digital Converter (ADC)
    - [X] High pass filter
    - [ ] Notch filter
    - [X] Digital ADC volume control
- [X] Input limiter/Automatic Level Control (ALC)
    - [X] Operation mode (Normal/Limiter)
    - [X] Attack and decay times
    - [X] Minimum and maximum gain
    - [X] Hold time and ALC level
    - [X] Noise gate
    - [X] ALC zero cross
- [ ] Output signal path
    - [X] Hi-Fi DAC volume control
    - [X] Hi-Fi Digital to Analogue Converter (DAC)
    - [X] Automute
    - [ ] DAC output limiter
    - [X] Volume boost
- [X] Analogue outputs
    - [X] SPKOUTP/SPKOUTN outputs
    - [X] Zero cross timeout
    - [X] Enabling the outputs
    - [X] Unused analogue inputs/outputs (VREF to analogue output resistance/VROI)
- [X] Output switch
    - [X] Thermal shutdown
- [X] Digital audio interfaces
    - [X] Set audio format
    - [X] Set word length
    - [X] Set frame clock and BCLK polarity
    - [X] Set LOUTR
    - [X] (ADC & DAC) data L&R swap
    - [X] Clock generation control
- [X] Audio sample rates
- [X] Master clock and Phase Locked Loop (PLL)
    - [X] Enable PLL
    - [X] Set PLL power
    - [X] Set PLL frequency
- [X] Companding
    - [X] (ADC & DAC) Loopback
    - [X] (ADC & DAC) Companding
    - [X] Enable 8-bit word length
- [X] General Purpose Input/Output
- [X] Control interface
    - [X] Selection of control mode
    - [X] Auto-incremental write setting
    - [X] Readback
- [X] Resetting the chip
- [X] Power management
    - [X] VMID control
    - [X] BIASEN control
- [X] POP minimisation
    - [X] Power on Bias control (configured on init)
    - [X] VMID soft start (configured on init)
    - [X] Fast VMID discharge
