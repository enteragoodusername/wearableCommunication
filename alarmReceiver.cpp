#include "alarmReceiver.hpp"
#include "comm_declarations.hpp"
#include "Arduino.h"
#include "mac.hpp"


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
    rf95.setModemConfig(LORA_MODEM);
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
    // std::vector<uint8_t> data(100,0);
    // uint8_t len = 100;

	uint8_t data[32] = {0};
	uint8_t len = sizeof(data);

	if (!rf95.available()) return AlarmReceiver::None;
    rf95.recv(data, &len);

	if (len < 2 + MAC_TAG_SIZE) return AlarmReceiver::None;

	if (!verify_mac(data, 2, &data[2])) {
		Serial.println("MAC failed, discarding packet");
		return AlarmReceiver::None;
	}


	uint8_t type = data[0];
	uint8_t seq  = data[1];
	last_received_seq = seq;
	Serial.print("RX packet | type=");
	Serial.print(type);
	Serial.print(" seq=");
	Serial.print(seq);
	Serial.print(" last_seq=");
	Serial.println(last_seq);

	//int message = data.front();
	Serial.println("Recieved Message");

	uint8_t diff = seq - last_seq;

	if (has_received_first) {
		if (diff == 0) {
			return MessageType::None;
		}
		if (type == MSG_ACK) {
			last_received_seq = seq;
			return MessageType::Ack;
		}
		if (diff > 128) {
			return MessageType::None;
		}
	}

	last_seq = seq;
	has_received_first = true;

	Serial.print("ACCEPTED packet | seq=");
	Serial.println(seq);

	if (type == 1) return MessageType::Heartbeat;
	else if (type == 2) return MessageType::Alarm;
	else if (type == MSG_ACK) return MessageType::Ack;
	else return MessageType::Unkown;
}

void AlarmReceiver::reset_sequence() {
    has_received_first = false;
    last_seq = 0;
}

bool AlarmReceiver::send_ack(uint8_t seq){
    uint8_t packet[2 + MAC_TAG_SIZE];

    packet[0] = MSG_ACK;
    packet[1] = seq;

	compute_mac(packet, 2, &packet[2]);

    rf95.send(packet, sizeof(packet));
    rf95.waitPacketSent();

    return true;
}

uint8_t AlarmReceiver::get_last_seq() const {
    return last_received_seq;
}


void AlarmReceiver::sleep(){
    rf95.sleep();
}
