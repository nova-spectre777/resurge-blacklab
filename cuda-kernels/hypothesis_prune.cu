// Experimental CUDA path: parallel elimination of cheap candidate constraints.
// Not built by the default CI until a CUDA runner is explicitly configured.
#include <cuda_runtime.h>
#include <cstdint>

extern "C" __global__ void resurge_prune_u32(
    const std::uint32_t* candidate_crc,
    const std::uint32_t* expected_crc,
    std::uint8_t* survives,
    std::size_t count) {
    const auto i = static_cast<std::size_t>(blockIdx.x) * blockDim.x + threadIdx.x;
    if (i < count) survives[i] = candidate_crc[i] == expected_crc[i] ? 1 : 0;
}
