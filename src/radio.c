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

#include <xc.h>
#include "mcu.h"
#include "periph_conf.h"
#include "periph/gpio.h"
#include "periph/timer1.h"
#include "periph/watchdog.h"
#include "radio.h"

static uint8_t pin;
static volatile uint16_t value;
static volatile uint8_t is_new;

static void radio_callback(void)
{
    if (IOCAP & (1U << (pin & 0x4))) {
        TMR1L = 0;
        TMR1H = 0;
        timer1_start();
        gpio_init_irq(pin, GPIO_FALLING, radio_callback);
    } else {
        timer1_stop();
        is_new = 1;
        value = TMR1H;
        value <<= 8;
        value |= TMR1L;

        watchdog_kick();
        gpio_init_irq(pin, GPIO_RISING, radio_callback);
    }
}

void radio_init(uint8_t input_pin)
{
    is_new = 0;
    pin = input_pin;
    gpio_init_irq(pin, GPIO_RISING, radio_callback);
    timer1_configure(TIMER1_PRESCALER_1, 0);
}

uint8_t radio_has_data(void)
{
    return is_new;
}

uint16_t radio_get_data(void)
{
    uint16_t v;
    mcu_disable_interrupts();
    is_new = 0;
    v = value;
    mcu_enable_interrupts();
    return v;
}
