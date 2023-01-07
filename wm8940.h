#ifndef MR_WM8940_H
#define MR_WM8940_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "wm8940_regs.h"

#define WM8940_REG_WRITE(handle, reg, val) MyI2C_Write(handle, WM8940_I2C_ADDRESS, reg, val)
#define WM8940_REG_READ(handle, reg) MyI2C_Read(handle, WM8940_I2C_ADDRESS, reg)

// Volume range: -57 - +6 dB
#define WM8940_SPKVOL_DB_TO_REG_VALUE(vol_db) ((vol_db + 57) & 0x3F)
#define WM8940_SPKVOL_REG_VALUE_TO_DB(vol_regval) (vol_regval - 57)


typedef struct _WM8940
{
    uint32_t comm_handle;
    uint16_t _register[57];
} WM8940_t;


typedef enum _wm8940_status
{
    WM8940_STATUS_OK = 0,
    WM8940_STATUS_ERROR = 1,
    WM8940_STATUS_INVALID = 2
} wm8940_status_t;


wm8940_status_t WM8940_Init(WM8940_t* wm8940);

// Input signal path
wm8940_status_t WM8940_Get_PGA_Input(WM8940_t* wm8940, wm8940_input_t* input);
wm8940_status_t WM8940_Set_PGA_Input(WM8940_t* wm8940, wm8940_input_t input);
uint8_t WM8940_Get_PGA_Volume(WM8940_t* wm8940);
wm8940_status_t WM8940_Set_PGA_Volume(WM8940_t* wm8940, uint8_t vol);
wm8940_status_t WM8940_Set_PGA_Mute(WM8940_t* wm8940, uint8_t state);
wm8940_status_t WM8940_Set_PGA_ZeroCross(WM8940_t* wm8940, uint8_t state);
wm8940_status_t WM8940_Set_PGA_Enable(WM8940_t* wm8940, uint8_t enable);
wm8940_status_t WM8940_Set_Aux_Enable(WM8940_t* wm8940, uint8_t enable);
wm8940_status_t WM8940_Set_Aux_Mode(WM8940_t* wm8940, wm8940_aux_mode_t mode);
wm8940_status_t WM8940_Set_PGA_Boost(WM8940_t* wm8940, uint8_t state);
wm8940_status_t WM8940_Set_Boost_Volume(WM8940_t* wm8940, wm8940_input_t input, uint8_t vol);
wm8940_status_t WM8940_Set_Boost_Enable(WM8940_t* wm8940, uint8_t enable);
wm8940_status_t WM8940_Set_MicBias_Enable(WM8940_t* wm8940, uint8_t enable);
wm8940_status_t WM8940_Set_MicBias_Voltage(WM8940_t* wm8940, wm8940_micbias_voltage_t percentage);

// ADC
wm8940_status_t WM8940_Set_ADC_Enable(WM8940_t* wm8940, uint8_t enable);
wm8940_status_t WM8940_Set_ADC_Polarity(WM8940_t* wm8940, uint8_t invert);
wm8940_status_t WM8940_Set_ADC_HighPassFilter(WM8940_t* wm8940, uint8_t enable, wm8940_hpf_mode_t mode, uint8_t freq_regval);
wm8940_status_t WM8940_Set_ADC_Volume(WM8940_t* wm8940, uint8_t regval);

// ALC
wm8940_status_t WM8940_Set_ALC_Enable(WM8940_t* wm8940, uint8_t enable);
wm8940_status_t WM8940_Set_ALC_Gain(WM8940_t* wm8940, uint8_t minval, uint8_t maxval);
wm8940_status_t WM8940_Set_ALC_Level(WM8940_t* wm8940, uint8_t val);
wm8940_status_t WM8940_Set_ALC_Hold(WM8940_t* wm8940, uint8_t val);
wm8940_status_t WM8940_Set_ALC_Mode(WM8940_t* wm8940, wm8940_alc_mode_t mode);
wm8940_status_t WM8940_Set_ALC_DecayTime(WM8940_t* wm8940, uint8_t val);
wm8940_status_t WM8940_Set_ALC_AttackTime(WM8940_t* wm8940, uint8_t val);
wm8940_status_t WM8940_Set_ALC_ZeroCross(WM8940_t* wm8940, uint8_t state);
wm8940_status_t WM8940_Set_ALC_NoiseGate_Threshold(WM8940_t* wm8940, uint8_t val);
wm8940_status_t WM8940_Set_ALC_NoiseGate_Enable(WM8940_t* wm8940, uint8_t enable);

// DAC
wm8940_status_t WM8940_Set_DAC_Enable(WM8940_t* em8940, uint8_t enable);
wm8940_status_t WM8940_Set_DAC_Polarity(WM8940_t* wm8940, uint8_t invert);
wm8940_status_t WM8940_Set_DAC_Volume(WM8940_t* wm8940, uint8_t val);
wm8940_status_t WM8940_Set_DAC_SoftMute(WM8940_t* wm8940, uint8_t state);
wm8940_status_t WM8940_Set_DAC_AutoMute(WM8940_t* wm8940, uint8_t state);
wm8940_status_t WM8940_Set_DAC_LimiterVolumeBoost(WM8940_t* wm8940, uint8_t value);

// Analogue outputs
wm8940_status_t WM8940_Set_Speaker_Source(WM8940_t* wm8940, wm8940_speaker_source_t source);
wm8940_status_t WM8940_Set_Speaker_FromBypass_Attenuation(WM8940_t* wm8940, uint8_t state);
wm8940_status_t WM8940_Set_Speaker_ZeroCross(WM8940_t* wm8940, uint8_t state);
wm8940_status_t WM8940_Set_Speaker_Mute(WM8940_t* wm8940, uint8_t state);
wm8940_status_t WM8940_Set_Speaker_Volume(WM8940_t* wm8940, uint8_t regval);
wm8940_status_t WM8940_Set_Speaker_Volume_db(WM8940_t* wm8940, int8_t vol_db);
uint8_t WM8940_Get_Speaker_Volume(WM8940_t* wm8940);
uint8_t WM8940_Get_Speaker_Volume_db(WM8940_t* wm8940);
wm8940_status_t WM8940_Set_Mono_Source(WM8940_t* wm8940, wm8940_speaker_source_t source);
wm8940_status_t WM8940_Set_Mono_FromBypass_Attenuation(WM8940_t* wm8940, uint8_t state);
wm8940_status_t WM8940_Set_Mono_Mute(WM8940_t* wm8940, uint8_t state);
wm8940_status_t WM8940_Set_Output_Enable(WM8940_t* wm8940, wm8940_output_t output);
wm8940_status_t WM8940_Set_VREFToAnalogueOutputResistance(WM8940_t* wm8940, wm8940_vroi_t vroi);
wm8940_status_t WM8940_Set_SlowClock_Enable(WM8940_t* wm8940, uint8_t state);

// Output switch
wm8940_status_t WM8940_Set_ThermalShutdown_Enable(WM8940_t* wm8940, uint8_t state);

// Digital audio interfaces
wm8940_status_t WM8940_Set_Clock(WM8940_t* wm8940, uint8_t is_master, wm8940_bclkdiv_t bclk_divider, wm8940_mclkdiv_t mclk_divider, wm8940_clksel_t clock_source);
wm8940_status_t WM8940_Set_AudioInterfaceFormat(WM8940_t* wm8940, wm8940_audio_iface_fmt_t format, wm8940_audio_iface_wl_t word_length);
wm8940_status_t WM8940_Set_FrameClock_Polarity(WM8940_t* wm8940, uint8_t invert);
wm8940_status_t WM8940_Set_BCLK_Polarity(WM8940_t* wm8940, uint8_t invert);
wm8940_status_t WM8940_Set_LOUTR(WM8940_t* wm8940, uint8_t enable);
wm8940_status_t WM8940_Set_DAC_SwapLRData(WM8940_t* wm8940, uint8_t swap);
wm8940_status_t WM8940_Set_ADC_SwapLRData(WM8940_t* wm8940, uint8_t swap);

// Audio sample rates
wm8940_status_t WM8940_Set_SampleRate(WM8940_t* wm8940, wm8940_sample_rate_t sample_rate);

// Master clock and PLL
wm8940_status_t WM8940_Set_PLL_Enable(WM8940_t* wm8940, uint8_t state);
wm8940_status_t WM8940_Set_PLL_PowerDown(WM8940_t* wm8940, uint8_t state);
wm8940_status_t WM8940_Set_PLL_FrequencyRatio(WM8940_t* wm8940, wm8940_pll_prescaler_t prescaler, uint8_t N, uint32_t K);

// Companding
wm8940_status_t WM8940_Set_ADC_Companding(WM8940_t* wm8940, wm8940_companding_t companding);
wm8940_status_t WM8940_Set_DAC_Companding(WM8940_t* wm8940, wm8940_companding_t companding);
wm8940_status_t WM8940_Set_ADC_Loopback(WM8940_t* wm8940, uint8_t enable);
wm8940_status_t WM8940_Set_DAC_Loopback(WM8940_t* wm8940, uint8_t enable);
wm8940_status_t WM8940_Set_WordLength8(WM8940_t* wm8940, uint8_t enable);

// GPIO control
wm8940_status_t WM8940_Set_GPIO_Control(WM8940_t* wm8940, wm8940_gpio_function_t function, uint8_t invert_polarity);

// Control interface
wm8940_status_t WM8940_Set_ModePin_Function(WM8940_t* wm8940, wm8940_mode_pin_function_t function);
wm8940_status_t WM8940_Set_AutoIncrementalWrite(WM8940_t* wm8940, uint8_t state);

// Readback registers
wm8940_status_t WM8940_Get_ChipID(WM8940_t* wm8940, uint16_t* chip_id);
wm8940_status_t WM8940_Get_DeviceRevision(WM8940_t* wm8940, uint8_t* device_revision);
wm8940_status_t WM8940_Get_ALC_Gain(WM8940_t* wm8940, uint8_t* alc_gain);

// Reset
wm8940_status_t WM8940_SoftwareReset(WM8940_t* wm8940);

// Power management
wm8940_status_t WM8940_Set_VMID_Impedance(WM8940_t* wm8940, wm8940_vmid_impedance_t impedance);
wm8940_status_t WM8940_Set_AnalogueAmplifierBias_Enable(WM8940_t* wm8940, uint8_t state);
wm8940_status_t WM8940_Get_PowerManagement2(WM8940_t* wm8940, uint16_t* status);
inline wm8940_status_t WM8940_Get_PowerManagement_Input(WM8940_t* wm8940, uint16_t* status) { return WM8940_Get_PowerManagement2(wm8940, status); };
wm8940_status_t WM8940_Get_PowerManagement3(WM8940_t* wm8940, uint16_t* status);
inline wm8940_status_t WM8940_Get_PowerManagement_Output(WM8940_t* wm8940, uint16_t* status) { return WM8940_Get_PowerManagement3(wm8940, status); };

// Pop minimisation
wm8940_status_t WM8940_Set_FastVMIDDischarge_Enable(WM8940_t* wm8940, uint8_t state);

#ifdef __cplusplus
}
#endif

#endif // MR_WM8940_H
