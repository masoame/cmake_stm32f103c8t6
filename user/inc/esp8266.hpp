#pragma once
#include<common.hpp>
#include "callback.hpp"
#include "serialport.hpp"

#include <initializer_list>
#include<chrono>




namespace wifi {

	class esp8266 : public ::serialport::Driver{

		enum ResponseType {
			UNKONW,

			OK,
			ERROR,

			WIFI_DISCONNECT,
			WIFI_CONNECTED,

			TCP_SEND_SUCCESS,
			TCP_SEND_FAILED,

			TCP_CLOSE,
			PING
		};



		UART_HandleTypeDef* m_huart = nullptr;
		std::string m_ip;
		unsigned short m_port = 0;
	public:
		esp8266(UART_HandleTypeDef* esp8266_huart);




		bool PowerOn();
		bool PowerOff();

		bool WifiIsConnected();

		bool ConnectWifi(const std::string& ssid, const std::string& password);
		bool DisconnectWifi();

		void Ping(const std::string& ip);
		bool LinkTcp(const std::string& ip, unsigned short port);
		bool SendTcp(const std::string& data);
		void CloseTcp();
	private:


	};

}