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

#ifndef INTERFACE_H_
#define INTERFACE_H_

// === Includes ===

// === Define the Interfaces ===

// => GPIO Interface
namespace GPIO {

    // === Enums ===
    enum PinPort
    {
        PORTA = 0, PORTB, PORTC, PORTD, PORTE, PORTF,
        PORTG, PORTH, PORTI, PORTJ, PORTK 
    };
    enum PinNumber: unsigned char
    {
        PIN0 = 0, PIN1, PIN2, PIN3, PIN4, PIN5, PIN6, PIN7,
        PIN8, PIN9, PIN10, PIN11, PIN12, PIN13, PIN14, PIN15
    };
    enum Mode: unsigned char
    {
        INPUT = 0, OUTPUT, AF_Mode, ANALOG
    };
    enum Type: bool
    {
        PUSH_PULL = false, OPEN_DRAIN = true
    };
    enum Speed: unsigned char
    {
        LOW = 0, MEDIUM, HIGH, VERY_HIGH
    };
    enum Pull: unsigned char
    {
        NO_PP = 0, PULL_UP, PULL_DOWN
    };

    // === GPIO Interface ===
    /**
     * @brief Abstract interface class for implementing
     * GPIO drivers. Use the `final` keyword in the implementation
     * of the final functions for the compiler to do its
     * optimization thing.
     */
    class PIN_Base
    {
    public:
        // Methods
        virtual void setMode    (const Mode NewMode)    = 0;
        virtual void setType    (const Type NewType)    = 0;
        virtual void setSpeed   (const Speed NewSpeed)  = 0;
        virtual void setPull    (const Pull NewPull)    = 0;
        virtual void set        (const bool NewState)   = 0;
        virtual void setHigh    (void)                  = 0;
        virtual void setLow     (void)                  = 0;
        virtual void toggle     (void)                  = 0;
        virtual bool get        (void) const            = 0;
    };
};

// => I2C Interface
namespace I2C {
    // === Enums ===
    enum Instance: unsigned char
    {
        I2C_1 = 0, I2C_2, I2C_3
    };

    enum State: unsigned char
    {
        Init = 1, Idle, Busy, Error
    };

    union Data_t
    {
        unsigned long value;
        unsigned int word[2];
        unsigned char byte[4];
    };
    

    // === Interface ===
    class Controller_Base
    {
    public:
        // *** Methods ***
        // virtual void            set_target          (const unsigned char address)   = 0;
        // virtual void            set_clock_pin       (GPIO::PIN_Base& clock_pin)     = 0;
        // virtual unsigned char   get_current_target  (void) const                    = 0;
        // virtual Data_t          get_rx_data         (void) const                    = 0;
        // virtual int             get_error           (void) const                    = 0;
    };

}
#endif