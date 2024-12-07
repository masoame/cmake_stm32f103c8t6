#pragma once
#include "common.hpp"
#include <map>


extern void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t Size);

namespace callback{
    using  UartCallbackType = std::function<void(char* ,std::size_t)>;

    extern std::map<USART_TypeDef*,callback::UartCallbackType> UART_Handle_To_ReceiveUartCallback;
}