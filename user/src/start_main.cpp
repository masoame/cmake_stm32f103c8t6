#include"start_main.hpp"
#include "common.hpp"
#include"esp8266.hpp"
#include "main.h"



void start_main() {

	//gps::atgm336h _gps(&huart1);
	// serialport::Driver _serialport(&huart1);
	// serialport::Driver _serialport2(&huart2);

	// _serialport.StartForwardSerialPort(_serialport2);
	// _serialport2.StartForwardSerialPort(_serialport);

	wifi::esp8266 _wifi(&huart1);
	//gps::atgm336h _gps(&huart1);
	// serialport::Driver _serialport(&huart1);
	// serialport::Driver _serialport2(&huart2);

	// _serialport.StartForwardSerialPort(_serialport2);
	// _serialport2.StartForwardSerialPort(_serialport);

	int i=0;
	static const std::string str = "hello world: ";


	while (true) {
		i++;
		_wifi.SendTcp(str + std::to_string(i) + "\n");
		common::LED1_Blink(1);
	}

}

