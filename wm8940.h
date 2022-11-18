#ifndef MR_WM8940_H
#define MR_WM8940_H

#include <stdint.h>
#include "wm8940_regs.h"

#define WM8940_I2C_WRITE(reg, val) MyI2C_Write(reg, val)
#define WM8940_I2C_READ(reg) MyI2C_Read(reg)

// Volume range: -57 - +6 dB
#define WM8940_SPKVOL_DB_TO_REG_VALUE(vol_db) ((vol_db + 57) & 0x3F)
#define WM8940_SPKVOL_REG_VALUE_TO_DB(vol_regval) (vol_regval - 57)


typedef struct _WM8940
{
    uint32_t i2c_handle;
    uint8_t _volume;
} WM8940_t;


typedef enum _wm8940_status
{
    WM8940_STATUS_OK = 0,
    WM8940_STATUS_ERROR = 1,
    WM8940_STATUS_ARGERROR = 2
} wm8940_status_t;


// Input signal path
wm8940_input_t WM8940_Get_PGAInput();
void WM8940_Set_PGAInput(wm8940_input_t input);
uint8_t WM8940_Get_PGAInput_Volume();
void WM8940_Set_PGAInput_Volume(uint8_t vol);
void WM8940_Set_PGAInput_Mute(uint8_t state);
void WM8940_Set_PGAInput_ZeroCross(uint8_t state);
void WM8940_Set_PGAInput_Enable(uint8_t state);
void WM8940_Set_Aux_Enable(uint8_t state);
void WM8940_Set_Aux_Mode(wm8940_aux_mode_t mode);
void WM8940_Set_PGA_Boost(uint8_t state);
void WM8940_Set_Boost_Volume(wm8940_input_t input, uint8_t vol);
void WM8940_Set_Boost_Enable(uint8_t state);
void WM8940_Set_MicBias_Enable(uint8_t state);
void WM8940_Set_MicBias_Voltage(wm8940_micbias_voltage_t percent);

// ADC
void WM8940_Set_ADC_Enable(uint8_t state);
void WM8940_Set_ADC_Polarity(uint8_t polarity);
void WM8940_Set_HighPassFilter_Enable(uint8_t state);
void WM8940_Set_HighPassFilter_Application(uint8_t val);
void WM8940_Set_HighPassFilter_CutOff(uint8_t freq);

// Analogue outputs
void WM8940_Set_Speaker_Source(wm8940_speaker_source_t source);
void WM8940_Set_Speaker_FromBypass_Attenuation(uint8_t state);
void WM8940_Set_Speaker_ZeroCross(uint8_t state);
void WM8940_Set_Speaker_Mute(uint8_t state);
void WM8940_Set_Speaker_Volume(uint8_t regval);
void WM8940_Set_Speaker_Volume_db(int8_t vol_db);
void WM8940_Set_Mono_Source(wm8940_speaker_source_t source);
void WM8940_Set_Mono_FromBypass_Attenuation(uint8_t state);
void WM8940_Set_Mono_Mute(uint8_t state);
void WM8940_Set_Output_Enable(wm8940_output_t output);

#endif // MR_WM8940_H
