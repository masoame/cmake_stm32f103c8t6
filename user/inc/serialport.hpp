#pragma once
#include <callback.hpp>
#include <common.hpp>
#include <memory>

namespace serialport {

    class Driver 
    {

        UART_HandleTypeDef* m_huart;
        constexpr static const std::size_t m_recv_buffer_size = 512;
		std::unique_ptr<uint8_t[]> m_recv_buffer;
    public:
        Driver(UART_HandleTypeDef* huart);
        ~Driver();
    private:

        callback::UartCallbackType& GetRecvCallback() const;
		void SetRecvCallback(const callback::UartCallbackType& recvCallback) const;
		bool HasRecvCallback() const;
        

		bool OpenAsyncRecv(callback::UartCallbackType task = nullptr) const;
		bool ContinueAsyncRecv() const;
		bool CloseAsyncRecv() const;


		bool WaitForRecvCallback(const std::chrono::milliseconds ms) const;

    public:
    	bool StartForwardSerialPort(::serialport::Driver& target_serialport) const;
		bool StopForwardSerialPort() const;
        

        enum ResponseType {
            RESPONSE_TYPE_OK,
            RESPONSE_TYPE_ERROR,
            RESPONSE_TYPE_TIMEOUT,
            RESPONSE_TYPE_BUSY,
            RESPONSE_TYPE_UNKNOWN,
        };
        //bool Only
        ResponseType GetResponse(const std::string& cmd,const std::chrono::milliseconds ms, const std::initializer_list<std::string>& search_list) const;

        

    };
}