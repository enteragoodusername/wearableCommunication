#include "alarmServer.hpp"
#include "comm_declarations.hpp"
#include "Arduino.h"

static uint8_t sequence_counter = 0;

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
    rf95.setModemConfig(RH_RF95::Bw500Cr45Sf128);
	if (!rf95.setFrequency(RF_FREQ_MHZ)) {
		Serial.println("setFrequency failed (unsupported freq?)");
		return false;
	}



	rf95.setTxPower(TX_POWER_DBM, false);
	Serial.println("LoRa ready.");
	Serial.print("Freq MHz: "); Serial.println(RF_FREQ_MHZ);
	return true;
}

// bool AlarmServer::send_heartbeat(){
// 	Serial.print("Sending Heartbeat... ");
// 	uint8_t heartbeat = 1;
// 	if (rf95.send(&heartbeat, sizeof(heartbeat) && rf95.waitPacketSent())){
// 		Serial.println("Heartbeat Sent");
// 		return true;
// 	}
// 	else{
// 		Serial.println("Heartbeat Failed!");
// 		return false;
// 	}
	
// }
// bool AlarmServer::send_alarm(){
// 	Serial.print("Sending Alarm... ");
// 	uint8_t heartbeat = 2;
// 	if (rf95.send(&heartbeat, sizeof(heartbeat) && rf95.waitPacketSent())){
// 		Serial.println("Alarm Sent");
// 		return true;
// 	}
// 	else{
// 		Serial.println("Alarm Failed!");
// 		return false;
// 	}
// }

bool AlarmServer::send_alarm(){
    uint8_t packet[2];
    packet[0] = 2;
    packet[1] = sequence_counter++;

	Serial.print("ALARM Seq = ");
	Serial.println(packet[1]);

    rf95.send(packet, sizeof(packet));
    rf95.waitPacketSent();

    return true;
}

bool AlarmServer::send_heartbeat(){
    uint8_t packet[2];
    packet[0] = 1;
    packet[1] = sequence_counter++;  

	Serial.print("HEARTBEAT seq=");
    Serial.println(packet[1]);

    rf95.send(packet, sizeof(packet));
    rf95.waitPacketSent();

    return true;
}

bool AlarmServer::check_for_ack(uint8_t &seq_out){
    uint8_t buf[10];
    uint8_t len = sizeof(buf);

    if (!rf95.available()) return false;
	if (rf95.available()){
		Serial.println("ACK check!!");
	}
    rf95.recv(buf, &len);
    if (len < 2) return false;

    if (buf[0] == MSG_ACK){
        seq_out = buf[1];
        return true;
    }

    return false;
}
