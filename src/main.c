/**
 * This file is part of motor-controller.
 *
 * motor-controller is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * motor-controller is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with motor-controller.  If not, see <http://www.gnu.org/licenses/>.
 */

/* Config 1 */
#pragma config FOSC = INTOSC
#pragma config WDTE = 1
#pragma config PWRTE = OFF
#pragma config MCLRE = OFF
#pragma config CP = OFF
#pragma config CPD = OFF
#pragma config BOREN = ON
#pragma config CLKOUTEN = OFF
#pragma config IESO = OFF
#pragma config FCMEN = ON

/* Config 2 */
#pragma config LVP = OFF
#pragma config BORV = LO
#pragma config STVREN = ON
#pragma config PLLEN = OFF
#pragma config WRT = OFF

#include <xc.h>
#include "constants.h"
#include "mcu.h"
#include "motor.h"
#include "periph_conf.h"
#include "periph/gpio.h"
#include "periph/timer0.h"
#include "periph/watchdog.h"
#include "radio.h"
#include "status.h"

#define STATUS_LED          (GPIO_PIN(PORT_C, 0))
#define RADIO_PIN           (GPIO_PIN(PORT_A, 4))

#define LEFT_PWM_PIN        (GPIO_PIN(PORT_C, 3))
#define RIGHT_PWM_PIN       (GPIO_PIN(PORT_C, 1))
#define LEFT_PWM            (2)
#define RIGHT_PWM           (4)

static uint16_t buffer[32];
static uint16_t past[3];

int main()
{
    uint8_t i;

    mcu_init();
    timer0_configure(TIMER0_PRESCALER_64);
    watchdog_configure(WATCHDOG_PERIOD_1S);
    watchdog_enable();

    /* Change clock speed to 2MHz */
    OSCCON = (0b1100 << _OSCCON_IRCF_POSITION);

    radio_init(RADIO_PIN);

    status_init(STATUS_LED);
    status_set_mode(STATUS_BLINK_FAST);

    uint16_t neutral = radio_find_neutral();
    if (neutral < MIN_TARGET || neutral > MAX_TARGET) {
        mcu_reset();
    }

    motor_init(LEFT_PWM_PIN, RIGHT_PWM_PIN, LEFT_PWM, RIGHT_PWM, neutral);
    status_set_mode(STATUS_FLASH);

    past[0] = neutral;
    past[1] = neutral;
    past[2] = neutral;
    for (i = 0; i < 32; ++i)
        buffer[i] = neutral;

    while (1) {
        uint16_t data, target;
        uint8_t i, invalid_data_count = 0;

        motor_tick();
        mcu_delay(1);
        if (!radio_has_data())
            continue;

        data = radio_get_data();

        /* Check if we get valid data */
        for (i = 31; i > 0; --i)
            buffer[i] = buffer[i - 1];
        buffer[0] = data;

        for (i = 0; i < 32; ++i) {
            if (buffer[i] < MIN_TARGET || buffer[i] > MAX_TARGET)
                invalid_data_count++;
        }
        if (invalid_data_count > 20)
            mcu_reset();

        /* Filter data from radio */
        target = data + past[0] + past[1] + past[2];
        target >>= 2;
        past[2] = past[1];
        past[1] = past[0];
        past[0] = target;

        motor_set_target(target);
    }

    return 0;
}
