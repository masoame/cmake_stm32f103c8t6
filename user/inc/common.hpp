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

#define KEY1() HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_11)
#define KEY2() HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_12)
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

    #define LED_GREEN_Blink(count) LED_Blink(GPIOC,GPIO_PIN_13,count)
    #define LED_RED_Blink(count) LED_Blink(GPIOC,GPIO_PIN_14,count)
    #define LED_YELLOW_Blink(count) LED_Blink(GPIOC,GPIO_PIN_15,count)
    #define LED_WHITE_Blink(count) LED_Blink(GPIOA,GPIO_PIN_0,count)

    // inline const auto& LED_GREEN_Blink = std::bind(LED_Blink, GPIOC, GPIO_PIN_13, std::placeholders::_1);
    // inline const auto& LED_RED_Blink = std::bind(LED_Blink, GPIOC, GPIO_PIN_14, std::placeholders::_1);
    // inline const auto& LED_YELLOW_Blink = std::bind(LED_Blink, GPIOC, GPIO_PIN_15, std::placeholders::_1);
    // inline const auto& LED_WHITE_Blink = std::bind(LED_Blink, GPIOA, GPIO_PIN_0, std::placeholders::_1);


    extern void puts(const std::string& str);

    template<typename...  Args>
    inline std::string FormatString(const char* format, Args&&... args){

        int len = std::snprintf(nullptr, 0, format, args...);
        std::string result(len + 1, '\0');
        std::snprintf(result.data(), len + 1, format, args...);
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

    enum KeyState : uint8_t
    {
        NO_Press = 0x00,
        Key1_Press = 0x01,
        Key2_Press = 0x02,

    };


// inline const auto& KEY1 = std::bind(HAL_GPIO_ReadPin, GPIOA, GPIO_PIN_11);
// inline const auto KEY2 = std::bind(HAL_GPIO_ReadPin, GPIOA, GPIO_PIN_12);

    inline KeyState KEY_Scan(uint8_t mode)
    {
        static bool key_up = true;  
        if(mode==true)
            key_up=true;
        if(key_up && (KEY1()==1/*||KEY2() ==1*/))
        {
            HAL_Delay(10);
            key_up= false;
            if(KEY1() == 1){
                return Key1_Press; 
            }// else if(KEY2() == 1){
            //     return Key2_Press;
            // } 
        }else if(KEY1()==0 /*&& KEY2() ==0*/){
            key_up=1;
        }
        return NO_Press; 
    }
}
