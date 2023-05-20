#pragma once

namespace esphome {
namespace es7210 {

static const uint8_t ES7210_RESET = 0x00;
static const uint8_t ES7210_CLOCK_OFF = 0x01;
static const uint8_t ES7210_MAIN_CLK = 0x02;
static const uint8_t ES7210_MASTER_CLK = 0x03;
static const uint8_t ES7210_LRCK_DIVH = 0x04;
static const uint8_t ES7210_LRCK_DIVL = 0x05;
static const uint8_t ES7210_PWR_DOWN = 0x06;
static const uint8_t ES7210_OSR = 0x07;
static const uint8_t ES7210_MODE_CONFIG = 0x08;
static const uint8_t ES7210_TIME_CONTROL0 = 0x09;
static const uint8_t ES7210_TIME_CONTROL1 = 0x0A;
static const uint8_t ES7210_CHIP_STATUS = 0x0B;
static const uint8_t ES7210_INTR_CNTRL = 0x0C;
static const uint8_t ES7210_MISC_CNTRL = 0x0D;
static const uint8_t ES7210_DMIC_CNTRL = 0x10;
static const uint8_t ES7210_SDP_INTERFACE1 = 0x11;
static const uint8_t ES7210_SDP_INTERFACE2 = 0x12;
static const uint8_t ES7210_ADC_AUTOMUTE = 0x13;
static const uint8_t ES7210_ADC34_MUTE = 0x14;
static const uint8_t ES7210_ADC12_MUTE = 0x15;
static const uint8_t ES7210_ALC_SEL = 0x16;
static const uint8_t ES7210_ALC_COMMON1 = 0x17;
static const uint8_t ES7210_ADC34_ALC_LVL = 0x18;
static const uint8_t ES7210_ADC12_ALC_LVL = 0x19;
static const uint8_t ES7210_ALC_COMMON_2 = 0x1A;
static const uint8_t ES7210_ADC4_MAX_GAIN = 0x1B;
static const uint8_t ES7210_ADC3_MAX_GAIN = 0x1C;
static const uint8_t ES7210_ADC2_MAX_GAIN = 0x1D;
static const uint8_t ES7210_ADC1_MAX_GAIN = 0x1E;
static const uint8_t ES7210_ADC34_HPF2 = 0x20;
static const uint8_t ES7210_ADC34_HPF1 = 0x21;
static const uint8_t ES7210_ADC12_HPF1 = 0x22;
static const uint8_t ES7210_ADC12_HPF2 = 0x23;
static const uint8_t ES7210_CHIP_ID1 = 0x3D;
static const uint8_t ES7210_CHIP_ID0 = 0x3E;
static const uint8_t ES7210_CHIP_VERSION = 0x3F;
static const uint8_t ES7210_ANALOG = 0x40;
static const uint8_t ES7210_MIC12_BIAS = 0x41;
static const uint8_t ES7210_MIC34_BIAS = 0x42;
static const uint8_t ES7210_MIC1_GAIN = 0x43;
static const uint8_t ES7210_MIC2_GAIN = 0x44;
static const uint8_t ES7210_MIC3_GAIN = 0x45;
static const uint8_t ES7210_MIC4_GAIN = 0x46;
static const uint8_t ES7210_MIC1_LOW_PWR = 0x47;
static const uint8_t ES7210_MIC2_LOW_PWR = 0x48;
static const uint8_t ES7210_MIC3_LOW_PWR = 0x49;
static const uint8_t ES7210_MIC4_LOW_PWR = 0x4A;
static const uint8_t ES7210_MIC12_PWR_DOWN = 0x4B;
static const uint8_t ES7210_MIC34_PWR_DOWN = 0x4C;

}  // namespace es7210
}  // namespace esphome
