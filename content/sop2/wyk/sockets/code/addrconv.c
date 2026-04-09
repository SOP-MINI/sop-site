#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <IPv4_ADDRESS>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *ipv4_text = argv[1];
    struct in_addr ipv4_binary;
    char ipv4_result[INET_ADDRSTRLEN];

    if (inet_pton(AF_INET, ipv4_text, &ipv4_binary) == 1) {
        printf("[+] PTON parsed: %s\n", ipv4_text);

        unsigned char *bytes = (unsigned char *)&ipv4_binary.s_addr;
        printf("    Raw memory (hex): %02x %02x %02x %02x\n",
               bytes[0], bytes[1], bytes[2], bytes[3]);

        if (inet_ntop(AF_INET, &ipv4_binary, ipv4_result, INET_ADDRSTRLEN) != NULL) {
            printf("[+] NTOP back to string: %s\n", ipv4_result);
        }
    } else {
        fprintf(stderr, "[-] PTON error: Invalid IPv4 address format.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}