/*
 * Autogenerated by the Meson build system.
 * Do not edit, your changes will be lost.
 */

#pragma once

#if defined(__x86_64__)
/*#define RTE_ARCH x86_64*/
#define RTE_ARCH_64
#define RTE_ARCH_X86 1
#define RTE_ARCH_X86_64 1
#define RTE_CACHE_LINE_SIZE 64
#endif //if defined (__x86_64__)


//#define RTE_COMPILE_TIME_CPUFLAGS RTE_CPUFLAG_SSE,RTE_CPUFLAG_SSE2,RTE_CPUFLAG_SSE3,RTE_CPUFLAG_SSSE3,RTE_CPUFLAG_SSE4_1,RTE_CPUFLAG_SSE4_2,RTE_CPUFLAG_AES,RTE_CPUFLAG_PCLMULQDQ,RTE_CPUFLAG_AVX,RTE_CPUFLAG_AVX2,RTE_CPUFLAG_RDRAND,RTE_CPUFLAG_RDSEED

//#define RTE_DRIVER_MEMPOOL_BUCKET_SIZE_KB 64

//#undef RTE_EAL_ALLOW_INV_SOCKET_ID

//#define RTE_EAL_PMD_PATH "/usr/local/lib/x86_64-linux-gnu/dpdk/pmds-19.08"

//#define RTE_EAL_VFIO

#define RTE_EXEC_ENV_LINUX 1

//#define RTE_IXGBE_INC_VECTOR 1

//#undef RTE_LIBEAL_USE_HPET

//#define RTE_LIBRTE_ACL 1

//#define RTE_LIBRTE_AF_PACKET_PMD 1

//#define RTE_LIBRTE_ARK_PMD 1

//#define RTE_LIBRTE_ATLANTIC_PMD 1

//#define RTE_LIBRTE_AVP_PMD 1

//#define RTE_LIBRTE_AXGBE_PMD 1

//#define RTE_LIBRTE_BBDEV 1

//#define RTE_LIBRTE_BBDEV_FPGA_LTE_FEC_PMD 1

//#define RTE_LIBRTE_BBDEV_NULL_PMD 1

//#define RTE_LIBRTE_BBDEV_TURBO_SW_PMD 1

//#define RTE_LIBRTE_BITRATESTATS 1

//#define RTE_LIBRTE_BNXT_PMD 1

//#define RTE_LIBRTE_BOND_PMD 1

//#define RTE_LIBRTE_BPF 1

//#define RTE_LIBRTE_BUCKET_MEMPOOL 1

//#define RTE_LIBRTE_CAAM_JR_PMD 1

//#define RTE_LIBRTE_CFGFILE 1

//#define RTE_LIBRTE_CMDLINE 1

//#define RTE_LIBRTE_COMPRESSDEV 1

//#define RTE_LIBRTE_CPT_COMMON 1

//#define RTE_LIBRTE_CRYPTODEV 1

//#define RTE_LIBRTE_CRYPTO_SCHEDULER_PMD 1

//#define RTE_LIBRTE_CXGBE_PMD 1

//#define RTE_LIBRTE_DISTRIBUTOR 1

//#define RTE_LIBRTE_DPAA2_EVENTDEV_PMD 1

//#define RTE_LIBRTE_DPAA2_MEMPOOL 1

//#define RTE_LIBRTE_DPAA2_PMD 1

//#define RTE_LIBRTE_DPAA2_SEC_PMD 1

//#define RTE_LIBRTE_DPAA2_USE_PHYS_IOVA

//#define RTE_LIBRTE_DPAAX_COMMON 1

//#define RTE_LIBRTE_DPAA_BUS 1

//#define RTE_LIBRTE_DPAA_EVENTDEV_PMD 1

//#define RTE_LIBRTE_DPAA_MEMPOOL 1

//#define RTE_LIBRTE_DPAA_PMD 1

//#define RTE_LIBRTE_DPAA_SEC_PMD 1

//#define RTE_LIBRTE_DSW_EVENTDEV_PMD 1

//#define RTE_LIBRTE_E1000_PMD 1

//#define RTE_LIBRTE_EAL 1

//#define RTE_LIBRTE_EFD 1

//#define RTE_LIBRTE_ENA_PMD 1

//#define RTE_LIBRTE_ENETC_PMD 1

//#define RTE_LIBRTE_ENIC_PMD 1

//#define RTE_LIBRTE_ETHDEV 1

//#define RTE_LIBRTE_EVENTDEV 1

//#define RTE_LIBRTE_FAILSAFE_PMD 1

//#define RTE_LIBRTE_FLOW_CLASSIFY 1

//#define RTE_LIBRTE_FM10K_INC_VECTOR 1

//#define RTE_LIBRTE_FM10K_PMD 1

//#define RTE_LIBRTE_FSLMC_BUS 1

//#define RTE_LIBRTE_GRO 1

//#define RTE_LIBRTE_GSO 1

//#define RTE_LIBRTE_HASH 1

//#define RTE_LIBRTE_HINIC_PMD 1

//#define RTE_LIBRTE_I40E_INC_VECTOR 1

//#define RTE_LIBRTE_I40E_PMD 1

//#define RTE_LIBRTE_IAVF_INC_VECTOR 1

//#define RTE_LIBRTE_IAVF_PMD 1

//#define RTE_LIBRTE_ICE_PMD 1

//#define RTE_LIBRTE_IFC_PMD 1

//#define RTE_LIBRTE_IFPGA_BUS 1

//#define RTE_LIBRTE_IPN3KE_PMD 1

//#define RTE_LIBRTE_IPSEC 1

//#define RTE_LIBRTE_IP_FRAG 1

//#define RTE_LIBRTE_IXGBE_PMD 1

//#define RTE_LIBRTE_JOBSTATS 1

//#define RTE_LIBRTE_KNI 1

//#define RTE_LIBRTE_KNI_PMD 1

//#define RTE_LIBRTE_KVARGS 1

//#define RTE_LIBRTE_LATENCYSTATS 1

//#define RTE_LIBRTE_LIQUIDIO_PMD 1

//#define RTE_LIBRTE_LPM 1

//#define RTE_LIBRTE_MBUF 1

//#define RTE_LIBRTE_MEMBER 1

//#define RTE_LIBRTE_MEMIF_PMD 1

//#define RTE_LIBRTE_MEMPOOL 1

//#define RTE_LIBRTE_METER 1

//#define RTE_LIBRTE_METRICS 1

//#define RTE_LIBRTE_NET 1

//#define RTE_LIBRTE_NETVSC_PMD 1

//#define RTE_LIBRTE_NFP_PMD 1

//#define RTE_LIBRTE_NULL_CRYPTO_PMD 1

//#define RTE_LIBRTE_NULL_PMD 1

//#define RTE_LIBRTE_OCTEONTX2_COMMON 1

//#define RTE_LIBRTE_OCTEONTX2_EVENTDEV_PMD 1

//#define RTE_LIBRTE_OCTEONTX2_MEMPOOL 1

//#define RTE_LIBRTE_OCTEONTX2_PMD 1

//#define RTE_LIBRTE_OCTEONTX_COMMON 1

//#define RTE_LIBRTE_OCTEONTX_COMPRESS_PMD 1

//#define RTE_LIBRTE_OCTEONTX_CRYPTO_PMD 1

//#define RTE_LIBRTE_OCTEONTX_EVENTDEV_PMD 1

//#define RTE_LIBRTE_OCTEONTX_MEMPOOL 1

//#define RTE_LIBRTE_OCTEONTX_PMD 1

//#define RTE_LIBRTE_OPDL_EVENTDEV_PMD 1

//#define RTE_LIBRTE_PCI 1

//#define RTE_LIBRTE_PCI_BUS 1

//#define RTE_LIBRTE_PDUMP 1

//#define RTE_LIBRTE_PIPELINE 1

//#define RTE_LIBRTE_PMD_DPAA2_CMDIF_RAWDEV 1

//#define RTE_LIBRTE_PMD_DPAA2_QDMA_RAWDEV 1

//#define RTE_LIBRTE_PMD_IOAT_RAWDEV 1

//#define RTE_LIBRTE_PMD_NTB_RAWDEV 1

//#define RTE_LIBRTE_PMD_OCTEONTX2_DMA_RAWDEV 1

//#define RTE_LIBRTE_PMD_SKELETON_RAWDEV 1

//#define RTE_LIBRTE_PORT 1

//#define RTE_LIBRTE_POWER 1

//#define RTE_LIBRTE_QAT_PMD 1

//#define RTE_LIBRTE_RAWDEV 1

//#define RTE_LIBRTE_RCU 1

//#define RTE_LIBRTE_REORDER 1

//#define RTE_LIBRTE_RING 1

//#define RTE_LIBRTE_RING_MEMPOOL 1

//#define RTE_LIBRTE_RING_PMD 1

//#define RTE_LIBRTE_SCHED 1

//#define RTE_LIBRTE_SECURITY 1

//#define RTE_LIBRTE_SFC_PMD 1

//#define RTE_LIBRTE_SKELETON_EVENTDEV_PMD 1

//#define RTE_LIBRTE_SOFTNIC_PMD 1

//#define RTE_LIBRTE_STACK 1

//#define RTE_LIBRTE_STACK_MEMPOOL 1

//#define RTE_LIBRTE_SW_EVENTDEV_PMD 1

//#define RTE_LIBRTE_TABLE 1

//#define RTE_LIBRTE_TAP_PMD 1

//#define RTE_LIBRTE_THUNDERX_PMD 1

//#define RTE_LIBRTE_TIMER 1

//#define RTE_LIBRTE_VDEV_BUS 1

//#define RTE_LIBRTE_VDEV_NETVSC_PMD 1

//#define RTE_LIBRTE_VHOST 1

//#define RTE_LIBRTE_VHOST_PMD 1

//#define RTE_LIBRTE_VHOST_POSTCOPY

//#define RTE_LIBRTE_VIRTIO_CRYPTO_PMD 1

//#define RTE_LIBRTE_VIRTIO_PMD 1

//#define RTE_LIBRTE_VMBUS_BUS 1

//#define RTE_LIBRTE_VMXNET3_PMD 1

//#define RTE_MACHINE native

//#define RTE_MACHINE_CPUFLAG_AES 1

//#define RTE_MACHINE_CPUFLAG_AVX 1

//#define RTE_MACHINE_CPUFLAG_AVX2 1

//#define RTE_MACHINE_CPUFLAG_PCLMULQDQ 1

//#define RTE_MACHINE_CPUFLAG_RDRAND 1

//#define RTE_MACHINE_CPUFLAG_RDSEED 1

//#define RTE_MACHINE_CPUFLAG_SSE 1

//#define RTE_MACHINE_CPUFLAG_SSE2 1

//#define RTE_MACHINE_CPUFLAG_SSE3 1

//#define RTE_MACHINE_CPUFLAG_SSE4_1 1

//#define RTE_MACHINE_CPUFLAG_SSE4_2 1

//#define RTE_MACHINE_CPUFLAG_SSSE3 1

//#define RTE_MAX_ETHPORTS 32

//#define RTE_MAX_LCORE 128

//#define RTE_MAX_NUMA_NODES 4

//#define RTE_MAX_VFIO_GROUPS 64

//#define RTE_TOOLCHAIN "gcc"

//#define RTE_TOOLCHAIN_GCC 1

//#define RTE_VER_MINOR 0

//#define RTE_VER_MONTH 8

//#define RTE_VER_RELEASE 99

//#define RTE_VER_SUFFIX ""

//#define RTE_VER_YEAR 19

//#define RTE_VIRTIO_USER 1