/**
 * @file ir_sensor.h
 * @brief 红外传感器驱动模块
 * @note PA3 -> 人体反射红外
 *       PA4 -> 门禁反射红外（DO 接此脚）
 *       无遮挡=高电平，有遮挡=低电平（IR_ACTIVE_LOW=1）
 */

#ifndef __IR_SENSOR_H
#define __IR_SENSOR_H

#include "main.h"

uint8_t IR_GetHumanState(void);
uint8_t IR_GetDoorState(void);
uint8_t IR_IsHumanDetected(void);
uint8_t IR_IsDoorDetected(void);

#endif /* __IR_SENSOR_H */
