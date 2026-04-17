#pragma once
#include <stdint.h>


// Command for generating secret:
// python3 -c "import os; print(', '.join(hex(x) for x in os.urandom(32)))"
// after generating, replace the key in secret.hpp with the key it generates,
// then upload code to receivers and sender

static const uint8_t SHARED_SECRET[32] = {
    // fill with key
};