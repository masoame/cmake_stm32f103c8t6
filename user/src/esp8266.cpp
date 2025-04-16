#include "esp8266.hpp"
#include "common.hpp"
#include "serialport.hpp"
#include <charconv>
#include <cstdint>
#include <algorithm>
#include <string>
#include <string_view>
#include <system_error>
namespace wifi {
using namespace std::chrono_literals;

esp8266::esp8266(UART_HandleTypeDef* esp8266_huart, const std::string& ssid, const std::string& password, const std::string& ip, unsigned short port)
    : ::serialport::Driver(esp8266_huart, serialport::Driver::INTERRUPT)
    , m_ssid(ssid)
    , m_password(password)
    , m_ip(ip)
    , m_port(port)
{
    this->Reset();
}

bool esp8266::Reset()
{
    if (this->m_ssid.empty() || this->m_ssid == "" || this->m_password.empty() || this->m_password == "" || this->m_ip.empty() || this->m_ip == "" || m_port == 0) {
        return false;
    }
    do  {
        if (this->ConnectWifi(m_ssid, m_password) == serialport::Driver::RESPONSE_TYPE_OK)
            break;
        common::LED_YELLOW_Blink(4);
    }while(this->WifiIsConnected() == false);
    while (this->LinkTcp(m_ip, m_port) != serialport::Driver::RESPONSE_TYPE_OK) {
        common::LED_YELLOW_Blink(4);
    }
    //this->SendTcp("hello world\r\n");
    return true;
}

bool esp8266::PowerOn()
{
    return this->GetResponse({}, 1min, { "WIFI CONNECTED" }) == Driver::ResponseType::RESPONSE_TYPE_OK;
}
bool esp8266::PowerOff()
{
}

inline bool esp8266::WifiIsConnected()
{
    return this->GetResponse("AT+CWJAP_CUR?\r\n", 3s, { "No AP" }) == Driver::ResponseType::RESPONSE_TYPE_ERROR;
}

bool esp8266::DisconnectWifi()
{
    return this->GetResponse("AT+CWQAP\r\n", 2s, { "OK", "busy p..." }, 3) == Driver::ResponseType::RESPONSE_TYPE_OK;
}

void esp8266::Ping([[maybe_unused]] const std::string& ip)
{
    //std::string _cmd = std::vformat("AT+PING={}\r\n", std::make_format_args(ip));
}

void esp8266::CloseTcp()
{
}

esp8266::Driver::ResponseFlag esp8266::ConnectWifi(const std::string& ssid, const std::string& password)
{
    std::string _cmd = common::FormatString("AT+CWJAP=\"%s\",\"%s\"\r\n", ssid.c_str(), password.c_str());

    return this->GetResponse(_cmd, 5s, { "WIFI CONNECTED" });
}

esp8266::Driver::ResponseFlag esp8266::LinkTcp(const std::string& ip, unsigned short port)
{

    std::string _cmd = common::FormatString("AT+CIPSTART=\"TCP\",\"%s\",%d\r\n", ip.c_str(), port);

    return this->GetResponse(_cmd, 5s, { "OK", "ALREADY CONNECTED" });
}

esp8266::Driver::ResponseFlag esp8266::SendTcp(std::string_view data)
{
    if (data.empty() || data == "") return Driver::ResponseType::RESPONSE_TYPE_ERROR;
    common::LED_WHITE_Blink(2);
    std::string _cmd = common::FormatString("AT+CIPSENDEX=%d\r\n", data.size());

    if (this->GetResponse(_cmd, 500ms, { "\r\n>" }) != Driver::ResponseType::RESPONSE_TYPE_OK)
        return Driver::ResponseType::RESPONSE_TYPE_ERROR;

    return this->GetResponse(data, 4s, { "busy s...", "Recv", "SEND OK" });
}

bool esp8266::Filter(uint16_t& len)
{ 
    static std::string_view search_str = "+IPD,";
    std::string_view recv_buf {(char*)this->m_recv_buffer.get(),len};
    std::size_t pos;
    if ((pos = recv_buf.find(search_str, 0)) != std::string_view::npos) {
        std::size_t num_start = pos + search_str.size();
        if((pos = recv_buf.find_first_of(':',num_start))!=std::string_view::npos){
            unsigned short value = 0;
            auto [ptr,err] = std::from_chars(recv_buf.data() + num_start, recv_buf.data()+pos, value);
            if(err == std::errc() && value!=0){
                std::string_view server_data {recv_buf.data()+pos+1,value};
                if(server_data == "ok!"){
                    common::LED_GREEN_Blink(2);
                }else if(server_data == "error!"){
                    common::LED_RED_Blink(2);
                }
            }
        }
        return false;
    } 
    return true; 
};

}