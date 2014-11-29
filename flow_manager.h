#ifndef TOKYOFLOWMANAGER
#define TOKYOFLOWMANAGER

#include <stdlib.h>
#include <set>

class flow_manager_t
{
   private:
   	std::set<uint16_t> used_udp_srcprts;

   public:

   	flow_manager_t();
   	uint16_t get_new_srcport();

};

#endif