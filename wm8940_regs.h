#ifndef MR_WM8940_REGS_H
#define MR_WM8940_REGS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define WM8940_I2C_ADDRESS (0x1A << 1)

typedef enum _wm8940_register
{
    WM8940_REG_SOFTWARE_RESET = 0,
    WM8940_REG_POWER_MANAGEMENT_1 = 1,
    WM8940_REG_POWER_MANAGEMENT_2 = 2,
    WM8940_REG_POWER_MANAGEMENT_3 = 3,
    WM8940_REG_AUDIO_INTERFACE = 4,
    WM8940_REG_COMPANDING_CTRL = 5,
    WM8940_REG_CLOCK_GEN_CTRL = 6,
    WM8940_REG_ADDITIONAL_CTRL = 7,
    WM8940_REG_GPIO_CTRL = 8,
    WM8940_REG_CONTROL_INTERFACE = 9,
    WM8940_REG_DAC_CTRL = 10,
    WM8940_REG_DAC_DIGITAL_VOL = 11,

    WM8940_REG_ADC_CTRL = 14,
    WM8940_REG_ADC_DIGITAL_VOL = 15,
    WM8940_REG_NOTCH_FILTER_1 = 16,
    WM8940_REG_NOTCH_FILTER_2 = 17,
    WM8940_REG_NOTCH_FILTER_3 = 18,
    WM8940_REG_NOTCH_FILTER_4 = 19,
    WM8940_REG_NOTCH_FILTER_5 = 20,
    WM8940_REG_NOTCH_FILTER_6 = 21,
    WM8940_REG_NOTCH_FILTER_7 = 22,
    WM8940_REG_NOTCH_FILTER_8 = 23,
    WM8940_REG_DAC_LIMITER_1 = 24,
    WM8940_REG_DAC_LIMITER_2 = 25,

    WM8940_REG_ALC_CTRL_1 = 32,
    WM8940_REG_ALC_CTRL_2 = 33,
    WM8940_REG_ALC_CTRL_3 = 34,
    WM8940_REG_NOISE_GATE = 35,
    WM8940_REG_PLL_N = 36,
    WM8940_REG_PLL_K1 = 37,
    WM8940_REG_PLL_K2 = 38,
    WM8940_REG_PLL_K3 = 39,

    WM8940_REG_ALC_CTRL_4 = 42,

    WM8940_REG_INPUT_CTRL = 44,
    WM8940_REG_INPPGA_GAIN_CTRL = 45,

    WM8940_REG_ADC_BOOST_CTRL = 47,

    WM8940_REG_OUTPUT_CTRL = 49,
    WM8940_REG_SPK_MIXER = 50,

    WM8940_REG_SPK_VOL_CTRL = 54,

    WM8940_REG_MONO_MIXER_CTRL = 56
} wm8940_register_t;

typedef enum _wm8940_input
{
    WM8940_INPUT_NONE = 0,
    WM8940_INPUT_MICP = (1 << 0),
    WM8940_INPUT_MICN = (1 << 1),
    WM8940_INPUT_AUX = (1 << 2)
} wm8940_input_t;

typedef enum _wm8940_aux_mode
{
    WM8940_AUXMODE_INVERTING = 0,
    WM8940_AUXMODE_MIXER = 1
} wm8940_aux_mode_t;

typedef enum _wm8940_micbias_voltage
{
    WM8940_MICBIAS_90PERCENT = (0 << 8),
    WM8940_MICBIAS_65PERCENT = (1 << 8)
} wm8940_micbias_voltage_t;

typedef enum _wm8940_hpf_mode
{
    WM8940_HPF_MODE_AUDIO = 0,
    WM8940_HPF_MODE_APP = 1
} wm8940_hpf_mode_t;

typedef enum _wm8940_alc_mode
{
    WM8940_ALC_MODE_NORMAL = 0,
    WM8940_ALC_MODE_LIMITER = 1
} wm8940_alc_mode_t;

typedef enum _wm8940_output_source
{
    WM8940_OUTPUT_FROM_NONE = 0,
    WM8940_OUTPUT_FROM_DAC = (1 << 0),
    WM8940_OUTPUT_FROM_BYPASS = (1 << 1),
    WM8940_OUTPUT_FROM_AUX = (1 << 2)
} wm8940_speaker_source_t;

typedef enum _wm8940_output
{
    WM8940_OUTPUT_NONE = 0,
    WM8940_OUTPUT_SPK = 1,
    WM8940_OUTPUT_MONO = 2
} wm8940_output_t;

typedef enum _wm8940_vroi
{
    WM8940_VROI_1KOHM = 0,
    WM8940_VROI_30KOHM = 1
} wm8940_vroi_t;

typedef enum _wm8940_power_management_2
{
    WM8940_PM2_ADC = (1 << 0),
    WM8940_PM2_PGA = (1 << 2),
    WM8940_PM2_BOOST = (1 << 4)
} wm8940_pm2_t;

typedef enum _wm8940_power_management_3
{
    WM8940_PM3_DAC = (1 << 0),
    WM8940_PM3_SPKMIX = (1 << 2),
    WM8940_PM3_MONOMIX = (1 << 3),
    WM8940_PM3_SPKP = (1 << 5),
    WM8940_PM3_SPKN = (1 << 6),
    WM8940_PM3_MONO = (1 << 7)
} wm8940_pm3_t;

typedef enum _wm8940_audio_iface_fmt
{
    WM8940_AUDIO_IFACE_FMT_RIGHT_JUSTIFIED = 0,
    WM8940_AUDIO_IFACE_FMT_LEFT_JUSTIFIED = 1,
    WM8940_AUDIO_IFACE_FMT_I2S = 2,
    WM8940_AUDIO_IFACE_FMT_PCM_DSP = 3
} wm8940_audio_iface_fmt_t;

typedef enum _wm8940_audio_iface_wl
{
    WM8940_AUDIO_IFACE_WL_16BITS = 0,
    WM8940_AUDIO_IFACE_WL_20BITS = 1,
    WM8940_AUDIO_IFACE_WL_24BITS = 2,
    WM8940_AUDIO_IFACE_WL_32BITS = 3
} wm8940_audio_iface_wl_t;

typedef enum _wm8940_bclkdiv
{
    WM8940_BCLKDIV_1 = 0,
    WM8940_BCLKDIV_2 = 1,
    WM8940_BCLKDIV_4 = 2,
    WM8940_BCLKDIV_8 = 3,
    WM8940_BCLKDIV_16 = 4,
    WM8940_BCLKDIV_32 = 5 
} wm8940_bclkdiv_t;

typedef enum _wm8940_mclkdiv
{
    WM8940_MCLKDIV_1 = 0,
    WM8940_MCLKDIV_1HALF = 1,
    WM8940_MCLKDIV_2 = 2,
    WM8940_MCLKDIV_3 = 3,
    WM8940_MCLKDIV_4 = 4,
    WM8940_MCLKDIV_6 = 5,
    WM8940_MCLKDIV_8 = 6,
    WM8940_MCLKDIV_12 = 7
} wm8940_mclkdiv_t;

typedef enum _wm8940_clksel
{
    WM8940_CLKSEL_MCLK = 0,
    WM8940_CLKSEL_PLL = 1
} wm8940_clksel_t;

typedef enum _wm8904_pll_prescaler
{
    WM8940_PLLPRESCALE_MCLK_TIM2 = 0,
    WM8940_PLLPRESCALE_MCLK = 1,
    WM8940_PLLPRESCALE_MCLK_DIV2 = 2,
    WM8940_PLLPRESCALE_MCLK_DIV4 = 3
} wm8940_pll_prescaler_t;

typedef enum _wm8940_sample_rate
{
    WM8940_SAMPLERATE_48KHZ = 0,
    WM8940_SAMPLERATE_32KHZ = 1,
    WM8940_SAMPLERATE_24KHZ = 2,
    WM8940_SAMPLERATE_16KHZ = 3,
    WM8940_SAMPLERATE_12KHZ = 4,
    WM8940_SAMPLERATE_8KHZ = 5,
} wm8940_sample_rate_t;

typedef enum _wm8940_companding
{
    WM8940_COMPANDING_OFF = 0,
    WM8940_COMPANDING_ALAW = 2,
    WM8940_COMPANDING_ULAW = 3
} wm8940_companding_t;

typedef enum _wm8940_gpio_function
{
    WM8940_GPIO_FUNCTION_CSB_INPUT = 0,
    WM8940_GPIO_FUNCTION_JACK_DETECT = 1,
    WM8940_GPIO_FUNCTION_TEMP_OK = 2,
    WM8940_GPIO_FUNCTION_AMUTE_ACTIVE = 3,
    WM8940_GPIO_FUNCTION_SYSCLK_CLOCK_OUTPUT = 4,
    WM8940_GPIO_FUNCTION_PLL_LOCK = 5
} wm8940_gpio_function_t;

typedef enum _wm8940_mode_pin_function
{
    WM8940_MODE_AS_MODE_SELECTOR = 0,
    WM8940_MODE_AS_GPIO = 1
} wm8940_mode_pin_function_t;

typedef enum _wm8940_vmid_impedance
{
    WM8940_VMID_IMPEDANCE_OPEN = 0,
    WM8940_VMID_IMPEDANCE_50K = 1,
    WM8940_VMID_IMPEDANCE_250K = 2,
    WM8940_VMID_IMPEDANCE_5K = 3
} wm8940_vmid_impedance_t;

#ifdef __cplusplus
}
#endif

#endif // MR_WM8940_REGS_H
