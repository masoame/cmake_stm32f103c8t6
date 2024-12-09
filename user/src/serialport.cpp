#include "serialport.hpp"
#include "callback.hpp"
#include "stm32f1xx_hal.h"
#include <chrono>
#include <cstdint>

namespace serialport {

    Driver::Driver(UART_HandleTypeDef* huart):m_huart(huart),m_recv_buffer(new uint8_t[Driver::m_recv_buffer_size])
    {

    }
    Driver::~Driver()
    {

    }



    inline callback::UartCallbackType& Driver::GetRecvCallback() const{
        return callback::UART_Handle_To_ReceiveUartCallback{}[this->m_huart->Instance];
    }

	inline void Driver::SetRecvCallback(const callback::UartCallbackType& recvCallback) const{
		callback::UART_Handle_To_ReceiveUartCallback{}[this->m_huart->Instance] = recvCallback;
	}

	inline bool Driver::HasRecvCallback() const{
		return callback::UART_Handle_To_ReceiveUartCallback{}[this->m_huart->Instance] != nullptr;
	}

	inline bool Driver::OpenAsyncRecv(callback::UartCallbackType task) const 
    {
		auto& uart_recv_callback = this->GetRecvCallback();

		if(uart_recv_callback == nullptr){

			if(task != nullptr) this->SetRecvCallback(task);
			
			while(HAL_UARTEx_ReceiveToIdle_IT(this->m_huart, this->m_recv_buffer.get(), Driver::m_recv_buffer_size)== HAL_ERROR){}
			return true;
		} 
		return false;
	}

	inline bool Driver::ContinueAsyncRecv() const{
        return HAL_UARTEx_ReceiveToIdle_IT(this->m_huart, this->m_recv_buffer.get(), Driver::m_recv_buffer_size) == HAL_OK;
	}

	inline bool Driver::CloseAsyncRecv() const {
        this->SetRecvCallback(nullptr);
        return HAL_UART_AbortReceive(this->m_huart) == HAL_OK;
	}

	bool Driver::StartForwardSerialPort(::serialport::Driver& target_serialport) const
	{
		return this->OpenAsyncRecv([this,&target_serialport](char* data, std::size_t len)->void{
			while(HAL_UART_Transmit_IT(target_serialport.m_huart, reinterpret_cast<uint8_t*>(data) , len)==HAL_BUSY){};
			this->ContinueAsyncRecv();
		});
	}
	bool Driver::StopForwardSerialPort() const
	{
		return this->CloseAsyncRecv();
	}


	bool Driver::WaitForRecvCallback(const std::chrono::milliseconds _ms) const
    {
		uint32_t tickstart = HAL_GetTick();
		uint32_t wait = _ms.count();
		if (wait < HAL_MAX_DELAY)
		{
			wait += (uint32_t)(uwTickFreq);
		}

		auto& uart_recv_callback = this->GetRecvCallback();
		
		while((uart_recv_callback != nullptr) && ((HAL_GetTick() - tickstart) < wait)) {};

		return uart_recv_callback == nullptr;
	}

	Driver::ResponseFlag Driver::GetResponse(const std::string& cmd,const std::chrono::milliseconds ms,const std::initializer_list<std::string>& search_list) const{

		Driver::ResponseFlag flag;

		if(OpenAsyncRecv([this,&search_list,&flag](char* data, std::size_t len)->void{

			char* _start = data;
			char* _end = data + len;

			flag = Driver::ResponseType::RESPONSE_TYPE_ERROR;
			uint8_t num = 0;

			for(auto& search_str : search_list){
				if(std::search(_start, _end, search_str.begin(), search_str.end()) != _end){
					flag = ResponseType::RESPONSE_TYPE_OK;
					flag = num;
					this->CloseAsyncRecv();
					return;
				}
				num++;
			}
			this->ContinueAsyncRecv();
		}) == false){
			this->CloseAsyncRecv();
			return ResponseType::RESPONSE_TYPE_UNKNOWN;
		}

		if(cmd.empty() == false && cmd.size()!=0 && HAL_UART_Transmit_IT(this->m_huart, reinterpret_cast<const uint8_t*>(cmd.data()), cmd.size())!= HAL_OK){
			flag = ResponseType::RESPONSE_TYPE_UNKNOWN;
		}
		
		if(this->WaitForRecvCallback(ms)==false && flag != ResponseType::RESPONSE_TYPE_ERROR){
			flag = ResponseType::RESPONSE_TYPE_TIMEOUT;
		}
        this->CloseAsyncRecv();
		return flag;
	}
}