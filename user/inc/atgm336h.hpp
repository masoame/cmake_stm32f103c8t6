#pragma once
#include<common.hpp>
#include<serialport.hpp>
namespace gps {

	class atgm336h : public ::serialport::Driver 
	{
	public:
		atgm336h(UART_HandleTypeDef* atgm336h_huart);
		~atgm336h();
	private:

	};
}