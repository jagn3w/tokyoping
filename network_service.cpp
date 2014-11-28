#include <arpa/inet.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <stdio.h>

#include "network_service.h"

in_addr_t get_src_ip(){

	struct ifaddrs *ifap, *ifa;

	getifaddrs (&ifap);

	std::string prefix("192");

	for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr->sa_family==AF_INET) {
            sa = (struct sockaddr_in *) ifa->ifa_addr;
            std::string addr (inet_ntoa(sa->sin_addr));
            if (!addr.compare(0, prefix.size(), prefix)) {
            	return sa->sin_addr.s_addr;
            }
        }
    }
    printf("Couldn't find local addr\n");
    return -1;
}


clock_t get_rtt(char* pack, uint16_t src_port, int timeout_ms) {

	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = src_port;
	sin.sin_addr.s_addr = get_src_ip();

	iphdr* tosend = (iphdr*) pack;

	clock_t send_time;
	clock_t recv_time;

	bool correct_response;

	send_sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
	recv_sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);


	sendto(send_sock, pack, tosend->ip_len, 0, )

	clock_t send_time = clock();

	do{
		recv

		//TODO: check we got the right icmp response
		clock_t recv_time = clock();
	}while(!correct_response);


	return recv_time - send_time;
}