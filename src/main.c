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
#pragma config WDTE = OFF
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
#include "periph_conf.h"
#include "periph/gpio.h"
#include "status.h"

#define STATUS_LED          (GPIO_PIN(PORT_C, 0))

int main()
{
    mcu_init();

    /* Change clock speed to 8MHz */
    OSCCON = (0b1110 << _OSCCON_IRCF_POSITION);

    status_init(STATUS_LED);
    status_set_mode(STATUS_BLINK_FAST);

    while (1) {

    }

    return 0;
}
