#include <RH_RF95.h>
#include <vector>
class AlarmReceiver {
	public: 
		AlarmReceiver(uint8_t pin_cs_, uint8_t pin_rst_, int pin_irq_);
		bool init();
		enum MessageType{
			Heartbeat,
			Alarm,
			Unkown,
			None
		};
        MessageType get_message();
	private:
		uint8_t pin_cs;
		int pin_rst;
		uint8_t pin_irq;
		RH_RF95 rf95;

};

