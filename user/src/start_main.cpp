
#include "start_main.hpp"
#include "atgm336h.hpp"
#include "common.hpp"
#include "esp8266.hpp"

#include "rc522.hpp"
#include <cstdint>

constexpr auto RC522_SDA_Pin = GPIO_PIN_8;
constexpr auto RC522_SDA_GPIO_Port = GPIOB_BASE;
constexpr auto RC522_RST_Pin = GPIO_PIN_9;
constexpr auto RC522_RST_GPIO_Port = GPIOB_BASE;

enum Model{
    recv,
    send
};

using namespace std::literals;
void start_main()
{
    wifi::esp8266 _wifi(&huart2, "gt", "1658932642", "192.168.149.253", 8080);
    gps::atgm336h _gps(&huart1);
    rfid::rc522 _rfid(&hspi2, reinterpret_cast<GPIO_TypeDef*>(RC522_SDA_GPIO_Port), RC522_SDA_Pin, reinterpret_cast<GPIO_TypeDef*>(RC522_RST_GPIO_Port), RC522_RST_Pin);

    //_gps.SendToWifi(_wifi);
    Model model = send;

    //uint16_t len;
    while (true) {

        switch(model)
        {
            case send:

            {            
                common::LED_WHITE_Blink(2);
                auto a = _rfid.ReaderCard();
                if (a.empty() == false)
                {
                    _wifi.SendTcp(a);
                    uint16_t RxLen;
                    _gps.Recv(RxLen, 2s);
                    if(RxLen!=0)
                    {
                        _wifi.SendTcp(std::string(_gps.m_recv_buffer.get(),_gps.m_recv_buffer.get()+RxLen));
                    }
                }
            }
            break;
            case recv:

            {
                
            }

            break;
        }


        if(model == send){

        }
        else {
        
        }


    }
}
