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

#ifndef __MOTOR_H
#define __MOTOR_H

#include <stdint.h>

/**
 * @brief Initiale motor
 *
 * This function does not initialise the pwm pins.
 * Both PWM are configured and enabled but duty cycle is set to 0.
 *
 * @param[in] left_pwm
 * @param[in] right_pwm
 */
void motor_init(uint8_t left_pwm, uint8_t right_pwm);

/**
 * @brief Set target
 *
 * @param[in] target in range 2000.4000
 */
void motor_set_target(uint16_t target);

/**
 * @brief Update PWM duty cycle given current target
 */
void motor_tick(void);

#endif
