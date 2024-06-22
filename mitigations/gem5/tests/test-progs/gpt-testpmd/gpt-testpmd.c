#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <time.h>
#include <unistd.h>

#define MAC_ADDR_LEN 6
#define PACKET_SIZE 1500  // Typical MTU size
#define PACKET_RATE 833333  // Packets per second for 100Gbps
#define PACKET_INTERVAL_NS (1000000000 / PACKET_RATE)  // Interval in nanoseconds
#define DROP_PROBABILITY 0.01  // 1% packet drop

typedef struct {
    uint8_t src_mac[MAC_ADDR_LEN];
    uint8_t dst_mac[MAC_ADDR_LEN];
    uint8_t payload[PACKET_SIZE - 2 * MAC_ADDR_LEN];
} Packet;

void generate_random_mac(uint8_t *mac) {
    for (int i = 0; i < MAC_ADDR_LEN; i++) {
        mac[i] = rand() % 256;
    }
}

void simulate_packet_arrival(Packet *pkt) {
    generate_random_mac(pkt->src_mac);
    generate_random_mac(pkt->dst_mac);
    memset(pkt->payload, 0, sizeof(pkt->payload));
}

void swap_mac_addresses(Packet *pkt) {
    uint8_t temp[MAC_ADDR_LEN];
    memcpy(temp, pkt->src_mac, MAC_ADDR_LEN);
    memcpy(pkt->src_mac, pkt->dst_mac, MAC_ADDR_LEN);
    memcpy(pkt->dst_mac, temp, MAC_ADDR_LEN);
}

void process_packet(Packet *pkt) {
    swap_mac_addresses(pkt);
    // printf("Processed packet with new src MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
    //        pkt->src_mac[0], pkt->src_mac[1], pkt->src_mac[2],
    //        pkt->src_mac[3], pkt->src_mac[4], pkt->src_mac[5]);
}

void send_packet(Packet *pkt) {
    // In a real application, this function would send the packet over the network.
    // Here, we just print a message.
    // printf("Sent packet with src MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
    //        pkt->src_mac[0], pkt->src_mac[1], pkt->src_mac[2],
    //        pkt->src_mac[3], pkt->src_mac[4], pkt->src_mac[5]);
}

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    srand(time(NULL));

    printf("Starting packet processing simulation...\n");

    int num_packets = 100000;  // Number of packets to simulate
    int total_packets_sent = 0;
    int total_packets_processed = 0;
    int total_packets_dropped = 0;

    Packet pkt;
    struct timespec interval = {0, PACKET_INTERVAL_NS};
    struct timespec rem;
    struct timespec start_time, end_time;

    clock_gettime(CLOCK_MONOTONIC, &start_time);

    while (total_packets_sent < num_packets) {
        simulate_packet_arrival(&pkt);
        total_packets_sent++;
        
        if ((rand() / (double)RAND_MAX) < DROP_PROBABILITY) {
            // printf("Packet dropped\n");
            total_packets_dropped++;
            continue;
        }

        process_packet(&pkt);
        total_packets_processed++;
        send_packet(&pkt);
        nanosleep(&interval, &rem);  // Simulate packet arrival interval
    }

    clock_gettime(CLOCK_MONOTONIC, &end_time);

    double total_time_ns = (end_time.tv_sec - start_time.tv_sec) * 1e9 + (end_time.tv_nsec - start_time.tv_nsec);

    printf("Packet processing simulation completed.\n");
    printf("Total packets sent: %d\n", total_packets_sent);
    printf("Total packets processed: %d\n", total_packets_processed);
    printf("Total packets dropped: %d\n", total_packets_dropped);
    printf("Total execution time: %.0f ns\n", total_time_ns);
    printf("Packet size: %d bytes\n", PACKET_SIZE);
    printf("Packet rate: %d packets/second\n", PACKET_RATE);

    return 0;
}
