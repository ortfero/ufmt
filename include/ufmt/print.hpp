/* This file is part of ufmt library
 * Copyright 2020-2022 Andrei Ilin <ortfero@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once


#include <atomic>
#include <mutex>
#include <thread>


#if defined(_WIN32)


#    if !defined(_X86_) && !defined(_AMD64_) && !defined(_ARM_) \
        && !defined(_ARM64_)
#        if defined(_M_IX86)
#            define _X86_
#        elif defined(_M_AMD64)
#            define _AMD64_
#        elif defined(_M_ARM)
#            define _ARM_
#        elif defined(_M_ARM64)
#            define _ARM64_
#        endif
#    endif

#    include <fileapi.h>
#    include <ProcessEnv.h>

#else

#    include <unistd.h>

#endif   // WIN32

#include "text.hpp"


namespace ufmt {

    namespace detail::printer {

        class alignas(64) spinlock {
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
        WriteFile(
            GetStdHandle(STD_OUTPUT_HANDLE),
            detail::printer::buffer.data(),
            detail::printer::buffer.size(),
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
        detail::printer::buffer.print(std::forward<Args>(args)..., '\n');

#if defined(_WIN32)

        WriteFile(
            GetStdHandle(STD_ERROR_HANDLE),
            detail::printer::buffer.data(),
            detail::printer::buffer.size(),
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
