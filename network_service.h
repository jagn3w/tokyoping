#ifndef TOKYONETWORKSERVICE
#define TOKYONETWORKSERVICE

#define DST_PORT 33434 

in_addr_t get_src_ip();

clock_t get_rtt(char* pack, uint16_t src_port, int timeout_ms);

#endif