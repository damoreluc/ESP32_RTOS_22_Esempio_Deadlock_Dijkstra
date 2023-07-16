#ifndef _GLOBALS_H
#define _GLOBALS_H

#include <Arduino.h>

// Globals
extern SemaphoreHandle_t mutex_1;
extern SemaphoreHandle_t mutex_2;

extern uint32_t mutex_timeout;

#endif