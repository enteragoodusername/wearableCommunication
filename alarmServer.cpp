#include "alarmServer.hpp"
#include "comm_declarations.hpp"
#include "Arduino.h"
#include "mac.hpp"

static uint8_t sequence_counter = 0;
volatile bool waiting_for_ack = false;
volatile uint32_t ack_deadline = 0;

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

bool AlarmServer::send_alarm(){
    uint8_t packet[2 + MAC_TAG_SIZE];
    packet[0] = 2;
    packet[1] = sequence_counter++;

	compute_mac(packet, 2, &packet[2]);
	//packet[2] ^= 0xFF; // test for inserting corrupted packet

	Serial.print("ALARM Seq = ");
	Serial.println(packet[1]);
	Serial.print("MAC: ");
	for (int i = 0; i < MAC_TAG_SIZE; i++) {
		Serial.print(packet[2+i], HEX);
		Serial.print(" ");
	}
	Serial.println();

	// waiting_for_ack = true;

    // rf95.send(packet, sizeof(packet));
    // rf95.waitPacketSent();

	// uint32_t start = millis();
	// uint8_t seq;

	// while (millis() - start < 200) {
	// 	if (check_for_ack(seq)) {
			
	// 		Serial.print("ACK received for seq=");
	// 		Serial.println(seq);
	// 		waiting_for_ack = false;
	// 		return true;
	// 	}
	// }

	// waiting_for_ack = false;

	// Serial.println("ACK timeout");
	

	waiting_for_ack = true;
	ack_deadline = millis() + 300;

	rf95.send(packet, sizeof(packet));
    rf95.waitPacketSent();

    return true;
}

bool AlarmServer::send_heartbeat(){
    uint8_t packet[2 + MAC_TAG_SIZE];
    packet[0] = 1;
    packet[1] = sequence_counter++;  

	compute_mac(packet, 2, &packet[2]);

	Serial.print("HEARTBEAT seq=");
    Serial.println(packet[1]);

    rf95.send(packet, sizeof(packet));
    rf95.waitPacketSent();

    return true;
}

bool AlarmServer::check_for_ack(uint8_t &seq_out){
    uint8_t buf[16];
    uint8_t len = sizeof(buf);

    if (!rf95.available()) return false;
	if (rf95.available()){
		Serial.println("ACK check!!");
	}
    rf95.recv(buf, &len);
    if (len < 2 + MAC_TAG_SIZE) return false;

	if (!verify_mac(buf, 2, &buf[2])) {
		Serial.println("BAD ACK MAC");
		return false;
	}

    if (buf[0] == MSG_ACK){
        seq_out = buf[1];
        return true;
    }

    return false;
}
