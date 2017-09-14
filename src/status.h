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

#ifndef __STATUS_H
#define __STATUS_H

#include <stdint.h>

enum STATUS_MODE {
    STATUS_OFF,
    STATUS_ON,
    STATUS_FLASH,       /* on: 100ms, off: 2s */
    STATUS_BLINK_FAST   /* on: 100ms, off:100ms */
};

/**
 * @brief Initialise status LED
 *
 * Set mode to STATUS_OFF, configure gpio and configure timer 2
 *
 * @param[in] pin
 */
void status_init(uint8_t pin);

/**
 * @brief Change mode of status LED
 *
 * Start timer 2 if mode is STATUS_FLASH or STATUS_BLINK_FAST
 * Stop timer 2 if mode is off or on.
 *
 * @param[in] mode
 */
void status_set_mode(uint8_t mode);

#endif
