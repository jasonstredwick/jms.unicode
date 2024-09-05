// Copyright © 2024 Jason Stredwick

#pragma once


#include <filesystem>
#include <format>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>

#include "jcu/ucd/utility.hpp"


namespace jcu::ucd {


template <typename Data_t, typename Derived_t>
class FileIterator {
public:
    using derived_type = Derived_t;
    using difference_type = ptrdiff_t;
    using value_type = Data_t;

    FileIterator() = default;
    FileIterator(bool /*sentinel*/) : is_done{true}, is_sentinel{true} {}
protected:
    FileIterator(const std::filesystem::path& path, std::ios_base::openmode mode)
        : fs{OpenDataFile(path, mode)}, file_path{path}, mode{mode}
        {}
public:
    FileIterator(const FileIterator& i) { *this = i; }
    FileIterator(FileIterator&&) noexcept = default;
    virtual ~FileIterator() = default;
    FileIterator& operator=(const FileIterator& i) { Copy(i); return *this; }
    FileIterator& operator=(FileIterator&&) noexcept = default;

    friend bool operator==(const FileIterator<value_type, Derived_t>& lhs,
                           const FileIterator<value_type, Derived_t>& rhs) {
        if (lhs.is_sentinel || rhs.is_sentinel) {
            return (lhs.is_sentinel && rhs.is_sentinel) ||
                   (lhs.is_sentinel && rhs.is_done) ||
                   (lhs.is_done && rhs.is_sentinel);
        }

        int is_open = static_cast<int>(lhs.fs.is_open()) + static_cast<int>(rhs.fs.is_open());
        if (lhs.is_done != rhs.is_done || is_open == 1 || lhs.line_num != rhs.line_num) { return false; }
        return lhs.file_path == rhs.file_path;
    }

    auto& operator++(this FileIterator& self) {
        while (std::getline(self.fs, self.buffer)) {
            ++self.line_num;
            if (self.AcceptLine()) { break; }
        }

        if      (self.fs.rdstate() & std::ios_base::eofbit)  { self.is_done = true; }
        else if (self.fs.rdstate() & std::ios_base::failbit) {
            throw std::runtime_error{std::format("Unexpected line of data ({}): {}", self.line_num, self.buffer)};
        }

        self.data = self.is_done ? value_type{} : self.ProcessLine();
        return dynamic_cast<derived_type&>(self);
    }

    auto operator++(this FileIterator& self, int) {
        auto tmp = dynamic_cast<derived_type&>(self);
        ++self;
        return tmp;
    }

    const value_type& operator* (this FileIterator const& self) noexcept { return self.data; }
    const value_type* operator->(this FileIterator const& self) noexcept { return std::addressof(self.data); }
    size_t GetLineNum() const noexcept { return line_num; }

protected:
    std::string buffer{};
    value_type data{};
    std::ifstream fs{};
    std::filesystem::path file_path{};
    std::ios_base::openmode mode{std::ios::in};
    uint32_t line_num{0};
    bool is_done{false};
    bool is_sentinel{false};

    virtual bool AcceptLine() const = 0;
    virtual value_type ProcessLine() = 0;

    void Init(this FileIterator& self) {
        self.buffer.reserve(4096);
        ++self; // sets buffer to first entry or is marked as end FileIterator if no valid lines.
    }

    void Copy(this auto&& self, const FileIterator& rhs) {
        self.buffer = rhs.buffer;
        self.data = rhs.data;
        self.file_path = rhs.file_path;
        self.mode = rhs.mode;
        self.line_num = rhs.line_num;
        self.is_done = rhs.is_done;
        self.is_sentinel = rhs.is_sentinel;
        if (self.fs.is_open()) { self.fs.close(); }
        if (!self.file_path.empty() && rhs.fs.is_open()) {
            self.fs = OpenDataFile(self.file_path, self.mode);
            // tellg can set the failure bit; ok to throw exception
            auto* ifs = const_cast<std::ifstream*>(std::addressof(rhs.fs));
            self.fs.seekg(ifs->tellg(), std::ios::beg);
        }
    }
};


}
