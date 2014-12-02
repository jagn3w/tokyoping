#ifndef TOKYONETWORKSERVICE
#define TOKYONETWORKSERVICE

#define DST_PORT 33434 
#include <string.h>
#include "packet.h"

//in_addr_t get_src_ip();

long double get_udp_rtt(char* pack, uint16_t src_port, int timeout_ms);
long double get_icmp_rtt(char* pack, uint16_t identifier, int timeout_ms);

#endif
