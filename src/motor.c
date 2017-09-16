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

#include "constants.h"
#include "motor.h"
#include "periph_conf.h"
#include "periph/gpio.h"
#include "periph/pwm.h"

#define MAX_STEP        (16)
#define MIN_POWER       (25)
#define FORWARD         (1)
#define BACKWARD        (0)

static uint16_t neutral_pos;
static uint16_t target;
static uint16_t position;
static uint8_t left_pwm;
static uint8_t right_pwm;

static void motor_stop(void)
{
    pwm_set_duty_cycle(left_pwm, 0);
    pwm_set_duty_cycle(right_pwm, 0);
}

void motor_init(uint8_t left_pin, uint8_t right_pin,
                uint8_t _left_pwm, uint8_t _right_pwm, uint16_t neutral)
{
    neutral_pos = neutral;
    position = neutral_pos;
    target = neutral_pos;

    left_pwm = _left_pwm;
    right_pwm = _right_pwm;

    pwm_configure(left_pwm, 4, 0xFF, TIMER4_PRESCALER_1, TIMER4_POSTCALER_1);
    pwm_configure(right_pwm, 6, 0xFF, TIMER6_PRESCALER_1, TIMER6_POSTCALER_1);
    pwm_enable(left_pwm);
    pwm_enable(right_pwm);
    motor_stop();
    gpio_init_out(left_pin, 0);
    gpio_init_out(right_pin, 0);
}

void motor_set_target(uint16_t _target)
{
    if (_target >= MIN_TARGET && _target <= MAX_TARGET)
        target = _target;
}

void motor_tick(void)
{
    uint8_t dir, target_dir;
    uint16_t power;

    /* Move position towards target */
    if (position > target) {
        uint16_t diff = position - target;
        if (diff > MAX_STEP)
            diff = MAX_STEP;
        position -= diff;
    } else {
        uint16_t diff = target - position;
        if (diff > MAX_STEP)
            diff = MAX_STEP;
        position += diff;
    }

    /* Check if direction must be changed */
    dir = position > neutral_pos;
    target_dir = target > neutral_pos;
    if (target_dir != dir) {
        motor_stop();
        return;
    }

    /* Apply power */
    if (dir == FORWARD)
        power = position - neutral_pos;
    else
        power = neutral_pos - position;

    if (power < MIN_POWER) {
        motor_stop();
        return;
    }

    /* Convert power to PWM duty cycle */
    if (power > 0xFF)
        power = 0xFF;

    if (dir == FORWARD) {
        pwm_set_duty_cycle(right_pwm, power);
        pwm_set_duty_cycle(left_pwm, 0);
    } else {
        pwm_set_duty_cycle(left_pwm, power);
        pwm_set_duty_cycle(right_pwm, 0);
    }
}
