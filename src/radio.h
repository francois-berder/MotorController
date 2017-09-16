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

#ifndef __RADIO_H
#define __RADIO_H

#include <stdint.h>

/**
 * @brief Initialse radio module
 *
 * Initialise input_pin as an input and configure timer 1.
 *
 * @param[in] input_pin
 */
void radio_init(uint8_t input_pin);

/**
 * @retval 0 if no data has been received
 * @retval 1 if new data is present
 */
uint8_t radio_has_data(void);

/**
 * @return 16-bit data
 */
uint16_t radio_get_data(void);

#endif
