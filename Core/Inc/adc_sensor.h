/**
 * @file adc_sensor.h
 * @brief ADC传感器驱动模块
 * @note PA1 -> ADC1_IN1 光敏模块 AO（模拟输出，勿接 DO）
 *       PA2 -> ADC1_IN2 热敏传感器
 *       模块 DO 脚上的指示灯由模块自身电位器控制，与 STM32 无关
 */

#ifndef __ADC_SENSOR_H
#define __ADC_SENSOR_H

#include "main.h"

#define ADC_CHANNEL_LIGHT   ADC_CHANNEL_1
#define ADC_CHANNEL_TEMP    ADC_CHANNEL_2

uint16_t ADC_ReadChannel(uint32_t channel);
uint16_t ADC_ReadChannelFiltered(uint32_t channel);
uint16_t ADC_GetLightRaw(void);
uint16_t ADC_GetTempRaw(void);
uint8_t ADC_GetLightPercent(void);
float ADC_GetTemperature(void);

#endif /* __ADC_SENSOR_H */
