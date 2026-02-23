#include "alarmReceiver.hpp"
#include "comm_declarations.hpp"
#include "Arduino.h"

AlarmReceiver::AlarmReceiver(uint8_t pin_cs_, uint8_t pin_rst_, int pin_irq_) : 
	pin_cs(pin_cs_),
	pin_rst(pin_rst_),
	pin_irq(pin_irq_),
	rf95(pin_cs_, pin_irq)
	{
}
bool AlarmReceiver::init(){
	SPI.begin();
	if (pin_rst != -1){
		pinMode(pin_rst, OUTPUT);
		digitalWrite(pin_rst, HIGH);
		delay(10);
		digitalWrite(pin_rst, LOW);
		delay(10);
		digitalWrite(pin_rst, HIGH);
		delay(10);
	}
	Serial.println("Starting LoRa Module");
	  if (!rf95.init()) {
		Serial.println("RH_RF95 init failed. Check wiring/pins/power.");
		return false;
	}

	if (!rf95.setFrequency(RF_FREQ_MHZ)) {
		Serial.println("setFrequency failed (unsupported freq?)");
		return false;
	}



	rf95.setTxPower(TX_POWER_DBM, false);
	Serial.println("LoRa ready.");
	Serial.print("Freq MHz: "); Serial.println(RF_FREQ_MHZ);
	return true;
}

AlarmReceiver::MessageType AlarmReceiver::get_message(){
    std::vector<uint8_t> data(100,0);
    uint8_t len = 100;
	if (!rf95.available()) return AlarmReceiver::None;
    rf95.recv(data.data(), &len);
	int message = data.front();
	Serial.println("Recieved Message");
	if (message == 1) return MessageType::Heartbeat;
	else if (message == 2) return MessageType::Alarm;
	else return MessageType::Unkown;
    
}