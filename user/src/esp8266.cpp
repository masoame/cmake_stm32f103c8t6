#include"esp8266.hpp"
#include "common.hpp"
#include "serialport.hpp"
#include <cstdio>



namespace wifi{
	using namespace std::chrono_literals;
	esp8266::esp8266(UART_HandleTypeDef* esp8266_huart): ::serialport::Driver(esp8266_huart) {
		while(this->WifiIsConnected()==false){
			if(this->ConnectWifi("gt", "1658932642")==true) break;
			common::LED1_Blink(4);
		}
		while(this->LinkTcp("113.219.237.121", 21004)==false){
			common::LED1_Blink(4);
		}
	}

	bool esp8266::PowerOn(){
		return this->GetResponse({}, 1min, {"WIFI CONNECTED"}) == serialport::Driver::RESPONSE_TYPE_OK;
	}
	bool esp8266::PowerOff(){

	}

	bool esp8266::WifiIsConnected(){
		return this->GetResponse("AT+CWJAP_CUR?\r\n", 3s, {"No AP"}) == serialport::Driver::RESPONSE_TYPE_ERROR;
	}




	
	bool esp8266::ConnectWifi(const std::string& ssid, const std::string& password)
	{
		std::string _cmd;
		int sz = std::snprintf(nullptr, 0, "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid.c_str(),password.c_str());
		_cmd.resize(sz + 1); 
		std::sprintf(_cmd.data(), "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid.c_str(),password.c_str());

		return this->GetResponse(_cmd,5s, {"WIFI CONNECTED"}) == serialport::Driver::RESPONSE_TYPE_OK;
	}
	bool esp8266::DisconnectWifi()
	{
		std::string _cmd = "AT+CWQAP\r\n";
		return this->GetResponse(_cmd,2s, {"OK","busy"}) == serialport::Driver::RESPONSE_TYPE_OK;
	}
	
	void esp8266::Ping([[maybe_unused]]const std::string& ip)
	{
		//std::string _cmd = std::vformat("AT+PING={}\r\n", std::make_format_args(ip));
	}


	bool esp8266::LinkTcp(const std::string& ip, unsigned short port)
	{
		std::string _cmd;
		int sz = std::snprintf(nullptr, 0, "AT+CIPSTART=\"TCP\",\"%s\",%d\r\n", ip.c_str(), port);
		_cmd.resize( sz + 1);
		std::sprintf(_cmd.data(), "AT+CIPSTART=\"TCP\",\"%s\",%d\r\n", ip.c_str(), port);

		return this->GetResponse(_cmd, 5s, {"OK","ALREADY CONNECTED"}) == serialport::Driver::RESPONSE_TYPE_OK;
	}
	bool esp8266::SendTcp(const std::string& data)
	{
		std::string _cmd;
		int sz = std::snprintf(nullptr, 0, "AT+CIPSENDEX=%d\r\n",data.size());
		_cmd.resize( sz + 1);
		std::sprintf(_cmd.data(), "AT+CIPSENDEX=%d\r\n",data.size());

		while(this->GetResponse(_cmd, 1s, {"\r\n>"}) != serialport::Driver::RESPONSE_TYPE_OK){}

		return this->GetResponse(data, 4s, {"Recv","SEND OK","busy s..."}) != serialport::Driver::RESPONSE_TYPE_OK;
	}
	void esp8266::CloseTcp()
	{
	}


}