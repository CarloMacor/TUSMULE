#include "DeepSleep.h"

#include <esp_sleep.h>

DeepSleepManager DeepSleep;

void DeepSleepManager::sleep(uint32_t seconds)
{
    Serial.print("Deep Sleep ");

    Serial.print(seconds);

    Serial.println(" sec");

    esp_sleep_enable_timer_wakeup(
        (uint64_t)seconds * 1000000ULL);

    esp_deep_sleep_start();
}