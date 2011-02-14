// -*- C++ -*-
#ifndef _ROPE_H_
#define _ROPE_H_

#include "rope_fwd.h"

#include <boost/iterator/iterator_facade.hpp>
#include <boost/iterator/reverse_iterator.hpp>

#include <cstring>
#include <iosfwd>
#include <string>


class rope
{
public:
#ifdef ROPE_64_BIT
    const static std::size_t c_max_depth = 91;
    BOOST_STATIC_ASSERT(sizeof(std::size_t) == 8);
#else
    const static std::size_t c_max_depth = 45;
#endif

    class const_iterator :
        public boost::iterator_facade<
            const_iterator,
            const char,
            boost::random_access_traversal_tag
        >
    {
    public:
        const_iterator();
        const_iterator(rope_detail::rope_node* head, std::size_t position);
        const_iterator(const const_iterator& rhs);
        const_iterator& operator=(const const_iterator& rhs);

    private:
        bool equal(const const_iterator& it) const;
        std::ptrdiff_t distance_to(const const_iterator& it) const;
        const char& dereference() const;
        void increment();
        void decrement();
        void advance(std::ptrdiff_t n);
        void increment(std::size_t n);
        void decrement(std::size_t n);
        void copy_internal_buffer(const const_iterator& rhs);

        static const std::size_t c_path_cache_size = 4;
        static const std::size_t c_internal_buffer_size = 15;

        std::size_t m_rope_position;
        rope_detail::rope_node* m_head;
        std::size_t m_current_node_start;
        const char* m_buffer_first;
        const char* m_buffer_it;
        const char* m_buffer_last;

        rope_detail::rope_node* m_path_cache[c_path_cache_size];
        int m_last_valid_index_in_path_cache;
        unsigned char m_right_turns_in_path;
        char m_internal_buffer[c_internal_buffer_size];

        static void fill_buffer(const_iterator& it);
        static void fill_path_and_buffer(const_iterator& it);
        static void fill_path_and_buffer_for_increment(const_iterator& it);

        friend class boost::iterator_core_access;
    };

    typedef boost::reverse_iterator<const_iterator> const_reverse_iterator;

    rope();
    explicit rope(const char* c_str);
    explicit rope(const std::string& str);
    rope(std::size_t size, char c);
    //rope(const rope& rhs); // = default;
    rope& operator=(const char* c_str);
    rope& operator=(const std::string& str);
    //rope& operator=(const rope& rhs); // = default;

    bool empty() const;
    std::size_t size() const;

    const_iterator begin() const;
    const_iterator end() const;
    const_reverse_iterator rbegin() const;
    const_reverse_iterator rend() const;

    char operator[](std::size_t i) const;

    rope substr(std::size_t i, std::size_t size) const;

#ifndef NDEBUG
    void dump(std::ostream& os) const;
#endif

    void clear();

    void push_back(char c);

    void append(const rope& r);
    void append(char c);
    void append(const char* c_str);
    void append(const char* c_str, std::size_t count);
    void append(const std::string& str);
    void append(const std::string& str, std::size_t offset, std::size_t count);

    rope& operator+=(const rope& r);
    rope& operator+=(char c);
    rope& operator+=(const char* c_str);
    rope& operator+=(const std::string& str);

    void rebalance();

    void swap(rope& rhs);

    static const std::size_t npos = -1;

private:
    void concatenate_forest(rope_detail::rope_node_ptr forest[],
                            std::size_t total_size);
    void prepend(rope_detail::rope_node_ptr& n);

    rope_detail::rope_node_ptr m_head;
};


void swap(rope& lhs, rope& rhs);


// rope - rope comparisons
int compare(const rope& lhs, const rope& rhs);

inline bool operator==(const rope& lhs, const rope& rhs)
{ return !compare(lhs, rhs); }

inline bool operator!=(const rope& lhs, const rope& rhs)
{ return compare(lhs, rhs); }

inline bool operator<(const rope& lhs, const rope& rhs)
{ return compare(lhs, rhs) < 0; }

inline bool operator>(const rope& lhs, const rope& rhs)
{ return 0 < compare(lhs, rhs); }

inline bool operator<=(const rope& lhs, const rope& rhs)
{ return compare(lhs, rhs) <= 0; }

inline bool operator>=(const rope& lhs, const rope& rhs)
{ return 0 <= compare(lhs, rhs); }


// rope - c-string comparisons
int compare(const rope& lhs, const char* rhs);

inline bool operator==(const rope& lhs, const char* rhs)
{ return !compare(lhs, rhs); }

inline bool operator!=(const rope& lhs, const char* rhs)
{ return compare(lhs, rhs); }

inline bool operator<(const rope& lhs, const char* rhs)
{ return compare(lhs, rhs) < 0; }

inline bool operator>(const rope& lhs, const char* rhs)
{ return 0 < compare(lhs, rhs); }

inline bool operator<=(const rope& lhs, const char* rhs)
{ return compare(lhs, rhs) <= 0; }

inline bool operator>=(const rope& lhs, const char* rhs)
{ return 0 <= compare(lhs, rhs); }


// c-string - rope comparisons
inline int compare(const char* lhs, const rope& rhs)
{ return -compare(rhs, lhs); }

inline bool operator==(const char* lhs, const rope& rhs)
{ return !compare(lhs, rhs); }

inline bool operator!=(const char* lhs, const rope& rhs)
{ return compare(lhs, rhs); }

inline bool operator<(const char* lhs, const rope& rhs)
{ return compare(lhs, rhs) < 0; }

inline bool operator>(const char* lhs, const rope& rhs)
{ return 0 < compare(lhs, rhs); }

inline bool operator<=(const char* lhs, const rope& rhs)
{ return compare(lhs, rhs) <= 0; }

inline bool operator>=(const char* lhs, const rope& rhs)
{ return 0 <= compare(lhs, rhs); }


// rope - std::string comparisons
int compare(const rope& lhs, const std::string& rhs);

inline bool operator==(const rope& lhs, const std::string& rhs)
{ return !compare(lhs, rhs); }

inline bool operator!=(const rope& lhs, const std::string& rhs)
{ return compare(lhs, rhs); }

inline bool operator<(const rope& lhs, const std::string& rhs)
{ return compare(lhs, rhs) < 0; }

inline bool operator>(const rope& lhs, const std::string& rhs)
{ return 0 < compare(lhs, rhs); }

inline bool operator<=(const rope& lhs, const std::string& rhs)
{ return compare(lhs, rhs) <= 0; }

inline bool operator>=(const rope& lhs, const std::string& rhs)
{ return 0 <= compare(lhs, rhs); }


// std::string - rope comparisons
inline int compare(const std::string& lhs, const rope& rhs)
{ return -compare(rhs, lhs); }

inline bool operator==(const std::string& lhs, const rope& rhs)
{ return !compare(lhs, rhs); }

inline bool operator!=(const std::string& lhs, const rope& rhs)
{ return compare(lhs, rhs); }

inline bool operator<(const std::string& lhs, const rope& rhs)
{ return compare(lhs, rhs) < 0; }

inline bool operator>(const std::string& lhs, const rope& rhs)
{ return 0 < compare(lhs, rhs); }

inline bool operator<=(const std::string& lhs, const rope& rhs)
{ return compare(lhs, rhs) <= 0; }

inline bool operator>=(const std::string& lhs, const rope& rhs)
{ return 0 <= compare(lhs, rhs); }


// operator+
inline rope operator+(const rope& lhs, const rope& rhs)
{
    rope retval(lhs);
    retval.append(rhs);
    return retval;
}

inline rope operator+(const rope& r, char c)
{
    rope retval(r);
    retval.append(c);
    return retval;
}

inline rope operator+(char c, const rope& r)
{
    rope retval;
    retval.append(c);
    retval.append(r);
    return retval;
}

inline rope operator+(const rope& r, const char* c_str)
{
    rope retval(r);
    retval.append(c_str);
    return retval;
}

inline rope operator+(const char* c_str, const rope& r)
{
    rope retval(c_str);
    retval.append(r);
    return retval;
}

inline rope operator+(const rope& r, const std::string& str)
{
    rope retval(r);
    retval.append(str);
    return retval;
}

inline rope operator+(const std::string& str, const rope& r)
{
    rope retval(str);
    retval.append(r);
    return retval;
}


// text insertion
rope insert(const rope& r, std::size_t i, const rope& other_rope);
rope insert(const rope& r, std::size_t i, const char* c_str);
rope insert(const rope& r, std::size_t i, const char* c_str, std::size_t count);
rope insert(const rope& r, std::size_t i, const std::string& str);
rope insert(const rope& r, std::size_t i, const std::string& str, std::size_t offset, std::size_t count);


// text erasure
rope erase(const rope& r, std::size_t i, std::size_t count);


// text replacement
rope replace(const rope& r, std::size_t i, std::size_t count, const rope& other_rope);
rope replace(const rope& r, std::size_t i, std::size_t count, const char* c_str);
rope replace(const rope& r, std::size_t i, std::size_t count, const char* c_str, std::size_t c_str_count);
rope replace(const rope& r, std::size_t i, std::size_t count, const std::string& str);
rope replace(const rope& r, std::size_t i, std::size_t count,
             const std::string& str, std::size_t offset, std::size_t str_count);


std::ostream& operator<<(std::ostream& os, const rope& r);

#include "rope.ipp"

#endif // _ROPE_H_
