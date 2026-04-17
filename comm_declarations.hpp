constexpr float RF_FREQ_MHZ = 915;
static constexpr uint8_t TX_POWER_DBM = 23;
constexpr uint8_t MSG_HEARTBEAT = 1;
constexpr uint8_t MSG_ALARM = 2;
constexpr uint8_t MSG_ACK = 3;
static const RH_RF95::ModemConfigChoice LORA_MODEM = RH_RF95::Bw125Cr45Sf128;