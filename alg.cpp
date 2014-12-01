#include <stdio.h>

#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <iostream>
#include <chrono>

#include "flow_manager.h"
#include "packet.h"
#include "network_service.h"

#define CLOCKTOMS(x) ((x)/1000)


int num_flows, timeout_ms, gap_ms, num_repeats;
in_addr_t dst_ip;
in_addr_t src_ip;

std::string dst;
bool is_udp;

void run_instrumentation() {
	FILE * pfile;
	flow_manager_t * manager = new flow_manager_t();
	for (int i = 0; i < num_flows; i++) {

		char* pack;
		if (is_udp) {
			uint16_t flow = manager->get_new_srcport();
			pack = get_ip_udp_probe(dst_ip, src_ip, flow);

			std::string name ("UDP Trace of: "+dst+" Timeout: "+std::to_string(timeout_ms)+" Gap: "+std::to_string(gap_ms)+" Repeats: "+std::to_string(num_repeats)+" Srcprt: "+std::to_string(flow));

			pfile = fopen(name.c_str(), "a");

			for (int j = 0; j < num_repeats; j++) {
				std::chrono::milliseconds before = std::chrono::duration_cast<std::chrono::milliseconds >(
    				std::chrono::high_resolution_clock::now().time_since_epoch()
  				);
				long double rtt = get_udp_rtt(pack, flow, timeout_ms);

				printf("rtt: %Lf in ms\n", rtt);
				
				if (rtt != -1) {
					fprintf(pfile,"%Lf\n", rtt);
				}

				//To get rid of icmp load-balancing in the network
				while ((std::chrono::duration_cast<std::chrono::milliseconds >(
    				std::chrono::high_resolution_clock::now().time_since_epoch()
  					).count() - before.count()) < gap_ms);
			}
		} /*else {
			pack = get_icmp_packet(flow);
			pfile = fopen()
		}*/

		

		fflush(pfile);
		fclose(pfile);
	}
}

int main(int argc, char *argv[]) {

	if(argc < 2) {
		printf("usage:\n");
		printf("\t./tokyoping [-i | -u] -t timeoutms -g gapms -r numrepeats -f numflows -h hostip\n");
	}
	int i = 1;

	while(i < argc) {
		if(!strcmp(argv[i], "-i")) {
			is_udp = false;
		} else if(!strcmp(argv[i], "-u")) {
			is_udp = true;
		} else if(!strcmp(argv[i], "-t")) {
			i++;
			timeout_ms = atoi(argv[i]);
		} else if(!strcmp(argv[i], "-g")) {
			i++;
			gap_ms = atoi(argv[i]);
		} else if(!strcmp(argv[i], "-r")) {
			i++;
			num_repeats = atoi(argv[i]);
		} else if(!strcmp(argv[i], "-f")) {
			i++;
			num_flows = atoi(argv[i]);
		} else if(!strcmp(argv[i], "-d")) {
			i++;
			dst = argv[i];
			dst_ip = inet_addr(argv[i]);
		} else if(!strcmp(argv[i], "-h")) {
			i++;
			src_ip = inet_addr(argv[i]);
		}
		i++;
	}

	run_instrumentation();
}
