#include <stdio.h>
#include <time.h>



int num_flows, timeout_ms, gap_ms, num_repeats, num_flows, host_ip;
std::string host;
bool is_udp;



void run_instrumentation() {
	FILE * pfile;
	flow_manager_t * manager = new flow_manager_t();
	for (int i = 0; i < num_flows; i++) {
		
		char* pack;
		if (is_udp) {
			uint32_t flow = manager->get_new_srcport();
			pack = get_udp_packet(flow);
			pfile = fopen("UDP Trace of: "+host+" Timeout: "+itoa(timeout_ms)+" Gap: "+itoa(gap_ms)+" Repeats: "+itoa(repeats)+" Srcprt: ", 'a')
		} /*else {

			pack = get_icmp_packet(flow);
			pfile = fopen()
		}*/

		for (int j = 0; j < num_repeats; j++) {
			clock_t t = clock();
			clock_t rtt = get_rtt(pack);
			fprintf(pfile,"%d\n",rtt);
		}

		fflush(pfile);
		fclose(pfile);
	}
}

int main(int argc, char *argv[]) {

	if(argc < 2) {
		printf("usage:\n");
		printf("\t./tokyoping [-i | -u] -t timeoutms -g gapms -r numprepeats -f numflows -h hostip");
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
			i++
			num_repeats = atoi(argv[i]);
		} else if(strcmp(argv[i], "-f")) {
			i++;
			num_flows = atoi(argv[i]);
		} else if(strcmp(argv[i], "-h")) {
			i++;
			host (argv[i]);
			host_ip = atoi(argv[i]);
		}
		i++;
	}

	run_instrumentation();
}

