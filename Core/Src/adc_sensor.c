/**
 * @file adc_sensor.c
 * @brief ADC传感器驱动模块实现
 */

#include "adc_sensor.h"

extern ADC_HandleTypeDef hadc1;

uint16_t ADC_ReadChannel(uint32_t channel)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    
    sConfig.Channel = channel;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);
    
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 100);
    uint16_t value = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);
    
    return value;
}

uint16_t ADC_ReadChannelFiltered(uint32_t channel)
{
    uint32_t sum = 0;
    uint16_t min_val = 4095, max_val = 0, value;
    
    for (uint8_t i = 0; i < 10; i++) {
        value = ADC_ReadChannel(channel);
        sum += value;
        if (value < min_val) min_val = value;
        if (value > max_val) max_val = value;
    }
    
    sum = sum - min_val - max_val;
    return (uint16_t)(sum / 8);
}

uint16_t ADC_GetLightRaw(void)
{
    return ADC_ReadChannelFiltered(ADC_CHANNEL_1);
}

uint16_t ADC_GetTempRaw(void)
{
    return ADC_ReadChannelFiltered(ADC_CHANNEL_2);
}

uint8_t ADC_GetLightPercent(void)
{
    uint16_t raw = ADC_ReadChannelFiltered(ADC_CHANNEL_1);
    
    /*
     * 光敏传感器典型工作特性：
     *   - 光照强 → 光敏电阻小 → ADC读数高 (接近4095)
     *   - 光照弱 → 光敏电阻大 → ADC读数低 (接近0)
     *   
     * 如果硬件接线是：VCC → 光敏电阻 → ADC → GND，则读数方向相反：
     *   - 光照强 → ADC读数低
     *   - 光照弱 → ADC读数高
     *   
     * 这里采用标准接线：VCC → 固定电阻 → ADC → 光敏电阻 → GND
     */
    return (uint8_t)((uint32_t)raw * 100 / 4095);
}

float ADC_GetTemperature(void)
{
    uint16_t raw = ADC_ReadChannelFiltered(ADC_CHANNEL_2);
    return -20.0f + (float)raw * 100.0f / 4095.0f;
}
