#include "start_main.hpp"
#include "atgm336h.hpp"
#include "common.hpp"
#include "esp8266.hpp"
#include "rc522.hpp"
#include "oled.hpp"
#include "stm32f1xx_hal.h"
#include <cstdint>
#include <string_view>

constexpr auto RC522_SDA_Pin = GPIO_PIN_8;
constexpr auto RC522_SDA_GPIO_Port = GPIOB_BASE;
constexpr auto RC522_RST_Pin = GPIO_PIN_9;
constexpr auto RC522_RST_GPIO_Port = GPIOB_BASE;

enum Request : std::uint8_t {
    login,
    update
};
typedef struct ProtocolHeader
{
    constexpr static const auto c_magic_number = 0xfeedbeef;
    std::uint32_t magic_number = c_magic_number;
    Request request;
    std::uint16_t packet_size;
	char packet[1];

}*LPProtocolHeader;

using namespace std::literals;
using namespace oled;

char gps_buff[256];
const LPProtocolHeader protocolPtr = reinterpret_cast<const LPProtocolHeader>(gps_buff);


void start_main(void)
{
	wifi::esp8266 _wifi(&huart2, "ChinaNet-srMh", "r5phqngt", "192.168.2.54", 8080);
    rfid::rc522 _rfid(&hspi2, reinterpret_cast<GPIO_TypeDef*>(RC522_SDA_GPIO_Port), RC522_SDA_Pin, reinterpret_cast<GPIO_TypeDef*>(RC522_RST_GPIO_Port), RC522_RST_Pin);
	gps::atgm336h _gps(&huart1);

	OLED_Init();
	OLED_ColorTurn(1);
    OLED_DisplayTurn(0);

	static char warehouse_number[] = ": 0";
	static char shelf_number[] = ": 0";

	protocolPtr->magic_number = ProtocolHeader::c_magic_number;


	OLED_ShowChinese(0, 0, 0, 16, 0);
	OLED_ShowChinese(16, 0, 1, 16, 0);
	OLED_ShowChinese(32, 0, 2, 16, 0);
	OLED_ShowString(48, 0, reinterpret_cast<unsigned char*>(warehouse_number), 16, 0);

	OLED_ShowChinese(0, 16, 3, 16, 0);
	OLED_ShowChinese(16, 16, 4, 16, 0);
	OLED_ShowChinese(32, 16, 2, 16, 0);
	OLED_ShowString(48, 16, reinterpret_cast<unsigned char*>(shelf_number), 16, 0);

	OLED_ShowChinese(0, 48, 5, 16, 0);
	OLED_ShowChinese(16, 48, 6, 16, 0);
	OLED_ShowChinese(32, 48, 7, 16, 0);
	OLED_ShowChinese(48, 48, 8, 16, 0);
	
	OLED_Refresh();

	//std::string gps_data = _gps.WaitGetData();

	while(true)
	{
		auto a = _rfid.ReaderCard();
		if(a.empty() || a == ""){

			static bool wait_release = false;
			if(KEY1()==true){
				wait_release = true;
				HAL_Delay(400);
				while(KEY1()==true){
					wait_release = false;
					warehouse_number[2] = (warehouse_number[2] - '0' + 1)%7 + '0';
					OLED_ShowString(48, 0, reinterpret_cast<unsigned char*>(warehouse_number), 16, 0);
					OLED_Refresh();
					HAL_Delay(200);
				}

			}else if(wait_release == true){
				wait_release = false;
				warehouse_number[2] = (warehouse_number[2] - '0' + 1)%7 + '0';
				OLED_ShowString(48, 0, reinterpret_cast<unsigned char*>(warehouse_number), 16, 0);
				OLED_Refresh();
			}
			continue;
		}
		unsigned char pos = 6;
		std::string pData;
		while(true){
			char tmp[17];
			tmp[16] = '\0';

			_rfid.PcdRead(pos, reinterpret_cast<unsigned char*>(tmp)); 
			pData += tmp;
			if((pData.size() == 0 )|| (pData.size() % 16 != 0)) 
				break;
			pos+=4;
		}
		protocolPtr->packet_size = pData.size();
		std::copy(pData.begin(),pData.end(),protocolPtr->packet);
		_wifi.SendTcp({gps_buff,sizeof(ProtocolHeader) + protocolPtr->packet_size});
		uint16_t len;
		_wifi.Recv(len, 2s);

	}
}