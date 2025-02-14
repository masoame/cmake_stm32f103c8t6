#include "start_main.hpp"
#include "atgm336h.hpp"
#include "common.hpp"
#include "esp8266.hpp"
#include "main.h"
#include "rc522.hpp"

constexpr inline auto RC522_SDA_Pin = GPIO_PIN_8;
constexpr inline auto RC522_SDA_GPIO_Port = GPIOB_BASE;
constexpr inline auto RC522_RST_Pin = GPIO_PIN_9;
constexpr inline auto RC522_RST_GPIO_Port = GPIOB_BASE;

void start_main()
{
    wifi::esp8266 _wifi(&huart2, "sola", "ZZH234900+", "183.7.14.73", 0x0721);
    gps::atgm336h _gps(&huart1);
    rfid::rc522 _rfid(&hspi2, reinterpret_cast<GPIO_TypeDef*>(RC522_SDA_GPIO_Port), RC522_SDA_Pin, reinterpret_cast<GPIO_TypeDef*>(RC522_RST_GPIO_Port), RC522_RST_Pin);

    _gps.SendForWifi(_wifi);

    while (true) {
        common::LED1_Blink(2);
        auto a = _rfid.ReaderCard();
        if (a.empty() == false)
            _wifi.SendTcp(a);
    }
}
