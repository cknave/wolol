#include <asm/fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdint.h>
#include "syscalls.h"

#define STDIN		0
#define STDOUT		1
#define STDERR		2

#define HEADER_LEN	6
#define HW_ADDR_LEN	6
#define PACKET_LEN	HEADER_LEN + 16*HW_ADDR_LEN

static void help();
static void init_packet(uint8_t *packet, uint8_t *hw_addr);
static int parse_hw_addr(char *hex_string, uint8_t *hw_addr);
static in_addr_t pack_ip_address(uint8_t octets[4]);


int main(int argc, char **argv) {
	// Parse args
	if(argc != 2) {
		help();
		return 1;
	}
	uint8_t hw_addr[HW_ADDR_LEN];
	int rc = parse_hw_addr(argv[1], hw_addr);
	if(rc < 0) {
		help();
		return 1;
	}
	// TODO: parse broadcast address

	// Create wake on LAN packet for address
	uint8_t packet[PACKET_LEN];
	init_packet(packet, hw_addr);

	// Create a UDP socket for broadcast
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	int broadcast = 1;
	rc = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast,
			sizeof(broadcast));
	if(rc < 0) {
		char msg[] = "Failed to set broadcast socket option\n";
		write(STDERR, msg, sizeof(msg));
		return 1;
	}
        
	// Bind to any address
        struct sockaddr_in src_addr;
	src_addr.sin_family = AF_INET;
	src_addr.sin_addr.s_addr = INADDR_ANY;
	src_addr.sin_port = 0;
	rc = bind(sock, (struct sockaddr *)&src_addr, sizeof(src_addr));
	if(rc < 0) {
		char msg[] = "Failed to bind UDP socket\n";
		write(STDERR, msg, sizeof(msg));
		return 1;
	}

	// Send to broadcast address
	struct sockaddr_in dest_addr;
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_addr.s_addr = pack_ip_address(
			(uint8_t[]){192, 168, 1, 255});
	dest_addr.sin_port = htons(9);
	ssize_t sent = sendto(sock, packet, PACKET_LEN, 0,
			(struct sockaddr *)&dest_addr, sizeof(dest_addr));
	if(sent != PACKET_LEN) {
		char msg[] = "Failed to send packet\n";
		write(STDERR, msg, sizeof(msg));
		return 1;
	}

        return 0;
}


static void help() {
    const char msg[] = "USAGE: wlolol <address>\n";
    write(STDERR, msg, sizeof(msg));
}

static void init_packet(uint8_t *packet, uint8_t *hw_addr) {
    for(int i = 0; i < HEADER_LEN; i++) {
        packet[i] = 0xff;
    }
    for(int i = HEADER_LEN; i < PACKET_LEN; i += HW_ADDR_LEN) {
        packet[i]   = hw_addr[0];
        packet[i+1] = hw_addr[1];
        packet[i+2] = hw_addr[2];
        packet[i+3] = hw_addr[3];
        packet[i+4] = hw_addr[4];
        packet[i+5] = hw_addr[5];
    }
}

// parse a single character ('F' => 15)
// return -1 on bad input
static int parse_hex_char(char c) {
	if(c >= '0' && c <= '9') {
		return c - '0';
	}
	if(c >= 'A' && c <= 'F') {
		return c - 'A' + 10;
	}
	if(c >= 'a' && c <= 'f') {
		return c - 'a' + 10;
	}
	return -1;
}

// parse a 1 or 2 character hex string ("f" => 15, "FF" => 255)
// advance the pointer accordingly
// return -1 on bad input
static int parse_hex_byte(char **sp) {
	char *s = *sp;
	int upper = parse_hex_char(*s++);
	if(upper == -1) {
		return -1;
	}
	int lower = parse_hex_char(*s);
	if(lower == -1) {
		(*sp)++;
		return upper;
	}
	(*sp) += 2;
	return (upper << 4) | lower;
}

// parse a hardware address, setting 6 bytes of hw_addr
// return -1 on bad input
static int parse_hw_addr(char *hex_string, uint8_t *hw_addr) {
	char *ptr = hex_string;
	char separator = -1;
	for(int i = 0; i < HW_ADDR_LEN; i++) {
		// Skip colons except on the first iteration
		if(*ptr == separator) {
			ptr++;
		}
		separator = ':';
		int value = parse_hex_byte(&ptr);  // advances ptr
		if(value == -1) {
			return -1;
		}
		hw_addr[i] = value;
	}
	return 0;
}

static in_addr_t pack_ip_address(uint8_t octets[4]) {
	return (octets[3] << 24) | (octets[2] << 16) | (octets[1] << 8) |
		octets[0];
}

// uint16_t htons(uint16_t port) {
// 	return (port << 8) | (port >> 8);
// }
