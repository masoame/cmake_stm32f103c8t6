#pragma once
//--------------------------------------------
#include<stm32f1xx_hal.h>
#include<stm32f1xx_hal_uart.h>
//--------------------------------------------
#include<main.h>
#include <cstdint>
#include<string>
#include<chrono>
#include <functional>

#define DISABLE_INTERRUPTS()  __asm__ __volatile__("": : :"memory")


namespace common {

    using namespace std::chrono_literals;
    constexpr std::chrono::milliseconds Delay_Time(50ms);

    using GPIO_PORT = GPIO_TypeDef*;
    using GPIO_PIN = uint16_t;

    inline void LED_Blink(GPIO_PORT port, GPIO_PIN pin ,std::size_t count){
        for(std::size_t i=0; i<count*2; ++i){
            HAL_GPIO_TogglePin(port, pin);
            HAL_Delay(Delay_Time.count());
        }
    }

    inline const auto LED1_Blink = std::bind(LED_Blink, GPIOC, GPIO_PIN_13, std::placeholders::_1);
}
