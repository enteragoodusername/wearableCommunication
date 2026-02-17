#include <RH_RF95.h>

class AlarmServer {
	public: 
		AlarmServer(uint8_t pin_cs_, uint8_t pin_rst_, int pin_irq_);
		bool init();
	private:
		uint8_t pin_cs;
		int pin_rst;
		uint8_t pin_irq;
		RH_RF95 rf95;

};
