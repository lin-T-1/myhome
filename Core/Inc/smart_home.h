/**
 * @file smart_home.h
 * @brief 智能家居控制逻辑模块
 */

#ifndef __SMART_HOME_H
#define __SMART_HOME_H

#include "main.h"

void SmartHome_Init(void);
void SmartHome_Update(void);
void SmartHome_ReadSensors(void);
void SmartHome_ProcessLogic(void);
void SmartHome_UpdateDisplay(void);

void Task_SensorRead(void);
void Task_LogicProcess(void);
void Task_DisplayUpdate(void);

#endif /* __SMART_HOME_H */
