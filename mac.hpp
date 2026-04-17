#pragma once
#include <stdint.h>
#include <stddef.h>

constexpr size_t MAC_TAG_SIZE = 8;

void compute_mac(const uint8_t* message, size_t msg_len, uint8_t* tag_out);
bool verify_mac(const uint8_t* message, size_t msg_len, const uint8_t* received_tag);