/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 ThundeRatz

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <arpa/inet.h>

int main(int argc, char **argv) {
	int udp_socket;
	struct sockaddr_in remote;
	char data[200];
	size_t len;
	socklen_t addr_len = sizeof(struct sockaddr_in);
	uint16_t target_port;
	if (argc < 3) {
		fprintf(stderr, "Uso:\n\t%s remote_ip remote_port\n", argv[0]);
		return -1;
	}

	if (sscanf(argv[2], "%hu", &target_port) != 1) {
		fprintf(stderr, "sscanf falhou\n");
		return -1;
	}

	if ((udp_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		return -1;
	}

	memset((char *) &remote, 0, sizeof(remote));
	remote.sin_family = AF_INET;	//IPv4
	if (!inet_pton(AF_INET, argv[1], &remote.sin_addr.s_addr)) {
		perror("inet_pton");
		return -1;
	}
	remote.sin_port = htons(target_port);

	if (fgets(data, sizeof(data), stdin) == NULL) {
		perror("fgets");
		return -1;
	}
	len = strlen(data);
	if (data[len] == '\n') {
		data[len] = 0;
		len--;
	}
	if (sendto(udp_socket, data, len - 1, 0, (struct sockaddr *) &remote, addr_len) == -1)
		perror("sendto");
	return 0;
}
