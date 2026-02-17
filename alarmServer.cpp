#include "alarmServer.hpp"
#include "comm_declarations.hpp"
#include "Arduino.h"

AlarmServer::AlarmServer(uint8_t pin_cs_, uint8_t pin_rst_, int pin_irq_ = -1) : 
	pin_cs(pin_cs_),
	pin_rst(pin_rst_),
	pin_irq(pin_irq_),
	rf95(pin_cs_, pin_irq)
	{
}
bool AlarmServer::init(){
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

	bool AlarmServer::send_heartbeat(){
		Serial.print("Sending Heartbeat... ");
		uint8_t heartbeat = 1;
		if (rf95.send(&heartbeat, sizeof(heartbeat) && rf95.waitPacketSent())){
			Serial.println("Heartbeat Sent");
		}
		else Serial.println("Heartbeat Failed!");
	}