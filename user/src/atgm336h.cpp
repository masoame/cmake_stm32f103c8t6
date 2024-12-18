#include"atgm336h.hpp"
#include "serialport.hpp"
#include <cstdint>
#include <optional>
#include"esp8266.hpp"

namespace gps {

	atgm336h::atgm336h(UART_HandleTypeDef* atgm336h_huart) : ::serialport::Driver(atgm336h_huart,::serialport::Driver::DMA) {

	}

	atgm336h::~atgm336h() {

	}
	void atgm336h::SendForWifi(serialport::Driver& wifi){
		auto& _wifi = reinterpret_cast<wifi::esp8266&>(wifi);

		this->OpenAsyncRecv([this,&_wifi](char* data, size_t len) 
		{
			_wifi.SendTcp(std::string(data, data + len));
			this->ContinueAsyncRecv();
		});
	}

}

