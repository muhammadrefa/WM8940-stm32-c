WM8940 audio codec library for STM32
======================================

_WIP (Work In Progress)_

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
- [X] Analogue to Digital Converter (ADC)
    - [X] High pass filter
    - [ ] Notch filter
    - [X] Digital ADC volume control
- [ ] Input limiter/Automatic Level Control (ALC)
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
    - [ ] Unused analogue inputs/outputs
- [ ] Output switch
    - [ ] Thermal shutdown
- [X] Digital audio interfaces
    - [X] Audio interface control
- [X] Audio sample rates
- [ ] Master clock and Phase Locked Loop (PLL)
- [X] Companding
- [X] General Purpose Input/Output
- [ ] Control interface
    - [X] Selection of control mode
    - [ ] Auto-incremental write setting
    - [ ] Readback
- [X] Resetting the chip
- [ ] Power supplies
    - [ ] Power up sequence
    - [ ] Power down sequence
- [ ] Power management
    - [ ] VMID control
    - [ ] BIASEN control
- [ ] Pop minimisation
