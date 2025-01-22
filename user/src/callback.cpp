#include "callback.hpp"
#include <functional>

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t Size)
{
    auto& receiveUartCallback = callback::UART_Handle_To_ReceiveUartCallback {}[huart->Instance];
    if (receiveUartCallback != nullptr)
        receiveUartCallback(reinterpret_cast<char*>(huart->pRxBuffPtr), Size);
}

namespace callback {

UartCallbackType UARTRecvCallback[4] { nullptr, nullptr, nullptr, nullptr };

}