/**
 * @file buzzer.c
 * @brief 蜂鸣器驱动模块实现
 */

#include "buzzer.h"

#define BUZZER_GPIO_PORT    GPIOB
#define BUZZER_GPIO_PIN     GPIO_PIN_1

/* 有源蜂鸣器常见：低电平响，高电平停 */
#define BUZZER_ON_LEVEL     GPIO_PIN_RESET
#define BUZZER_OFF_LEVEL    GPIO_PIN_SET

static uint8_t current_state = 0;

void Buzzer_Init(void)
{
    current_state = 0;
    HAL_GPIO_WritePin(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN, BUZZER_OFF_LEVEL);
}

void Buzzer_On(void)
{
    HAL_GPIO_WritePin(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN, BUZZER_ON_LEVEL);
    current_state = 1;
}

void Buzzer_Off(void)
{
    HAL_GPIO_WritePin(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN, BUZZER_OFF_LEVEL);
    current_state = 0;
}

void Buzzer_Toggle(void)
{
    HAL_GPIO_TogglePin(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN);
    current_state = (current_state == 1) ? 0 : 1;
}

uint8_t Buzzer_GetState(void)
{
    return current_state;
}

void Buzzer_Alarm(uint32_t times, uint32_t interval_ms)
{
    for (uint32_t i = 0; i < times; i++) {
        Buzzer_On();
        HAL_Delay(interval_ms);
        Buzzer_Off();
        if (i < times - 1) {
            HAL_Delay(interval_ms);
        }
    }
}
