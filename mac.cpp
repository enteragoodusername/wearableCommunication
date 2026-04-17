#include "mac.hpp"
#include "secret.hpp"
#include <Crypto.h>
#include <SHA256.h>
#include <string.h>

void compute_mac(const uint8_t* message, size_t msg_len, uint8_t* tag_out) {
    SHA256 hash;

    hash.reset();
    hash.update(SHARED_SECRET, sizeof(SHARED_SECRET));
    hash.update(message, msg_len);
    hash.update(SHARED_SECRET, sizeof(SHARED_SECRET));

    uint8_t full_mac[32];
    hash.finalize(full_mac, sizeof(full_mac));

    memcpy(tag_out, full_mac, MAC_TAG_SIZE);
}

bool verify_mac(const uint8_t* message, size_t msg_len, const uint8_t* received_tag) {
    uint8_t computed[MAC_TAG_SIZE];
    compute_mac(message, msg_len, computed);
    return memcmp(computed, received_tag, MAC_TAG_SIZE) == 0;
}