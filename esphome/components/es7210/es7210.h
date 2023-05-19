#pragma once

#include "esphome/components/i2c/i2c.h"
#include "esphome/core/component.h"

namespace esphome {
namespace es7210 {

/**
 * @brief Select I2S interface format for ES7210
 */
typedef enum {
    ES7210_I2S_FMT_I2S   = 0x00,
    ES7210_I2S_FMT_LJ    = 0x01,
    ES7210_I2S_FMT_DSP_A = 0x03,
    ES7210_I2S_FMT_DSP_B = 0x13
} es7210_i2s_fmt_t;

/**
 * @brief Select I2S bit width for ES7210
 *
 */
typedef enum {
    ES7210_I2S_BITS_16B = 16,
    ES7210_I2S_BITS_18B = 18,
    ES7210_I2S_BITS_20B = 20,
    ES7210_I2S_BITS_24B = 24,
    ES7210_I2S_BITS_32B = 32
} es7210_i2s_bits_t;

/**
 * @brief Select MIC gain for ES7210
 *
 */
typedef enum {
    ES7210_MIC_GAIN_0DB  = 0,
    ES7210_MIC_GAIN_3DB  = 1,
    ES7210_MIC_GAIN_6DB  = 2,
    ES7210_MIC_GAIN_9DB  = 3,
    ES7210_MIC_GAIN_12DB = 4,
    ES7210_MIC_GAIN_15DB = 5,
    ES7210_MIC_GAIN_18DB = 6,
    ES7210_MIC_GAIN_21DB = 7,
    ES7210_MIC_GAIN_24DB = 8,
    ES7210_MIC_GAIN_27DB = 9,
    ES7210_MIC_GAIN_30DB = 10,
    ES7210_MIC_GAIN_33DB = 11,
    ES7210_MIC_GAIN_34_5DB = 12,
    ES7210_MIC_GAIN_36DB = 13,
    ES7210_MIC_GAIN_37_5DB = 14
} es7210_mic_gain_t;

/**
 * @brief Select MIC bias for ES7210
 *
 */
typedef enum {
    ES7210_MIC_BIAS_2V18 = 0x00,
    ES7210_MIC_BIAS_2V26 = 0x10,
    ES7210_MIC_BIAS_2V36 = 0x20,
    ES7210_MIC_BIAS_2V45 = 0x30,
    ES7210_MIC_BIAS_2V55 = 0x40,
    ES7210_MIC_BIAS_2V66 = 0x50,
    ES7210_MIC_BIAS_2V78 = 0x60,
    ES7210_MIC_BIAS_2V87 = 0x70
} es7210_mic_bias_t;

/**
 * @brief ES7210 codec config struct
 *
 */
typedef struct {
    uint32_t sample_rate_hz;        /*!< Sample rate in Hz, common values are supported */
    uint32_t mclk_ratio;            /*!< MCLK-to-Sample-rate clock ratio, typically 256 */
    es7210_i2s_fmt_t i2s_format;    /*!< I2S format of ES7210's output, can be any value in es7210_i2s_fmt_t */
    es7210_i2s_bits_t bit_width;    /*!< I2S bit width of ES7210's output, can be any value in es7210_i2s_bits_t */
    es7210_mic_bias_t mic_bias;     /*!< Bias volatge of analog MIC, please refer to your MIC's datasheet */
    es7210_mic_gain_t mic_gain;     /*!< Gain of analog MIC, please adjust according to your MIC's sensitivity */
    uint32_t tdm_enable;            /*!< Choose whether to enable TDM mode */
} es7210_codec_config_t;

/**
 * @brief Clock coefficient structure
 *
 */
typedef struct {
    uint32_t mclk;            /*!< mclk frequency */
    uint32_t lrck;            /*!< lrck */
    uint8_t  ss_ds;           /*!< not used */
    uint8_t  adc_div;         /*!< adcclk divider */
    uint8_t  dll;             /*!< dll_bypass */
    uint8_t  doubler;         /*!< doubler enable */
    uint8_t  osr;             /*!< adc osr */
    uint8_t  mclk_src;        /*!< select mclk  source */
    uint32_t lrck_h;          /*!< The high 4 bits of lrck */
    uint32_t lrck_l;          /*!< The low 8 bits of lrck */
} ES7210Coefficient;


class ES7210Component : public Component, public i2c::I2CDevice {
 public:
  void setup() override;
  float get_setup_priority() const override { return setup_priority::LATE - 1; }

  void set_volume(int8_t volume);

 protected:
  const ES7210Coefficient *get_coeff(uint32_t mclk, uint32_t rate);

  void config_codec(const es7210_codec_config_t *codec_conf);
  void set_i2s_format(es7210_i2s_fmt_t i2s_format, es7210_i2s_bits_t bit_width, bool tdm_enable);
  void set_i2s_sample_rate(uint32_t sample_rate_hz, uint32_t mclk_ratio);
  void set_mic_gain(es7210_mic_gain_t mic_gain);
  void set_mic_bias(es7210_mic_bias_t mic_bias);
};

}  // namespace es7210
}  // namespace esphome
