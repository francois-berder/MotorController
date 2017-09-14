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

#include <assert.h>
#include "status.h"
#include "periph/gpio.h"
#include "periph/timer.h"
#include "periph_conf.h"

static uint8_t pin;
static uint8_t counter;
static uint8_t mode;

static void status_callback(void)
{
    switch (mode) {
    case STATUS_BLINK_FAST:
        gpio_toggle(pin);
        break;
    case STATUS_FLASH:
        ++counter;
        if (counter == 20 && !gpio_read(pin)) {
            gpio_write(pin, 1);
            counter = 0;
        } else if (counter == 1 && gpio_read(pin)) {
            gpio_write(pin, 0);
            counter = 0;
        }
        break;
    }
}

void status_init(uint8_t led_pin)
{
    mode = STATUS_OFF;
    counter = 0;
    pin = led_pin;
    gpio_init_out(pin, 0);

    /* Timer 2 configured to interrupt 10 times/second with Fosc = 8MHz */
    timer_configure(2, TIMER2_PRESCALER_64, TIMER2_POSTCALER_14, 223, status_callback);
}

void status_set_mode(uint8_t status_mode)
{
    assert(status_mode <= STATUS_BLINK_FAST);

    mode = status_mode;
    counter = 0;

    if (mode == STATUS_ON || mode == STATUS_OFF) {
        timer_stop(2);
    } else {
        timer_start(2);
    }

    if (mode == STATUS_ON) {
        gpio_write(pin, 1);
    } else {
        gpio_write(pin, 0);
    }
}
