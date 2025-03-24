#pragma once
//关闭编译器警告
#pragma GCC diagnostic push
#include "stm32f103xb.h"
#include "main.h"
//打开编译器警告
#pragma GCC diagnostic pop
#include<string>
#include<chrono>
#include <functional>

#define DISABLE_INTERRUPTS()  __asm__ __volatile__("": : :"memory")
#define BREAK_POINT() __asm__ __volatile__("bkpt #0")

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

    inline const auto LED_GREEN_Blink = std::bind(LED_Blink, GPIOC, GPIO_PIN_13, std::placeholders::_1);
    inline const auto LED_RED_Blink = std::bind(LED_Blink, GPIOC, GPIO_PIN_14, std::placeholders::_1);
    inline const auto LED_YELLOW_Blink = std::bind(LED_Blink, GPIOC, GPIO_PIN_15, std::placeholders::_1);
    inline const auto LED_WHITE_Blink = std::bind(LED_Blink, GPIOA, GPIO_PIN_0, std::placeholders::_1);
    //inline const auto& LED1_Blink = LED_GREEN_Blink;

    extern void puts(const std::string& str);

    template<typename...  Args>
    inline std::string FormatString(const char* format, Args&&... args){

        int len = std::snprintf(nullptr, 0, format, args...);
        std::string result(len + 1, '\0');
        std::snprintf(&result[0], len + 1, format, args...);
        return result;
    }

    __weak void puts([[maybe_unused]] const std::string& str);

    void puts([[maybe_unused]] const std::string& str){
        HAL_UART_Transmit(&huart1, (uint8_t*)str.c_str(), str.size(), 1000);
    };

    template <typename... Args>
    inline void printf(const char* format, Args&&... args){
        auto str = FormatString(format, std::forward<Args>(args)...);
        puts(str);
    }

    inline void delay_ns(const uint32_t ns)
    {
        uint32_t i;
        for(i=0; i<ns ; ++i)
        {
            __asm("nop");
            __asm("nop");
            __asm("nop");
        }
    }
}
