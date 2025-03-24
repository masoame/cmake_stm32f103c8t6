#pragma once
#include <callback.hpp>
#include <common.hpp>
#include <cstddef>
#include <cstdint>
#include <memory>


namespace serialport {

    //template<DriverType driver_type = INTERRUPT>
    class Driver 
    {
        constexpr static const std::size_t m_recv_buffer_size = 512;
    public:

        enum DriverMode{
            DMA = 0,
            INTERRUPT,
            POLLING,
        };

        UART_HandleTypeDef* m_huart;
        std::unique_ptr<uint8_t[]> m_recv_buffer;
        callback::UartCallbackType& m_recv_callback;


        Driver(UART_HandleTypeDef* huart,DriverMode driver_type);
        ~Driver();

    public:
        
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

            inline Driver::ResponseFlag& operator=(const ResponseType Type){
                m_data.num &= ~0x3;
                m_data.num |= Type;
                return *this;
            }

            inline Driver::ResponseFlag& operator=(const uint8_t num){
                m_data.num &= 0x3;
                m_data.num |= (num << 2);
                return *this;
            }

            ResponseFlag(ResponseType Type):m_data{Type}{}
            ResponseFlag():m_data{RESPONSE_TYPE_UNKNOWN}{}
        };

        //异步读
		bool OpenAsyncRecv(callback::UartCallbackType task = nullptr) const;
		bool ContinueAsyncRecv() const;
		bool CloseAsyncRecv() const;
		bool WaitForRecvCallback(const std::chrono::milliseconds& ms) const;

        //同步读
        bool Recv(uint16_t& RxLen,const std::chrono::milliseconds& time_out) ;
        ResponseFlag GetResponse(const std::string& cmd,const std::chrono::milliseconds& ms, const std::initializer_list<std::string>& search_list, const uint8_t count = 1) ;
        std::string GetResponseString(const std::string& cmd,const std::chrono::milliseconds& ms, const uint8_t count = 1);

        //转发端口信息
        bool StartForwardSerialPort(::serialport::Driver& target_serialport) const;
		bool StopForwardSerialPort() const;
        
        //信息过滤器
        virtual bool Filter(uint16_t& len) = 0;
    };
}