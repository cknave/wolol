#pragma once
#include <sys/socket.h>
#include <netinet/in.h>

int open(const char *path, int flags, int mode);
int write(int fd, const void *buf, int count);
int close(int fd);
void exit(int status);
int socket(int domain, int type, int protocol);
int setsockopt(int sockfd, int level, int optname, const void *optval,
		socklen_t optlen);
