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


#include <cerrno>
#include <optional>
#include <string>
#include <system_error>


#if defined(_WIN32)

#if !defined(_X86_) && !defined(_AMD64_) && !defined(_ARM_) && !defined(_ARM64_)
#if defined(_M_IX86)
#define _X86_
#elif defined(_M_AMD64)
#define _AMD64_
#elif defined(_M_ARM)
#define _ARM_
#elif defined(_M_ARM64)
#define _ARM64_
#endif
#endif

#include <minwindef.h>
#include <fileapi.h>
#include <handleapi.h>
#include <errhandlingapi.h>

#else
	
#include <fcntl.h>
#include <unistd.h>

#endif


#include "text.hpp"
 
 
 namespace ufmt {
	 
	template<class T = text> class text_file {
		T text_;
		
		static auto const invalid_handle = -1;
		
#if defined(_WIN32)
		
		using handle_type = HANDLE;
		
		static auto constexpr file_read_data = DWORD(1);
		static auto constexpr file_write_data = DWORD(1);
		static auto constexpr file_append_data = DWORD(4);
		static auto constexpr file_share_read = DWORD(1);
		static auto constexpr create_always_flag = DWORD(2);
		static auto constexpr open_existing_flag = DWORD(3);
		static auto constexpr open_always_flag = DWORD(4);
		static auto constexpr file_attribute_normal = DWORD(0x80);
			
		handle_type handle_{invalid_handle};
		
		explicit text_file(HANDLE handle) noexcept: handle_{handle} { }
		
		static std::optional<text_file> open(std::string const& path, DWORD desired_access,
			                                 DWORD creation_disposition, std::error_code& ec) {
			auto const created = CreateFileA(path.data(), desired_access,
				file_share_read, nullptr, creation_disposition, file_attribute_normal, nullptr);
			if(created == INVALID_HANDLE_VALUE) {
				ec = {int(GetLastError()), std::system_category()};
				return std::nullopt;
			}
			return {text_file{created}};
		}
		
#else
		using handle_type = int;
	
		handle_type handle_{invalid_handle};
		
		explicit text_file(int handle) noexcept: handle_{handle} { }
		
		static std::optional<text_file> open(std::string const& path, int flags, std::error_code& ec) {
			auto const opened = ::open(path.data(), flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
			if(opened == -1) {
				ec = {errno, std::system_category()};
				return std::nullopt;
			}
			return {text_file{opened}};
		}
		
#endif

	
	public:
	
	
		static std::optional<text_file> create_always(std::string const& path, std::error_code& ec) {
#if defined(_WIN32)
			return open(path, file_write_data, create_always_flag, ec);
#else
			return open(path, O_CREAT | O_TRUNC | O_WRONLY, ec); 
#endif
		}


		static std::optional<text_file> open_existing(std::string const& path, std::error_code& ec) {
#if defined(_WIN32)
			return open(path, file_write_data, open_existing_flag, ec);
#else
			return open(path, O_WRONLY, ec);
#endif
		}
		
		
		static std::optional<text_file> open_always_to_append(std::string const& path, std::error_code& ec) {
#if defined(_WIN32)
			return open(path, file_append_data, open_always_flag, ec);
#else
			return open(path, O_CREAT | O_APPEND | O_WRONLY, ec);
#endif
		}

	
		~text_file() noexcept { close(); }
		text_file(text_file const&) = delete;
		text_file& operator = (text_file const&) = delete;
		
		
		explicit operator bool () const noexcept {
			return handle_ != handle_type(invalid_handle);
		}
		
		
		text_file(text_file&& other) noexcept: handle_{other.handle_} {
			other.handle_ = handle_type(invalid_handle);
		}
		
		
		text_file& operator = (text_file&& other) noexcept {
			close();
			handle_ = other.handle_;
			other.handle_ = handle_type(invalid_handle);
			return *this;
		}
		
		
		void close() noexcept {
			if(handle_ == handle_type(invalid_handle))
				return;
#if defined(_WIN32)
			CloseHandle(handle_);
#else
			::close(handle_);
#endif
			handle_ = handle_type(invalid_handle);
		}
		
		
		bool write(std::string_view sv, std::error_code& ec) noexcept {
#if defined(_WIN32)
			if(!WriteFile(handle_, text_.data(), text_.size(), nullptr, nullptr)) {
				ec = {int(GetLastError()), std::system_category()};
				return false;
			}
#else
			if(::write(handle_, text_.data(), text_.size()) == -1) {
				ec = {errno, std::system_category()};
				return false;
			}
#endif
			return true;
		}

		
		template<typename... Args>
		void print(Args&&... args) {
			text_.clear();
			text_.format(std::forward<Args>(args)..., '\n');

#if defined(_WIN32)
			WriteFile(handle_, text_.data(), text_.size(), nullptr, nullptr);
#else
			::write(handle_, text_.data(), text_.size());
#endif
		}
		
		
		
	}; // file
	 
 } // namespace ufmt