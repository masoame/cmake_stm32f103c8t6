
#include "start_main.hpp"
#include "atgm336h.hpp"
#include "common.hpp"
#include "esp8266.hpp"

#include "rc522.hpp"

#include "oled.hpp"

#include "stm32f1xx_hal.h"
#include <cstdint>

constexpr auto RC522_SDA_Pin = GPIO_PIN_8;
constexpr auto RC522_SDA_GPIO_Port = GPIOB_BASE;
constexpr auto RC522_RST_Pin = GPIO_PIN_9;
constexpr auto RC522_RST_GPIO_Port = GPIOB_BASE;

using namespace std::literals;
// void start_main()
// {
//     wifi::esp8266 _wifi(&huart2, "ChinaNet-srMh", "r5phqngt", "192.168.2.52", 8080);
//     gps::atgm336h _gps(&huart1);
//     rfid::rc522 _rfid(&hspi2, reinterpret_cast<GPIO_TypeDef*>(RC522_SDA_GPIO_Port), RC522_SDA_Pin, reinterpret_cast<GPIO_TypeDef*>(RC522_RST_GPIO_Port), RC522_RST_Pin);

//     //_gps.SendToWifi(_wifi);

//     //uint16_t len;
//     while (true) {
//         common::LED_WHITE_Blink(2);
//         auto a = _rfid.ReaderCard();
//         if (a.empty() == false)
//         {
//             char pos = 6;
//             std::string pData;
//             while(true){
//                 char tmp[17];
//                 tmp[16] = '\0';

//                 _rfid.PcdRead(pos, reinterpret_cast<unsigned char*>(tmp)); 
//                 pData += tmp;
//                 if((pData.size() == 0 )|| (pData.size() % 16 != 0)) 
//                     break;
//                 pos+=4;
//             }
//             _wifi.SendTcp(a );
//             _wifi.SendTcp(pData);
//         }
//     }
// }

// void start_main()
// {
//     wifi::esp8266 _wifi(&huart2, "ChinaNet-srMh", "r5phqngt", "192.168.2.52", 8080);
//     while(true)
//     {
//         switch(common::KEY_Scan(0))
//         {
//             case common::Key1_Press:
//                 _wifi.SendTcp("Key1 Press\r\n");
//             break;
//             case common::Key2_Press:
//                 _wifi.SendTcp("Key2 Press\r\n");
//             break;
//             default:
//                 HAL_Delay(100);
//             break;
//         }
//     }
// }
using namespace oled;
char buf[1024];
void start_main(void)
{
	wifi::esp8266 _wifi(&huart2, "ChinaNet-srMh", "r5phqngt", "192.168.2.52", 8080);
    gps::atgm336h _gps(&huart1);
    rfid::rc522 _rfid(&hspi2, reinterpret_cast<GPIO_TypeDef*>(RC522_SDA_GPIO_Port), RC522_SDA_Pin, reinterpret_cast<GPIO_TypeDef*>(RC522_RST_GPIO_Port), RC522_RST_Pin);
	

	
	OLED_Init();
	OLED_ColorTurn(0);
    OLED_DisplayTurn(0);
	while(1)
	{
		auto a = _rfid.ReaderCard();
		if(a.empty() || a == ""){
			HAL_Delay(50);
			continue;
		}
		OLED_ShowString(0,0,(u8*)a.c_str(),12,1);
		OLED_Refresh();
		uint16_t len;
		_gps.Recv(len, 5s);
		_gps.m_recv_buffer[len]=0;
		_wifi.SendTcp(std::string((char*)_gps.m_recv_buffer.get()));
	}
}