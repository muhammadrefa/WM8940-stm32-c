#include "wm8940.h"
#include "wm8940_regs.h"


wm8940_status_t WM8940_Init(WM8940_t* wm8940)
{
    // Datasheet page 64
    WM8940_SoftwareReset(wm8940);

    // Power up sequence

    // Enable VMID_OP_EN and LVLSHIFT_EN
    wm8940->_register[WM8940_REG_POWER_MANAGEMENT_1] |= (1 << 8) | (1 << 7);
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_POWER_MANAGEMENT_1, wm8940->_register[WM8940_REG_POWER_MANAGEMENT_1]);

    // Enable POB_CTRL and VMID SOFT_START
    wm8940->_register[WM8940_REG_ADDITIONAL_CTRL] |= (1 << 6) | (1 << 5);
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_ADDITIONAL_CTRL, wm8940->_register[WM8940_REG_ADDITIONAL_CTRL]);

    // Set VMIDSEL to 50 ohm
    wm8940->_register[WM8940_REG_POWER_MANAGEMENT_1] |= (2 << 0);
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_POWER_MANAGEMENT_1, wm8940->_register[WM8940_REG_POWER_MANAGEMENT_1]);

    // Wait for the VMID supply to settle
    HAL_Delay(500);

    // Enable analogue amplifier BIASEN and VMID buffer BUFIOEN
    wm8940->_register[WM8940_REG_POWER_MANAGEMENT_1] |= (1 << 3) | (1 << 2);
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_POWER_MANAGEMENT_1, wm8940->_register[WM8940_REG_POWER_MANAGEMENT_1]);

    // Disable POB_CTRL and VMID SOFT_START
    wm8940->_register[WM8940_REG_ADDITIONAL_CTRL] &= ~((1 << 6) | (1 << 5));
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_ADDITIONAL_CTRL, wm8940->_register[WM8940_REG_ADDITIONAL_CTRL]);

    return WM8940_STATUS_OK;
}

/* ----- Input signal path ----- */
wm8940_status_t WM8940_Get_PGA_Input(WM8940_t* wm8940, wm8940_input_t* input)
{
    uint16_t val = wm8940->_register[WM8940_REG_INPUT_CTRL];
    *input = val & 0x0007;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_PGA_Input(WM8940_t* wm8940, wm8940_input_t input)
{
    uint16_t regval = wm8940->_register[WM8940_REG_INPUT_CTRL];
    regval &= ~(0x0007);
    regval |= (uint16_t)input;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_INPUT_CTRL, regval);
    wm8940->_register[WM8940_REG_INPUT_CTRL] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_PGA_Volume(WM8940_t* wm8940, uint8_t val)
{
    if (val > 0x3F)
        return WM8940_STATUS_INVALID;

    uint16_t regval = wm8940->_register[WM8940_REG_INPPGA_GAIN_CTRL];
    regval &= ~(0x003F);
    regval |= (val & 0x003F);
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_INPPGA_GAIN_CTRL, regval);
    wm8940->_register[WM8940_REG_INPPGA_GAIN_CTRL] = regval;
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
    return wm8940->_register[WM8940_REG_INPPGA_GAIN_CTRL] & 0x003F;
}

wm8940_status_t WM8940_Set_PGA_Mute(WM8940_t* wm8940, uint8_t state)
{
    uint16_t regval = wm8940->_register[WM8940_REG_INPPGA_GAIN_CTRL];
    regval &= ~(1 << 6);
    regval |= (state ? 1 : 0) << 6;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_INPPGA_GAIN_CTRL, regval);
    wm8940->_register[WM8940_REG_INPPGA_GAIN_CTRL] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_PGA_ZeroCross(WM8940_t* wm8940, uint8_t state)
{
    uint16_t regval = wm8940->_register[WM8940_REG_INPPGA_GAIN_CTRL];
    regval &= ~(1 << 7);
    regval |= (state ? 1 : 0) << 7;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_INPPGA_GAIN_CTRL, regval);
    wm8940->_register[WM8940_REG_INPPGA_GAIN_CTRL] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_PGA_Enable(WM8940_t* wm8940, uint8_t enable)
{
    uint16_t regval = wm8940->_register[WM8940_REG_POWER_MANAGEMENT_2];
    regval &= ~(1 << 2);
    regval |= (enable ? 1 : 0) << 2;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_POWER_MANAGEMENT_2, regval);
    wm8940->_register[WM8940_REG_POWER_MANAGEMENT_2] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_Aux_Enable(WM8940_t* wm8940, uint8_t enable)
{
    uint16_t regval = wm8940->_register[WM8940_REG_POWER_MANAGEMENT_1];
    regval &= ~(1 << 6);
    regval |= (enable ? 1 : 0) << 6;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_POWER_MANAGEMENT_1, regval);
    wm8940->_register[WM8940_REG_POWER_MANAGEMENT_1] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_Aux_Mode(WM8940_t* wm8940, wm8940_aux_mode_t mode)
{
    uint16_t regval = wm8940->_register[WM8940_REG_INPUT_CTRL];
    regval &= ~(1 << 3);
    regval |= mode << 3;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_INPUT_CTRL, regval);
    wm8940->_register[WM8940_REG_INPUT_CTRL] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_PGA_Boost(WM8940_t* wm8940, uint8_t state)
{
    uint16_t regval = wm8940->_register[WM8940_REG_ADC_BOOST_CTRL];
    regval &= ~(1 << 8);
    regval |= (state ? 1 : 0) << 8;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_ADC_BOOST_CTRL, regval);
    wm8940->_register[WM8940_REG_ADC_BOOST_CTRL] = regval;
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
        case WM8940_INPUT_MICP: shift = 4; break;
        default: break;
    }
    if (shift < 0) return WM8940_STATUS_INVALID;

    uint16_t regval = wm8940->_register[WM8940_REG_ADC_BOOST_CTRL];
    regval &= ~(0x07 << shift);
    regval |= (vol & 0x07) << shift;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_ADC_BOOST_CTRL, regval);
    wm8940->_register[WM8940_REG_ADC_BOOST_CTRL] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_Boost_Enable(WM8940_t* wm8940, uint8_t enable)
{
    uint16_t regval = wm8940->_register[WM8940_REG_POWER_MANAGEMENT_2];
    regval &= ~(1 << 4);
    regval |= (enable ? 1 : 0) << 4;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_POWER_MANAGEMENT_2, regval);
    wm8940->_register[WM8940_REG_POWER_MANAGEMENT_2] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_MicBias_Enable(WM8940_t* wm8940, uint8_t enable)
{
    uint16_t regval = wm8940->_register[WM8940_REG_POWER_MANAGEMENT_1];
    regval &= ~(1 << 4);
    regval |= (enable ? 1 : 0) << 4;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_POWER_MANAGEMENT_1, regval);
    wm8940->_register[WM8940_REG_POWER_MANAGEMENT_1] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_MicBias_Voltage(WM8940_t* wm8940, wm8940_micbias_voltage_t percentage)
{
    uint16_t regval = wm8940->_register[WM8940_REG_INPUT_CTRL];
    regval &= ~(1 << 8);
    regval |= percentage;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_INPUT_CTRL, regval);
    wm8940->_register[WM8940_REG_INPUT_CTRL] = regval;
    return WM8940_STATUS_OK;
}

/* ----- ADC ----- */
wm8940_status_t WM8940_Set_ADC_Enable(WM8940_t* wm8940, uint8_t enable)
{
    uint16_t regval = wm8940->_register[WM8940_REG_POWER_MANAGEMENT_2];
    regval &= ~(1 << 0);
    regval |= (enable ? 1 : 0) << 0;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_POWER_MANAGEMENT_2, regval);
    wm8940->_register[WM8940_REG_POWER_MANAGEMENT_2] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_ADC_Polarity(WM8940_t* wm8940, uint8_t invert)
{
    uint16_t regval = wm8940->_register[WM8940_REG_ADC_CTRL];
    regval &= ~(1 << 0);
    regval |= (invert ? 1 : 0) << 0;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_ADC_CTRL, regval);
    wm8940->_register[WM8940_REG_ADC_CTRL] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_ADC_HighPassFilter(WM8940_t* wm8940, uint8_t enable, wm8940_hpf_mode_t mode, uint8_t freq_regval)
{
    if (freq_regval > 0x07)
        return WM8940_STATUS_INVALID;

    uint16_t regval = wm8940->_register[WM8940_REG_ADC_CTRL];
    regval &= ~(0x1F << 4);
    regval |= (enable ? 1 : 0) << 8;
    regval |= (mode ? 1 : 0) << 7;
    regval |= (freq_regval & 0x07) << 4;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_ADC_CTRL, regval);
    wm8940->_register[WM8940_REG_ADC_CTRL] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_ADC_Volume(WM8940_t* wm8940, uint8_t regval)
{
    if (regval > 0xFF)
        return WM8940_STATUS_INVALID;

    uint16_t val = 0;
    val |= (regval & 0xFF) << 0;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_ADC_DIGITAL_VOL, val);
    wm8940->_register[WM8940_REG_ADC_DIGITAL_VOL] = regval;
    return WM8940_STATUS_OK;
}

/* ----- ALC ----- */
wm8940_status_t WM8940_Set_ALC_Enable(WM8940_t* wm8940, uint8_t enable)
{
    uint16_t regval = wm8940->_register[WM8940_REG_ALC_CTRL_1];
    regval &= ~(1 << 8);
    regval |= (enable ? 1 : 0) << 8;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_ALC_CTRL_1, regval);
    wm8940->_register[WM8940_REG_ALC_CTRL_1] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_ALC_Gain(WM8940_t* wm8940, uint8_t minval, uint8_t maxval)
{
    if ((minval > 0x07) || (maxval > 0x07))
        return WM8940_STATUS_INVALID;

    uint16_t regval = wm8940->_register[WM8940_REG_ALC_CTRL_1];
    regval &= ~(0x3F << 0);
    regval |= (minval & 0x07) << 0;
    regval |= (maxval & 0x07) << 3;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_ALC_CTRL_1, regval);
    wm8940->_register[WM8940_REG_ALC_CTRL_1] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_ALC_Level(WM8940_t* wm8940, uint8_t val)
{
    if (val > 0x0F)
        return WM8940_STATUS_INVALID;
    
    uint16_t regval = wm8940->_register[WM8940_REG_ALC_CTRL_2];
    regval &= ~(0x0F << 0);
    regval |= (val & 0x0F) << 0;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_ALC_CTRL_2, regval);
    wm8940->_register[WM8940_REG_ALC_CTRL_2] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_ALC_Hold(WM8940_t* wm8940, uint8_t val)
{
    if (val > 0x0F)
        return WM8940_STATUS_INVALID;
    
    uint16_t regval = wm8940->_register[WM8940_REG_ALC_CTRL_2];
    regval &= ~(0x0F << 4);
    regval |= (val & 0x0F) << 4;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_ALC_CTRL_2, regval);
    wm8940->_register[WM8940_REG_ALC_CTRL_2] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_ALC_Mode(WM8940_t* wm8940, wm8940_alc_mode_t mode)
{
    uint16_t regval = wm8940->_register[WM8940_REG_ALC_CTRL_3];
    regval &= ~(1 << 8);
    regval |= (mode & 0x01) << 8;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_ALC_CTRL_3, regval);
    wm8940->_register[WM8940_REG_ALC_CTRL_3] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_ALC_DecayTime(WM8940_t* wm8940, uint8_t val)
{
    if (val > 0x0F)
        return WM8940_STATUS_INVALID;
    
    uint16_t regval = wm8940->_register[WM8940_REG_ALC_CTRL_3];
    regval &= ~(0x0F << 4);
    regval |= (val & 0x0F) << 4;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_ALC_CTRL_3, regval);
    wm8940->_register[WM8940_REG_ALC_CTRL_3] = regval;
    return WM8940_STATUS_OK;
}
wm8940_status_t WM8940_Set_ALC_AttackTime(WM8940_t* wm8940, uint8_t val)
{
    if (val > 0x0F)
        return WM8940_STATUS_INVALID;
    
    uint16_t regval = wm8940->_register[WM8940_REG_ALC_CTRL_3];
    regval &= ~(0x0F << 0);
    regval |= (val & 0x0F) << 0;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_ALC_CTRL_3, regval);
    wm8940->_register[WM8940_REG_ALC_CTRL_3] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_ALC_ZeroCross(WM8940_t* wm8940, uint8_t state)
{
    uint16_t regval = wm8940->_register[WM8940_REG_ALC_CTRL_4];
    regval &= ~(1 << 1);
    regval |= (state ? 1 : 0) << 1;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_ALC_CTRL_4, regval);
    wm8940->_register[WM8940_REG_ALC_CTRL_4] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_ALC_NoiseGate_Threshold(WM8940_t* wm8940, uint8_t val)
{
    if (val > 7)
        return WM8940_STATUS_INVALID;

    uint16_t regval = wm8940->_register[WM8940_REG_NOISE_GATE];
    regval &= ~(0x07 << 0);
    regval |= (val & 0x07) << 0;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_NOISE_GATE, regval);
    wm8940->_register[WM8940_REG_NOISE_GATE] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_ALC_NoiseGate_Enable(WM8940_t* wm8940, uint8_t enable)
{
    uint16_t regval = wm8940->_register[WM8940_REG_NOISE_GATE];
    regval &= ~(1 << 3);
    regval |= (enable ? 1 : 0) << 3;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_NOISE_GATE, regval);
    wm8940->_register[WM8940_REG_NOISE_GATE] = regval;
    return WM8940_STATUS_OK;
}

/* ----- DAC ----- */
wm8940_status_t WM8940_Set_DAC_Enable(WM8940_t* wm8940, uint8_t enable)
{
    uint16_t regval = wm8940->_register[WM8940_REG_POWER_MANAGEMENT_3];
    regval &= ~(1 << 0);
    regval |= (enable ? 1 : 0) << 0;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_POWER_MANAGEMENT_3, regval);
    wm8940->_register[WM8940_REG_POWER_MANAGEMENT_3] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_DAC_Polarity(WM8940_t* wm8940, uint8_t invert)
{
    uint16_t regval = wm8940->_register[WM8940_REG_DAC_CTRL];
    regval &= ~(1 << 0);
    regval |= (invert ? 1 : 0) << 0;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_DAC_CTRL, regval);
    wm8940->_register[WM8940_REG_DAC_CTRL] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_DAC_Volume(WM8940_t* wm8940, uint8_t val)
{
    uint16_t regval = 0;
    regval |= (val & 0xFF) << 0;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_DAC_DIGITAL_VOL, regval);
    wm8940->_register[WM8940_REG_DAC_DIGITAL_VOL] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_DAC_SoftMute(WM8940_t* wm8940, uint8_t state)
{
    uint16_t regval = wm8940->_register[WM8940_REG_DAC_CTRL];
    regval &= ~(1 << 6);
    regval |= (state ? 1 : 0) << 6;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_DAC_CTRL, regval);
    wm8940->_register[WM8940_REG_DAC_CTRL] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_DAC_AutoMute(WM8940_t* wm8940, uint8_t state)
{
    uint16_t regval = wm8940->_register[WM8940_REG_DAC_CTRL];
    regval &= ~(1 << 2);
    regval |= (state ? 1 : 0) << 2;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_DAC_CTRL, regval);
    wm8940->_register[WM8940_REG_DAC_CTRL] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_DAC_Limiter_Enable(WM8940_t* wm8940, uint8_t enable)
{
    uint16_t regval = wm8940->_register[WM8940_REG_DAC_LIMITER_1];
    regval &= ~(1 << 8);
    regval |= (enable ? 1 : 0) << 8;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_DAC_LIMITER_1, regval);
    wm8940->_register[WM8940_REG_DAC_LIMITER_1] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_DAC_Limiter_DecayTime(WM8940_t* wm8940, uint8_t val)
{
    if (val > 0x0F)
        return WM8940_STATUS_INVALID;
    
    uint16_t regval = wm8940->_register[WM8940_REG_DAC_LIMITER_1];
    regval &= ~(0x0F << 4);
    regval |= (val & 0x0F) << 4;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_DAC_LIMITER_1, regval);
    wm8940->_register[WM8940_REG_DAC_LIMITER_1] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_DAC_Limiter_AttackTime(WM8940_t* wm8940, uint8_t val)
{
    if (val > 0x0F)
        return WM8940_STATUS_INVALID;
    
    uint16_t regval = wm8940->_register[WM8940_REG_DAC_LIMITER_1];
    regval &= ~(0x0F << 0);
    regval |= (val & 0x0F) << 0;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_DAC_LIMITER_1, regval);
    wm8940->_register[WM8940_REG_DAC_LIMITER_1] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_DAC_Limiter_Level(WM8940_t* wm8940, uint8_t val)
{
    if (val > 0x07)
        return WM8940_STATUS_INVALID;
    
    uint16_t regval = wm8940->_register[WM8940_REG_DAC_LIMITER_2];
    regval &= ~(0x07 << 4);
    regval |= (val & 0x07) << 4;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_DAC_LIMITER_2, regval);
    wm8940->_register[WM8940_REG_DAC_LIMITER_2] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_DAC_Limiter_VolumeBoost(WM8940_t* wm8940, uint8_t value)
{
    if (value > 12)
        return WM8940_STATUS_INVALID;
    uint16_t regval = wm8940->_register[WM8940_REG_DAC_LIMITER_2];
    regval &= ~(0x0F << 0);
    regval |= (value) << 0;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_DAC_LIMITER_2, regval);
    wm8940->_register[WM8940_REG_DAC_LIMITER_2] = regval;
    return WM8940_STATUS_OK;
}

/* ----- Analogue outputs ----- */
wm8940_status_t WM8940_Set_Speaker_Source(WM8940_t* wm8940, wm8940_output_source_t source)
{
    uint16_t regval = wm8940->_register[WM8940_REG_SPK_MIXER];
    regval &= ~(0x03 << 0);
    regval &= ~(1 << 5);
    if (source == WM8940_OUTPUT_FROM_AUX) source = (1 << 5);
    regval |= source;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_SPK_MIXER, regval);
    wm8940->_register[WM8940_REG_SPK_MIXER] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_Speaker_FromBypass_Attenuation(WM8940_t* wm8940, uint8_t state)
{
    uint16_t regval = wm8940->_register[WM8940_REG_SPK_VOL_CTRL];
    regval &= ~(1 << 8);
    regval |= (state ? 1 : 0) << 8;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_SPK_VOL_CTRL, regval);
    wm8940->_register[WM8940_REG_SPK_VOL_CTRL] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_Speaker_ZeroCross(WM8940_t* wm8940, uint8_t state)
{
    uint16_t regval = wm8940->_register[WM8940_REG_SPK_VOL_CTRL];
    regval &= ~(1 << 7);
    regval |= (state ? 1 : 0) << 7;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_SPK_VOL_CTRL, regval);
    wm8940->_register[WM8940_REG_SPK_VOL_CTRL] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_Speaker_Mute(WM8940_t* wm8940, uint8_t state)
{
    uint16_t regval = wm8940->_register[WM8940_REG_SPK_VOL_CTRL];
    regval &= ~(1 << 6);
    regval |= (state ? 1 : 0) << 6;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_SPK_VOL_CTRL, regval);
    wm8940->_register[WM8940_REG_SPK_VOL_CTRL] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_Speaker_Volume(WM8940_t* wm8940, uint8_t val)
{
    if (val > 0x3F)
        return WM8940_STATUS_INVALID;

    uint16_t regval = wm8940->_register[WM8940_REG_SPK_VOL_CTRL];
    regval &= ~(0x3F << 0);
    regval |= (val & 0x3F) << 0;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_SPK_VOL_CTRL, regval);
    wm8940->_register[WM8940_REG_SPK_VOL_CTRL] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_Speaker_Volume_db(WM8940_t* wm8940, int8_t vol_db)
{
    if (vol_db < -57)
        return WM8940_STATUS_INVALID;
    else if (vol_db >= 6)
        return WM8940_STATUS_INVALID;

    return WM8940_Set_Speaker_Volume(wm8940, WM8940_SPKVOL_DB_TO_REG_VALUE(vol_db));
}

uint8_t WM8940_Get_Speaker_Volume(WM8940_t* wm8940)
{
    return wm8940->_register[WM8940_REG_SPK_VOL_CTRL] & 0x3F;
}

uint8_t WM8940_Get_Speaker_Volume_db(WM8940_t* wm8940)
{
    return WM8940_SPKVOL_REG_VALUE_TO_DB(WM8940_Get_Speaker_Volume(wm8940));
}

wm8940_status_t WM8940_Set_Mono_Source(WM8940_t* wm8940, wm8940_output_source_t source)
{
    uint16_t regval = wm8940->_register[WM8940_REG_MONO_MIXER_CTRL];
    regval &= ~(0x07 << 0);
    regval |= source;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_MONO_MIXER_CTRL, regval);
    wm8940->_register[WM8940_REG_MONO_MIXER_CTRL] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_Mono_FromBypass_Attenuation(WM8940_t* wm8940, uint8_t state)
{
    uint16_t regval = wm8940->_register[WM8940_REG_MONO_MIXER_CTRL];
    regval &= ~(1 << 7);
    regval |= (state ? 1 : 0) << 7;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_MONO_MIXER_CTRL, regval);
    wm8940->_register[WM8940_REG_MONO_MIXER_CTRL] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_Mono_Mute(WM8940_t* wm8940, uint8_t state)
{
    uint16_t regval = wm8940->_register[WM8940_REG_MONO_MIXER_CTRL];
    regval &= ~(1 << 6);
    regval |= (state ? 1 : 0) << 6;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_MONO_MIXER_CTRL, regval);
    wm8940->_register[WM8940_REG_MONO_MIXER_CTRL] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_Output_Enable(WM8940_t* wm8940, wm8940_output_t output)
{
    uint16_t regval = wm8940->_register[WM8940_REG_POWER_MANAGEMENT_3];
    regval &= ~(WM8940_PM3_SPKMIX | WM8940_PM3_SPKP | WM8940_PM3_SPKN);
    regval &= ~(WM8940_PM3_MONOMIX | WM8940_PM3_MONO);

    if (output & WM8940_OUTPUT_SPK)
        regval |= WM8940_PM3_SPKMIX | WM8940_PM3_SPKP | WM8940_PM3_SPKN;
    if (output & WM8940_OUTPUT_MONO)
        regval |= WM8940_PM3_MONOMIX | WM8940_PM3_MONO;

    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_POWER_MANAGEMENT_3, regval);
    wm8940->_register[WM8940_REG_POWER_MANAGEMENT_3] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_VREFToAnalogueOutputResistance(WM8940_t* wm8940, wm8940_vroi_t vroi)
{
    uint16_t regval = wm8940->_register[WM8940_REG_OUTPUT_CTRL];
    regval &= ~(1 << 0);
    regval |= (vroi & 0x01) << 0;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_OUTPUT_CTRL, regval);
    wm8940->_register[WM8940_REG_OUTPUT_CTRL] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_SlowClock_Enable(WM8940_t* wm8940, uint8_t state)
{
    uint16_t regval = wm8940->_register[WM8940_REG_ADDITIONAL_CTRL];
    regval &= ~(1 << 0);
    regval |= (state ? 1 : 0) << 0;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_ADDITIONAL_CTRL, regval);
    regval = wm8940->_register[WM8940_REG_ADDITIONAL_CTRL] = regval;
    return WM8940_STATUS_OK;
}

/* ----- Output switch ----- */
wm8940_status_t WM8940_Set_ThermalShutdown_Enable(WM8940_t* wm8940, uint8_t state)
{
    uint16_t regval = wm8940->_register[WM8940_REG_OUTPUT_CTRL];
    regval &= ~(1 << 1);
    regval |= (state ? 1 : 0) << 1;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_OUTPUT_CTRL, regval);
    regval = wm8940->_register[WM8940_REG_OUTPUT_CTRL] = regval;
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

wm8940_status_t WM8940_Set_AudioInterfaceFormat(WM8940_t* wm8940, wm8940_audio_iface_fmt_t format, wm8940_audio_iface_wl_t word_length)
{
    uint16_t regval = wm8940->_register[WM8940_REG_AUDIO_INTERFACE];
    regval &= ~(0x0F << 3);
    regval |= (format << 3);
    regval |= (word_length << 5);
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_AUDIO_INTERFACE, regval);
    wm8940->_register[WM8940_REG_AUDIO_INTERFACE] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_FrameClock_Polarity(WM8940_t* wm8940, uint8_t invert)
{
    uint16_t regval = wm8940->_register[WM8940_REG_AUDIO_INTERFACE];
    regval &= ~(1 << 7);
    regval |= ((invert ? 1 : 0) << 7);
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_AUDIO_INTERFACE, regval);
    wm8940->_register[WM8940_REG_AUDIO_INTERFACE] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_BCLK_Polarity(WM8940_t* wm8940, uint8_t invert)
{
    uint16_t regval = wm8940->_register[WM8940_REG_AUDIO_INTERFACE];
    regval &= ~(1 << 8);
    regval |= ((invert ? 1 : 0) << 8);
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_AUDIO_INTERFACE, regval);
    wm8940->_register[WM8940_REG_AUDIO_INTERFACE] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_LOUTR(WM8940_t* wm8940, uint8_t enable)
{
    uint16_t regval = wm8940->_register[WM8940_REG_AUDIO_INTERFACE];
    regval &= ~(1 << 9);
    regval |= ((enable ? 1 : 0) << 9);
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_AUDIO_INTERFACE, regval);
    wm8940->_register[WM8940_REG_AUDIO_INTERFACE] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_DAC_SwapLRData(WM8940_t* wm8940, uint8_t swap)
{
    uint16_t regval = wm8940->_register[WM8940_REG_AUDIO_INTERFACE];
    regval &= ~(1 << 2);
    regval |= ((swap ? 1 : 0) << 2);
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_AUDIO_INTERFACE, regval);
    wm8940->_register[WM8940_REG_AUDIO_INTERFACE] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_ADC_SwapLRData(WM8940_t* wm8940, uint8_t swap)
{
    uint16_t regval = wm8940->_register[WM8940_REG_AUDIO_INTERFACE];
    regval &= ~(1 << 1);
    regval |= ((swap ? 1 : 0) << 1);
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_AUDIO_INTERFACE, regval);
    wm8940->_register[WM8940_REG_AUDIO_INTERFACE] = regval;
    return WM8940_STATUS_OK;
}

/* ----- Audio sample rates ----- */
wm8940_status_t WM8940_Set_SampleRate(WM8940_t* wm8940, wm8940_sample_rate_t sample_rate)
{
    uint16_t regval = wm8940->_register[WM8940_REG_ADDITIONAL_CTRL];
    regval &= ~(0x07 << 1);
    regval |= (sample_rate << 1);
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_ADDITIONAL_CTRL, regval);
    wm8940->_register[WM8940_REG_ADDITIONAL_CTRL] = regval;
    return WM8940_STATUS_OK;
}

/* ----- Master clock and PLL ----- */
wm8940_status_t WM8940_Set_PLL_Enable(WM8940_t* wm8940, uint8_t state)
{
    if (state && !(wm8940->_register[WM8940_REG_POWER_MANAGEMENT_1] & 0x03))
        return WM8940_STATUS_INVALID;

    uint16_t regval = wm8940->_register[WM8940_REG_POWER_MANAGEMENT_1];
    regval &= ~(1 << 5);
    regval |= (state ? 1 : 0) << 5;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_POWER_MANAGEMENT_1, regval);
    wm8940->_register[WM8940_REG_POWER_MANAGEMENT_1] = regval;

    regval = wm8940->_register[WM8940_REG_PLL_N];
    regval &= ~(1 << 7);
    regval |= (state ? 0 : 1) << 7;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_PLL_N, regval);
    wm8940->_register[WM8940_REG_PLL_N] = regval;
    return WM8940_STATUS_OK;

}
wm8940_status_t WM8940_Set_PLL_PowerDown(WM8940_t* wm8940, uint8_t state)
{
    uint16_t regval = wm8940->_register[WM8940_REG_PLL_N];
    regval &= ~(1 << 7);
    regval |= (state ? 1 : 0) << 7;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_PLL_N, regval);
    wm8940->_register[WM8940_REG_PLL_N] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_PLL_FrequencyRatio(WM8940_t* wm8940, wm8940_pll_prescaler_t prescaler, uint8_t N, uint32_t K)
{
    if (N < 5 || N > 13)
        return WM8940_STATUS_INVALID;
    if (K > 0xFFFFFF)
        return WM8940_STATUS_INVALID;
    
    uint16_t n_regval = wm8940->_register[WM8940_REG_PLL_N] & (1 << 7);
    n_regval |= (N & 0x0F) | ((prescaler & 0x03) << 4) | ((K ? 1 : 0) << 6);
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_PLL_K3, K & 0x1FF);
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_PLL_K2, (K >> 9) & 0x1FF);
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_PLL_K1, (K >> 18) & 0x3F);
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_PLL_N, n_regval);
    wm8940->_register[WM8940_REG_PLL_K3] = K & 0x1FF;
    wm8940->_register[WM8940_REG_PLL_K2] = (K >> 9) & 0x1FF;
    wm8940->_register[WM8940_REG_PLL_K1] = (K >> 18) & 0x3F;
    wm8940->_register[WM8940_REG_PLL_N] = n_regval;
    return WM8940_STATUS_OK;
}

/* ----- Companding ----- */
wm8940_status_t WM8940_Set_ADC_Companding(WM8940_t* wm8940, wm8940_companding_t companding)
{
    if (companding == 2 || companding > 3)
        return WM8940_STATUS_INVALID;
    uint16_t regval = wm8940->_register[WM8940_REG_COMPANDING_CTRL];
    regval &= ~(0x03 << 1);
    regval |= companding << 1;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_COMPANDING_CTRL, regval);
    wm8940->_register[WM8940_REG_COMPANDING_CTRL] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_DAC_Companding(WM8940_t* wm8940, wm8940_companding_t companding)
{
    if (companding == 2 || companding > 3)
        return WM8940_STATUS_INVALID;
    uint16_t regval = wm8940->_register[WM8940_REG_COMPANDING_CTRL];
    regval &= ~(0x03 << 3);
    regval |= companding << 3;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_COMPANDING_CTRL, regval);
    wm8940->_register[WM8940_REG_COMPANDING_CTRL] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_ADC_Loopback(WM8940_t* wm8940, uint8_t enable)
{
    uint16_t regval = wm8940->_register[WM8940_REG_COMPANDING_CTRL];
    regval &= ~(1 << 0);
    regval |= (enable ? 1 : 0) << 1;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_COMPANDING_CTRL, regval);
    wm8940->_register[WM8940_REG_COMPANDING_CTRL] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_DAC_Loopback(WM8940_t* wm8940, uint8_t enable)
{
    uint16_t regval = wm8940->_register[WM8940_REG_COMPANDING_CTRL];
    regval &= ~(1 << 6);
    regval |= (enable ? 1 : 0) << 6;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_COMPANDING_CTRL, regval);
    wm8940->_register[WM8940_REG_COMPANDING_CTRL] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_WordLength8(WM8940_t* wm8940, uint8_t enable)
{
    uint16_t regval = wm8940->_register[WM8940_REG_COMPANDING_CTRL];
    regval &= ~(1 << 5);
    regval |= (enable ? 1 : 0) << 5;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_COMPANDING_CTRL, regval);
    wm8940->_register[WM8940_REG_COMPANDING_CTRL] = regval;
    return WM8940_STATUS_OK;
}

/* ----- GPIO control ----- */
wm8940_status_t WM8940_Set_GPIO_Control(WM8940_t* wm8940, wm8940_gpio_function_t function, uint8_t invert_polarity)
{
    if (function > 5)
        return WM8940_STATUS_INVALID;

    uint16_t regval = wm8940->_register[WM8940_REG_GPIO_CTRL];
    
    if (!(regval & (1 << 7)))  // Mode pin as mode selector
        return WM8940_STATUS_INVALID;
    
    regval &= (0x07 << 0);
    regval |= function << 0;
    regval |= (invert_polarity ? 1 : 0) << 3;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_GPIO_CTRL, regval);
    wm8940->_register[WM8940_REG_GPIO_CTRL] = regval;
    return WM8940_STATUS_OK;
}

/* ----- Control interface ----- */
wm8940_status_t WM8940_Set_ModePin_Function(WM8940_t* wm8940, wm8940_mode_pin_function_t function)
{
    uint16_t regval = wm8940->_register[WM8940_REG_GPIO_CTRL];
    regval &= (1 << 7);
    regval |= (function ? 1 : 0) << 7;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_GPIO_CTRL, regval);
    wm8940->_register[WM8940_REG_GPIO_CTRL] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_AutoIncrementalWrite(WM8940_t* wm8940, uint8_t state)
{
    uint16_t regval = wm8940->_register[WM8940_REG_CONTROL_INTERFACE];
    regval &= ~(1 << 1);
    regval |= (state ? 1 : 0) << 1;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_CONTROL_INTERFACE, regval);
    wm8940->_register[WM8940_REG_CONTROL_INTERFACE] = regval;
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
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_SOFTWARE_RESET, 1);

    // Set register default values based on datasheet
    // Doesn't need to set the software reset register value
    wm8940->_register[WM8940_REG_POWER_MANAGEMENT_1] = 0x0000;
    wm8940->_register[WM8940_REG_POWER_MANAGEMENT_2] = 0x0000;
    wm8940->_register[WM8940_REG_POWER_MANAGEMENT_3] = 0x0000;
    wm8940->_register[WM8940_REG_AUDIO_INTERFACE] = 0x0050;
    wm8940->_register[WM8940_REG_COMPANDING_CTRL] = 0x0000;
    wm8940->_register[WM8940_REG_CLOCK_GEN_CTRL] = 0x0140;
    wm8940->_register[WM8940_REG_ADDITIONAL_CTRL] = 0x0000;
    wm8940->_register[WM8940_REG_GPIO_CTRL] = 0x0000;
    wm8940->_register[WM8940_REG_CONTROL_INTERFACE] = 0x0002;
    wm8940->_register[WM8940_REG_DAC_CTRL] = 0x0000;
    wm8940->_register[WM8940_REG_DAC_DIGITAL_VOL] = 0x00FF;
    // reserved
    wm8940->_register[WM8940_REG_ADC_CTRL] = 0x0100;
    wm8940->_register[WM8940_REG_ADC_DIGITAL_VOL] = 0x00FF;
    wm8940->_register[WM8940_REG_NOTCH_FILTER_1] = 0x0000;
    wm8940->_register[WM8940_REG_NOTCH_FILTER_2] = 0x0000;
    wm8940->_register[WM8940_REG_NOTCH_FILTER_3] = 0x0000;
    wm8940->_register[WM8940_REG_NOTCH_FILTER_4] = 0x0000;
    wm8940->_register[WM8940_REG_NOTCH_FILTER_5] = 0x0000;
    wm8940->_register[WM8940_REG_NOTCH_FILTER_6] = 0x0000;
    wm8940->_register[WM8940_REG_NOTCH_FILTER_7] = 0x0000;
    wm8940->_register[WM8940_REG_NOTCH_FILTER_8] = 0x0000;
    wm8940->_register[WM8940_REG_DAC_LIMITER_1] = 0x0032;
    wm8940->_register[WM8940_REG_DAC_LIMITER_2] = 0x0000;
    // reserved
    wm8940->_register[WM8940_REG_ALC_CTRL_1] = 0x0034;
    wm8940->_register[WM8940_REG_ALC_CTRL_2] = 0x000B;
    wm8940->_register[WM8940_REG_ALC_CTRL_3] = 0x0032;
    wm8940->_register[WM8940_REG_NOISE_GATE] = 0x0000;
    wm8940->_register[WM8940_REG_PLL_N] = 0x0048;
    wm8940->_register[WM8940_REG_PLL_K1] = 0x000C;
    wm8940->_register[WM8940_REG_PLL_K2] = 0x0093;
    wm8940->_register[WM8940_REG_PLL_K3] = 0x00E9;
    // reserved
    wm8940->_register[WM8940_REG_ALC_CTRL_4] = 0x0030;
    // reserved
    wm8940->_register[WM8940_REG_INPUT_CTRL] = 0x0002;
    wm8940->_register[WM8940_REG_INPPGA_GAIN_CTRL] = 0x0050;
    // reserved
    wm8940->_register[WM8940_REG_ADC_BOOST_CTRL] = 0x0000;
    // reserved
    wm8940->_register[WM8940_REG_OUTPUT_CTRL] = 0x0002;
    wm8940->_register[WM8940_REG_SPK_MIXER] = 0x0000;
    // reserved
    wm8940->_register[WM8940_REG_SPK_VOL_CTRL] = 0x0079;
    // reserved
    wm8940->_register[WM8940_REG_MONO_MIXER_CTRL] = 0x0000;
    
    return WM8940_STATUS_OK;
}

/* ----- Power management ----- */
wm8940_status_t WM8940_Set_VMID_Impedance(WM8940_t* wm8940, wm8940_vmid_impedance_t impedance)
{
    if (impedance < 0 || impedance > 3)
        return WM8940_STATUS_INVALID;

    uint16_t regval = wm8940->_register[WM8940_REG_POWER_MANAGEMENT_1];
    regval &= ~(0x03 << 0);
    regval |= impedance << 0;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_POWER_MANAGEMENT_1, regval);
    wm8940->_register[WM8940_REG_POWER_MANAGEMENT_1] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Set_AnalogueAmplifierBias_Enable(WM8940_t* wm8940, uint8_t enable)
{
    uint16_t regval = wm8940->_register[WM8940_REG_POWER_MANAGEMENT_1];
    regval &= ~(1 << 3);
    regval |= (enable ? 1 : 0) << 3;
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_POWER_MANAGEMENT_1, regval);
    wm8940->_register[WM8940_REG_POWER_MANAGEMENT_1] = regval;
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Get_PowerManagement2(WM8940_t* wm8940, uint16_t* status)
{
    *status = wm8940->_register[WM8940_REG_POWER_MANAGEMENT_2];
    return WM8940_STATUS_OK;
}

wm8940_status_t WM8940_Get_PowerManagement3(WM8940_t* wm8940, uint16_t* status)
{
    *status = wm8940->_register[WM8940_REG_POWER_MANAGEMENT_3];
    return WM8940_STATUS_OK;
}

/* ----- Pop minimisation ----- */
wm8940_status_t WM8940_Set_FastVMIDDischarge_Enable(WM8940_t* wm8940, uint8_t state)
{
    uint16_t regval = wm8940->_register[WM8940_REG_ADDITIONAL_CTRL];
    regval &= ~(1 << 4);
    regval |= ((state ? 1 : 0) << 4);
    WM8940_REG_WRITE(wm8940->comm_handle, WM8940_REG_ADDITIONAL_CTRL, regval);
    wm8940->_register[WM8940_REG_ADDITIONAL_CTRL] = regval;
    return WM8940_STATUS_OK;
}
