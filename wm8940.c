#include "wm8940.h"
#include "wm8940_regs.h"


uint16_t _WM8940_Get_PowerManagement1(WM8940_t* wm8940)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_POWER_MANAGEMENT_1);
    val &= ~(0x07);
    val |= (wm8940->_vmid_sel & 0x03) << 0;
    val |= (wm8940->_bufioen & 0x01) << 3;
    return val;
}

// TODO: WM8940 init
void WM8940_Init(WM8940_t* wm8940)
{
    // Datasheet page 64
    WM8940_SoftwareReset(wm8940);
    // Enable VMID_OP_EN and LVLSHIFT_EN
    // Enable DAC soft mute
    // Set clock source
    // Enable POB_CTRL and VMID soft start
    
}

// TODO: WM8940 deinit
void WM8940_Deinit(WM8940_t* wm8940)
{
    // Datasheet page 65
}

/* ----- Input signal path ----- */
wm8940_input_t WM8940_Get_PGA_Input(WM8940_t* wm8940)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_INPUT_CTRL);
    return val & 0x0007;
}

void WM8940_Set_PGA_Input(WM8940_t* wm8940, wm8940_input_t input)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_INPUT_CTRL);
    val &= ~(0x0007);
    val |= (uint16_t)input;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_INPUT_CTRL, val);
}

uint8_t WM8940_Get_PGA_Volume(WM8940_t* wm8940)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_INPPGA_GAIN_CTRL);
    return val & 0x003F;
}

void WM8940_Set_PGA_Volume(WM8940_t* wm8940, uint8_t regval)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_INPPGA_GAIN_CTRL);
    val &= ~(0x003F);
    val |= (regval & 0x003F);
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_INPPGA_GAIN_CTRL, val);
}

// TODO
void WM8940_Set_PGA_Volume_db(WM8940_t* wm8940, float vol_db)
{
    if (vol_db < -12)
        vol_db = -12;
    else if (vol_db >= 35.25)
        vol_db = 35.25;

    // Round to floor by 0.25
    // Convert to regval
    uint8_t regval = (vol_db + 12) / 0.75;
}

void WM8940_Set_PGA_Mute(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_INPPGA_GAIN_CTRL);
    val &= ~(1 << 6);
    val |= (state ? 1 : 0) << 6;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_INPPGA_GAIN_CTRL, val);
}

void WM8940_Set_PGA_ZeroCross(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_INPPGA_GAIN_CTRL);
    val &= ~(1 << 7);
    val |= (state ? 1 : 0) << 7;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_INPPGA_GAIN_CTRL, val);
}

void WM8940_Set_PGA_Enable(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_POWER_MANAGEMENT_2);
    val &= ~(1 << 2);
    val |= (state ? 1 : 0) << 2;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_POWER_MANAGEMENT_2, val);
}

void WM8940_Set_Aux_Enable(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = _WM8940_Get_PowerManagement1(wm8940);;
    val &= ~(1 << 6);
    val |= (state ? 1 : 0) << 6;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_POWER_MANAGEMENT_1, val);
}

void WM8940_Set_Aux_Mode(WM8940_t* wm8940, wm8940_aux_mode_t mode)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_INPUT_CTRL);
    val &= ~(1 << 3);
    val |= mode << 3;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_INPUT_CTRL, val);
}

void WM8940_Set_PGA_Boost(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_ADC_BOOST_CTRL);
    val &= ~(1 << 8);
    val |= (state ? 1 : 0) << 8;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_ADC_BOOST_CTRL, val);
}

void WM8940_Set_Boost_Volume(WM8940_t* wm8940, wm8940_input_t input, uint8_t vol)
{
    int8_t shift = -1;
    switch (input)
    {
        case WM8940_INPUT_AUX: shift = 0; break;
        case WM8940_INPUT_MICP: shift = 1; break;
        default: break;
    }
    if (shift < 0) return;

    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_ADC_BOOST_CTRL);
    val &= ~(0x07 << shift);
    val |= (vol & 0x07) << shift;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_ADC_BOOST_CTRL, val);
}

void WM8940_Set_Boost_Enable(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_POWER_MANAGEMENT_2);
    val &= ~(1 << 4);
    val |= (state ? 1 : 0) << 4;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_POWER_MANAGEMENT_2, val);
}

void WM8940_Set_MicBias_Enable(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = _WM8940_Get_PowerManagement1(wm8940);
    val &= ~(1 << 4);
    val |= (state ? 1 : 0) << 4;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_POWER_MANAGEMENT_1, val);
}

void WM8940_Set_MicBias_Voltage(WM8940_t* wm8940, wm8940_micbias_voltage_t percentage)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_INPUT_CTRL);
    val &= ~(1 << 8);
    val |= percentage;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_INPUT_CTRL, val);
}

/* ----- ADC ----- */
void WM8940_Set_ADC_Enable(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_POWER_MANAGEMENT_2);
    val &= ~(1 << 0);
    val |= (state ? 1 : 0) << 0;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_POWER_MANAGEMENT_2, val);
}

void WM8940_Set_ADC_Polarity(WM8940_t* wm8940, uint8_t invert)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_ADC_CTRL);
    val &= ~(1 << 0);
    val |= (invert ? 1 : 0) << 0;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_ADC_CTRL, val);
}

void WM8940_Set_ADC_HighPassFilter(WM8940_t* wm8940, uint8_t enable, wm8940_hpf_mode_t mode, uint8_t freq_regval)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_ADC_CTRL);
    val &= ~(0x1F << 4);
    val |= (enable ? 1 : 0) << 8;
    val |= (mode ? 1 : 0) << 7;
    val |= (freq_regval & 0x07) << 4;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_ADC_CTRL, val);
}

void WM8940_Set_ADC_Volume(WM8940_t* wm8940, uint8_t regval)
{
    uint16_t val = 0;
    val |= (regval & 0xFF) << 0;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_ADC_DIGITAL_VOL, val);
}

/* ----- DAC ----- */
void WM8940_Set_DAC_Enable(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_POWER_MANAGEMENT_3);
    val &= ~(1 << 0);
    val |= (state ? 1 : 0) << 0;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_POWER_MANAGEMENT_3, val);
}

void WM8940_Set_DAC_Polarity(WM8940_t* wm8940, uint8_t invert)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_DAC_CTRL);
    val &= ~(1 << 0);
    val |= (invert ? 1 : 0) << 0;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_DAC_CTRL, val);
}

void WM8940_Set_DAC_Volume(WM8940_t* wm8940, uint8_t regval)
{
    uint16_t val = 0;
    val |= (regval & 0xFF) << 0;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_DAC_DIGITAL_VOL, val);
}

void WM8940_Set_DAC_Mute(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_DAC_CTRL);
    val &= ~(1 << 6);
    val |= (state ? 1 : 0) << 6;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_DAC_CTRL, val);
}

void WM8940_Set_DAC_AutoMute(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_DAC_CTRL);
    val &= ~(1 << 2);
    val |= (state ? 1 : 0) << 2;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_DAC_CTRL, val);
}

/* ----- Analogue outputs ----- */
void WM8940_Set_Speaker_Source(WM8940_t* wm8940, wm8940_speaker_source_t source)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_SPK_MIXER);
    val &= ~(0x03 << 0);
    val &= ~(1 << 5);
    if (source == WM8940_OUTPUT_FROM_AUX) source = (1 << 5);
    val |= source;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_SPK_MIXER, val);
}

void WM8940_Set_Speaker_FromBypass_Attenuation(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_SPK_VOL_CTRL);
    val &= ~(1 << 8);
    val |= (state ? 1 : 0) << 8;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_SPK_VOL_CTRL, val);
}

void WM8940_Set_Speaker_ZeroCross(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_SPK_VOL_CTRL);
    val &= ~(1 << 7);
    val |= (state ? 1 : 0) << 7;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_SPK_VOL_CTRL, val);
}

void WM8940_Set_Speaker_Mute(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_SPK_VOL_CTRL);
    val &= ~(1 << 6);
    val |= (state ? 1 : 0) << 6;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_SPK_VOL_CTRL, val);
}

void WM8940_Set_Speaker_Volume(WM8940_t* wm8940, uint8_t regval)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_SPK_VOL_CTRL);
    val &= ~(0x3F << 0);
    val |= (regval & 0x3F) << 0;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_SPK_VOL_CTRL, val);
}

void WM8940_Set_Speaker_Volume_db(WM8940_t* wm8940, int8_t vol_db)
{
    if (vol_db < -57)
        vol_db = -57;
    else if (vol_db >= 6)
        vol_db = 6;
    uint8_t regval = WM8940_VOL_DB_TO_REG_VALUE(vol_db);

    WM8940_Set_Speaker_Volume(wm8940, regval);
}

void WM8940_Set_Mono_Source(WM8940_t* wm8940, wm8940_speaker_source_t source)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_MONO_MIXER_CTRL);
    val &= ~(0x07 << 0);
    val |= source;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_MONO_MIXER_CTRL, val);
}

void WM8940_Set_Mono_FromBypass_Attenuation(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_MONO_MIXER_CTRL);
    val &= ~(1 << 7);
    val |= (state ? 1 : 0) << 7;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_MONO_MIXER_CTRL, val);
}

void WM8940_Set_Mono_Mute(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_MONO_MIXER_CTRL);
    val &= ~(1 << 6);
    val |= (state ? 1 : 0) << 6;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_MONO_MIXER_CTRL, val);
}

void WM8940_Set_Output_Enable(WM8940_t* wm8940, wm8940_output_t output)
{
    uint16_t val = 0;
    if (output == WM8940_OUTPUT_SPK)
        val |= WM8940_PM3_SPKMIX | WM8940_PM3_SPKP | WM8940_PM3_SPKN;
    if (output == WM8940_OUTPUT_MONO)
        val |= WM8940_PM3_MONOMIX | WM8940_PM3_MONO;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_POWER_MANAGEMENT_3, val);
}

/* ----- Digital audio interfaces ----- */
void WM8940_Set_Clock(WM8940_t* wm8940, uint8_t is_master, wm8940_bclkdiv_t bclk_divider, wm8940_mclkdiv_t mclk_divider, wm8940_clksel_t clock_source)
{
    if (bclk_divider >= 6)
        return;
    if (mclk_divider >= 8)
        return;
    if (clock_source >= 2)
        return;

    uint16_t val = 0;
    val |= (is_master ? 1 : 0) << 0;
    val |= bclk_divider << 2;
    val |= mclk_divider << 5;
    val |= clock_source << 8;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_CLOCK_GEN_CTRL, val);
}

void WM8940_Set_AudioInterfaceFormat(WM8940_t* wm8940, wm8940_audio_iface_fmt_t format)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_AUDIO_INTERFACE);
    val &= ~(0x03 << 3);
    val |= (format << 3);
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_AUDIO_INTERFACE, val);
}

/* ----- Audio sample rates ----- */
void WM8940_Set_SampleRate(WM8940_t* wm8940, wm8940_sample_rate_t sample_rate)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_ADDITIONAL_CTRL);
    val &= ~(0x07 << 1);
    val |= (sample_rate << 1);
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_ADDITIONAL_CTRL, val);
}

/* ----- Master clock and PLL ----- */
void WM8940_Set_PLL_Enable(WM8940_t* wm8940, uint8_t state)
{
    if (state && !wm8940->_vmid_sel)
        return;
    uint16_t val = _WM8940_Get_PowerManagement1(wm8940);
    val &= ~(1 << 5);
    val |= (state ? 1 : 0) << 5;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_POWER_MANAGEMENT_1, val);
    val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_PLL_N);
    val &= ~(1 << 7);
    val |= (state ? 0 : 1) << 7;

}
void WM8940_Set_PLL_PowerDown(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_PLL_N);
    val &= ~(1 << 7);
    val |= (state ? 1 : 0) << 7;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_PLL_N, val);
}

/* Companding */
void WM8940_Set_ADC_Companding(WM8940_t* wm8940, wm8940_companding_t companding)
{
    if (companding == 2 || companding > 3)
        return;
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_COMPANDING_CTRL);
    val &= ~(0x03 << 1);
    val |= companding << 1;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_COMPANDING_CTRL, val);
}

void WM8940_Set_DAC_Companding(WM8940_t* wm8940, wm8940_companding_t companding)
{
    if (companding == 2 || companding > 3)
        return;
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_COMPANDING_CTRL);
    val &= ~(0x03 << 3);
    val |= companding << 3;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_COMPANDING_CTRL, val);
}

void WM8940_Set_ADC_Loopback(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_COMPANDING_CTRL);
    val &= ~(1 << 0);
    val |= (state ? 1 : 0) << 1;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_COMPANDING_CTRL, val);
}

void WM8940_Set_DAC_Loopback(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_COMPANDING_CTRL);
    val &= ~(1 << 6);
    val |= (state ? 1 : 0) << 6;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_COMPANDING_CTRL, val);
}

/* ----- GPIO control ----- */
void WM8940_Set_GPIO_Control(WM8940_t* wm8940, wm8940_gpio_function_t function, uint8_t invert_polarity)
{
    if (function > 5)
        return;

    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_GPIO_CTRL);
    
    if (!(val & (1 << 7)))  // Mode pin as mode selector
        return;
    
    val &= (0x07 << 0);
    val |= function << 0;
    val |= (invert_polarity ? 1 : 0) << 3;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_GPIO_CTRL, val);
}

/* ----- Control interface ----- */
void WM8940_Set_ModePin_Function(WM8940_t* wm8940, wm8940_mode_pin_function_t function)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_GPIO_CTRL);
    val &= (1 << 7);
    val |= (function ? 1 : 0) << 7;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_GPIO_CTRL, val);
}

/* ----- Readback registers ----- */
uint8_t WM8940_Get_ChipID(WM8940_t* wm8940)
{
    return WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_SOFTWARE_RESET);
}

uint8_t WM8940_Get_DeviceRevision(WM8940_t* wm8940)
{
    return WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_POWER_MANAGEMENT_1) & 0x03;
}

uint8_t WM8940_Get_ALC_Gain(WM8940_t* wm8940)
{
    return (WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_ALC_CTRL_1) >> 10) & 0x3F;
}

/* ----- Reset ----- */
void WM8940_SoftwareReset(WM8940_t* wm8940)
{
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_SOFTWARE_RESET, 0);
}
