#pragma once
#include<serialport.hpp>
namespace gps {

	class atgm336h : public ::serialport::Driver 
	{
	public:
		atgm336h(UART_HandleTypeDef* atgm336h_huart);
		~atgm336h();

		void SendToWifi(serialport::Driver& wifi);
		std::string WaitGetData();
		
		inline bool Filter([[maybe_unused]]uint16_t& len) override { return true; };
	};
}