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
#include "mcu.h"
#include "motor.h"
#include "periph_conf.h"
#include "periph/gpio.h"
#include "periph/timer0.h"
#include "periph/timer1.h"
#include "periph/watchdog.h"
#include "radio.h"
#include "status.h"

#define STATUS_LED          (GPIO_PIN(PORT_C, 0))
#define RADIO_PIN           (GPIO_PIN(PORT_A, 4))

#define LEFT_PWM_PIN        (GPIO_PIN(PORT_C, 3))
#define RIGHT_PWM_PIN       (GPIO_PIN(PORT_C, 1))
#define LEFT_PWM            (2)
#define RIGHT_PWM           (4)

int main()
{
    mcu_init();
    timer0_configure(TIMER0_PRESCALER_256);
    watchdog_configure(WATCHDOG_PERIOD_1S);
    watchdog_enable();

    /* Change clock speed to 8MHz */
    OSCCON = (0b1110 << _OSCCON_IRCF_POSITION);

    radio_init(RADIO_PIN);

    motor_init(LEFT_PWM, RIGHT_PWM);
    gpio_init_out(LEFT_PWM_PIN, 0);
    gpio_init_out(RIGHT_PWM_PIN, 0);

    status_init(STATUS_LED);
    status_set_mode(STATUS_BLINK_FAST);

    while (1) {
        motor_tick();
        mcu_delay(1);
        if (!radio_has_data())
            continue;

        motor_set_target(radio_get_data());
    }

    return 0;
}
