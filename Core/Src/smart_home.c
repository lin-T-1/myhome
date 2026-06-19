/**
 * @file smart_home.c
 * @brief 智能家居控制逻辑模块实现
 */

#include "smart_home.h"
#include "led.h"
#include "buzzer.h"
#include "adc_sensor.h"
#include "oled.h"

#define LIGHT_DARK_RAW_ON       2000  /* 遮光>此值开灯（实测遮光2300~3600） */
#define LIGHT_DARK_RAW_OFF      2150  /* 亮灯后ADC降至此值以下关灯（实测正常光1800~1900） */
#define TEMP_HIGH_RAW_THRESH    1800  /* NTC常见：高温→ADC低，低于此值判定为高温 */
#define TEMP_INVERTED           1     /* 1: 高温→ADC低; 0: 高温→ADC高 */

#define LIGHT_INVERTED          1   /* 5516 模块：光照强→ADC低, 遮光→ADC高 */

#define BUZZER_ARM_MS           800  /* 上电后延时再允许报警，避免误触发 */

/* ─── 内部状态缓存 ─── */
static uint32_t system_boot_ms = 0;
static uint8_t has_light = 0;
static uint8_t temp_high = 0;
static uint16_t light_raw = 0;
static uint16_t temp_raw = 0;
static uint8_t led_state = 0;
static uint8_t buzzer_state = 0;

static uint8_t SmartHome_HasLight(uint16_t raw_light)
{
#if LIGHT_INVERTED
    return (raw_light < LIGHT_DARK_RAW_ON) ? 1 : 0;
#else
    return (raw_light >= LIGHT_DARK_RAW_ON) ? 1 : 0;
#endif
}

static uint8_t SmartHome_IsDark(uint16_t raw_light)
{
#if LIGHT_INVERTED
    if (!led_state)
        return (raw_light > LIGHT_DARK_RAW_ON) ? 1 : 0;
    return (raw_light > LIGHT_DARK_RAW_OFF) ? 1 : 0;
#else
    if (!led_state)
        return (raw_light < LIGHT_DARK_RAW_ON) ? 1 : 0;
    return (raw_light < LIGHT_DARK_RAW_OFF) ? 1 : 0;
#endif
}

static uint8_t SmartHome_IsTempHigh(uint16_t raw_temp)
{
#if TEMP_INVERTED
    return (raw_temp < TEMP_HIGH_RAW_THRESH) ? 1 : 0;
#else
    return (raw_temp > TEMP_HIGH_RAW_THRESH) ? 1 : 0;
#endif
}

/**
 * @brief  系统初始化（在 main.c 的外设初始化之后调用）
 */
void SmartHome_Init(void)
{
    LED_Init();
    Buzzer_Init();
    BoardLed_Init();

    /* 板载 LED（PC13）闪 3 次，确认程序已运行 */
    for (uint8_t i = 0; i < 3; i++)
    {
        BoardLed_On();
        HAL_Delay(150);
        BoardLed_Off();
        HAL_Delay(150);
    }

    OLED_Init();

    OLED_ShowString(0, 0, (char *)"SMART HOME", OLED_8X16);
    OLED_ShowString(0, 16, (char *)"Ready...", OLED_8X16);
    OLED_Update();
    HAL_Delay(800);

    system_boot_ms = HAL_GetTick();
    Buzzer_Off();
    buzzer_state = 0;
}

/**
 * @brief  读取所有传感器
 */
void SmartHome_ReadSensors(void)
{
    light_raw = ADC_GetLightRaw();
    temp_raw = ADC_GetTempRaw();

    has_light = SmartHome_HasLight(light_raw);
    temp_high = SmartHome_IsTempHigh(temp_raw);
}

/**
 * @brief  处理控制逻辑
 */
void SmartHome_ProcessLogic(void)
{
    uint8_t dark = SmartHome_IsDark(light_raw);

    /* 无光照时自动开灯；有光照时强制关灯 */
    if (dark)
    {
        if (!led_state)
        {
            LED_On();
            led_state = 1;
        }
    }
    else if (led_state)
    {
        LED_Off();
        led_state = 0;
    }

    /* 温度模块：Tp 低于阈值时报警，恢复正常后停止 */
    if (HAL_GetTick() - system_boot_ms < BUZZER_ARM_MS)
    {
        if (buzzer_state)
        {
            Buzzer_Off();
            buzzer_state = 0;
        }
    }
    else if (temp_high)
    {
        if (!buzzer_state)
        {
            Buzzer_On();
            buzzer_state = 1;
        }
    }
    else if (buzzer_state)
    {
        Buzzer_Off();
        buzzer_state = 0;
    }
}

/**
 * @brief  更新 OLED 显示内容（仅在数据变化时刷新，减少闪烁）
 */
void SmartHome_UpdateDisplay(void)
{
    static uint8_t prev_has_light = 0xFF;
    static uint8_t prev_lamp_on = 0xFF;
    static uint16_t prev_light_raw = 0xFFFF;
    static uint16_t prev_temp_raw = 0xFFFF;

    uint16_t light_delta = (light_raw > prev_light_raw)
                           ? (light_raw - prev_light_raw)
                           : (prev_light_raw - light_raw);
    uint16_t temp_delta = (temp_raw > prev_temp_raw)
                          ? (temp_raw - prev_temp_raw)
                          : (prev_temp_raw - temp_raw);

    if (has_light == prev_has_light &&
        led_state == prev_lamp_on &&
        light_delta < 30 &&
        temp_delta < 30)
    {
        return;
    }

    OLED_ShowDashboard(has_light, led_state, light_raw, temp_raw, temp_high);

    prev_has_light = has_light;
    prev_lamp_on = led_state;
    prev_light_raw = light_raw;
    prev_temp_raw = temp_raw;
}

/**
 * @brief  主循环体（在 while(1) 中持续调用）
 */
void SmartHome_Update(void)
{
    static uint32_t last_sensor_ms = 0;
    uint32_t now = HAL_GetTick();

    if (last_sensor_ms == 0U || (now - last_sensor_ms >= 200U))
    {
        SmartHome_ReadSensors();
        SmartHome_UpdateDisplay();
        last_sensor_ms = now;
    }

    SmartHome_ProcessLogic();
}

void Task_SensorRead(void)
{
    SmartHome_ReadSensors();
}

void Task_LogicProcess(void)
{
    SmartHome_ProcessLogic();
}

void Task_DisplayUpdate(void)
{
    SmartHome_UpdateDisplay();
}
