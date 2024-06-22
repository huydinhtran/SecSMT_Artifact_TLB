#include <getopt.h>
#include <signal.h>
// #include <rte_ethdev.h>
#include <pcap.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <sys/time.h>




static inline uint64_t
rte_rdtsc_main(void)
{
	union {
		uint64_t tsc_64;
		struct {
			uint32_t lo_32;
			uint32_t hi_32;
		};
	} tsc;
	asm volatile("rdtsc" :
		     "=a" (tsc.lo_32),
		     "=d" (tsc.hi_32));
	return tsc.tsc_64;
}

#define unlikely(x)	__builtin_expect(!!(x), 0)
// /** Minimum Cache line size. */

/** Force alignment to cache line. */
#define __rte_cache_aligned_main __rte_aligned_main(64)

// /** Force minimum cache line alignment. */
#define __rte_cache_min_aligned_main __rte_aligned_main(64)

/**
 * Force alignment
 */
#define __rte_aligned_main(a) __attribute__((__aligned__(a)))

//  /* Constants used in link management.
//  */
#define RTE_ETH_LINK_HALF_DUPLEX_MAIN 0 /**< Half-duplex connection (see link_duplex). */
#define ETH_LINK_HALF_DUPLEX_MAIN     RTE_ETH_LINK_HALF_DUPLEX_MAIN
#define RTE_ETH_LINK_FULL_DUPLEX_MAIN 1 /**< Full-duplex connection (see link_duplex). */
#define ETH_LINK_FULL_DUPLEX_MAIN     RTE_ETH_LINK_FULL_DUPLEX_MAIN
#define RTE_ETH_LINK_DOWN_MAIN        0 /**< Link is down (see link_status). */
#define ETH_LINK_DOWN_MAIN            RTE_ETH_LINK_DOWN_MAIN
#define RTE_ETH_LINK_UP_MAIN          1 /**< Link is up (see link_status). */
#define ETH_LINK_UP_MAIN              RTE_ETH_LINK_UP_MAIN
#define RTE_ETH_LINK_FIXED_MAIN       0 /**< No autonegotiation (see link_autoneg). */
#define ETH_LINK_FIXED_MAIN           RTE_ETH_LINK_FIXED_MAIN
#define RTE_ETH_LINK_AUTONEG_MAIN     1 /**< Autonegotiated (see link_autoneg). */
#define ETH_LINK_AUTONEG_MAIN         RTE_ETH_LINK_AUTONEG_MAIN
#define RTE_ETH_LINK_MAX_STR_LEN_MAIN 40 /**< Max length of default link string. */

#define MAX_RX_QUEUE_PER_LCORE 16
#define MAX_TX_QUEUE_PER_PORT 16

#define RTE_MAX_ETHPORTS 32
#define RTE_ETHER_ADDR_LEN  6 /**< Length of Ethernet address. */


#define RTE_LOGTYPE_L2FWD RTE_LOGTYPE_USER1

#define MAX_PKT_BURST 32
#define BURST_TX_DRAIN_US 100 /* TX drain every ~100us */
#define MEMPOOL_CACHE_SIZE 256

/*
 * Configurable number of RX/TX ring descriptors
 */
#define RTE_TEST_RX_DESC_DEFAULT 1024
#define RTE_TEST_TX_DESC_DEFAULT 1024

#define MAX_TIMER_PERIOD 86400 /* 1 day max */

/* mempool defines */
#define RTE_MEMPOOL_CACHE_MAX_SIZE 512

#define MS_PER_S_MAIN 1000
#define US_PER_S_MAIN 1000000
#define NS_PER_S_MAIN 1000000000

#define rte_pktmbuf_mtod_offset_main(m, t, o)	\
	((t)(void *)((char *)(m)->buf_addr + (m)->data_off + (o)))

#define rte_pktmbuf_mtod_main(m, t) rte_pktmbuf_mtod_offset_main(m, t, 0)


struct timeval start, end;
long seconds, microseconds, elapsed_microseconds, elapsed_nanoseconds;

/** Generic marker for any place in a structure. */
__extension__ typedef void    *RTE_MARKER_MAIN[0];
/** Marker for 1B alignment in a structure. */
__extension__ typedef uint8_t  RTE_MARKER8_MAIN[0];
/** Marker for 2B alignment in a structure. */
__extension__ typedef uint16_t RTE_MARKER16_MAIN[0];
/** Marker for 4B alignment in a structure. */
__extension__ typedef uint32_t RTE_MARKER32_MAIN[0];
/** Marker for 8B alignment in a structure. */
__extension__ typedef uint64_t RTE_MARKER64_MAIN[0];

/**
 * The RTE mempool structure.
 */
struct rte_mempool_main {
	char name[64]; /**< Name of mempool. */
	union {
		void *pool_data;         /**< Ring or pool to store objects. */
		uint64_t pool_id;        /**< External mempool identifier. */
	};
	void *pool_config;               /**< optional args for ops alloc. */
	const struct rte_memzone *mz;    /**< Memzone where pool is alloc'd. */
	unsigned int flags;              /**< Flags of the mempool. */
	int socket_id;                   /**< Socket id passed at create. */
	uint32_t size;                   /**< Max size of the mempool. */
	uint32_t cache_size;
	/**< Size of per-lcore default local cache. */

	uint32_t elt_size;               /**< Size of an element. */
	uint32_t header_size;            /**< Size of header (before elt). */
	uint32_t trailer_size;           /**< Size of trailer (after elt). */

	unsigned private_data_size;      /**< Size of private data. */
	/**
	 * Index into rte_mempool_ops_table array of mempool ops
	 * structs, which contain callback function pointers.
	 * We're using an index here rather than pointers to the callbacks
	 * to facilitate any secondary processes that may want to use
	 * this mempool.
	 */
	int32_t ops_index;

	struct rte_mempool_cache_main *local_cache; /**< Per-lcore local cache */

	uint32_t populated_size;         /**< Number of populated objects. */
	// struct rte_mempool_objhdr_list elt_list; /**< List of objects in pool */
	uint32_t nb_mem_chunks;          /**< Number of memory chunks */
	// struct rte_mempool_memhdr_list mem_list; /**< List of memory chunks */

}  __rte_cache_aligned_main;


struct rte_mbuf_sched_main {
	uint32_t queue_id;   /**< Queue ID. */
	uint8_t traffic_class;
	/**< Traffic class ID. Traffic class 0
	 * is the highest priority traffic class.
	 */
	uint8_t color;
	/**< Color. @see enum rte_color.*/
	uint16_t reserved;   /**< Reserved. */
}; /**< Hierarchical scheduler */


struct rte_mbuf_main {
	RTE_MARKER_MAIN cacheline0;

	void *buf_addr;           /**< Virtual address of segment buffer. */
	/**
	 * Physical address of segment buffer.
	 * Force alignment to 8-bytes, so as to ensure we have the exact
	 * same mbuf cacheline0 layout for 32-bit and 64-bit. This makes
	 * working on vector drivers easier.
	 */
	// rte_iova_t buf_iova __rte_aligned(sizeof(rte_iova_t));

	/* next 8 bytes are initialised on RX descriptor rearm */
	RTE_MARKER64_MAIN rearm_data;
	uint16_t data_off;

	/**
	 * Reference counter. Its size should at least equal to the size
	 * of port field (16 bits), to support zero-copy broadcast.
	 * It should only be accessed using the following functions:
	 * rte_mbuf_refcnt_update(), rte_mbuf_refcnt_read(), and
	 * rte_mbuf_refcnt_set(). The functionality of these functions (atomic,
	 * or non-atomic) is controlled by the RTE_MBUF_REFCNT_ATOMIC flag.
	 */
	uint16_t refcnt;

	/**
	 * Number of segments. Only valid for the first segment of an mbuf
	 * chain.
	 */
	uint16_t nb_segs;

	/** Input port (16 bits to support more than 256 virtual ports).
	 * The event eth Tx adapter uses this field to specify the output port.
	 */
	uint16_t port;

	uint64_t ol_flags;        /**< Offload features. */

	/* remaining bytes are set on RX when pulling packet from descriptor */
	RTE_MARKER_MAIN rx_descriptor_fields1;

	/*
	 * The packet type, which is the combination of outer/inner L2, L3, L4
	 * and tunnel types. The packet_type is about data really present in the
	 * mbuf. Example: if vlan stripping is enabled, a received vlan packet
	 * would have RTE_PTYPE_L2_ETHER and not RTE_PTYPE_L2_VLAN because the
	 * vlan is stripped from the data.
	 */
	union {
		uint32_t packet_type; /**< L2/L3/L4 and tunnel information. */
		__extension__
		struct {
			uint8_t l2_type:4;   /**< (Outer) L2 type. */
			uint8_t l3_type:4;   /**< (Outer) L3 type. */
			uint8_t l4_type:4;   /**< (Outer) L4 type. */
			uint8_t tun_type:4;  /**< Tunnel type. */
			union {
				uint8_t inner_esp_next_proto;
				/**< ESP next protocol type, valid if
				 * RTE_PTYPE_TUNNEL_ESP tunnel type is set
				 * on both Tx and Rx.
				 */
				__extension__
				struct {
					uint8_t inner_l2_type:4;
					/**< Inner L2 type. */
					uint8_t inner_l3_type:4;
					/**< Inner L3 type. */
				};
			};
			uint8_t inner_l4_type:4; /**< Inner L4 type. */
		};
	};

	uint32_t pkt_len;         /**< Total pkt len: sum of all segments. */
	uint16_t data_len;        /**< Amount of data in segment buffer. */
	/** VLAN TCI (CPU order), valid if RTE_MBUF_F_RX_VLAN is set. */
	uint16_t vlan_tci;

	
	union {
		union {
			uint32_t rss;     /**< RSS hash result if RSS enabled */
			struct {
				union {
					struct {
						uint16_t hash;
						uint16_t id;
					};
					uint32_t lo;
					/**< Second 4 flexible bytes */
				};
				uint32_t hi;
				/**< First 4 flexible bytes or FD ID, dependent
				 * on RTE_MBUF_F_RX_FDIR_* flag in ol_flags.
				 */
			} fdir;	/**< Filter identifier if FDIR enabled */
			struct rte_mbuf_sched_main sched;
			/**< Hierarchical scheduler : 8 bytes */
			struct {
				uint32_t reserved1;
				uint16_t reserved2;
				uint16_t txq;
				/**< The event eth Tx adapter uses this field
				 * to store Tx queue id.
				 * @see rte_event_eth_tx_adapter_txq_set()
				 */
			} txadapter; /**< Eventdev ethdev Tx adapter */
			/**< User defined tags. See rte_distributor_process() */
			uint32_t usr;
		} hash;                   /**< hash information */
	};

	/** Outer VLAN TCI (CPU order), valid if RTE_MBUF_F_RX_QINQ is set. */
	uint16_t vlan_tci_outer;

	uint16_t buf_len;         /**< Length of segment buffer. */

	struct rte_mempool_main *pool; /**< Pool from which mbuf was allocated. */

	/* second cache line - fields only used in slow path or on TX */
	RTE_MARKER_MAIN cacheline1 __rte_cache_min_aligned_main;

	/**
	 * Next segment of scattered packet. Must be NULL in the last segment or
	 * in case of non-segmented packet.
	 */
	struct rte_mbuf_main *next;

	/* fields to support TX offloads */
	union {
		uint64_t tx_offload;       /**< combined for easy fetch */
		__extension__
		struct {
			uint64_t l2_len:7;
			/**< L2 (MAC) Header Length for non-tunneling pkt.
			 * Outer_L4_len + ... + Inner_L2_len for tunneling pkt.
			 */
			uint64_t l3_len:9;
			/**< L3 (IP) Header Length. */
			uint64_t l4_len:8;
			/**< L4 (TCP/UDP) Header Length. */
			uint64_t tso_segsz:16;
			/**< TCP TSO segment size */

			/*
			 * Fields for Tx offloading of tunnels.
			 * These are undefined for packets which don't request
			 * any tunnel offloads (outer IP or UDP checksum,
			 * tunnel TSO).
			 *
			 * PMDs should not use these fields unconditionally
			 * when calculating offsets.
			 *
			 * Applications are expected to set appropriate tunnel
			 * offload flags when they fill in these fields.
			 */
			uint64_t outer_l3_len:9;
			/**< Outer L3 (IP) Hdr Length. */
			uint64_t outer_l2_len:7;
			/**< Outer L2 (MAC) Hdr Length. */

			/* uint64_t unused:RTE_MBUF_TXOFLD_UNUSED_BITS; */
		};
	};

	/** Shared data for external buffer attached to mbuf. See
	 * rte_pktmbuf_attach_extbuf().
	 */
	// struct rte_mbuf_ext_shared_info *shinfo;

	/** Size of the application private data. In case of an indirect
	 * mbuf, it stores the direct mbuf private data size.
	 */
	uint16_t priv_size;

	/** Timesync flags for use with IEEE1588. */
	uint16_t timesync;

	uint32_t dynfield1[9]; /**< Reserved for dynamic fields. */
} __rte_cache_aligned_main;



/**
 * A structure that stores a per-core object cache.
 */
struct rte_mempool_cache_main {
	uint32_t size;	      /**< Size of the cache */
	uint32_t flushthresh; /**< Threshold before we flush excess elements */
	uint32_t len;	      /**< Current cache count */
	/*
	 * Cache is allocated to this size to allow it to overflow in certain
	 * cases to avoid needless emptying of cache.
	 */
	void *objs[RTE_MEMPOOL_CACHE_MAX_SIZE * 3]; /**< Cache objects */
} __rte_cache_aligned_main;

struct rte_ether_addr_main {
	uint8_t addr_bytes[RTE_ETHER_ADDR_LEN]; /**< Addr bytes in tx order */
} __rte_aligned_main(2);

uint64_t total_packets_dropped=0, total_packets_tx=0, total_packets_rx=0;

static struct rte_ether_addr_main dummy_src_addr = {
    .addr_bytes = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55}
};
static struct rte_ether_addr_main dummy_dst_addr = {
    .addr_bytes = {0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB}
};
// Define dummy source and destination MAC addresses
static const struct rte_ether_addr_main dummy_eth_src_addr = {
    .addr_bytes = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55} // Example dummy source MAC address
};
static const struct rte_ether_addr_main dummy_eth_dst_addr = {
    .addr_bytes = {0x66, 0x77, 0x88, 0x99, 0xAA, 0xBB} // Example dummy destination MAC address
};
// static volatile bool force_quit;
/* MAC updating enabled by default */
static int mac_updating = 1;
/* Ports set in promiscuous mode off by default. */
static int promiscuous_on;

#define RTE_LOGTYPE_L2FWD RTE_LOGTYPE_USER1

#define MAX_PKT_BURST 32
#define BURST_TX_DRAIN_US 100 /* TX drain every ~100us */
#define MEMPOOL_CACHE_SIZE 256

struct rte_ether_hdr_main {
	struct rte_ether_addr_main dst_addr; /**< Destination address. */
	struct rte_ether_addr_main src_addr; /**< Source address. */
} __rte_aligned_main(2);


/*
 * Configurable number of RX/TX ring descriptors
 */
#define RTE_TEST_RX_DESC_DEFAULT 1024
#define RTE_TEST_TX_DESC_DEFAULT 1024
static uint16_t nb_rxd = RTE_TEST_RX_DESC_DEFAULT;
static uint16_t nb_txd = RTE_TEST_TX_DESC_DEFAULT;

/* ethernet addresses of ports */
static struct rte_ether_addr_main l2fwd_ports_eth_addr[RTE_MAX_ETHPORTS];

/* mask of enabled ports */
static uint32_t l2fwd_enabled_port_mask = 0;

/* list of enabled ports */
static uint32_t l2fwd_dst_ports[RTE_MAX_ETHPORTS];

struct port_pair_params {
#define NUM_PORTS   1
    uint16_t port[NUM_PORTS];
} __rte_cache_aligned;

static struct port_pair_params port_pair_params_array[RTE_MAX_ETHPORTS / 2];
static struct port_pair_params *port_pair_params;
static uint16_t nb_port_pair_params;

static unsigned int l2fwd_rx_queue_per_lcore = 1;

#define MAX_RX_QUEUE_PER_LCORE 16
#define MAX_TX_QUEUE_PER_PORT 16
/* List of queues to be polled for a given lcore. 8< */
struct lcore_queue_conf {
    unsigned n_rx_port;
    unsigned rx_port_list[MAX_RX_QUEUE_PER_LCORE];
} __rte_cache_aligned_main;
struct lcore_queue_conf lcore_queue_conf[1];
/* >8 End of list of queues to be polled for a given lcore. */

static struct rte_eth_dev_tx_buffer *tx_buffer[RTE_MAX_ETHPORTS];

struct rte_mempool_main * l2fwd_pktmbuf_pool = NULL;

/* Per-port statistics struct */
struct l2fwd_port_statistics {
    uint64_t tx;
    uint64_t rx;
    uint64_t dropped;
} __rte_cache_aligned_main;
struct l2fwd_port_statistics port_statistics[RTE_MAX_ETHPORTS];

/* A tsc-based timer responsible for triggering statistics printout */
static uint64_t timer_period = 500000000; /* default period is 10 seconds */

/* Print out statistics on packets dropped */
static void
print_stats(void)
{
    
    unsigned portid = 0;
    const char clr[] = { 27, '[', '2', 'J', '\0' };
    const char topLeft[] = { 27, '[', '1', ';', '1', 'H','\0' };

    /* Clear screen and move to top left */
    // printf("%s%s", clr, topLeft);
    for (portid = 0; portid < RTE_MAX_ETHPORTS; portid++) {
        total_packets_dropped += port_statistics[portid].dropped;
        total_packets_tx += port_statistics[portid].tx;
        total_packets_rx += port_statistics[portid].rx;
    }

    printf("\n=============== l2fwd statistics ================\n"
           "\nTotal packets sent: %18"PRIu64
           "\nTotal packets received: %14"PRIu64
           "\nElapsed time: %ld microseconds",
           total_packets_tx,
           total_packets_rx,
           elapsed_microseconds);
    printf("\n====================================================\n");
    fflush(stdout);
}



// Function to update MAC addresses in the Ethernet header
static void
l2fwd_mac_updating(struct rte_mbuf_main *m, unsigned dest_portid)
{
    struct rte_ether_hdr_main *eth;
	
	eth = rte_pktmbuf_mtod_main(m, struct rte_ether_hdr_main *);
	struct rte_ether_addr_main tmp;

    // Copy source MAC address to a temporary variable
    memcpy(&tmp, &dummy_src_addr, sizeof(struct rte_ether_addr_main));
    // Copy destination MAC address to source MAC address field
    memcpy(&dummy_src_addr, &dummy_dst_addr, sizeof(struct rte_ether_addr_main));
    // Copy temporary variable (original source MAC) to destination MAC address field
    memcpy(&dummy_dst_addr, &tmp, sizeof(struct rte_ether_addr_main));
}


/* Simple forward. 8< */
static void
l2fwd_simple_forward(struct rte_mbuf *m, unsigned portid)
{
    unsigned dst_port;
    int sent;
    struct rte_eth_dev_tx_buffer *buffer;

    // dst_port = l2fwd_dst_ports[portid];
    dst_port = 0;


    l2fwd_mac_updating(m, 0);
    
    /* Create a new mbuf with random data */
    // struct rte_mbuf *new_mbuf = rte_pktmbuf_alloc(l2fwd_pktmbuf_pool);
    struct rte_mbuf_main *new_mbuf = malloc(sizeof(struct rte_mbuf_main));
    // rte_pktmbuf_reset(new_mbuf);

    if (new_mbuf == NULL) {
        printf("Failed to allocate mbuf for sending random buffer\n");
        return;
    }

    // /* Fill the buffer with random data */
    uint8_t *data = rte_pktmbuf_mtod_main(new_mbuf, uint8_t *);
    for (unsigned i = 0; i < new_mbuf->buf_len; i++) {
        data[i] = rand() % 256; // Random value between 0 and 255
    }
    
    // buffer = tx_buffer[dst_port];
    memcpy(buffer, data, new_mbuf->buf_len);
    port_statistics[dst_port].tx += 1;

    /* Free the allocated mbuf */
    // rte_pktmbuf_free(new_mbuf);
    free(new_mbuf);
}
/* >8 End of simple forward. */

/* main processing loop */
static void
l2fwd_main_loop(pcap_t *handle)
{
    struct rte_mbuf *pkts_burst[MAX_PKT_BURST];
    struct rte_mbuf *m;
    int sent;
    unsigned lcore_id;
    uint64_t prev_tsc, diff_tsc, cur_tsc, timer_tsc;
    unsigned i, j, portid, nb_rx;
    struct lcore_queue_conf *qconf;
    qconf = NULL;


    /* Assigned a new logical core in the loop above. */
    qconf = &lcore_queue_conf[0];
    qconf->n_rx_port++;

	uint64_t rte_get_tsc_hz(void);

    const uint64_t drain_tsc = (US_PER_S_MAIN - 1) / US_PER_S_MAIN *
            BURST_TX_DRAIN_US;
    struct rte_eth_dev_tx_buffer *buffer;

    prev_tsc = 0;
    timer_tsc = 0;
    // lcore_id = rte_lcore_id();
    lcore_id = 0;


    // pcap_t *handle;
    // char errbuf[PCAP_ERRBUF_SIZE];
    struct pcap_pkthdr header;
    const u_char *packet;
    const u_char *temp_packet;

    while (1) {

        /* Drains TX queue in its main loop. 8< */
        cur_tsc = rte_rdtsc_main();
        /*
         * TX burst queue drain
         */
        diff_tsc = cur_tsc - prev_tsc;
        if (unlikely(diff_tsc > drain_tsc)) {

            // portid = l2fwd_dst_ports[qconf->rx_port_list[i]];
            portid = 0;
            buffer = tx_buffer[portid];
            // sent = rte_eth_tx_buffer_flush(portid, 0, buffer);

            /* if timer is enabled */
            if (timer_period > 0) {
                /* advance the timer */
                timer_tsc += diff_tsc;
                
                /* if timer has reached its timeout */
                if (unlikely(timer_tsc >= timer_period)) {
                    /* do this only on main core */
					gettimeofday(&end, NULL);
					seconds = end.tv_sec - start.tv_sec;
					microseconds = end.tv_usec - start.tv_usec;
					elapsed_microseconds = seconds * 1000000 + microseconds;
					elapsed_nanoseconds = elapsed_microseconds * 1000;
                    print_stats();
					gettimeofday(&start, NULL);
                    /* reset the timer */
                    timer_tsc = 0;
                }
            }
            prev_tsc = cur_tsc;
        }
        

        /* >8 End of draining TX queue. */
        for (i = 0; i < qconf->n_rx_port; i++) {
            portid = qconf->rx_port_list[i];
            m = pcap_next(handle, &header);


            if (m == NULL) {
                /* End of file */
                break;
            }
            else {
                // temp_packet = malloc(header.caplen*2);
                // memcpy(temp_packet, packet, header.caplen);
                port_statistics[portid].rx += 1;

                if (port_statistics[portid].rx % 1000000 == 0) {
                    // print_stats(); 
                }
                l2fwd_simple_forward(m, portid);
            }
        }   
    }
}


int
main(int argc, char **argv)
{
    pcap_t *handle;
    char errbuf[PCAP_ERRBUF_SIZE];
    handle = pcap_open_offline("/home/h255t794/testpmd-self/request-dpdk-100k.pcap", errbuf);
    if (handle == NULL) {
        fprintf(stderr, "Couldn't open pcap file %s: %s\n", "test.pcap", errbuf);
        return;
    }
    
	gettimeofday(&start, NULL);
    /////////////////
    for(int i = 0; i < 1000000000000000; i++) {

        l2fwd_main_loop(handle);
    }
    // pcap_close(handle);
    ////////////////

    printf("Bye...\n");

    return 0;
}