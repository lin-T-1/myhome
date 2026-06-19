/**
 * @file ir_sensor.c
 * @brief 红外传感器驱动模块实现
 */

#include "ir_sensor.h"

#define IR_HUMAN_GPIO_PORT      GPIOA
#define IR_HUMAN_GPIO_PIN       GPIO_PIN_3

#define IR_DOOR_GPIO_PORT       GPIOA
#define IR_DOOR_GPIO_PIN        GPIO_PIN_4

/* 反射式红外 DO：无遮挡=高，有遮挡=低 */
#define IR_ACTIVE_LOW           1
#define IR_DEBOUNCE_COUNT       3     /* 连续 N 次相同读数才切换状态，滤除抖动 */

static uint8_t IR_ReadDetected(GPIO_TypeDef *port, uint16_t pin)
{
    GPIO_PinState level = HAL_GPIO_ReadPin(port, pin);
#if IR_ACTIVE_LOW
    return (level == GPIO_PIN_RESET) ? 1 : 0;
#else
    return (level == GPIO_PIN_SET) ? 1 : 0;
#endif
}

/* 软件消抖：只有稳定检测到遮挡才返回 1 */
static uint8_t IR_ReadDebounced(GPIO_TypeDef *port, uint16_t pin,
                                uint8_t *stable, uint8_t *pending, uint8_t *count)
{
    uint8_t raw = IR_ReadDetected(port, pin);

    if (raw == *stable)
    {
        *pending = raw;
        *count = 0;
        return *stable;
    }

    if (raw == *pending)
    {
        (*count)++;
        if (*count >= IR_DEBOUNCE_COUNT)
        {
            *stable = raw;
            *count = 0;
        }
    }
    else
    {
        *pending = raw;
        *count = 1;
    }

    return *stable;
}

uint8_t IR_GetHumanState(void)
{
    static uint8_t stable = 0;
    static uint8_t pending = 0;
    static uint8_t count = 0;

    return IR_ReadDebounced(IR_HUMAN_GPIO_PORT, IR_HUMAN_GPIO_PIN,
                            &stable, &pending, &count);
}

uint8_t IR_GetDoorState(void)
{
    static uint8_t stable = 0;
    static uint8_t pending = 0;
    static uint8_t count = 0;

    return IR_ReadDebounced(IR_DOOR_GPIO_PORT, IR_DOOR_GPIO_PIN,
                            &stable, &pending, &count);
}

uint8_t IR_IsHumanDetected(void)
{
    return IR_GetHumanState();
}

uint8_t IR_IsDoorDetected(void)
{
    return IR_GetDoorState();
}
