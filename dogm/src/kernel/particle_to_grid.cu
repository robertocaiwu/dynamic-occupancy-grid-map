// Copyright (c) 2020 Michael Koesel and respective contributors
// SPDX-License-Identifier: MIT
// See accompanying LICENSE file for detailed information

#include "dogm/cuda_utils.h"
#include "dogm/dogm_types.h"
#include "dogm/kernel/particle_to_grid.h"

#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <thrust/device_ptr.h>
#include <thrust/sort.h>

namespace dogm
{

__device__ bool is_first_particle(ParticlesSoA& particle_array, int i)
{
    return i == 0 || particle_array[i].grid_cell_idx != particle_array[i - 1].grid_cell_idx;
}

__device__ bool is_last_particle(ParticlesSoA& particle_array, int particle_count, int i)
{
    return i == particle_count - 1 || particle_array[i].grid_cell_idx != particle_array[i + 1].grid_cell_idx;
}

__global__ void particleToGridKernel(ParticlesSoA particle_array, GridCell* __restrict__ grid_cell_array,
                                     float* __restrict__ weight_array, int particle_count)
{
    for (int i = blockIdx.x * blockDim.x + threadIdx.x; i < particle_count; i += blockDim.x * gridDim.x)
    {
        int j = particle_array[i].grid_cell_idx;

        if (is_first_particle(particle_array, i))
        {
            grid_cell_array[j].start_idx = i;
        }
        if (is_last_particle(particle_array, particle_count, i))
        {
            grid_cell_array[j].end_idx = i;
        }

        // printf("Cell: %d, Start idx: %d, End idx: %d\n", j, grid_cell_array[j].start_idx,
        // grid_cell_array[j].end_idx);
        weight_array[i] = particle_array[i].weight;
    }
}

} /* namespace dogm */
