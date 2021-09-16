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

#ifndef TASK_H_
#define TASK_H_

// === Includes ===
#include "processors.h"

// === defines ===

/**
 * @brief This macro can be used to wait as long as 
 * the condition is true without blocking other tasks.
 */

#define YIELD_WHILE(condition) while(condition) { __otos_yield(); }

namespace OTOS {

    // *** Class Definitions ***
    class Task
    {
    private:
        unsigned int LastTick;
    public:
        Task();
        static void lock(void);
        static void unlock(void);
        // static void waitFor(bool Condition);
        static void yield(void);
        void sleep(unsigned int Time);
    };
};

#endif