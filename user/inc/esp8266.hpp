#pragma once
#include "serialport.hpp"


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

		std::string m_ssid;
		std::string m_password;
		std::string m_ip;
		unsigned short m_port = 0;

	public:
		esp8266(UART_HandleTypeDef* esp8266_huart,const std::string& ssid = {}, const std::string& password = {}, const std::string& ip = {}, unsigned short port = 0);

		bool Reset();
		bool ConnectTcp();

		bool PowerOn();
		bool PowerOff();

		bool WifiIsConnected();
		bool DisconnectWifi();

		void Ping(const std::string& ip);
		void CloseTcp();
		Driver::ResponseFlag SendTcp(const std::string& data);
		bool Filter(uint16_t& len) override;

	private:
		//Driver::ResponseType AddTcpLink(const std::string& ip, unsigned short port);
		//Driver::ResponseType RemoveTcpLink(const std::string& ip, unsigned short port);
		Driver::ResponseFlag ConnectWifi(const std::string& ssid, const std::string& password);
		Driver::ResponseFlag LinkTcp(const std::string& ip, unsigned short port);
	};

}