#include <stdio.h>

#include <time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>

#include "flow_manager.h"
#include "packet.h"
#include "network_service.h"

#define CLOCKTOMS(x) ((x)/1000)


int num_flows, timeout_ms, gap_ms, num_repeats;
in_addr_t dst_ip;

std::string dst;
bool is_udp;

void run_instrumentation() {
	FILE * pfile;
	flow_manager_t * manager = new flow_manager_t();
	for (int i = 0; i < num_flows; i++) {
		
		char* pack;
		if (is_udp) {
			uint16_t flow = manager->get_new_srcport();
			pack = get_ip_udp_probe(dst_ip, flow);

			std::string name ("UDP Trace of: "+dst+" Timeout: "+std::to_string(timeout_ms)+" Gap: "+std::to_string(gap_ms)+" Repeats: "+std::to_string(num_repeats)+" Srcprt: ");

			pfile = fopen(name.c_str(), "a");

			for (int j = 0; j < num_repeats; j++) {
				clock_t before = clock();
				clock_t rtt = get_rtt(pack, flow, timeout_ms);
				if (rtt != -1) {
					fprintf(pfile,"%d\n", (int) CLOCKTOMS(rtt));
				}

				//To get rid of icmp load-balancing in the network
				while (CLOCKTOMS(clock() - before) < gap_ms);
			}
		} /*else {

			//TODO: Add ICMP instrumenting
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
		printf("\t./tokyoping [-i | -u] -t timeoutms -g gapms -r numrepeats -f numflows -h hostip");
	}
	int i = 1;

	while(i < argc) {
		if(strcmp(argv[i], "-i")) {
			is_udp = false;
		} else if(strcmp(argv[i], "-u")) {
			is_udp = true;
		} else if(strcmp(argv[i], "-t")) {
			i++;
			timeout_ms = atoi(argv[i]);
		} else if(strcmp(argv[i], "-g")) {
			i++;
			gap_ms = atoi(argv[i]);
		} else if(strcmp(argv[i], "-r")) {
			i++;
			num_repeats = atoi(argv[i]);
		} else if(strcmp(argv[i], "-f")) {
			i++;
			num_flows = atoi(argv[i]);
		} else if(strcmp(argv[i], "-h")) {
			i++;
			dst = argv[i];
			dst_ip = inet_addr(argv[i]);
		}
		i++;
	}

	run_instrumentation();
}