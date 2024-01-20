// This file is part of ufmt library
// Copyright 2020-2022 Andrei Ilin <ortfero@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once


#include <atomic>
#include <mutex>
#include <thread>


#if defined(_WIN32)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>

#else

#    include <unistd.h>

#endif   // WIN32

#include "text.hpp"


namespace ufmt {

    namespace detail::printer {

        class spinlock {
        public:

            spinlock() noexcept = default;
            spinlock(spinlock const&) noexcept = delete;
            spinlock& operator = (spinlock const&) noexcept = delete;

            bool try_lock() noexcept {
                if(flag_.load(std::memory_order_relaxed))
                    return false;

                return !flag_.exchange(true, std::memory_order_acquire);
            }


            void unlock() noexcept {
                flag_.store(false, std::memory_order_release);
            }


            void lock() noexcept {
                while(!try_lock())
                    std::this_thread::yield();
            }


            bool try_lock_shared() noexcept {
                return try_lock();
            }


            void unlock_shared() noexcept {
                unlock();
            }


            void lock_shared() noexcept {
                lock();
            }


        private:

            std::atomic_bool flag_{false};

        }; // spinlock


        inline spinlock sync;
        inline text buffer;

    }   // namespace detail::printer


    template<typename... Args>
    void print(Args&&... args) {
        std::unique_lock g {detail::printer::sync};
        detail::printer::buffer.clear();
        detail::printer::buffer.format(std::forward<Args>(args)..., '\n');

#if defined(_WIN32)
        WriteFile(GetStdHandle(DWORD(-11)),
            detail::printer::buffer.data(),
            DWORD(detail::printer::buffer.size()),
            nullptr,
            nullptr
        );
#else
        write(1, detail::printer::buffer.data(), detail::printer::buffer.size());
#endif
    }


    template<typename R, typename... Args>
    R print_with(R&& result, Args&&... args) {
        print(std::forward<Args>(args)...);
        return std::move(std::forward<R>(result));
    }


    template<typename... Args>
    void error(Args&&... args) {
        std::unique_lock g {detail::printer::sync};
        detail::printer::buffer.clear();
        detail::printer::buffer.format(std::forward<Args>(args)..., '\n');

#if defined(_WIN32)

        WriteFile(GetStdHandle(DWORD(-12)),
            detail::printer::buffer.data(),
            DWORD(detail::printer::buffer.size()),
            nullptr,
            nullptr
        );

#else
        write(2, detail::printer::buffer.data(), detail::printer::buffer.size());
#endif
    }


    template<typename R, typename... Args>
    R error_with(R&& result, Args&&... args) {
        error(std::forward<Args>(args)...);
        return std::move(std::forward<R>(result));
    }


}   // namespace ufmt
