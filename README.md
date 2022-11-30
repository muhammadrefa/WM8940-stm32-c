WM8940 audio codec library for STM32
======================================

WIP (Work In Progress) and untested

### Useful links:
- Product page: https://www.cirrus.com/products/wm8940/
- Datasheet: https://statics.cirrus.com/pubs/proDatasheet/WM8940_v4.4.pdf

### Implemented (based on device description):
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
- [ ] Input limiter/Automatic Level Control (ALC)
    - [X] Operation mode (Normal/Limiter)
    - [X] Attack and decay times
    - [X] Minimum and maximum gain
    - [X] Hold time and ALC level
    - [ ] Noise gate
    - [X] ALC zero cross
- [ ] Output signal path
    - [X] Hi-Fi DAC volume control
    - [X] Hi-Fi Digital to Analogue Converter (DAC)
    - [X] Automute
    - [ ] DAC output limiter
    - [ ] Volume boost
- [ ] Analogue outputs
    - [X] SPKOUTP/SPKOUTN outputs
    - [ ] Zero cross timeout
    - [X] Enabling the outputs
    - [X] Unused analogue inputs/outputs (VREF to analogue output resistance/VROI)
- [X] Output switch
    - [X] Thermal shutdown
- [ ] Digital audio interfaces
    - [X] Set audio format
    - [X] Set word length
    - [X] Set frame clock and BCLK polarity
    - [X] Set LOUTR
    - [ ] (ADC & DAC) data L&R swap
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
- [ ] POP minimisation
