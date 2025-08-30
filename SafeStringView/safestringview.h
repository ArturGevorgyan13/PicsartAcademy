#ifndef S_H
#define S_H

#include <iostream>
#include <memory>
#include <type_traits>
#include <utility>
#include <limits>
#include <cstring>

class safe_string_view;

class safe_string{
public:
    //Constructors, assignments, destructor
    safe_string();
    explicit safe_string(std::string s);

    template <typename ...Args, std::enable_if_t<std::is_constructible_v<std::string, Args&&...>, int> = 0>
    explicit safe_string(Args&&... args);

    safe_string(const safe_string& other);
    safe_string& operator=(const safe_string& other);

    safe_string(safe_string&& other) noexcept = default;
    safe_string& operator=(safe_string&& other) noexcept = default;

    ~safe_string();

    //Access functions
    std::string& main_str() &;
    const std::string& main_str() const &;
    std::string&& main_str() &&;

    //Cast operators
    operator std::string&();
    operator const std::string&() const;

    //Safe view
    safe_string_view view() const;

    //Flaf access
    std::weak_ptr<bool> weak() const;

private:
    std::string str;
    std::shared_ptr<bool> flag;

};

safe_string::safe_string() : flag{std::make_shared<bool>(true)} {}

safe_string::safe_string(std::string s) : str{std::move(s)}, flag{std::make_shared<bool>(true)} {}

template <typename ...Args, std::enable_if_t<std::is_constructible_v<std::string, Args&&...>, int> = 0>
safe_string::safe_string(Args&& ...args)
    : str(std::forward<Args>(args)...), flag(std::make_shared<bool>(true)) {}

safe_string::safe_string(const safe_string& other)
    : str(other.str), flag(std::make_shared<bool>(true)) {}

safe_string& safe_string::operator=(const safe_string& other){

    if(this != &other) {

        str = other.str;

        flag = std::make_shared<bool>(true);

    }

    return *this;

}

safe_string::~safe_string(){

    if(flag) {

        *flag = false;

    }

}

std::string& safe_string::main_str() & {

    return str;

}

const std::string& safe_string::main_str() const & {

    return str;

}

std::string&& safe_string::main_str() && {

    return std::move(str);

}

safe_string::operator std::string&() {

    return str;

}

safe_string::operator const std::string&() const {

    return str;

}

safe_string_view safe_string::view() const {

    return safe_string_view{str, flag};

}

std::weak_ptr<bool> safe_string::weak() const {

    return flag;

}

class safe_string_view{
public:
    //Type aliases
    using value_type = char;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using reference = char&;
    using const_reference = const char&;
    using pointer = char*;
    using const_pointer = const char*;
    using iterator = const char*;
    using const_iterator = const char*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    static constexpr size_type npos = static_cast<size_type>(-1);

    //Constructors
    safe_string_view() noexcept = default;
    constexpr safe_string_view(const safe_string_view&) noexcept = default;

    //(from string literals)
    constexpr safe_string_view(const char* str);
    constexpr safe_string_view(const char* str, size_type len);

    //(from string)
    safe_string_view(const std::string& s, std::weak_ptr <bool> f);
    safe_string_view(const std::string& str) = delete;
    safe_string_view(const std::string& str, size_type pos, size_type len, std::weak_ptr<bool> f);

    //Assignment
    safe_string_view& operator=(const safe_string_view&) noexcept = default;

    //Iterators
    constexpr const_iterator begin() const noexcept;
    constexpr const_iterator end() const noexcept;
    constexpr const_iterator cbegin() const noexcept;
    constexpr const_iterator cend() const noexcept;
    constexpr const_reverse_iterator rbegin() const noexcept;
    constexpr const_reverse_iterator rend() const noexcept;
    constexpr const_reverse_iterator crbegin() const noexcept;
    constexpr const_reverse_iterator crend() const noexcept;

    //Element access
    constexpr const_reference operator[](size_type pos) const;
    constexpr const_reference at(size_type pos) const;
    constexpr const_reference front() const;
    constexpr const_reference back() const;
    constexpr const_pointer fdata() const noexcept;

    //Capacity
    constexpr size_type fsize() const noexcept;
    constexpr size_type length() const noexcept;
    constexpr size_type max_size() const noexcept;
    constexpr bool empty() const noexcept;

    //Modifiers
    constexpr void remove_prefix(size_type n);
    constexpr void remove_postfix(size_type n);
    constexpr void swap(safe_string_view& v) noexcept;

    //String operators
    size_type copy(char* dest, size_type len, size_type pos = 0) const;
    constexpr safe_string_view substr(size_type pos = 0, size_type len = npos) const;

    int compare(safe_string_view v) const noexcept;
    int compare(size_type pos1, size_type len1, safe_string_view v) const;
    int compare(const char* s) const;

    //Search operations
    constexpr size_type find(safe_string_view v, size_type pos = 0) const noexcept;
    constexpr size_type find(char c, size_type pos = 0) const noexcept;
    constexpr size_type find(const char* s, size_type pos, size_type len) const;
    constexpr size_type find(const char* s, size_type pos = 0) const;

    constexpr size_type rfind(safe_string_view v, size_type pos = 0) const noexcept;
    constexpr size_type rfind(char c, size_type pos = npos) const noexcept;

    constexpr size_type find_first_of(safe_string_view v, size_type pos = 0) const noexcept;
    constexpr size_type find_last_of(safe_string_view v, size_type pos = npos) const noexcept;
    constexpr size_type find_first_not_of(safe_string_view v, size_type pos = 0) const noexcept;
    constexpr size_type find_last_not_of(safe_string_view v, size_type pos = npos) const noexcept;

    //Safety-specific methods
    bool is_valid() const noexcept;
    void invalidate() noexcept;
    bool is_string_literal() const noexcept;

    //Conversion
    explicit operator std::string() const;
    std::string to_string() const;

    //Helper
    std::string_view helper() const noexcept;

private:
    enum class Type: unsigned char { Empty, Static, Dynamic };

    const char* data = "";
    size_type size = 0;
    Type type = Type::Empty;

    std::weak_ptr<bool> flag{};
    const void* address = nullptr;

    void ensure_valid() const {

        if(!is_valid()) {

            throw std::runtime_error("Invalid access!\n");

        }

    }

    friend std::ostream& operator<<(std::ostream& os, safe_string_view v);

};

safe_string_view::safe_string_view(const char* str)
    : data(str), size(str ? std::char_traits<char>::length(str) : 0), type(Type::Static) {}
    
safe_string_view::safe_string_view(const char* str, size_type len)
    : data(str), size(len), type(Type::Static) {}

safe_string_view::safe_string_view(const std::string& s, std::weak_ptr<bool> f)
    : data(s.data()), size(s.size()), type(Type::Dynamic), flag(std::move(f)), address(&s) {}

safe_string_view::safe_string_view(const std::string& s, size_type pos, size_type len, std::weak_ptr<bool> f)
    : type(Type::Dynamic), flag(std::move(f)), address(&s)

{

    if(pos > s.size()) throw std::out_of_range("safe_string_view(std::string, pos, len)");

    size = (len == npos || len + pos > s.size()) ? (s.size() - pos) : len;

    data = s.data() + pos;

} 

safe_string_view::const_iterator safe_string_view::begin() const {

    return data;

}

safe_string_view::const_iterator safe_string_view::end() const {

    return data + size;

}

safe_string_view::const_iterator safe_string_view::cbegin() const {

    return begin();

}

safe_string_view::const_iterator safe_string_view::cend() const {

    return end();

}

safe_string_view::const_reverse_iterator safe_string_view::rbegin() const {

    return const_reverse_iterator(end());

}

safe_string_view::const_reverse_iterator safe_string_view::rend() const {

    return const_reverse_iterator(begin());

}

safe_string_view::const_reverse_iterator safe_string_view::crbegin() const {

    return rbegin();

}

safe_string_view::const_reverse_iterator safe_string_view::crend() const {

    return rend();

}

safe_string_view::size_type safe_string_view::fsize() const{

    return size;

}

safe_string_view::size_type safe_string_view::length() const{

    return size;

}

safe_string_view::size_type safe_string_view::max_size() const{

    return std::numeric_limits<size_type>::max() / 2;

}

bool safe_string_view::empty() const{

    return size == 0;

}

safe_string_view::const_reference safe_string_view::operator[](size_type pos) const {

    ensure_valid();
    
    return data[pos];

}

safe_string_view::const_reference safe_string_view::at(size_type pos) const {

    ensure_valid();

    if(pos >= size) throw std::out_of_range("safe_string_view::at");

    return data[pos];

}

safe_string_view::const_reference safe_string_view::front() const {

    ensure_valid();

    if(empty()) throw std::out_of_range("front");

    return data[0];

}

safe_string_view::const_reference safe_string_view::back() const {

    ensure_valid();

    if(empty()) throw std::out_of_range("back");

    return data[size - 1];

}

safe_string_view::const_pointer safe_string_view::fdata() const {

    return data;

}

void safe_string_view::remove_prefix(size_type n) {

    ensure_valid();

    if(n > size) n = size;

    data += n;

    size -= n;

}

void safe_string_view::remove_postfix(size_type n) {

    ensure_valid();

    if(n > size) n = size;

    size -= n;

}

void safe_string_view::swap(safe_string_view& v) {

    std::swap(*this, v);

}

safe_string_view::size_type safe_string_view::copy(char* dest ,size_type len, size_type pos = 0) const {

    ensure_valid();

    if(pos > size) throw std::out_of_range("safe_string_view::copy");

    size_type tmp_len = std::min(len, size - pos);

    std::memcpy(dest, data + pos, tmp_len);

    return tmp_len;

}

safe_string_view safe_string_view::substr(size_type pos = 0, size_type len = npos) const {

    ensure_valid();

    if(pos > size) throw std::out_of_range("safe_string_view::substr");

    size_type tmp_len = (len == npos || pos + len > size) ? (size - pos) : len;

    safe_string_view v;

    v.data = data + pos;

    v.size = tmp_len;

    v.type = type;

    v.flag = flag;

    v.address = address;

    return v;

}

int safe_string_view::compare(safe_string_view v) const {

    return helper().compare(v.helper());

}

int safe_string_view::compare(size_type pos1 ,size_type len1, safe_string_view v) const {

    return helper().substr(pos1, len1).compare(v.helper());

}

int safe_string_view::compare(const char* s) const {

    return helper().compare(s);

}

safe_string_view::size_type safe_string_view::find(safe_string_view v, size_type pos = 0) const {

    return helper().find(v.helper(), pos);

}

safe_string_view::size_type safe_string_view::find(char c, size_type pos = 0) const {

    return helper().find(c, pos);

}

safe_string_view::size_type safe_string_view::find(const char* s, size_type pos, size_type len) const {

    return helper().find(std::string_view{s, len}, pos);

}

safe_string_view::size_type safe_string_view::find(const char* s, size_type pos = 0) const {

    return helper().find(s, pos);

}

safe_string_view::size_type safe_string_view::rfind(safe_string_view v, size_type pos = npos) const {

    return helper().rfind(v.helper(), pos);

}

safe_string_view::size_type safe_string_view::rfind(char c, size_type pos = npos) const {

    return helper().rfind(c, pos);

}

safe_string_view::size_type safe_string_view::find_first_of(safe_string_view v, size_type pos = 0) const {

    return helper().find_first_of(v.helper(), pos);

}

safe_string_view::size_type safe_string_view::find_last_of(safe_string_view v, size_type pos = npos) const {

    return helper().find_last_of(v.helper(), pos);

}

safe_string_view::size_type safe_string_view::find_first_not_of(safe_string_view v, size_type pos = 0) const {

    return helper().find_first_not_of(v.helper(), pos);

}

safe_string_view::size_type safe_string_view::find_last_not_of(safe_string_view v, size_type pos = npos) const {

    return helper().find_last_not_of(v.helper(), pos);

}

bool safe_string_view::is_valid() const {

    switch(type) {

        case Type::Static:
            return data != nullptr;

        case Type::Dynamic:
            return !flag.expired();

        case Type::Empty:
        default:
            return true;

    }

}

void safe_string_view::invalidate() {
 
    type = Type::Empty;

    flag.reset();

}

bool safe_string_view::is_string_literal() const {

    return type == Type::Static;

}

safe_string_view::operator std::string() const {

    ensure_valid();

    return std::string(data, size); 

}

std::string safe_string_view::to_string() const {

    ensure_valid();

    return std::string(data, size);

}

std::string_view safe_string_view::helper() const {

    if(!is_valid()) return std::string_view{};
    
    return std::string_view{data, size};

}

//Non-members functions
bool operator==(safe_string_view lhs, safe_string_view rhs) noexcept {

    return lhs.helper() == rhs.helper();

}

bool operator!=(safe_string_view lhs, safe_string_view rhs) noexcept {

    return !(lhs == rhs);

}

bool operator<(safe_string_view lhs, safe_string_view rhs) noexcept {

    return lhs.helper() < rhs.helper();

}

bool operator<=(safe_string_view lhs, safe_string_view rhs) noexcept {

    return !(rhs < lhs);

}

bool operator>(safe_string_view lhs, safe_string_view rhs) noexcept {

    return rhs.helper() < lhs.helper();

}

bool operator>=(safe_string_view lhs, safe_string_view rhs) noexcept {

    return !(rhs > lhs);

}

std::ostream& operator<<(std::ostream& os, safe_string_view v) {

    if(!v.is_valid()) throw std::runtime_error("Invalid safe_string_view access");

    return os.write(v.data, static_cast<std::streamsize>(v.size));

}

#endif