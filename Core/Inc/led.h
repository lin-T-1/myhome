/**
 * @file led.h
 * @brief LED 驱动
 * @note PA5  -> 智能家居照明输出（需外接 LED）
 *       PC13 -> Blue Pill 板载 LED（低电平点亮）
 */

#ifndef __LED_H
#define __LED_H

#include "main.h"

void LED_Init(void);
void LED_On(void);
void LED_Off(void);
void LED_Toggle(void);
uint8_t LED_GetState(void);

void BoardLed_Init(void);
void BoardLed_On(void);
void BoardLed_Off(void);
void BoardLed_Toggle(void);

#endif /* __LED_H */
