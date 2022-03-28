// This file is part of ufmt library
// Copyright 2020-2022 Andrei Ilin <ortfero@gmail.com>
// SPDX-License-Identifier: MIT
 
#pragma once

#include <optional>
#include <string>
#include <system_error>

#include <errno.h>
#include <stdio.h>

#include "text.hpp"


namespace ufmt {
	
	
	template<class T = text> class buffered_file {
		
		text text_;
		FILE* handle_{nullptr};
		
		explicit buffered_file(FILE* handle) noexcept: handle_{handle} { }
		
		template<std::size_t N>
		static std::optional<buffered_file> open(std::string const& path, char const (&mode)[N],
		                                         std::error_code& ec) {
			auto* opened = fopen(path.data(), mode);
			if(!opened) {
				ec = {errno, std::system_category()};
				return std::nullopt;
			}
			return buffered_file{opened};
		}
		
	public:
	
		~buffered_file() noexcept { close(); }
		buffered_file(buffered_file const&) = delete;
		buffered_file& operator = (buffered_file const&) = delete;
		
		
		static std::optional<buffered_file> create_always(std::string const& path, std::error_code& ec) {
			return open(path, "w+b", ec);
		}


		static std::optional<text_file> open_existing(std::string const& path, std::error_code& ec) {
			return open(path, "wb", ec);
		}
		
		
		static std::optional<text_file> open_always_to_append(std::string const& path, std::error_code& ec) {
			return open(path, "a+b", ec);
		}
		
		
		explicit operator bool () const noexcept {
			return handle_ != nullptr;
		}
		
		
		buffered_file(buffered_file&& other) noexcept: handle_{other.handle_} {
			other.handle_ = nullptr;
			return *this;
		}
		
		
		buffered_file& operator = (buffered_file&& other) noexcept {
			close();
			handle_ = other.handle_; other.handle_ = nullptr;
			return *this;
		}
	
		
		void close() noexcept {
			if(handle_ == nullptr)
				return;
			fclose(handle_);
			handle_ = nullptr;
		}
		
		
		bool write(std::string_view sv, std::error_code& ec) noexcept {
#if defined(_WIN32)
			if(_fwrite_nolock(text_.data(), text_.size(), sizeof(char), handle_) != text_.size()) {
#else
			if(fwrite_unlocked(text_.data(), text_.size(), sizeof(char), handle_) != text_.size()) {
#endif
				ec = {errno, std::system_category()};
				return false;
			}
			return true;
		}
		
		
		template<typename... Args>
		void print(Args&&... args) {
			text_.clear();
			text_.format(std::forward<Args>(args)..., '\n');
#if defined(_WIN32)
			_fwrite_nolock(text_.data(), text_.size(), sizeof(char), handle_);
#else
			fwrite_unlocked(text_.data(), text_.size(), sizeof(char), handle_);
#endif
		}
	}; // buffered_file
	
	
} // namespace ufmt
