#include "wm8940.h"
#include "wm8940_regs.h"

/* ----- Input signal path ----- */
wm8940_input_t WM8940_Get_PGAInput(WM8940_t* wm8940)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_INPUT_CTRL);
    return val & 0x0007;
}

void WM8940_Set_PGAInput(WM8940_t* wm8940, wm8940_input_t input)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_INPUT_CTRL);
    val &= ~(0x0007);
    val |= (uint16_t)input;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_INPUT_CTRL, val);
}

uint8_t WM8940_Get_PGAInput_Volume(WM8940_t* wm8940)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_INPPGA_GAIN_CTRL);
    return val & 0x003F;
}

void WM8940_Set_PGAInput_Volume(WM8940_t* wm8940, uint8_t regval)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_INPPGA_GAIN_CTRL);
    val &= ~(0x003F);
    val |= (regval & 0x003F);
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_INPPGA_GAIN_CTRL, val);
}

// TODO
void WM8940_Set_PGAInput_Volume_db(WM8940_t* wm8940, float vol_db)
{
    if (vol_db < -12)
        vol_db = -12;
    else if (vol_db >= 35.25)
        vol_db = 35.25;

    // Round to floor by 0.25
    // Convert to regval
    uint8_t regval = (vol_db + 12) / 0.75;
}

void WM8940_Set_PGAInput_Mute(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_INPPGA_GAIN_CTRL);
    val &= ~(1 << 6);
    val |= (state ? 1 : 0) << 6;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_INPPGA_GAIN_CTRL, val);
}

void WM8940_Set_PGAInput_ZeroCross(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_INPPGA_GAIN_CTRL);
    val &= ~(1 << 7);
    val |= (state ? 1 : 0) << 7;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_INPPGA_GAIN_CTRL, val);
}

void WM8940_Set_PGAInput_Enable(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_POWER_MANAGEMENT_2);
    val &= ~(1 << 2);
    val |= (state ? 1 : 0) << 2;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_POWER_MANAGEMENT_2, val);
}

void WM8940_Set_Aux_Enable(WM8940_t* wm8940, uint8_t state)
{
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_POWER_MANAGEMENT_1);
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
    uint16_t val = WM8940_I2C_READ(wm8940->i2c_handle, WM8940_REG_POWER_MANAGEMENT_1);
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
        val |= WM8940_PM3_DAC | WM8940_PM3_SPKMIX | WM8940_PM3_SPKP | WM8940_PM3_SPKN;
    if (output == WM8940_OUTPUT_MONO)
        val |= WM8940_PM3_DAC | WM8940_PM3_MONOMIX | WM8940_PM3_MONO;
    WM8940_I2C_WRITE(wm8940->i2c_handle, WM8940_REG_POWER_MANAGEMENT_3, val);
}
