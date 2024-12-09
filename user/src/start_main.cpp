#include"start_main.hpp"
#include "callback.hpp"
#include "common.hpp"
#include"esp8266.hpp"
#include "main.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_uart.h"
#include <cstdint>
#include <cstring>



void start_main() {

	//gps::atgm336h _gps(&huart1);
	// serialport::Driver _serialport(&huart1);
	// serialport::Driver _serialport2(&huart2);

	// _serialport.StartForwardSerialPort(_serialport2);
	// _serialport2.StartForwardSerialPort(_serialport);

	wifi::esp8266 _wifi(&huart2,"gt","1658932642","113.219.237.121",21004);
	//gps::atgm336h _gps(&huart1);
	// serialport::Driver _serialport(&huart1);
	// serialport::Driver _serialport2(&huart2);

	// _serialport.StartForwardSerialPort(_serialport2);
	// _serialport2.StartForwardSerialPort(_serialport);

	char data[1024];


	while (true) {
		memset(data, 0, 512);
		HAL_UART_Receive(&huart1, (uint8_t*)data, 512, 1000);

		_wifi.SendTcp(data);
		common::LED1_Blink(2);
	}
	//√∞≈›≈≈–Ú
	
	

}

