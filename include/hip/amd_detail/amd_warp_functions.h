/*
Copyright (c) 2022 Advanced Micro Devices, Inc. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef HIP_INCLUDE_HIP_AMD_DETAIL_WARP_FUNCTIONS_H
#define HIP_INCLUDE_HIP_AMD_DETAIL_WARP_FUNCTIONS_H

static constexpr int warpSize = __AMDGCN_WAVEFRONT_SIZE;

__device__
inline
int __shfl(int var, int src_lane, int width = warpSize) {
    int self = __lane_id();
    int index = src_lane + (self & ~(width-1));
#if __has_builtin(__builtin_amdgcn_ds_bpermute)
    return __builtin_amdgcn_ds_bpermute(index<<2, var);
#else
    return amdgcn_ds_bpermute(index<<2, var);
#endif
}
__device__
inline
unsigned int __shfl(unsigned int var, int src_lane, int width = warpSize) {
     union { int i; unsigned u; float f; } tmp; tmp.u = var;
    tmp.i = __shfl(tmp.i, src_lane, width);
    return tmp.u;
}
__device__
inline
float __shfl(float var, int src_lane, int width = warpSize) {
    union { int i; unsigned u; float f; } tmp; tmp.f = var;
    tmp.i = __shfl(tmp.i, src_lane, width);
    return tmp.f;
}
__device__
inline
double __shfl(double var, int src_lane, int width = warpSize) {
    static_assert(sizeof(double) == 2 * sizeof(int), "");
    static_assert(sizeof(double) == sizeof(uint64_t), "");

    int tmp[2]; __builtin_memcpy(tmp, &var, sizeof(tmp));
    tmp[0] = __shfl(tmp[0], src_lane, width);
    tmp[1] = __shfl(tmp[1], src_lane, width);

    uint64_t tmp0 = (static_cast<uint64_t>(tmp[1]) << 32ull) | static_cast<uint32_t>(tmp[0]);
    double tmp1;  __builtin_memcpy(&tmp1, &tmp0, sizeof(tmp0));
    return tmp1;
}
__device__
inline
long __shfl(long var, int src_lane, int width = warpSize)
{
    #ifndef _MSC_VER
    static_assert(sizeof(long) == 2 * sizeof(int), "");
    static_assert(sizeof(long) == sizeof(uint64_t), "");

    int tmp[2]; __builtin_memcpy(tmp, &var, sizeof(tmp));
    tmp[0] = __shfl(tmp[0], src_lane, width);
    tmp[1] = __shfl(tmp[1], src_lane, width);

    uint64_t tmp0 = (static_cast<uint64_t>(tmp[1]) << 32ull) | static_cast<uint32_t>(tmp[0]);
    long tmp1;  __builtin_memcpy(&tmp1, &tmp0, sizeof(tmp0));
    return tmp1;
    #else
    static_assert(sizeof(long) == sizeof(int), "");
    return static_cast<long>(__shfl(static_cast<int>(var), src_lane, width));
    #endif
}
__device__
inline
unsigned long __shfl(unsigned long var, int src_lane, int width = warpSize) {
    #ifndef _MSC_VER
    static_assert(sizeof(unsigned long) == 2 * sizeof(unsigned int), "");
    static_assert(sizeof(unsigned long) == sizeof(uint64_t), "");

    unsigned int tmp[2]; __builtin_memcpy(tmp, &var, sizeof(tmp));
    tmp[0] = __shfl(tmp[0], src_lane, width);
    tmp[1] = __shfl(tmp[1], src_lane, width);

    uint64_t tmp0 = (static_cast<uint64_t>(tmp[1]) << 32ull) | static_cast<uint32_t>(tmp[0]);
    unsigned long tmp1;  __builtin_memcpy(&tmp1, &tmp0, sizeof(tmp0));
    return tmp1;
    #else
    static_assert(sizeof(unsigned long) == sizeof(unsigned int), "");
    return static_cast<unsigned long>(__shfl(static_cast<unsigned int>(var), src_lane, width));
    #endif
}
__device__
inline
long long __shfl(long long var, int src_lane, int width = warpSize)
{
    static_assert(sizeof(long long) == 2 * sizeof(int), "");
    static_assert(sizeof(long long) == sizeof(uint64_t), "");

    int tmp[2]; __builtin_memcpy(tmp, &var, sizeof(tmp));
    tmp[0] = __shfl(tmp[0], src_lane, width);
    tmp[1] = __shfl(tmp[1], src_lane, width);

    uint64_t tmp0 = (static_cast<uint64_t>(tmp[1]) << 32ull) | static_cast<uint32_t>(tmp[0]);
    long long tmp1;  __builtin_memcpy(&tmp1, &tmp0, sizeof(tmp0));
    return tmp1;
}
__device__
inline
unsigned long long __shfl(unsigned long long var, int src_lane, int width = warpSize) {
    static_assert(sizeof(unsigned long long) == 2 * sizeof(unsigned int), "");
    static_assert(sizeof(unsigned long long) == sizeof(uint64_t), "");

    unsigned int tmp[2]; __builtin_memcpy(tmp, &var, sizeof(tmp));
    tmp[0] = __shfl(tmp[0], src_lane, width);
    tmp[1] = __shfl(tmp[1], src_lane, width);

    uint64_t tmp0 = (static_cast<uint64_t>(tmp[1]) << 32ull) | static_cast<uint32_t>(tmp[0]);
    unsigned long long tmp1;  __builtin_memcpy(&tmp1, &tmp0, sizeof(tmp0));
    return tmp1;
}

__device__
inline
int __shfl_up(int var, unsigned int lane_delta, int width = warpSize) {
    int self = __lane_id();
    int index = self - lane_delta;
    index = (index < (self & ~(width-1)))?self:index;
#if __has_builtin(__builtin_amdgcn_ds_bpermute)
    return __builtin_amdgcn_ds_bpermute(index<<2, var);
#else
    return amdgcn_ds_bpermute(index<<2, var);
#endif
}
__device__
inline
unsigned int __shfl_up(unsigned int var, unsigned int lane_delta, int width = warpSize) {
    union { int i; unsigned u; float f; } tmp; tmp.u = var;
    tmp.i = __shfl_up(tmp.i, lane_delta, width);
    return tmp.u;
}
__device__
inline
float __shfl_up(float var, unsigned int lane_delta, int width = warpSize) {
    union { int i; unsigned u; float f; } tmp; tmp.f = var;
    tmp.i = __shfl_up(tmp.i, lane_delta, width);
    return tmp.f;
}
__device__
inline
double __shfl_up(double var, unsigned int lane_delta, int width = warpSize) {
    static_assert(sizeof(double) == 2 * sizeof(int), "");
    static_assert(sizeof(double) == sizeof(uint64_t), "");

    int tmp[2]; __builtin_memcpy(tmp, &var, sizeof(tmp));
    tmp[0] = __shfl_up(tmp[0], lane_delta, width);
    tmp[1] = __shfl_up(tmp[1], lane_delta, width);

    uint64_t tmp0 = (static_cast<uint64_t>(tmp[1]) << 32ull) | static_cast<uint32_t>(tmp[0]);
    double tmp1;  __builtin_memcpy(&tmp1, &tmp0, sizeof(tmp0));
    return tmp1;
}
__device__
inline
long __shfl_up(long var, unsigned int lane_delta, int width = warpSize)
{
    #ifndef _MSC_VER
    static_assert(sizeof(long) == 2 * sizeof(int), "");
    static_assert(sizeof(long) == sizeof(uint64_t), "");

    int tmp[2]; __builtin_memcpy(tmp, &var, sizeof(tmp));
    tmp[0] = __shfl_up(tmp[0], lane_delta, width);
    tmp[1] = __shfl_up(tmp[1], lane_delta, width);

    uint64_t tmp0 = (static_cast<uint64_t>(tmp[1]) << 32ull) | static_cast<uint32_t>(tmp[0]);
    long tmp1;  __builtin_memcpy(&tmp1, &tmp0, sizeof(tmp0));
    return tmp1;
    #else
    static_assert(sizeof(long) == sizeof(int), "");
    return static_cast<long>(__shfl_up(static_cast<int>(var), lane_delta, width));
    #endif
}

__device__
inline
unsigned long __shfl_up(unsigned long var, unsigned int lane_delta, int width = warpSize)
{
    #ifndef _MSC_VER
    static_assert(sizeof(unsigned long) == 2 * sizeof(unsigned int), "");
    static_assert(sizeof(unsigned long) == sizeof(uint64_t), "");

    unsigned int tmp[2]; __builtin_memcpy(tmp, &var, sizeof(tmp));
    tmp[0] = __shfl_up(tmp[0], lane_delta, width);
    tmp[1] = __shfl_up(tmp[1], lane_delta, width);

    uint64_t tmp0 = (static_cast<uint64_t>(tmp[1]) << 32ull) | static_cast<uint32_t>(tmp[0]);
    unsigned long tmp1;  __builtin_memcpy(&tmp1, &tmp0, sizeof(tmp0));
    return tmp1;
    #else
    static_assert(sizeof(unsigned long) == sizeof(unsigned int), "");
    return static_cast<unsigned long>(__shfl_up(static_cast<unsigned int>(var), lane_delta, width));
    #endif
}

__device__
inline
long long __shfl_up(long long var, unsigned int lane_delta, int width = warpSize)
{
    static_assert(sizeof(long long) == 2 * sizeof(int), "");
    static_assert(sizeof(long long) == sizeof(uint64_t), "");
    int tmp[2]; __builtin_memcpy(tmp, &var, sizeof(tmp));
    tmp[0] = __shfl_up(tmp[0], lane_delta, width);
    tmp[1] = __shfl_up(tmp[1], lane_delta, width);
    uint64_t tmp0 = (static_cast<uint64_t>(tmp[1]) << 32ull) | static_cast<uint32_t>(tmp[0]);
    long long tmp1;  __builtin_memcpy(&tmp1, &tmp0, sizeof(tmp0));
    return tmp1;
}

__device__
inline
unsigned long long __shfl_up(unsigned long long var, unsigned int lane_delta, int width = warpSize)
{
    static_assert(sizeof(unsigned long long) == 2 * sizeof(unsigned int), "");
    static_assert(sizeof(unsigned long long) == sizeof(uint64_t), "");
    unsigned int tmp[2]; __builtin_memcpy(tmp, &var, sizeof(tmp));
    tmp[0] = __shfl_up(tmp[0], lane_delta, width);
    tmp[1] = __shfl_up(tmp[1], lane_delta, width);
    uint64_t tmp0 = (static_cast<uint64_t>(tmp[1]) << 32ull) | static_cast<uint32_t>(tmp[0]);
    unsigned long long tmp1;  __builtin_memcpy(&tmp1, &tmp0, sizeof(tmp0));
    return tmp1;
}

__device__
inline
int __shfl_down(int var, unsigned int lane_delta, int width = warpSize) {
    int self = __lane_id();
    int index = self + lane_delta;
    index = (int)((self&(width-1))+lane_delta) >= width?self:index;
#if __has_builtin(__builtin_amdgcn_ds_bpermute)
    return __builtin_amdgcn_ds_bpermute(index<<2, var);
#else
    return amdgcn_ds_bpermute(index<<2, var);
#endif
}
__device__
inline
unsigned int __shfl_down(unsigned int var, unsigned int lane_delta, int width = warpSize) {
    union { int i; unsigned u; float f; } tmp; tmp.u = var;
    tmp.i = __shfl_down(tmp.i, lane_delta, width);
    return tmp.u;
}
__device__
inline
float __shfl_down(float var, unsigned int lane_delta, int width = warpSize) {
    union { int i; unsigned u; float f; } tmp; tmp.f = var;
    tmp.i = __shfl_down(tmp.i, lane_delta, width);
    return tmp.f;
}
__device__
inline
double __shfl_down(double var, unsigned int lane_delta, int width = warpSize) {
    static_assert(sizeof(double) == 2 * sizeof(int), "");
    static_assert(sizeof(double) == sizeof(uint64_t), "");

    int tmp[2]; __builtin_memcpy(tmp, &var, sizeof(tmp));
    tmp[0] = __shfl_down(tmp[0], lane_delta, width);
    tmp[1] = __shfl_down(tmp[1], lane_delta, width);

    uint64_t tmp0 = (static_cast<uint64_t>(tmp[1]) << 32ull) | static_cast<uint32_t>(tmp[0]);
    double tmp1;  __builtin_memcpy(&tmp1, &tmp0, sizeof(tmp0));
    return tmp1;
}
__device__
inline
long __shfl_down(long var, unsigned int lane_delta, int width = warpSize)
{
    #ifndef _MSC_VER
    static_assert(sizeof(long) == 2 * sizeof(int), "");
    static_assert(sizeof(long) == sizeof(uint64_t), "");

    int tmp[2]; __builtin_memcpy(tmp, &var, sizeof(tmp));
    tmp[0] = __shfl_down(tmp[0], lane_delta, width);
    tmp[1] = __shfl_down(tmp[1], lane_delta, width);

    uint64_t tmp0 = (static_cast<uint64_t>(tmp[1]) << 32ull) | static_cast<uint32_t>(tmp[0]);
    long tmp1;  __builtin_memcpy(&tmp1, &tmp0, sizeof(tmp0));
    return tmp1;
    #else
    static_assert(sizeof(long) == sizeof(int), "");
    return static_cast<long>(__shfl_down(static_cast<int>(var), lane_delta, width));
    #endif
}
__device__
inline
unsigned long __shfl_down(unsigned long var, unsigned int lane_delta, int width = warpSize)
{
    #ifndef _MSC_VER
    static_assert(sizeof(unsigned long) == 2 * sizeof(unsigned int), "");
    static_assert(sizeof(unsigned long) == sizeof(uint64_t), "");

    unsigned int tmp[2]; __builtin_memcpy(tmp, &var, sizeof(tmp));
    tmp[0] = __shfl_down(tmp[0], lane_delta, width);
    tmp[1] = __shfl_down(tmp[1], lane_delta, width);

    uint64_t tmp0 = (static_cast<uint64_t>(tmp[1]) << 32ull) | static_cast<uint32_t>(tmp[0]);
    unsigned long tmp1;  __builtin_memcpy(&tmp1, &tmp0, sizeof(tmp0));
    return tmp1;
    #else
    static_assert(sizeof(unsigned long) == sizeof(unsigned int), "");
    return static_cast<unsigned long>(__shfl_down(static_cast<unsigned int>(var), lane_delta, width));
    #endif
}
__device__
inline
long long __shfl_down(long long var, unsigned int lane_delta, int width = warpSize)
{
    static_assert(sizeof(long long) == 2 * sizeof(int), "");
    static_assert(sizeof(long long) == sizeof(uint64_t), "");
    int tmp[2]; __builtin_memcpy(tmp, &var, sizeof(tmp));
    tmp[0] = __shfl_down(tmp[0], lane_delta, width);
    tmp[1] = __shfl_down(tmp[1], lane_delta, width);
    uint64_t tmp0 = (static_cast<uint64_t>(tmp[1]) << 32ull) | static_cast<uint32_t>(tmp[0]);
    long long tmp1;  __builtin_memcpy(&tmp1, &tmp0, sizeof(tmp0));
    return tmp1;
}
__device__
inline
unsigned long long __shfl_down(unsigned long long var, unsigned int lane_delta, int width = warpSize)
{
    static_assert(sizeof(unsigned long long) == 2 * sizeof(unsigned int), "");
    static_assert(sizeof(unsigned long long) == sizeof(uint64_t), "");
    unsigned int tmp[2]; __builtin_memcpy(tmp, &var, sizeof(tmp));
    tmp[0] = __shfl_down(tmp[0], lane_delta, width);
    tmp[1] = __shfl_down(tmp[1], lane_delta, width);
    uint64_t tmp0 = (static_cast<uint64_t>(tmp[1]) << 32ull) | static_cast<uint32_t>(tmp[0]);
    unsigned long long tmp1;  __builtin_memcpy(&tmp1, &tmp0, sizeof(tmp0));
    return tmp1;
}

__device__
inline
int __shfl_xor(int var, int lane_mask, int width = warpSize) {
    int self = __lane_id();
    int index = self^lane_mask;
    index = index >= ((self+width)&~(width-1))?self:index;
#if __has_builtin(__builtin_amdgcn_ds_bpermute)
    return __builtin_amdgcn_ds_bpermute(index<<2, var);
#else
    return amdgcn_ds_bpermute(index<<2, var);
#endif
}
__device__
inline
unsigned int __shfl_xor(unsigned int var, int lane_mask, int width = warpSize) {
    union { int i; unsigned u; float f; } tmp; tmp.u = var;
    tmp.i = __shfl_xor(tmp.i, lane_mask, width);
    return tmp.u;
}
__device__
inline
float __shfl_xor(float var, int lane_mask, int width = warpSize) {
    union { int i; unsigned u; float f; } tmp; tmp.f = var;
    tmp.i = __shfl_xor(tmp.i, lane_mask, width);
    return tmp.f;
}
__device__
inline
double __shfl_xor(double var, int lane_mask, int width = warpSize) {
    static_assert(sizeof(double) == 2 * sizeof(int), "");
    static_assert(sizeof(double) == sizeof(uint64_t), "");

    int tmp[2]; __builtin_memcpy(tmp, &var, sizeof(tmp));
    tmp[0] = __shfl_xor(tmp[0], lane_mask, width);
    tmp[1] = __shfl_xor(tmp[1], lane_mask, width);

    uint64_t tmp0 = (static_cast<uint64_t>(tmp[1]) << 32ull) | static_cast<uint32_t>(tmp[0]);
    double tmp1;  __builtin_memcpy(&tmp1, &tmp0, sizeof(tmp0));
    return tmp1;
}
__device__
inline
long __shfl_xor(long var, int lane_mask, int width = warpSize)
{
    #ifndef _MSC_VER
    static_assert(sizeof(long) == 2 * sizeof(int), "");
    static_assert(sizeof(long) == sizeof(uint64_t), "");

    int tmp[2]; __builtin_memcpy(tmp, &var, sizeof(tmp));
    tmp[0] = __shfl_xor(tmp[0], lane_mask, width);
    tmp[1] = __shfl_xor(tmp[1], lane_mask, width);

    uint64_t tmp0 = (static_cast<uint64_t>(tmp[1]) << 32ull) | static_cast<uint32_t>(tmp[0]);
    long tmp1;  __builtin_memcpy(&tmp1, &tmp0, sizeof(tmp0));
    return tmp1;
    #else
    static_assert(sizeof(long) == sizeof(int), "");
    return static_cast<long>(__shfl_xor(static_cast<int>(var), lane_mask, width));
    #endif
}
__device__
inline
unsigned long __shfl_xor(unsigned long var, int lane_mask, int width = warpSize)
{
    #ifndef _MSC_VER
    static_assert(sizeof(unsigned long) == 2 * sizeof(unsigned int), "");
    static_assert(sizeof(unsigned long) == sizeof(uint64_t), "");

    unsigned int tmp[2]; __builtin_memcpy(tmp, &var, sizeof(tmp));
    tmp[0] = __shfl_xor(tmp[0], lane_mask, width);
    tmp[1] = __shfl_xor(tmp[1], lane_mask, width);

    uint64_t tmp0 = (static_cast<uint64_t>(tmp[1]) << 32ull) | static_cast<uint32_t>(tmp[0]);
    unsigned long tmp1;  __builtin_memcpy(&tmp1, &tmp0, sizeof(tmp0));
    return tmp1;
    #else
    static_assert(sizeof(unsigned long) == sizeof(unsigned int), "");
    return static_cast<unsigned long>(__shfl_xor(static_cast<unsigned int>(var), lane_mask, width));
    #endif
}
__device__
inline
long long __shfl_xor(long long var, int lane_mask, int width = warpSize)
{
    static_assert(sizeof(long long) == 2 * sizeof(int), "");
    static_assert(sizeof(long long) == sizeof(uint64_t), "");
    int tmp[2]; __builtin_memcpy(tmp, &var, sizeof(tmp));
    tmp[0] = __shfl_xor(tmp[0], lane_mask, width);
    tmp[1] = __shfl_xor(tmp[1], lane_mask, width);
    uint64_t tmp0 = (static_cast<uint64_t>(tmp[1]) << 32ull) | static_cast<uint32_t>(tmp[0]);
    long long tmp1;  __builtin_memcpy(&tmp1, &tmp0, sizeof(tmp0));
    return tmp1;
}
__device__
inline
unsigned long long __shfl_xor(unsigned long long var, int lane_mask, int width = warpSize)
{
    static_assert(sizeof(unsigned long long) == 2 * sizeof(unsigned int), "");
    static_assert(sizeof(unsigned long long) == sizeof(uint64_t), "");
    unsigned int tmp[2]; __builtin_memcpy(tmp, &var, sizeof(tmp));
    tmp[0] = __shfl_xor(tmp[0], lane_mask, width);
    tmp[1] = __shfl_xor(tmp[1], lane_mask, width);
    uint64_t tmp0 = (static_cast<uint64_t>(tmp[1]) << 32ull) | static_cast<uint32_t>(tmp[0]);
    unsigned long long tmp1;  __builtin_memcpy(&tmp1, &tmp0, sizeof(tmp0));
    return tmp1;
}

#endif
