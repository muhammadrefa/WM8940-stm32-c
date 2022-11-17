#include "wm8940.h"
#include "wm8940_regs.h"

/* ----- Input signal path ----- */
wm8940_input_t WM8940_Get_PGAInput()
{
    uint16_t val = I2C_READ(WM8940_REG_INPUT_CTRL);
    return val & 0x0007;
}

void WM8940_Set_PGAInput(wm8940_input_t input)
{
    uint16_t val = I2C_READ(WM8940_REG_INPUT_CTRL);
    val &= ~(0x0007);
    val |= (uint16_t)input;
    I2C_WRITE(WM8940_REG_INPUT_CTRL, val);
}

uint8_t WM8940_Get_PGAInput_Volume()
{
    uint16_t val = I2C_READ(WM8940_REG_INPPGA_GAIN_CTRL);
    return val & 0x003F;
}

void WM8940_Set_PGAInput_Volume(uint8_t vol)
{
    uint16_t val = I2C_READ(WM8940_REG_INPPGA_GAIN_CTRL);
    val &= ~(0x003F);
    val |= (vol & 0x003F);
    I2C_WRITE(WM8940_REG_INPPGA_GAIN_CTRL, val);
}

void WM8940_Set_PGAInput_Mute(uint8_t state)
{
    uint16_t val = I2C_READ(WM8940_REG_INPPGA_GAIN_CTRL);
    val &= ~(1 << 6);
    val |= (state ? 1 : 0) << 6;
    I2C_WRITE(WM8940_REG_INPPGA_GAIN_CTRL, val);
}

void WM8940_Set_PGAInput_ZeroCross(uint8_t state)
{
    uint16_t val = I2C_READ(WM8940_REG_INPPGA_GAIN_CTRL);
    val &= ~(1 << 7);
    val |= (state ? 1 : 0) << 7;
    I2C_WRITE(WM8940_REG_INPPGA_GAIN_CTRL, val);
}

void WM8940_Set_PGAInput_Enable(uint8_t state)
{
    uint16_t val = I2C_READ(WM8940_REG_POWER_MANAGEMENT_2);
    val &= ~(1 << 2);
    val |= (state ? 1 : 0) << 2;
    I2C_WRITE(WM8940_REG_POWER_MANAGEMENT_2, val);
}

void WM8940_Set_Aux_Enable(uint8_t state)
{
    uint16_t val = I2C_READ(WM8940_REG_POWER_MANAGEMENT_1);
    val &= ~(1 << 6);
    val |= (state ? 1 : 0) << 6;
    I2C_WRITE(WM8940_REG_POWER_MANAGEMENT_1, val);
}

void WM8940_Set_Aux_Mode(wm8940_aux_mode_t mode)
{
    uint16_t val = I2C_READ(WM8940_REG_INPUT_CTRL);
    val &= ~(1 << 3);
    val |= mode << 3;
    I2C_WRITE(WM8940_REG_INPUT_CTRL, val);
}

void WM8940_Set_PGA_Boost(uint8_t state)
{
    uint16_t val = I2C_READ(WM8940_REG_ADC_BOOST_CTRL);
    val &= ~(1 << 8);
    val |= (state ? 1 : 0) << 8;
    I2C_WRITE(WM8940_REG_ADC_BOOST_CTRL, val);
}

void WM8940_Set_Boost_Volume(wm8940_input_t input, uint8_t vol)
{
    int8_t shift = -1;
    switch (input)
    {
        case WM8940_INPUT_AUX: shift = 0; break;
        case WM8940_INPUT_MICP: shift = 1; break;
        default: break;
    }
    if (shift < 0) return;

    uint16_t val = I2C_READ(WM8940_REG_ADC_BOOST_CTRL);
    val &= ~(0x07 << shift);
    val |= (vol & 0x07) << shift;
    I2C_WRITE(WM8940_REG_ADC_BOOST_CTRL, val);
}

void WM8940_Set_Boost_Enable(uint8_t state)
{
    uint16_t val = I2C_READ(WM8940_REG_POWER_MANAGEMENT_2);
    val &= ~(1 << 4);
    val |= (state ? 1 : 0) << 4;
    I2C_WRITE(WM8940_REG_POWER_MANAGEMENT_2, val);
}

void WM8940_Set_MicBias_Enable(uint8_t state)
{
    uint16_t val = I2C_READ(WM8940_REG_POWER_MANAGEMENT_1);
    val &= ~(1 << 4);
    val |= (state ? 1 : 0) << 4;
    I2C_WRITE(WM8940_REG_POWER_MANAGEMENT_1, val);
}

void WM8940_Set_MicBias_Voltage(wm8940_micbias_voltage_t percent)
{
    uint16_t val = I2C_READ(WM8940_REG_INPUT_CTRL);
    val &= ~(1 << 8);
    val |= percent;
    I2C_WRITE(WM8940_REG_INPUT_CTRL, val);
}

/* ----- Analogue outputs ----- */
void WM8940_Set_Speaker_Source(wm8940_speaker_source_t source)
{
    uint16_t val = I2C_READ(WM8940_REG_SPK_MIXER);
    val &= ~(0x03 << 0);
    val &= ~(1 << 5);
    if (source == WM8940_OUTPUT_FROM_AUX) source = (1 << 5);
    val |= source;
    I2C_WRITE(WM8940_REG_SPK_MIXER, val);
}

void WM8940_Set_Speaker_FromBypass_Attenuation(uint8_t state)
{
    uint16_t val = I2C_READ(WM8940_REG_SPK_VOL_CTRL);
    val &= ~(1 << 8);
    val |= (state ? 1 : 0) << 8;
    I2C_WRITE(WM8940_REG_SPK_VOL_CTRL, val);
}

void WM8940_Set_Speaker_ZeroCross(uint8_t state)
{
    uint16_t val = I2C_READ(WM8940_REG_SPK_VOL_CTRL);
    val &= ~(1 << 7);
    val |= (state ? 1 : 0) << 7;
    I2C_WRITE(WM8940_REG_SPK_VOL_CTRL, val);
}

void WM8940_Set_Speaker_Mute(uint8_t state)
{
    uint16_t val = I2C_READ(WM8940_REG_SPK_VOL_CTRL);
    val &= ~(1 << 6);
    val |= (state ? 1 : 0) << 6;
    I2C_WRITE(WM8940_REG_SPK_VOL_CTRL, val);
}

void WM8940_Set_Speaker_Volume(int8_t vol_db)
{
    if (vol_db < -57)
        vol_db = -57;
    else if (vol_db >= 6)
        vol_db = 6;
    uint8_t vol_val = WM8940_VOL_DB_TO_REG_VALUE(vol_db);

    uint16_t val = I2C_READ(WM8940_REG_SPK_VOL_CTRL);
    val &= ~(0x3F << 0);
    val |= WM8940_SPKVOL_DB_TO_REG_VALUE(vol_db) << 0;
    I2C_WRITE(WM8940_REG_SPK_VOL_CTRL, val);
}

void WM8940_Set_Mono_Source(wm8940_speaker_source_t source)
{
    uint16_t val = I2C_READ(WM8940_REG_MONO_MIXER_CTRL);
    val &= ~(0x07 << 0);
    val |= source;
    I2C_WRITE(WM8940_REG_MONO_MIXER_CTRL, val);
}

void WM8940_Set_Mono_FromBypass_Attenuation(uint8_t state)
{
    uint16_t val = I2C_READ(WM8940_REG_MONO_MIXER_CTRL);
    val &= ~(1 << 7);
    val |= (state ? 1 : 0) << 7;
    I2C_WRITE(WM8940_REG_MONO_MIXER_CTRL, val);
}

void WM8940_Set_Mono_Mute(uint8_t state)
{
    uint16_t val = I2C_READ(WM8940_REG_MONO_MIXER_CTRL);
    val &= ~(1 << 6);
    val |= (state ? 1 : 0) << 6;
    I2C_WRITE(WM8940_REG_MONO_MIXER_CTRL, val);
}

void WM8940_Set_Output_Enable(wm8940_output_t output)
{
    uint16_t val = 0;
    if (output == WM8940_OUTPUT_SPK)
        val |= WM8940_PM3_DAC | WM8940_PM3_SPKMIX | WM8940_PM3_SPKP | WM8940_PM3_SPKN;
    if (output == WM8940_OUTPUT_MONO)
        val |= WM8940_PM3_DAC | WM8940_PM3_MONOMIX | WM8940_PM3_MONO;
    I2C_WRITE(WM8940_REG_POWER_MANAGEMENT_3, val);
}
