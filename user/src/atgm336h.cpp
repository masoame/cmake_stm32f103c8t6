#include"atgm336h.hpp"
#include "serialport.hpp"
#include <optional>

#include<algorithm>

namespace gps {

	atgm336h::atgm336h(UART_HandleTypeDef* atgm336h_huart) : ::serialport::Driver(atgm336h_huart) {

	}

	atgm336h::~atgm336h() {

	}

	// std::optional<std::string> gps::SearchDataLine(const std::string& search_data){

	// 	char* start_ptr = reinterpret_cast<char*>(this->rx_buffer.get());
	// 	char* end_ptr = start_ptr + this->rx_size;
		
	// 	if ((start_ptr = std::search(start_ptr, end_ptr, search_data.cbegin(), search_data.cend())) != end_ptr) {
	// 		end_ptr = std::find(start_ptr, end_ptr, '\n');
	// 		if (start_ptr == end_ptr || *(end_ptr-1) != '\n')return std::nullopt;
	// 		return std::string(reinterpret_cast<char*>(start_ptr), end_ptr - start_ptr);
	// 	}
	// 	return std::nullopt;
	// }


}

