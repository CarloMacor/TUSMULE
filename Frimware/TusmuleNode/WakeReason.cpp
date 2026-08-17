#include "WakeReason.h"

#include <esp_sleep.h>

WakeReason Wake;

void WakeReason::begin()
{
    cause = esp_sleep_get_wakeup_cause();

    Serial.print("Wake Cause = ");

    Serial.println((int)cause);
}

bool WakeReason::timerWakeup() const
{
    return cause == ESP_SLEEP_WAKEUP_TIMER;
}

bool WakeReason::powerOn() const
{
    return cause == ESP_SLEEP_WAKEUP_UNDEFINED;
}