/* -----------------------------------------------------------------------------
 * Copyright (c) 2020 Advanced Micro Devices, Inc. All Rights Reserved.
 * See 'LICENSE' in the project root for license information.
 * -------------------------------------------------------------------------- */
#pragma once

#if !defined(__HIP_CPU_RT__)
    #error Private HIP-CPU RT implementation headers must not be included directly.
#endif

#include <version>
#if defined(__cpp_lib_bit_cast)
    #include <bit>

    namespace hip
    {
        namespace detail
        {
            using std::bit_cast;
        } // Namespace hip::detail.
    } // Namespace hip.
#else
    #include <cstring>
    #include <type_traits>

    namespace hip
    {
        namespace detail
        {
            template<
                typename T,
                typename U,
                std::enable_if_t<
                    sizeof(T) == sizeof(U) &&
                    std::is_trivially_copyable_v<T> &&
                    std::is_trivially_copyable_v<U>>* = nullptr>
            inline
            T bit_cast(const U& x) noexcept
            {
                [[maybe_unused]]
                constexpr auto bc{[](auto&& v) constexpr noexcept {
                     // TODO: incorrect, e.g. half_float half is not trivial.
                    static_assert(std::is_nothrow_default_constructible_v<T>);

                    T r;
                    std::memcpy(&r, &v, sizeof(T));

                    return r;
                }};
                #if defined(__has_builtin)
                    #if __has_builtin(__builtin_bit_cast)
                        return __builtin_bit_cast(T, x);
                    #else
                        return bc(x);
                    #endif
                #else
                    return bc(x);
                #endif
            }
        } // Namespace hip::detail.
    } // Namespace hip.
#endif