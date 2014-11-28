
#include "flow_manager.h"

uint16_t get_new_srcport() {
	uint16_t candidate;
	do{
		candidate = (rand() % 55535) + 10000;
	}while(used_udp_srcprts.find(candidate) != used_udp_srcprts.end());

	used_udp_srcprts.insert(candidate);
	return candidate;
}
