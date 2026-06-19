/**
 * @file buzzer.h
 * @brief 蜂鸣器驱动模块
 * @note PB1 -> 有源蜂鸣器（低电平响，高电平停）
 */

#ifndef __BUZZER_H
#define __BUZZER_H

#include "main.h"

void Buzzer_Init(void);
void Buzzer_On(void);
void Buzzer_Off(void);
void Buzzer_Toggle(void);
uint8_t Buzzer_GetState(void);
void Buzzer_Alarm(uint32_t times, uint32_t interval_ms);

#endif /* __BUZZER_H */
