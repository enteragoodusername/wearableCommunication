#include <RH_RF95.h>
#include <vector>
class AlarmReceiver {
	public: 
		AlarmReceiver(uint8_t pin_cs_, uint8_t pin_rst_, int pin_irq_);
		bool init();
		enum MessageType{
			Heartbeat,
			Alarm,
			Ack,
			Unkown,
			None
		};
        MessageType get_message();
		void reset_sequence();
		bool send_ack(uint8_t seq);
		uint8_t get_last_seq() const;
	private:
		uint8_t pin_cs;
		int pin_rst;
		uint8_t pin_irq;
		RH_RF95 rf95;
		uint8_t last_seq = 0;
		uint8_t last_received_seq = 0;
		bool has_received_first = false;

};

