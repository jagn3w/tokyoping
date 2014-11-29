#include "packet.h"

//sending to this port will make an ICMP Port Unreachable message

unsigned short ip_chksum(unsigned short *buf, int nwords){
        unsigned long sum;
        for(sum=0; nwords>0; nwords--)
                sum += *buf++;
        sum = (sum >> 16) + (sum &0xffff);
        sum += (sum >> 16);
        return (unsigned short)(~sum);
}

void init_udphdr(struct udphdr* hdr, uint16_t srcprt) {
	hdr->udp_src = htons(srcprt);
	hdr->udp_dst = htons(DST_PORT);
	hdr->udp_len = htons(sizeof(struct udphdr));
	hdr->udp_chk = 0; // can be omitted, and we want to. remember this nonsense? :)
}

void init_iphdr(struct iphdr* hdr, in_addr_t dst_ip) {
	hdr->ip_hdr_len = 5;
	hdr->ip_version = 4;   

	hdr->ip_tos = 16;

	hdr->ip_len = sizeof(struct iphdr) + sizeof(struct udphdr);

	hdr->ip_id = htons(54321);

	hdr->ip_off = 0;

	hdr->ip_ttl = 64;

	hdr->ip_proto = 17; // 17 == UDP

	

	//! IP source address (in network format).
	hdr->ip_src = get_src_ip();
	//! IP destination address (in network format).
	hdr->ip_dst = dst_ip;

	hdr->ip_chk = ip_chksum((unsigned short *)hdr, hdr->ip_len);
}

char* get_ip_udp_probe(in_addr_t dst_ip, uint16_t srcprt) {
	char * buffer = (char*) malloc(sizeof(iphdr)+sizeof(udphdr));

	init_udphdr((struct udphdr*) (buffer + sizeof(iphdr)), srcprt);
	init_iphdr((struct iphdr* ) buffer, dst_ip);

	return buffer;
}