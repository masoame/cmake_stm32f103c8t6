#include"start_main.hpp"
#include"common.hpp"
#include "main.h"
#include "rc522.hpp"
#include "esp8266.hpp"
#include "atgm336h.hpp"
#include "stm32f1xx_hal.h"

void start_main() 
{
	// rfid::rc522 _rfid;
	// _rfid.run();

	wifi::esp8266 _wifi(&huart2,"gt","1658932642","113.219.237.121",21004);
	gps::atgm336h _gps(&huart1);
	rfid::rc522 _rfid;
	_wifi.SendTcp("hello world\r\n");

	// serialport::Driver _serialport(&huart1);
	// serialport::Driver _serialport2(&huart2);

	// _serialport.StartForwardSerialPort(_serialport2);
	// _serialport2.StartForwardSerialPort(_serialport);

	//_gps.SendForWifi(_wifi);


	while(true)
	{
		common::LED1_Blink(2);
		auto  a = _rfid.ReaderCard();
		if(a.empty()==false) _wifi.SendTcp(a);

	}

}

