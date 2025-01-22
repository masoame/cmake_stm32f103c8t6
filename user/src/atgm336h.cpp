#include"atgm336h.hpp"
#include "serialport.hpp"
#include <algorithm>
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
			constexpr char search_data[] = "$GNGGA";
			char* start_ptr = data;
			char* end_ptr = start_ptr + len;

			if ((start_ptr = std::search(start_ptr, end_ptr, search_data, search_data+sizeof(search_data)-1)) != end_ptr) {
				end_ptr = std::find(start_ptr, end_ptr, '\n');
				if (start_ptr == end_ptr || *end_ptr != '\n') return;
				 _wifi.SendTcp(std::string(start_ptr, end_ptr+1));
			}
			this->ContinueAsyncRecv();
		});
	}

}

