#include <RH_RF95.h>

class AlarmServer {
	public: 
		AlarmServer(uint8_t pin_cs_, uint8_t pin_rst_, uint8_t pin_irq_);
		bool init();
	private:
		RH_RF95 rf95;
		uint8_t pin_cs;
		uint8_t pin_rst;
		uint8_t pin_irq;

};
