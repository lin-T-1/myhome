/**
 * @file led.c
 * @brief LED照明驱动模块实现
 */

#include "led.h"

#define LED_GPIO_PORT   GPIOA
#define LED_GPIO_PIN    GPIO_PIN_5

#define BOARD_LED_PORT  GPIOC
#define BOARD_LED_PIN   GPIO_PIN_13

static uint8_t current_state = 0;

void BoardLed_Init(void)
{
    __HAL_RCC_GPIOC_CLK_ENABLE();
    HAL_GPIO_WritePin(BOARD_LED_PORT, BOARD_LED_PIN, GPIO_PIN_SET);
}

void BoardLed_On(void)
{
    HAL_GPIO_WritePin(BOARD_LED_PORT, BOARD_LED_PIN, GPIO_PIN_RESET);
}

void BoardLed_Off(void)
{
    HAL_GPIO_WritePin(BOARD_LED_PORT, BOARD_LED_PIN, GPIO_PIN_SET);
}

void BoardLed_Toggle(void)
{
    HAL_GPIO_TogglePin(BOARD_LED_PORT, BOARD_LED_PIN);
}

void LED_Init(void)
{
    current_state = 0;
    HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_RESET);
}

void LED_On(void)
{
    HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_SET);
    current_state = 1;
}

void LED_Off(void)
{
    HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_RESET);
    current_state = 0;
}

void LED_Toggle(void)
{
    HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_GPIO_PIN);
    current_state = (current_state == 1) ? 0 : 1;
}

uint8_t LED_GetState(void)
{
    return current_state;
}
