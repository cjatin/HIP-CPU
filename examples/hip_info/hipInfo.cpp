/* -----------------------------------------------------------------------------
 * Copyright (c) 2020 Advanced Micro Devices, Inc. All Rights Reserved.
 * See 'LICENSE' in the project root for license information.
 * -------------------------------------------------------------------------- */
#include <iostream>
#include <iomanip>
#include "hip/hip_runtime.h"

#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"

#define failed(...)                                                                                \
    printf("%serror: ", KRED);                                                                     \
    printf(__VA_ARGS__);                                                                           \
    printf("\n");                                                                                  \
    printf("error: TEST FAILED\n%s", KNRM);                                                        \
    exit(EXIT_FAILURE);

#define HIPCHECK(error)                                                                            \
    if (error != hipSuccess) {                                                                     \
        printf("%serror: '%s'(%d) at %s:%d%s\n", KRED, hipGetErrorString(error), error, __FILE__,  \
               __LINE__, KNRM);                                                                    \
        failed("API returned error code.");                                                        \
    }

void printCompilerInfo() {
#ifdef __HCC__
    printf("compiler: hcc version=%s, workweek (YYWWD) = %u\n", __hcc_version__, __hcc_workweek__);
#endif
#ifdef __NVCC__
    printf("compiler: nvcc\n");
#endif
}

double bytesToKB(size_t s) { return (double)s / (1024.0); }
double bytesToGB(size_t s) { return (double)s / (1024.0 * 1024.0 * 1024.0); }

#define printLimit(w1, limit, units)                                                               \
    {                                                                                              \
        size_t val;                                                                                \
        cudaDeviceGetLimit(&val, limit);                                                           \
        std::cout << setw(w1) << #limit ": " << val << " " << units << std::endl;                  \
    }


void printDeviceProp(int deviceId) {
    using namespace std;
    const int w1 = 34;

    cout << left;

    cout << setw(w1)
         << "--------------------------------------------------------------------------------"
         << endl;
    cout << setw(w1) << "device#" << deviceId << endl;

    hipDeviceProp_t props;
    HIPCHECK(hipGetDeviceProperties(&props, deviceId));

    cout << setw(w1) << "Name: " << props.name << endl;
    cout << setw(w1) << "pciBusID: " << props.pciBusID << endl;
    cout << setw(w1) << "pciDeviceID: " << props.pciDeviceID << endl;
    cout << setw(w1) << "pciDomainID: " << props.pciDomainID << endl;
    cout << setw(w1) << "multiProcessorCount: " << props.multiProcessorCount << endl;
    cout << setw(w1) << "maxThreadsPerMultiProcessor: " << props.maxThreadsPerMultiProcessor
         << endl;
    cout << setw(w1) << "isMultiGpuBoard: " << props.isMultiGpuBoard << endl;
    cout << setw(w1) << "clockRate: " << (float)props.clockRate / 1000.0 << " Mhz" << endl;
    cout << setw(w1) << "memoryClockRate: " << (float)props.memoryClockRate / 1000.0 << " Mhz"
         << endl;
    cout << setw(w1) << "memoryBusWidth: " << props.memoryBusWidth << endl;
    cout << setw(w1) << "clockInstructionRate: " << (float)props.clockInstructionRate / 1000.0
         << " Mhz" << endl;
    cout << setw(w1) << "totalGlobalMem: " << fixed << setprecision(2)
         << bytesToGB(props.totalGlobalMem) << " GB" << endl;
    cout << setw(w1) << "maxSharedMemoryPerMultiProcessor: " << fixed << setprecision(2)
         << bytesToKB(props.maxSharedMemoryPerMultiProcessor) << " KB" << endl;
    cout << setw(w1) << "totalConstMem: " << props.totalConstMem << endl;
    cout << setw(w1) << "sharedMemPerBlock: " << (float)props.sharedMemPerBlock / 1024.0 << " KB"
         << endl;
    cout << setw(w1) << "canMapHostMemory: " << props.canMapHostMemory << endl;
    cout << setw(w1) << "regsPerBlock: " << props.regsPerBlock << endl;
    cout << setw(w1) << "warpSize: " << props.warpSize << endl;
    cout << setw(w1) << "l2CacheSize: " << props.l2CacheSize << endl;
    cout << setw(w1) << "computeMode: " << props.computeMode << endl;
    cout << setw(w1) << "maxThreadsPerBlock: " << props.maxThreadsPerBlock << endl;
    cout << setw(w1) << "maxThreadsDim.x: " << props.maxThreadsDim[0] << endl;
    cout << setw(w1) << "maxThreadsDim.y: " << props.maxThreadsDim[1] << endl;
    cout << setw(w1) << "maxThreadsDim.z: " << props.maxThreadsDim[2] << endl;
    cout << setw(w1) << "maxGridSize.x: " << props.maxGridSize[0] << endl;
    cout << setw(w1) << "maxGridSize.y: " << props.maxGridSize[1] << endl;
    cout << setw(w1) << "maxGridSize.z: " << props.maxGridSize[2] << endl;
    cout << setw(w1) << "major: " << props.major << endl;
    cout << setw(w1) << "minor: " << props.minor << endl;
    cout << setw(w1) << "concurrentKernels: " << props.concurrentKernels << endl;
    cout << setw(w1) << "cooperativeLaunch: " << props.cooperativeLaunch << endl;
    cout << setw(w1) << "cooperativeMultiDeviceLaunch: " << props.cooperativeMultiDeviceLaunch << endl;
    cout << setw(w1) << "arch.hasGlobalInt32Atomics: " << props.arch.hasGlobalInt32Atomics << endl;
    cout << setw(w1) << "arch.hasGlobalFloatAtomicExch: " << props.arch.hasGlobalFloatAtomicExch
         << endl;
    cout << setw(w1) << "arch.hasSharedInt32Atomics: " << props.arch.hasSharedInt32Atomics << endl;
    cout << setw(w1) << "arch.hasSharedFloatAtomicExch: " << props.arch.hasSharedFloatAtomicExch
         << endl;
    cout << setw(w1) << "arch.hasFloatAtomicAdd: " << props.arch.hasFloatAtomicAdd << endl;
    cout << setw(w1) << "arch.hasGlobalInt64Atomics: " << props.arch.hasGlobalInt64Atomics << endl;
    cout << setw(w1) << "arch.hasSharedInt64Atomics: " << props.arch.hasSharedInt64Atomics << endl;
    cout << setw(w1) << "arch.hasDoubles: " << props.arch.hasDoubles << endl;
    cout << setw(w1) << "arch.hasWarpVote: " << props.arch.hasWarpVote << endl;
    cout << setw(w1) << "arch.hasWarpBallot: " << props.arch.hasWarpBallot << endl;
    cout << setw(w1) << "arch.hasWarpShuffle: " << props.arch.hasWarpShuffle << endl;
    cout << setw(w1) << "arch.hasFunnelShift: " << props.arch.hasFunnelShift << endl;
    cout << setw(w1) << "arch.hasThreadFenceSystem: " << props.arch.hasThreadFenceSystem << endl;
    cout << setw(w1) << "arch.hasSyncThreadsExt: " << props.arch.hasSyncThreadsExt << endl;
    cout << setw(w1) << "arch.hasSurfaceFuncs: " << props.arch.hasSurfaceFuncs << endl;
    cout << setw(w1) << "arch.has3dGrid: " << props.arch.has3dGrid << endl;
    cout << setw(w1) << "arch.hasDynamicParallelism: " << props.arch.hasDynamicParallelism << endl;
    cout << setw(w1) << "gcnArch: " << props.gcnArch << endl;
    cout << setw(w1) << "isIntegrated: " << props.integrated << endl;
    cout << setw(w1) << "maxTexture1D: " << props.maxTexture1D << endl;
    cout << setw(w1) << "maxTexture2D.width: " << props.maxTexture2D[0] << endl;
    cout << setw(w1) << "maxTexture2D.height: " << props.maxTexture2D[1] << endl;
    cout << setw(w1) << "maxTexture3D.width: " << props.maxTexture3D[0] << endl;
    cout << setw(w1) << "maxTexture3D.height: " << props.maxTexture3D[1] << endl;
    cout << setw(w1) << "maxTexture3D.depth: " << props.maxTexture3D[2] << endl;

    int deviceCnt;
    hipGetDeviceCount(&deviceCnt);
    cout << setw(w1) << "peers: ";
    for (int i = 0; i < deviceCnt; i++) {
        int isPeer;
        hipDeviceCanAccessPeer(&isPeer, i, deviceId);
        if (isPeer) {
            cout << "device#" << i << " ";
        }
    }
    cout << endl;
    cout << setw(w1) << "non-peers: ";
    for (int i = 0; i < deviceCnt; i++) {
        int isPeer;
        hipDeviceCanAccessPeer(&isPeer, i, deviceId);
        if (!isPeer) {
            cout << "device#" << i << " ";
        }
    }
    cout << endl;


#ifdef __HIP_PLATFORM_NVCC__
    // Limits:
    cout << endl;
    printLimit(w1, cudaLimitStackSize, "bytes/thread");
    printLimit(w1, cudaLimitPrintfFifoSize, "bytes/device");
    printLimit(w1, cudaLimitMallocHeapSize, "bytes/device");
    printLimit(w1, cudaLimitDevRuntimeSyncDepth, "grids");
    printLimit(w1, cudaLimitDevRuntimePendingLaunchCount, "launches");
#endif


    cout << endl;


    size_t free, total;
    hipMemGetInfo(&free, &total);

    cout << fixed << setprecision(2);
    cout << setw(w1) << "memInfo.total: " << bytesToGB(total) << " GB" << endl;
    cout << setw(w1) << "memInfo.free:  " << bytesToGB(free) << " GB (" << setprecision(0)
         << (float)free / total * 100.0 << "%)" << endl;
}

int main(int argc, char* argv[]) {
    using namespace std;

    cout << endl;

    printCompilerInfo();

    int deviceCnt;

    HIPCHECK(hipGetDeviceCount(&deviceCnt));

    for (int i = 0; i < deviceCnt; i++) {
        hipSetDevice(i);
        printDeviceProp(INT_MAX);
    }

    std::cout << std::endl;
}