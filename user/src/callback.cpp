#include "callback.hpp"



void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t Size)
{
    huart->pRxBuffPtr[0]=0;
	huart->pRxBuffPtr -= Size;
    auto& receiveUartCallback = callback::UART_Handle_To_ReceiveUartCallback[huart->Instance];

    if(receiveUartCallback != nullptr) receiveUartCallback(reinterpret_cast<char*>(huart->pRxBuffPtr), Size);
    
}


namespace callback{

    std::map<USART_TypeDef*,callback::UartCallbackType> UART_Handle_To_ReceiveUartCallback{
        //{USART1,nullptr},
        //{USART2,nullptr},
        //{USART3,nullptr},
    };
}