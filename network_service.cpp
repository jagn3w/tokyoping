#include <arpa/inet.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <unistd.h>
#include <string>

#include "network_service.h"


in_addr_t get_src_ip(){

	struct ifaddrs *ifap, *ifa;

	getifaddrs (&ifap);

	struct sockaddr_in *sa;
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

	struct iphdr* ip_pack = (struct iphdr*) pack;

	struct sockaddr_in din;
	din.sin_family = AF_INET;
	din.sin_port = DST_PORT;
	din.sin_addr.s_addr = ip_pack->ip_dst;
	clock_t send_time;
	clock_t recv_time;

	bool correct_response;

	struct timeval timeout;      
    timeout.tv_sec = timeout_ms / 1000;
    timeout.tv_usec = (timeout_ms % 1000) * 1000;

	int send_sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
	int recv_sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);


	// TODO: Do we really need to create our own IP headers?
	// we are creating our own IP headers
	int hdrincl=1;
	if (setsockopt(send_sock, IPPROTO_IP, IP_HDRINCL,&hdrincl,sizeof(hdrincl)) < 0) {
    	printf("error disabling header\n");
    	return -1;
	}

	// Set timeout for the receive socket
	if (setsockopt (recv_sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,
                sizeof(timeout)) < 0) {
		printf("Couldn't set timeout");
		return -1;
	}

	if (sendto(send_sock, pack, sizeof(iphdr)+sizeof(udphdr), 0, (struct sockaddr*) &din, sizeof(din)) < 0) {
		printf("Could not send packet\n");
		return -1;
	}
	send_time = clock();

	do {
		char buf[100];
		memset(buf, 0, 100);

		struct sockaddr_in rcv_addr;

		int len = sizeof(rcv_addr);
		if(recvfrom(recv_sock, buf, 100, 0, (struct sockaddr*) &recv_sock, (socklen_t *) &len) <= 0) {
			printf("Received nothing\n");
			return -1;
		}
		recv_time = clock();

		if (rcv_addr.sin_addr.s_addr == din.sin_addr.s_addr) {
			correct_response = true;
		}
	} while(!correct_response);

	return recv_time - send_time;
}