/**
 * OTOS - Open Tec Operating System
 * Copyright (c) 2021 Sebastian Oberschwendtner, sebastian.oberschwendtner@gmail.com
 *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#ifndef I2C_STM32_H_
#define I2C_STM32_H_

// === Includes ===
#include "vendors.h"

// === Needed Interfaces ===
#include "interface.h"

// === Declarations ===
namespace I2C {

    // === Classes ===
    class Controller: public Controller_Base
    {
    private:
        // *** Properties ***
        volatile I2C_TypeDef*   peripheral;
        unsigned char           target;

    public:
        // *** Constructor ***
        Controller(const Instance i2c_instance, const unsigned long frequency);

        // *** Methods ***
        void            set_target_address  (const unsigned char address);
        void            assign_pin          (GPIO::PIN_Base& output_pin) const;
        unsigned char   get_target_address  (void) const;
        Data_t          get_rx_data         (void) const;
        int             get_error           (void) const;
    };

};
#endif