#include "es7210.h"
#include "es7210_registers.h"
#include "esphome/core/hal.h"
#include "esphome/core/log.h"

namespace esphome {
namespace es7210 {

static const char *const TAG = "es7210";

#define ES7210_ERROR_CHECK(func) \
  if (!(func)) { \
    this->mark_failed(); \
    return; \
  }
#define ES7210_READ_BYTE(reg, value) ES7210_ERROR_CHECK(this->read_byte(reg, value));
#define ES7210_WRITE_BYTE(reg, value) ES7210_ERROR_CHECK(this->write_byte(reg, value));

void ES7210Component::setup() {
  ESP_LOGCONFIG(TAG, "Setting up ES7210...");

  es7210_codec_config_t codec_conf = {
      .sample_rate_hz = 16000,
      .mclk_ratio = 768,
      .i2s_format = ES7210_I2S_FMT_I2S,
      .bit_width = ES7210_I2S_BITS_16B,
      .mic_bias = ES7210_MIC_BIAS_2V87,
      .mic_gain = ES7210_MIC_GAIN_30DB,
      .tdm_enable = false
  };

  this->config_codec(&codec_conf);
  this->set_volume(0);
}

void ES7210Component::config_codec(const es7210_codec_config_t *codec_conf) {
  /* Perform software reset */
  ES7210_WRITE_BYTE(ES7210_RESET, 0xFF);
  ES7210_WRITE_BYTE(ES7210_RESET, 0x32);
  /* Set the initialization time when device powers up */
  ES7210_WRITE_BYTE(ES7210_TIME_CONTROL0, 0x30);
  ES7210_WRITE_BYTE(ES7210_TIME_CONTROL1, 0x30);
  /* Configure HPF for ADC1-4 */
  ES7210_WRITE_BYTE(ES7210_ADC12_HPF1, 0x2A);
  ES7210_WRITE_BYTE(ES7210_ADC12_HPF2, 0x0A);
  ES7210_WRITE_BYTE(ES7210_ADC34_HPF1, 0x2A);
  ES7210_WRITE_BYTE(ES7210_ADC34_HPF2, 0x0A);
  /* Set bits per sample to 16, data protocal to I2S, enable 1xFS TDM */
  set_i2s_format(codec_conf->i2s_format, codec_conf->bit_width, codec_conf->tdm_enable);
  /* Configure analog power and VMID voltage */
  ES7210_WRITE_BYTE(ES7210_ANALOG, 0xC3);
  /* Set MIC14 bias to 2.87V */
  set_mic_bias(codec_conf->mic_bias);
  /* Set MIC1-4 gain to 30dB */
  set_mic_gain(codec_conf->mic_gain);
  /* Power on MIC1-4 */
  ES7210_WRITE_BYTE(ES7210_MIC1_LOW_PWR, 0x08);
  ES7210_WRITE_BYTE(ES7210_MIC2_LOW_PWR, 0x08);
  ES7210_WRITE_BYTE(ES7210_MIC3_LOW_PWR, 0x08);
  ES7210_WRITE_BYTE(ES7210_MIC4_LOW_PWR, 0x08);
  /* Set ADC sample rate to 48kHz */
  set_i2s_sample_rate(codec_conf->sample_rate_hz, codec_conf->mclk_ratio);
  /* Power down DLL */
  ES7210_WRITE_BYTE(ES7210_PWR_DOWN, 0x04);
  /* Power on MIC1-4 bias & ADC1-4 & PGA1-4 Power */
  ES7210_WRITE_BYTE(ES7210_MIC12_PWR_DOWN, 0x0F);
  ES7210_WRITE_BYTE(ES7210_MIC34_PWR_DOWN, 0x0F);
  /* Enable device */
  ES7210_WRITE_BYTE(ES7210_RESET, 0x71);
  ES7210_WRITE_BYTE(ES7210_RESET, 0x41);
}

void ES7210Component::set_volume(int8_t volume_db) {
    /*
     * reg_val: 0x00 represents -95.5dB, 0xBF represents 0dB (default after reset),
     * and 0xFF represents +32dB, with a 0.5dB step
     */
    uint8_t reg_val = 191 + volume_db * 2;

    ES7210_WRITE_BYTE(ES7210_ADC1_MAX_GAIN, reg_val);
    ES7210_WRITE_BYTE(ES7210_ADC2_MAX_GAIN, reg_val);
    ES7210_WRITE_BYTE(ES7210_ADC3_MAX_GAIN, reg_val);
    ES7210_WRITE_BYTE(ES7210_ADC4_MAX_GAIN, reg_val);
}


void ES7210Component::set_i2s_format(es7210_i2s_fmt_t i2s_format, es7210_i2s_bits_t bit_width, bool tdm_enable) {
    uint8_t reg_val = 0;

    switch (bit_width) {
    case ES7210_I2S_BITS_16B:
        reg_val = 0x60;
        break;
    case ES7210_I2S_BITS_18B:
        reg_val = 0x40;
        break;
    case ES7210_I2S_BITS_20B:
        reg_val = 0x20;
        break;
    case ES7210_I2S_BITS_24B:
        reg_val = 0x00;
        break;
    case ES7210_I2S_BITS_32B:
        reg_val = 0x80;
        break;
    default:
        abort();
    }
    ES7210_WRITE_BYTE(ES7210_SDP_INTERFACE1, i2s_format | reg_val);

    const char *mode_str = NULL;
    switch (i2s_format) {
    case ES7210_I2S_FMT_I2S:
        reg_val = 0x02;
        mode_str = "standard i2s";
        break;
    case ES7210_I2S_FMT_LJ:
        reg_val = 0x02;
        mode_str = "left justify";
        break;
    case ES7210_I2S_FMT_DSP_A:
        reg_val = 0x01;
        mode_str = "DSP-A";
        break;
    case ES7210_I2S_FMT_DSP_B:
        reg_val = 0x01;
        mode_str = "DSP-B";
        break;
    default:
        abort();
    }

    if (tdm_enable) { // enable 1xFS TDM
        ES7210_WRITE_BYTE(ES7210_SDP_INTERFACE2, reg_val);
    } else {
        ES7210_WRITE_BYTE(ES7210_SDP_INTERFACE2, 0x00);
    }

    ESP_LOGI(TAG, "format: %s, bit width: %d, tdm mode %s", mode_str, bit_width, tdm_enable ? "enabled" : "disabled");
}

void ES7210Component::set_i2s_sample_rate(uint32_t sample_rate_hz, uint32_t mclk_ratio) {
    uint32_t mclk_freq_hz = sample_rate_hz * mclk_ratio;
    const ES7210Coefficient *coeff_div = this->get_coeff(mclk_freq_hz, sample_rate_hz);
    if (!coeff_div) {
      ESP_LOGI(TAG, "unable to set %d Hz sample rate with %d Hz MCLK", sample_rate_hz, mclk_freq_hz);
    }
    /* Set osr */
    ES7210_WRITE_BYTE(ES7210_OSR, coeff_div->osr);
    /* Set adc_div & doubler & dll */
    ES7210_WRITE_BYTE(ES7210_MAIN_CLK, (coeff_div->adc_div) | (coeff_div->doubler << 6) | (coeff_div->dll << 7));
    /* Set lrck */
    ES7210_WRITE_BYTE(ES7210_LRCK_DIVH, coeff_div->lrck_h);
    ES7210_WRITE_BYTE(ES7210_LRCK_DIVL, coeff_div->lrck_l);

    ESP_LOGI(TAG, "sample rate: %d Hz, mclk frequency: %d Hz", sample_rate_hz, mclk_freq_hz);
}

void ES7210Component::set_mic_gain(es7210_mic_gain_t mic_gain) {
    ES7210_WRITE_BYTE(ES7210_MIC1_GAIN, mic_gain | 0x10);
    ES7210_WRITE_BYTE(ES7210_MIC2_GAIN, mic_gain | 0x10);
    ES7210_WRITE_BYTE(ES7210_MIC3_GAIN, mic_gain | 0x10);
    ES7210_WRITE_BYTE(ES7210_MIC4_GAIN, mic_gain | 0x10);
}

void ES7210Component::set_mic_bias(es7210_mic_bias_t mic_bias) {
    ES7210_WRITE_BYTE(ES7210_MIC12_BIAS, mic_bias);
    ES7210_WRITE_BYTE(ES7210_MIC34_BIAS, mic_bias);
}

}  // namespace es7210
}  // namespace esphome
