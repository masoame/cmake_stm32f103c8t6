#pragma once
#include <callback.hpp>
#include <common.hpp>
#include <cstdint>
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
        
        using ResponseType_Origin = uint8_t;
        enum ResponseType: ResponseType_Origin
        {
            RESPONSE_TYPE_ERROR,
            RESPONSE_TYPE_OK,
            RESPONSE_TYPE_TIMEOUT,
            RESPONSE_TYPE_UNKNOWN,
        };
        struct ResponseFlag{
            union{
                ResponseType _T;
                ResponseType_Origin num;
            } m_data;

            inline operator ResponseType() const{
                return static_cast<ResponseType>(m_data.num & 0x3);
            }

            inline operator uint8_t() const{
                return (m_data.num >> 2);
            }

            inline Driver::ResponseFlag& operator=(ResponseType Type){
                m_data.num &= ~0x3;
                m_data.num |= Type;
                return *this;
            }

            inline Driver::ResponseFlag& operator=(uint8_t num){
                m_data.num &= 0x3;
                m_data.num |= (num << 2);
                return *this;
            }

            ResponseFlag(ResponseType Type):m_data{Type}{}
            ResponseFlag():m_data{RESPONSE_TYPE_UNKNOWN}{}
        };

        //GetResponseType GetResponseType(const std::string& response) const;

        //bool Only
        ResponseFlag GetResponse(const std::string& cmd,const std::chrono::milliseconds ms, const std::initializer_list<std::string>& search_list) const;

        

    };
}