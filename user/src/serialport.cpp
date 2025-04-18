#include "serialport.hpp"
#include "callback.hpp"
#include <cstddef>
#include <cstdint>

using namespace serialport;

Driver::Driver(UART_HandleTypeDef* huart, [[maybe_unused]] DriverMode driver_type)
    : m_huart(huart)
    , m_recv_buffer(new uint8_t[Driver::m_recv_buffer_size])
    , m_recv_callback(callback::UART_Handle_To_ReceiveUartCallback {}[this->m_huart->Instance])
{
}
Driver::~Driver()
{
}

bool Driver::OpenAsyncRecv(callback::UartCallbackType task) const
{
    if (this->m_recv_callback == nullptr) {
        if (task != nullptr) this->m_recv_callback = task;

        if (HAL_UARTEx_ReceiveToIdle_DMA(this->m_huart, this->m_recv_buffer.get(), Driver::m_recv_buffer_size) != HAL_OK) {
            return false;
        }
        return true;
    }

    return false;
}

bool Driver::ContinueAsyncRecv() const
{
    return HAL_UARTEx_ReceiveToIdle_DMA(this->m_huart, this->m_recv_buffer.get(), Driver::m_recv_buffer_size) == HAL_OK;
}

bool Driver::CloseAsyncRecv() const
{
    m_recv_callback = nullptr;
    return HAL_UART_Abort(this->m_huart) == HAL_OK;
}

bool Driver::StartForwardSerialPort(::serialport::Driver& target_serialport) const
{
    return this->OpenAsyncRecv([this, &target_serialport](char* data, std::size_t len) -> void {
        while (HAL_UART_Transmit_IT(target_serialport.m_huart, reinterpret_cast<uint8_t*>(data), len) == HAL_BUSY) { };
        this->ContinueAsyncRecv();
    });
}
bool Driver::StopForwardSerialPort() const
{
    return this->CloseAsyncRecv();
}

Driver::ResponseFlag Driver::GetResponse(std::string_view cmd, const std::chrono::milliseconds& ms, const std::initializer_list<std::string>& search_list, const uint8_t count)
{
    Driver::ResponseFlag flag;
    auto str = this->GetResponseString(cmd, ms, count);

    if (str.empty() == true || str == "") {
        flag = ResponseType::RESPONSE_TYPE_UNKNOWN;
        return flag;
    }
    uint8_t num = 0;
    for (const auto& search_str : search_list) {
        if (std::search(str.begin(), str.end(), search_str.begin(), search_str.end()) != str.end()) {
            flag = ResponseType::RESPONSE_TYPE_OK;
            flag = num;
            break;
        } else {
            flag = ResponseType::RESPONSE_TYPE_ERROR;
        }
        num++;
    }
    return flag;
}
std::string Driver::GetResponseString(std::string_view cmd, const std::chrono::milliseconds& ms, const uint8_t count)
{
    std::string response;
    uint16_t RxLen;
    if (cmd.empty() == false && cmd != "") {
        auto ret = HAL_UART_Transmit(&huart2, (const uint8_t*)cmd.data(), cmd.size(), ms.count());
        if (ret != HAL_OK) {
            return response;
        }
    }
    for (int i = 0; i < count; i++) {
        if(Recv(RxLen,ms) == false) continue;
        response.append(this->m_recv_buffer.get(), this->m_recv_buffer.get() + RxLen);
    }
    return response;
}

bool Driver::Recv(uint16_t& RxLen,const std::chrono::milliseconds& ms)
{
    auto ret = HAL_UARTEx_ReceiveToIdle(this->m_huart, this->m_recv_buffer.get(), Driver::m_recv_buffer_size, (uint16_t*)&RxLen, ms.count());
    if (ret == HAL_OK && Filter(RxLen) == true) {
        return true;
    }
    return false;
}

