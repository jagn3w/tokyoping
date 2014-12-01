#include <arpa/inet.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <chrono>

#include "network_service.h"

in_addr_t get_src_ip(){

	struct ifaddrs *ifap, *ifa;

	getifaddrs (&ifap);

	struct sockaddr_in *sa;

	//TODO: This is dependent on the network we're on, figure out a smarter way
	std::string prefix("172.");

	for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr->sa_family==AF_INET) {
            sa = (struct sockaddr_in *) ifa->ifa_addr;
            std::string addr (inet_ntoa(sa->sin_addr));
            //printf("found addr %s\n", addr.c_str());
            if (!addr.compare(0, prefix.size(), prefix)) {
            	return sa->sin_addr.s_addr;
            }
        }
    }
    printf("Couldn't find local addr\n");
    return -1;
}


long double get_udp_rtt(char* pack, uint16_t src_port, int timeout_ms) {

	struct iphdr* ip_pack = (struct iphdr*) pack;

	char * buf = (char*) malloc(4096);
	memset(buf, 0, 4096);

	struct sockaddr_in din;
	din.sin_family = AF_INET;
	din.sin_port = DST_PORT;
	din.sin_addr.s_addr = ip_pack->ip_dst;
	unsigned long send_time;
	unsigned long recv_time;

	bool correct_response;

	struct timeval timeout;      
    timeout.tv_sec = timeout_ms / 1000;
    timeout.tv_usec = (timeout_ms % 1000) * 1000;

    int send_sock, recv_sock;

	if( (send_sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0) {
		perror ("The following error occurred");
	}
	if( (recv_sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0) {
		perror ("The following error occurred");
	}



	// TODO: Do we really need to create our own IP headers?
	// we are creating our own IP headers
	int hdrincl=1;
	if (setsockopt(send_sock, IPPROTO_IP, IP_HDRINCL,&hdrincl,sizeof(hdrincl)) < 0) {
    	perror ("The following error occurred");
    	return -1;
	}

	// Set timeout for the receive socket
	if (setsockopt (recv_sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,
                sizeof(timeout)) < 0) {
		perror ("The following error occurred");
		return -1;
	}

	send_time = std::chrono::duration_cast<std::chrono::microseconds >(
    				std::chrono::high_resolution_clock::now().time_since_epoch()
  				).count();
	if (sendto(send_sock, pack, sizeof(iphdr) + sizeof(udphdr), 0, (struct sockaddr*) &din, sizeof(din)) < 0) {
		perror ("The following error occurred");
		return -1;
	}

	do {
		struct sockaddr_in * rcv_addr = (sockaddr_in*) malloc(sizeof(sockaddr_in));
		socklen_t* len = (socklen_t*) malloc(sizeof(socklen_t));

		if(recvfrom(recv_sock, buf, 4096, 0, (struct sockaddr*) rcv_addr, (socklen_t *) len) < 0) {
			perror("Received nothing\n");

			free(len);
			free(rcv_addr);
			free(buf);
			close(send_sock);
			close(recv_sock);
			return -1;
		}

		recv_time = std::chrono::duration_cast<std::chrono::microseconds >(
    				std::chrono::high_resolution_clock::now().time_since_epoch()
  				).count();

		iphdr* hdr = (iphdr*) buf;

		if (hdr->ip_src == din.sin_addr.s_addr) {
			correct_response = true;
		}

		free(len);
		free(rcv_addr);

	} while(!correct_response);

	close(send_sock);
	close(recv_sock);
	free(buf);

	return ((double)(recv_time - send_time))/1000;
}

unsigned long get_icmp_rtt(char* pack) {
	return 0;
}








