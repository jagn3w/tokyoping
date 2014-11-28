#ifndef TOKYOFLOWMANAGER
#define TOKYOFLOWMANAGER




class flow_manager_t
{
   private:
   	private static std::set<uint32_t> used_udp_srcprts;

   public:
   	flow_manager_t(){

   	}
   	uint32_t get_new_srcport();

};

#endif