#include <fcntl.h>
#include <stdint.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define HEADER_LEN 6
#define HW_ADDR_LEN 6
#define PACKET_LEN HEADER_LEN + 16*HW_ADDR_LEN

void help();
void init_packet(uint8_t *packet, uint8_t *hw_addr);
int write(int fd, const void *buf, int count);
int close(int fd);
void exit(int status);


int main(int argc, char **argv) {
	// if(argc != 2) {
	// 	help();
	// 	return 1;
	// }

	uint8_t hw_addr[HW_ADDR_LEN];
	// int rc = sscanf(argv[1], "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
	// 		&hw_addr[0], &hw_addr[1], &hw_addr[2], &hw_addr[3],
	// 		&hw_addr[4], &hw_addr[5]);
        // if(rc != HW_ADDR_LEN) {
        //     help();
        //     return 1;
        // }

	uint8_t packet[PACKET_LEN];
	init_packet(packet, hw_addr);

        // int fd = open("packet.bin", O_CREAT | O_WRONLY, 0644);
        int fd = open("packet.bin", O_CREAT | O_WRONLY, 0644);
        if(fd < 0) {
            char msg[] = "Failed to open packet.bin\n";
            write(STDERR, msg, sizeof(msg));
            return 1;
        }
        // write(fd, packet, PACKET_LEN);
        write(fd, packet, PACKET_LEN);
        // close(fd);
        close(fd);

        return 0;
}


void help() {
    // const char msg[] = "USAGE: wlolol <address>\n";
    // write(STDOUT_FILENO, msg, sizeof(msg));
}

void init_packet(uint8_t *packet, uint8_t *hw_addr) {
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
