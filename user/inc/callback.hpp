#pragma once
#include "common.hpp"
#include "stm32f103xb.h"


extern void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t Size);

namespace callback{
    using  UartCallbackType = std::function<void(char* ,std::size_t)>;

    extern UartCallbackType UARTRecvCallback[4];

    struct UART_Handle_To_ReceiveUartCallback{
        inline UartCallbackType& operator[](USART_TypeDef* uart){
            switch (reinterpret_cast<unsigned long>(uart)) {
                case USART1_BASE:
                    return UARTRecvCallback[0];
                case USART2_BASE:
                    return UARTRecvCallback[1];
                case USART3_BASE:
                    return UARTRecvCallback[2];
                default:
                    return UARTRecvCallback[3];
            }
        }
    };
}