// Silence compiler warnings

// Save diagnostic state
#pragma GCC diagnostic push 

// Turn off the specific warning(s)
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wswitch-default"
#pragma GCC diagnostic ignored "-Wcast-qual"
#pragma GCC diagnostic ignored "-Wshadow"

#include "crow_all.h"

// Turn the warning(s) back on
#pragma GCC diagnostic pop

