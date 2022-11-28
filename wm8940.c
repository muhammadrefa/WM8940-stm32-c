#include "wm8940.h"
#include "wm8940_regs.h"


// Private function prototypes
wm8940_status_t _WM8940_Get_PowerManagement1(WM8940_t* wm8940, uint16_t* regval);
wm8940_status_t _WM8940_Get_PGA_Volume(WM8940_t* wm8940, uint8_t* volume);
wm8940_status_t _WM8940_Get_Speaker_Volume(WM8940_t* wm8940, uint8_t* val);


wm8940_status_t _WM8940_Get_PowerManagement1(WM8940_t* wm8940, uint16_t* regval)
{
    uint16_t val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_POWER_MANAGEMENT_1);
    val &= ~(0x07);
    val |= (wm8940->_vmid_sel & 0x03) << 0;
    val |= (wm8940->_bufioen & 0x01) << 3;
    *regval = val;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Init(WM8940_t* wm8940)
{
    // Datasheet page 64
    WM8940_SoftwareReset(wm8940);
    // Enable VMID_OP_EN and LVLSHIFT_EN
    uint8_t regval = (1 << 8) | (1 << 7);
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_POWER_MANAGEMENT_1, regval);
}

/* ----- Input signal path ----- */
wm8940_status_t WM8940_Get_PGA_Input(WM8940_t* wm8940, wm8940_input_t* input)
{
    uint16_t val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_INPUT_CTRL);
    *input = val & 0x0007;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_PGA_Input(WM8940_t* wm8940, wm8940_input_t input)
{
    uint16_t val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_INPUT_CTRL);
    val &= ~(0x0007);
    val |= (uint16_t)input;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_INPUT_CTRL, val);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_PGA_Volume(WM8940_t* wm8940, uint8_t val)
{
    if (val > 0x3F)
        return WM8940_STATUS_INVALID;

    uint16_t regval = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_INPPGA_GAIN_CTRL);
    regval &= ~(0x003F);
    regval |= (val & 0x003F);
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_INPPGA_GAIN_CTRL, regval);
    wm8940->_vol_pga = val;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_PGA_Volume_db(WM8940_t* wm8940, float vol_db)
{
    if (vol_db < -12)
        return WM8940_STATUS_INVALID;
    else if (vol_db >= 35.25)
        return WM8940_STATUS_INVALID;

    // Round to floor by 0.75
    int16_t rounding = vol_db * 100;
    rounding %= 75;
    if (rounding < 0)
        rounding += 75;
    vol_db -= rounding / 100.0;
    // Convert to val for register
    uint8_t val = (vol_db + 12) / 0.75;
    return WM8940_Set_PGA_Volume(wm8940, val);
}

uint8_t WM8940_Get_PGA_Volume(WM8940_t* wm8940)
{
    return wm8940->_vol_pga;
}

wm8940_status_t _WM8940_Get_PGA_Volume(WM8940_t* wm8940, uint8_t* val)
{
    uint16_t regval = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_INPPGA_GAIN_CTRL);
    *val = regval & 0x003F;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_PGA_Mute(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_INPPGA_GAIN_CTRL);
    val &= ~(1 << 6);
    val |= (state ? 1 : 0) << 6;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_INPPGA_GAIN_CTRL, val);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_PGA_ZeroCross(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_INPPGA_GAIN_CTRL);
    val &= ~(1 << 7);
    val |= (state ? 1 : 0) << 7;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_INPPGA_GAIN_CTRL, val);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_PGA_Enable(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_POWER_MANAGEMENT_2);
    val &= ~(1 << 2);
    val |= (state ? 1 : 0) << 2;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_POWER_MANAGEMENT_2, val);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_Aux_Enable(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val;
    wm8940_status_t status;
    status = _WM8940_Get_PowerManagement1(wm8940, &val);
    if (status != WM8940_STATUS_OK)
        return status;
    val &= ~(1 << 6);
    val |= (state ? 1 : 0) << 6;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_POWER_MANAGEMENT_1, val);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_Aux_Mode(WM8940_t* wm8940, wm8940_aux_mode_t mode)
{
    uint16_t val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_INPUT_CTRL);
    val &= ~(1 << 3);
    val |= mode << 3;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_INPUT_CTRL, val);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_PGA_Boost(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_ADC_BOOST_CTRL);
    val &= ~(1 << 8);
    val |= (state ? 1 : 0) << 8;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_ADC_BOOST_CTRL, val);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_Boost_Volume(WM8940_t* wm8940, wm8940_input_t input, uint8_t vol)
{
    if (vol > 0x07)
        return WM8940_STATUS_INVALID;

    int8_t shift = -1;
    switch (input)
    {
        case WM8940_INPUT_AUX: shift = 0; break;
        case WM8940_INPUT_MICP: shift = 1; break;
        default: break;
    }
    if (shift < 0) return WM8940_STATUS_INVALID;

    uint16_t val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_ADC_BOOST_CTRL);
    val &= ~(0x07 << shift);
    val |= (vol & 0x07) << shift;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_ADC_BOOST_CTRL, val);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_Boost_Enable(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_POWER_MANAGEMENT_2);
    val &= ~(1 << 4);
    val |= (state ? 1 : 0) << 4;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_POWER_MANAGEMENT_2, val);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_MicBias_Enable(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val;
    wm8940_status_t status;
    status = _WM8940_Get_PowerManagement1(wm8940, &val);
    if (status != WM8940_STATUS_OK)
        return status;
    val &= ~(1 << 4);
    val |= (state ? 1 : 0) << 4;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_POWER_MANAGEMENT_1, val);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_MicBias_Voltage(WM8940_t* wm8940, wm8940_micbias_voltage_t percentage)
{
    uint16_t val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_INPUT_CTRL);
    val &= ~(1 << 8);
    val |= percentage;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_INPUT_CTRL, val);
    return WM8940_STATUS_OK;
}

/* ----- ADC ----- */
wm8940_status_t WM8940_Set_ADC_Enable(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_POWER_MANAGEMENT_2);
    val &= ~(1 << 0);
    val |= (state ? 1 : 0) << 0;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_POWER_MANAGEMENT_2, val);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_ADC_Polarity(WM8940_t* wm8940, uint8_t invert)
{
    uint16_t val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_ADC_CTRL);
    val &= ~(1 << 0);
    val |= (invert ? 1 : 0) << 0;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_ADC_CTRL, val);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_ADC_HighPassFilter(WM8940_t* wm8940, uint8_t enable, wm8940_hpf_mode_t mode, uint8_t freq_regval)
{
    if (freq_regval > 0x07)
        return WM8940_STATUS_INVALID;

    uint16_t val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_ADC_CTRL);
    val &= ~(0x1F << 4);
    val |= (enable ? 1 : 0) << 8;
    val |= (mode ? 1 : 0) << 7;
    val |= (freq_regval & 0x07) << 4;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_ADC_CTRL, val);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_ADC_Volume(WM8940_t* wm8940, uint8_t regval)
{
    if (regval > 0xFF)
        return WM8940_STATUS_INVALID;

    uint16_t val = 0;
    val |= (regval & 0xFF) << 0;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_ADC_DIGITAL_VOL, val);
    return WM8940_STATUS_OK;
}

/* ----- ALC ----- */
wm8940_status_t WM8940_Set_ALC_Enable(WM8940_t* wm8940, uint8_t state)
{
    uint16_t regval = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_ALC_CTRL_1);
    regval &= ~(1 << 8);
    regval |= (state ? 1 : 0) << 8;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_ALC_CTRL_1, regval);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_ALC_Gain(WM8940_t* wm8940, uint8_t minval, uint8_t maxval)
{
    if ((minval > 0x07) || (maxval > 0x07))
        return WM8940_STATUS_INVALID;

    uint16_t regval = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_ALC_CTRL_1);
    regval &= ~(0x3F << 0);
    regval |= (minval & 0x07) << 0;
    regval |= (maxval & 0x07) << 3;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_ALC_CTRL_1, regval);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_ALC_Level(WM8940_t* wm8940, uint8_t val)
{
    if (val > 0x0F)
        return WM8940_STATUS_INVALID;
    
    uint16_t regval = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_ALC_CTRL_2);
    regval &= ~(0x0F << 0);
    regval |= (val & 0x0F) << 0;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_ALC_CTRL_2, regval);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_ALC_Hold(WM8940_t* wm8940, uint8_t val)
{
    if (val > 0x0F)
        return WM8940_STATUS_INVALID;
    
    uint16_t regval = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_ALC_CTRL_2);
    regval &= ~(0x0F << 4);
    regval |= (val & 0x0F) << 4;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_ALC_CTRL_2, regval);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_ALC_Mode(WM8940_t* wm8940, wm8940_alc_mode_t mode)
{
    uint16_t regval = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_ALC_CTRL_3);
    regval &= ~(1 << 8);
    regval |= (mode & 0x01) << 8;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_ALC_CTRL_3, regval);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_ALC_DecayTime(WM8940_t* wm8940, uint8_t val)
{
    if (val > 0x0F)
        return WM8940_STATUS_INVALID;
    
    uint16_t regval = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_ALC_CTRL_3);
    regval &= ~(0x0F << 4);
    regval |= (val & 0x0F) << 4;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_ALC_CTRL_3, regval);
    return WM8940_STATUS_OK;
}
wm8940_status_t WM8940_Set_ALC_AttackTime(WM8940_t* wm8940, uint8_t val)
{
    if (val > 0x0F)
        return WM8940_STATUS_INVALID;
    
    uint16_t regval = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_ALC_CTRL_3);
    regval &= ~(0x0F << 0);
    regval |= (val & 0x0F) << 0;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_ALC_CTRL_3, regval);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_ALC_ZeroCross(WM8940_t* wm8940, uint8_t state)
{
    uint16_t regval = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_ALC_CTRL_4);
    regval &= ~(1 << 1);
    regval |= (state ? 1 : 0) << 1;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_ALC_CTRL_4, regval);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_ALC_NoiseGate_Threshold(WM8940_t* wm8940, uint8_t val)
{
    if (val > 7)
        return WM8940_STATUS_INVALID;

    uint16_t regval = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_NOISE_GATE);
    regval &= ~(0x07 << 0);
    regval |= (val & 0x07) << 0;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_NOISE_GATE, regval);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_ALC_NoiseGate_Enable(WM8940_t* wm8940, uint8_t state)
{
    uint16_t regval = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_NOISE_GATE);
    regval &= ~(1 << 3);
    regval |= (state ? 1 : 0) << 3;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_NOISE_GATE, regval);
    return WM8940_STATUS_OK;
}

/* ----- DAC ----- */
wm8940_status_t WM8940_Set_DAC_Enable(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_POWER_MANAGEMENT_3);
    val &= ~(1 << 0);
    val |= (state ? 1 : 0) << 0;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_POWER_MANAGEMENT_3, val);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_DAC_Polarity(WM8940_t* wm8940, uint8_t invert)
{
    uint16_t val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_DAC_CTRL);
    val &= ~(1 << 0);
    val |= (invert ? 1 : 0) << 0;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_DAC_CTRL, val);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_DAC_Volume(WM8940_t* wm8940, uint8_t regval)
{
    uint16_t val = 0;
    val |= (regval & 0xFF) << 0;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_DAC_DIGITAL_VOL, val);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_DAC_SoftMute(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_DAC_CTRL);
    val &= ~(1 << 6);
    val |= (state ? 1 : 0) << 6;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_DAC_CTRL, val);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_DAC_AutoMute(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_DAC_CTRL);
    val &= ~(1 << 2);
    val |= (state ? 1 : 0) << 2;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_DAC_CTRL, val);
    return WM8940_STATUS_OK;
}

/* ----- Analogue outputs ----- */
wm8940_status_t WM8940_Set_Speaker_Source(WM8940_t* wm8940, wm8940_speaker_source_t source)
{
    uint16_t val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_SPK_MIXER);
    val &= ~(0x03 << 0);
    val &= ~(1 << 5);
    if (source == WM8940_OUTPUT_FROM_AUX) source = (1 << 5);
    val |= source;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_SPK_MIXER, val);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_Speaker_FromBypass_Attenuation(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_SPK_VOL_CTRL);
    val &= ~(1 << 8);
    val |= (state ? 1 : 0) << 8;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_SPK_VOL_CTRL, val);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_Speaker_ZeroCross(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_SPK_VOL_CTRL);
    val &= ~(1 << 7);
    val |= (state ? 1 : 0) << 7;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_SPK_VOL_CTRL, val);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_Speaker_Mute(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_SPK_VOL_CTRL);
    val &= ~(1 << 6);
    val |= (state ? 1 : 0) << 6;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_SPK_VOL_CTRL, val);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_Speaker_Volume(WM8940_t* wm8940, uint8_t val)
{
    if (val > 0x3F)
        return WM8940_STATUS_INVALID;

    uint16_t regval = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_SPK_VOL_CTRL);
    regval &= ~(0x3F << 0);
    regval |= (val & 0x3F) << 0;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_SPK_VOL_CTRL, regval);
    wm8940->_vol_spk = val;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_Speaker_Volume_db(WM8940_t* wm8940, int8_t vol_db)
{
    if (vol_db < -57)
        return WM8940_STATUS_INVALID;
    else if (vol_db >= 6)
        return WM8940_STATUS_INVALID;

    return WM8940_Set_Speaker_Volume(wm8940, WM8940_VOL_DB_TO_REG_VALUE(vol_db));
}

uint8_t WM8940_Get_Speaker_Volume(WM8940_t* wm8940)
{
    return wm8940->_vol_spk;
}

uint8_t WM8940_Get_Speaker_Volume_db(WM8940_t* wm8940)
{
    return WM8940_SPKVOL_REG_VALUE_TO_DB(wm8940->_vol_spk);
}

wm8940_status_t _WM8940_Get_Speaker_Volume(WM8940_t* wm8940, uint8_t* val)
{
    uint16_t regval = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_SPK_VOL_CTRL);
    regval &= 0x3F;
    *val = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_Mono_Source(WM8940_t* wm8940, wm8940_speaker_source_t source)
{
    uint16_t val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_MONO_MIXER_CTRL);
    val &= ~(0x07 << 0);
    val |= source;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_MONO_MIXER_CTRL, val);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_Mono_FromBypass_Attenuation(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_MONO_MIXER_CTRL);
    val &= ~(1 << 7);
    val |= (state ? 1 : 0) << 7;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_MONO_MIXER_CTRL, val);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_Mono_Mute(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_MONO_MIXER_CTRL);
    val &= ~(1 << 6);
    val |= (state ? 1 : 0) << 6;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_MONO_MIXER_CTRL, val);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_Output_Enable(WM8940_t* wm8940, wm8940_output_t output)
{
    uint16_t val = 0;
    if (output == WM8940_OUTPUT_SPK)
        val |= WM8940_PM3_SPKMIX | WM8940_PM3_SPKP | WM8940_PM3_SPKN;
    if (output == WM8940_OUTPUT_MONO)
        val |= WM8940_PM3_MONOMIX | WM8940_PM3_MONO;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_POWER_MANAGEMENT_3, val);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_VREFToAnalogueOutputResistance(WM8940_t* wm8940, wm8940_vroi_t vroi)
{
    uint16_t val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_OUTPUT_CTRL);
    val &= ~(1 << 0);
    val |= (vroi & 0x01) << 0;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_OUTPUT_CTRL, val);
    return WM8940_STATUS_OK;
}

/* ----- Output switch ----- */
wm8940_status_t WM8940_Set_ThermalShutdown_Enable(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_OUTPUT_CTRL);
    val &= ~(1 << 1);
    val |= (state ? 1 : 0) << 1;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_OUTPUT_CTRL, val);
    return WM8940_STATUS_OK;
}

/* ----- Digital audio interfaces ----- */
wm8940_status_t WM8940_Set_Clock(WM8940_t* wm8940, uint8_t is_master, wm8940_bclkdiv_t bclk_divider, wm8940_mclkdiv_t mclk_divider, wm8940_clksel_t clock_source)
{
    if (bclk_divider >= 6)
        return WM8940_STATUS_INVALID;
    if (mclk_divider >= 8)
        return WM8940_STATUS_INVALID;
    if (clock_source >= 2)
        return WM8940_STATUS_INVALID;

    uint16_t val = 0;
    val |= (is_master ? 1 : 0) << 0;
    val |= bclk_divider << 2;
    val |= mclk_divider << 5;
    val |= clock_source << 8;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_CLOCK_GEN_CTRL, val);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_AudioInterfaceFormat(WM8940_t* wm8940, wm8940_audio_iface_fmt_t format)
{
    uint16_t val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_AUDIO_INTERFACE);
    val &= ~(0x03 << 3);
    val |= (format << 3);
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_AUDIO_INTERFACE, val);
    return WM8940_STATUS_OK;
}

/* ----- Audio sample rates ----- */
wm8940_status_t WM8940_Set_SampleRate(WM8940_t* wm8940, wm8940_sample_rate_t sample_rate)
{
    uint16_t val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_ADDITIONAL_CTRL);
    val &= ~(0x07 << 1);
    val |= (sample_rate << 1);
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_ADDITIONAL_CTRL, val);
    return WM8940_STATUS_OK;
}

/* ----- Master clock and PLL ----- */
wm8940_status_t WM8940_Set_PLL_Enable(WM8940_t* wm8940, uint8_t state)
{
    if (state && !wm8940->_vmid_sel)
        return WM8940_STATUS_INVALID;

    uint16_t val;
    wm8940_status_t status;
    status = _WM8940_Get_PowerManagement1(wm8940, &val);
    if (status != WM8940_STATUS_OK)
        return status;
    val &= ~(1 << 5);
    val |= (state ? 1 : 0) << 5;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_POWER_MANAGEMENT_1, val);
    val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_PLL_N);
    val &= ~(1 << 7);
    val |= (state ? 0 : 1) << 7;
    return WM8940_STATUS_OK;

}
wm8940_status_t WM8940_Set_PLL_PowerDown(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_PLL_N);
    val &= ~(1 << 7);
    val |= (state ? 1 : 0) << 7;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_PLL_N, val);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_PLL_FrequencyRatio(WM8940_t* wm8940, wm8940_pll_prescaler_t prescaler, uint8_t N, uint32_t K)
{
    if (N < 5 || N > 13)
        return WM8940_STATUS_INVALID;
    if (K > 0xFFFFFF)
        return WM8940_STATUS_INVALID;
    
    uint16_t n_regval = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_PLL_N) & (1 << 7);
    n_regval |= (N & 0x0F) | ((prescaler & 0x03) << 4) | ((K ? 1 : 0) << 6);
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_PLL_K3, K & 0x1FF);
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_PLL_K2, (K >> 9) & 0x1FF);
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_PLL_K1, (K >> 18) & 0x3F);
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_PLL_N, n_regval);
    return WM8940_STATUS_OK;
}

/* ----- Companding ----- */
wm8940_status_t WM8940_Set_ADC_Companding(WM8940_t* wm8940, wm8940_companding_t companding)
{
    if (companding == 2 || companding > 3)
        return WM8940_STATUS_INVALID;
    uint16_t val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_COMPANDING_CTRL);
    val &= ~(0x03 << 1);
    val |= companding << 1;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_COMPANDING_CTRL, val);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_DAC_Companding(WM8940_t* wm8940, wm8940_companding_t companding)
{
    if (companding == 2 || companding > 3)
        return WM8940_STATUS_INVALID;
    uint16_t val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_COMPANDING_CTRL);
    val &= ~(0x03 << 3);
    val |= companding << 3;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_COMPANDING_CTRL, val);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_ADC_Loopback(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_COMPANDING_CTRL);
    val &= ~(1 << 0);
    val |= (state ? 1 : 0) << 1;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_COMPANDING_CTRL, val);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_DAC_Loopback(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_COMPANDING_CTRL);
    val &= ~(1 << 6);
    val |= (state ? 1 : 0) << 6;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_COMPANDING_CTRL, val);
    return WM8940_STATUS_OK;
}

/* ----- GPIO control ----- */
wm8940_status_t WM8940_Set_GPIO_Control(WM8940_t* wm8940, wm8940_gpio_function_t function, uint8_t invert_polarity)
{
    if (function > 5)
        return WM8940_STATUS_INVALID;

    uint16_t val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_GPIO_CTRL);
    
    if (!(val & (1 << 7)))  // Mode pin as mode selector
        return WM8940_STATUS_INVALID;
    
    val &= (0x07 << 0);
    val |= function << 0;
    val |= (invert_polarity ? 1 : 0) << 3;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_GPIO_CTRL, val);
    return WM8940_STATUS_OK;
}

/* ----- Control interface ----- */
wm8940_status_t WM8940_Set_ModePin_Function(WM8940_t* wm8940, wm8940_mode_pin_function_t function)
{
    uint16_t val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_GPIO_CTRL);
    val &= (1 << 7);
    val |= (function ? 1 : 0) << 7;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_GPIO_CTRL, val);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_AutoIncrementalWrite(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_CONTROL_INTERFACE);
    val &= ~(1 << 1);
    val |= (state ? 1 : 0) << 1;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_CONTROL_INTERFACE, val);
    return WM8940_STATUS_OK;
}

/* ----- Readback registers ----- */
wm8940_status_t WM8940_Get_ChipID(WM8940_t* wm8940, uint16_t* chip_id)
{
    *chip_id = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_SOFTWARE_RESET);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Get_DeviceRevision(WM8940_t* wm8940, uint8_t* device_revision)
{
    *device_revision = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_POWER_MANAGEMENT_1) & 0x03;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Get_ALC_Gain(WM8940_t* wm8940, uint8_t* alc_gain)
{
    *alc_gain = (WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_ALC_CTRL_1) >> 10) & 0x3F;
    return WM8940_STATUS_OK;
}

/* ----- Reset ----- */
wm8940_status_t WM8940_SoftwareReset(WM8940_t* wm8940)
{
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_SOFTWARE_RESET, 0);
    return WM8940_STATUS_OK;
}

/* ----- Power management ----- */
wm8940_status_t WM8940_Set_VMID_Impedance(WM8940_t* wm8940, wm8940_vmid_impedance_t impedance)
{
    if (impedance < 0 || impedance > 3)
        return WM8940_STATUS_INVALID;

    uint16_t val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_POWER_MANAGEMENT_1);
    val &= ~(0x03 << 0);
    val |= impedance << 0;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_POWER_MANAGEMENT_1, val);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_AnalogueAmplifierBias_Enable(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_POWER_MANAGEMENT_1);
    val &= ~(1 << 3);
    val |= (state ? 1 : 0) << 3;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_POWER_MANAGEMENT_1, val);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Get_PowerManagement2(WM8940_t* wm8940, uint16_t* status)
{
    *status = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_POWER_MANAGEMENT_2);
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Get_PowerManagement3(WM8940_t* wm8940, uint16_t* status)
{
    *status = WM8940_REG_READ(wm8940->comm_handle, WM8940_REG_POWER_MANAGEMENT_3);
    return WM8940_STATUS_OK;
}
