#pragma once

#include <cuda_gl_interop.h>
#include <cuda_runtime.h>

#include <optix.h>
#include <optix_function_table_definition.h>
#include <optix_stubs.h>

#include <sampleConfig.h>

#include <optix_stack_size.h>
#include <sutil/CUDAOutputBuffer.h>
#include <sutil/Camera.h>
#include <sutil/Exception.h>
#include <sutil/GLDisplay.h>
#include <sutil/Matrix.h>
#include <sutil/Trackball.h>
#include <sutil/sutil.h>
#include <sutil/vec_math.h>

#include <type_traits>
#include <utility>
#include <cudaMemMarco.hpp>

namespace xinxinoptix {

template <class T> struct raii_traits {
    static void deallocate(OptixDeviceContext p) {
        OPTIX_CHECK(optixDeviceContextDestroy(p));
    }

    static void deallocate(OptixModule p) {
        OPTIX_CHECK(optixModuleDestroy(p));
    }

    static void deallocate(OptixPipeline p) {
        OPTIX_CHECK(optixPipelineDestroy(p));
    }

    static void deallocate(OptixProgramGroup p) {
        OPTIX_CHECK(optixProgramGroupDestroy(p));
    }

    static void deallocate(CUstream p) {
        CUDA_CHECK(cudaStreamDestroy(p));
    }

    static void allocate(CUdeviceptr **pp, size_t n) {
        CUDA_CHECK(cudaMalloc(pp, n));
    }

    static void deallocate(CUdeviceptr p) {
        if(p) {
            CUDA_CHECK(cudaFree((void *)p));
        }
    }
};

template <class T, class traits = raii_traits<T>,
         class = std::enable_if_t<std::is_same_v<std::decay_t<T>, T> &&
         std::is_void_v<decltype(traits::deallocate(std::declval<T>()))>>>
struct raii {
    T handle;
    std::size_t size, capacity;

    operator T &() noexcept {
        return handle;
    }

    operator T const &() const noexcept {
        return handle;
    }

    T *operator&() noexcept {
        return std::addressof(handle);
    }

    T const *operator&() const noexcept {
        return std::addressof(handle);
    }

    raii() noexcept {
        handle = 0;
        size = 0;
        capacity = 0;
    }

    T &reset() noexcept {
        if (handle)
            traits::deallocate(handle);
        handle = 0;
        size = 0;
        capacity = 0;
        return handle;
    }

    void swap(raii &o) {
        std::swap(handle, o.handle);
        std::swap(size, o.size);
        std::swap(capacity, o.capacity);
    }

    template <typename TT = T, std::enable_if_t<std::is_same_v<TT, CUdeviceptr>> * = nullptr>
    bool resize(std::size_t newSize, std::size_t incSize = 0) {
#if 0
        if (newSize != size) {  // temporary
            printf("\n\nreallocating %d bytes (previous %d bytes)\n\n\n", (int)size, (int)newSize);
            CUDA_CHECK(cudaMalloc(reinterpret_cast<void **>(&reset()), newSize));
            size = newSize;
            capacity = newSize;
            return true;
        }
#else
        if (newSize > capacity) {
            auto newCapacity = newSize + std::min(incSize * 4ull, 128ull * 1024 * 1024);
            //printf("\n\nreallocating %d bytes (previous %d bytes)\n\n\n", (int)size, (int)newCapacity);
            CUDA_CHECK(cudaMalloc(reinterpret_cast<void **>(&reset()), newCapacity));
            size = newSize;
            capacity = newCapacity;
            return true;
        }
#endif
        return false;
    }

    raii(raii const &) = delete;
    raii &operator=(raii const &) = delete;

    raii(raii &&that) noexcept {
        handle = that.handle;
        size = that.size;
        capacity = that.capacity;
        that.handle = 0;
        that.size = 0;
        that.capacity = 0;
    }

    raii &operator=(raii &&that) noexcept {
        if (std::addressof(that) == this)
            return *this;
        if (handle)
            traits::deallocate(handle);
        handle = that.handle;
        size = that.size;
        capacity = that.capacity;
        that.handle = 0;
        that.size = 0;
        that.capacity = 0;
        return *this;
    }

    ~raii() noexcept {
        if (handle)
            traits::deallocate(handle);
        handle = 0;
        size = 0;
        capacity = 0;
    }
};

#if 0
template <class T, class traits>
struct raii<T, traits, std::void_t<decltype(traits::allocate)>> : raii<T, traits> {
    using raii<T, traits>::raii;

    template <class ...Ts, class = std::enable_if_t<sizeof...(Ts) && std::is_void_v<
        decltype(traits::allocate(std::declval<T *>(), std::declval<Ts>()...))>>>
    explicit raii(Ts &&...ts) noexcept(noexcept(traits::allocate(
                std::declval<T *>(), std::forward<Ts>(ts)...))) {
        handle = 0;
        traits::allocate(std::addressof(handle), std::forward<Ts>(ts)...);
    }

    template <class ...Ts, class = std::enable_if_t<std::is_void_v<
        decltype(traits::allocate(std::declval<T *>(), std::declval<Ts>()...))>>>
    void assign(Ts &&...ts) noexcept(noexcept(traits::allocate(
                std::declval<T *>(), std::forward<Ts>(ts)...))) {
        if (handle)
            traits::deallocate(handle);
        handle = 0;
        traits::allocate(std::addressof(handle), std::forward<Ts>(ts)...);
    }

};
#endif

} // namespace xinxinoptix
