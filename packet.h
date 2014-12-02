#ifndef TOKYOPACKET
#define TOKYOPACKET

#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include "network_service.h"

char* get_ip_udp_probe(in_addr_t dst_ip, in_addr_t src_ip, uint16_t srcprt);

char* get_icmp_echo_probe(in_addr_t dst_ip, in_addr_t src_ip, uint16_t identifier);


typedef struct iphdr {

	uint8_t ip_hdr_len:4;   //!< The header length.
	uint8_t ip_version:4;   //!< The IP version.

	//! Type of Service. This is unimportant for us, yeah?
	uint8_t ip_tos;
	//! IP packet length (both data and header).
	uint16_t ip_len;
	//! Identification.
	uint16_t ip_id;
	//! Fragment offset.
	uint16_t ip_off;
	//! Time To Live.
	uint8_t ip_ttl;
	//! The type of the upper-level protocol.
	uint8_t ip_proto;
	//! IP header checksum.
	uint16_t ip_chk;

	//! IP source address (in network format).
	uint32_t ip_src;
	//! IP destination address (in network format).
	uint32_t ip_dst;
} __attribute__ ((packed)) ip_t;

typedef struct udphdr {
	uint16_t udp_src;
	uint16_t udp_dst;
	uint16_t udp_len;
	uint16_t udp_chk;
} __attribute__ ((packed)) udphdr;

typedef struct icmphdr {
	uint8_t type;		/* message type */
	uint8_t code;		/* type sub-code */
	u_int16_t checksum;
	u_int16_t	id;
	u_int16_t	sequence;
} __attribute__((packed)) icmphdr;

#endif