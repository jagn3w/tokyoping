#include "packet.h"

//sending to this port will make an ICMP Port Unreachable message

unsigned short chksum(unsigned short *buf, int nwords){
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

void init_icmphdr(struct icmphdr* hdr, uint16_t identifier) {
	hdr->type = 8;
	hdr->code = 0;
	hdr->id = identifier;
	hdr->sequence = 0;
	hdr->checksum = chksum((unsigned short*) hdr, sizeof(icmphdr));
}

void init_iphdr(struct iphdr* hdr, in_addr_t dst_ip, in_addr_t src_ip, int protocol, int payload_len) {
	hdr->ip_hdr_len = 5;
	hdr->ip_version = 4;   

	hdr->ip_tos = 16;

	hdr->ip_len = sizeof(struct iphdr) + payload_len;

	hdr->ip_id = htons(54321);

	hdr->ip_off = 0;

	hdr->ip_ttl = 64;

	hdr->ip_proto = protocol; // 17 == UDP

	//! IP source address (in network format).
	hdr->ip_src = src_ip;
	//! IP destination address (in network format).
	hdr->ip_dst = dst_ip;

	hdr->ip_chk = chksum((unsigned short *)hdr, hdr->ip_len);
}

char* get_ip_udp_probe(in_addr_t dst_ip, in_addr_t src_ip, uint16_t srcprt) {
	char * buffer = (char*) malloc(sizeof(iphdr)+sizeof(udphdr));

	init_udphdr((struct udphdr*) (buffer + sizeof(iphdr)), srcprt);
	init_iphdr((struct iphdr* ) buffer, dst_ip, src_ip, IPPROTO_UDP, sizeof(udphdr));

	return buffer;
}

char* get_icmp_echo_probe(in_addr_t dst_ip, in_addr_t src_ip, uint16_t identifier){
	char * buffer = (char*) malloc(sizeof(iphdr)+sizeof(udphdr));

	init_icmphdr((struct icmphdr *) (buffer + sizeof(iphdr)), identifier);
	init_iphdr((struct iphdr*) buffer, dst_ip, src_ip, IPPROTO_ICMP, sizeof(icmphdr));

	return buffer;
}