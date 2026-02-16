#include "alarmServer.hpp"
#include "comm_declarations.hpp"


AlarmServer::AlarmServer(uint8_t pin_cs_, uint8_t pin_rst_, uint8_t pin_irq_ ) : 
	pin_cs(pin_cs_),
	pin_rst(pin_rst_),
	pin_irq(pin_irq_),
	rf95(pin_cs_, pin_irq_)
	{
}
bool AlarmServer::init(){
	SPI.begin();
	Serial.println("Starting LoRa Module");
	  if (!rf95.init()) {
	Serial.println("RH_RF95 init failed. Check wiring/pins/power.");
		return false;
	}

	if (!rf95.setFrequency(RF_FREQ_MHZ)) {
	Serial.println("setFrequency failed (unsupported freq?)");
		return false;
	}



	Serial.println("LoRa ready.");
	Serial.print("Freq MHz: "); Serial.println(RF_FREQ_MHZ);
	return true;
}


