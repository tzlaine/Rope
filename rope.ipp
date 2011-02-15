// -*- C++ -*-
#include <cstring>
#include <ostream>


namespace rope_detail {

    const std::size_t c_short_string_max = 512;

    inline std::size_t min_size_of_balanced_tree(std::size_t depth)
    {
        // Fibonacci < 2^32
        static const std::size_t c_min_size_of_balanced_tree[rope::c_max_depth + 1] = {
            1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584,
            4181, 6765u, 10946u, 17711u, 28657u, 46368u, 75025u, 121393u,
            196418u, 317811u, 514229u, 832040u, 1346269u, 2178309u, 3524578u,
            5702887u, 9227465u, 14930352u, 24157817u, 39088169u, 63245986u,
            102334155u, 165580141u, 267914296u, 433494437u, 701408733u,
            1134903170u, 1836311903u, 2971215073u
#ifdef ROPE_64_BIT // Fibonacci < 2^64 (implies max depth of 91)
            , 4807526976u, 7778742049u, 12586269025u, 20365011074u, 32951280099u,
            53316291173u, 86267571272u, 139583862445u, 225851433717u, 365435296162u,
            591286729879u, 956722026041u, 1548008755920u, 2504730781961u,
            4052739537881u, 6557470319842u, 10610209857723u, 17167680177565u,
            27777890035288u, 44945570212853u, 72723460248141u, 117669030460994u,
            190392490709135u, 308061521170129u, 498454011879264u, 806515533049393u,
            1304969544928657u, 2111485077978050u, 3416454622906707u,
            5527939700884757u, 8944394323791464u, 14472334024676221u,
            23416728348467685u, 37889062373143906u, 61305790721611591u,
            99194853094755497u, 160500643816367088u, 259695496911122585u,
            420196140727489673u, 679891637638612258u, 1100087778366101931u,
            1779979416004714189u, 2880067194370816120u, 4660046610375530309u,
            7540113804746346429u, 12200160415121876738u
#endif
        };
        return c_min_size_of_balanced_tree[depth];
    }

    struct repeat
    {
        repeat() : m_c() {}
        repeat(char c) : m_c(c) {}
        char operator()(std::size_t) const { return m_c; }
        char m_c;
    };

    struct rope_node
    {
        virtual ~rope_node() {}

        bool is_string() const
            { return m_type_code == string_; }

        bool is_concatenation() const
            { return m_type_code == concatenation_; }

        bool is_function() const
            { return m_type_code == function_; }

        virtual char get_char(std::size_t i) const
            { return '\0'; }

        virtual std::size_t left_size() const
            { return 0; }

        virtual void fill_buffer(const char*& buffer_first,
                                 const char*& buffer_last,
                                 char internal_buffer[],
                                 std::size_t internal_buffer_size,
                                 std::size_t offset) const
            {
                const std::size_t elements =
                    (std::min)(m_size - offset, internal_buffer_size);
                for (std::size_t i = 0; i < elements; ++i) {
                    internal_buffer[i] = get_char(offset + i);
                }
                buffer_first = internal_buffer;
                buffer_last = buffer_first + elements;
            }

#ifndef NDEBUG
        void dump(std::ostream& os, std::size_t indent) const;
#endif

        virtual rope_node_ptr append(const char* c_str, std::size_t c_str_size) = 0;

        virtual rope_node_ptr substr(std::size_t i, std::size_t size) = 0;

        void append_c_str(const char* c_str, std::size_t size);

        enum { string_, concatenation_, function_ };

        // exactly one of: left-size + right-size, string-size, or function-size
        std::size_t m_size;
        std::size_t m_ref_count;

        unsigned char m_depth;
        unsigned char m_type_code;
        bool m_is_substring : 8;

    protected:
        rope_node(std::size_t size,
                  unsigned char depth,
                  unsigned char type_code,
                  bool is_substring) :
            m_size(size),
            m_ref_count(0),
            m_depth(depth),
            m_type_code(type_code),
            m_is_substring(is_substring)
            {}

    private:
        rope_node& operator=(const rope_node& rhs); // = delete;
    };

    inline void intrusive_ptr_add_ref(rope_node* n)
    { ++n->m_ref_count; }

    inline void intrusive_ptr_release(rope_node* n)
    {
        if (!--n->m_ref_count)
            delete n;
    }

    struct string :
        rope_node
    {
        string(const char* c_str) :
            rope_node(0, 0, string_, false),
            m_str()
            {
                if (c_str) {
                    assert(c_str[0]);
                    m_str = c_str;
                    m_size = m_str.size();
                }
            }

        string(const char* c_str, std::size_t size) :
            rope_node(size, 0, string_, false),
            m_str()
            {
                assert(bool(c_str) == bool(m_size));
                if (c_str) {
                    assert(c_str[0]);
                    m_str = c_str;
                }
            }

        string(const string& rhs, const char* c_str, std::size_t c_str_size) :
            rope_node(rhs.m_size + c_str_size, 0, string_, false),
            m_str(rhs.m_str)
            {
                assert(c_str);
                assert(c_str_size);
                m_str.append(c_str, c_str_size);
            }

        virtual char get_char(std::size_t i) const
            { return m_str[i]; }

        virtual void fill_buffer(const char*& buffer_first,
                                 const char*& buffer_last,
                                 char internal_buffer[],
                                 std::size_t,
                                 std::size_t offset) const
            {
                const char* data = m_str.data();
                buffer_first = data + offset;
                buffer_last = data + m_str.size();
            }

        virtual rope_node_ptr append(const char* c_str, std::size_t c_str_size);

        virtual rope_node_ptr substr(std::size_t i, std::size_t size);

        std::string m_str;

    private:
        string(const string& rhs) :
            rope_node(rhs.m_size, 0, string_, false),
            m_str(rhs.m_str)
            {}

        string& operator=(const string& rhs); // = delete;
    };
    typedef boost::intrusive_ptr<string> string_ptr;

    struct concatenation :
        rope_node
    {
        concatenation(const rope_node_ptr& left, const rope_node_ptr& right) :
            rope_node(left->m_size + right->m_size,
                      (std::max)(left->m_depth, right->m_depth) + 1,
                      concatenation_,
                      false),
            m_left(left),
            m_right(right)
            {}

        virtual std::size_t left_size() const
            { return m_left->m_size; }

        virtual rope_node_ptr append(const char* c_str, std::size_t c_str_size)
            {
                rope_node_ptr retval;
                if (m_right->is_string() &&
                    m_right->m_size + c_str_size < c_short_string_max) {
                    if (1u < m_right->m_ref_count) {
                        string_ptr new_right(
                            new string(static_cast<string&>(*m_right), c_str, c_str_size)
                        );
                        retval = new concatenation(m_left, new_right);
                    } else {
                        m_right->append_c_str(c_str, c_str_size);
                        retval = this;
                    }
                } else {
                    rope_node_ptr right(new string(c_str, c_str_size));
                    retval = new concatenation(this, right);
                }
                return retval;
            }

        virtual rope_node_ptr substr(std::size_t i, std::size_t size)
            {
                const std::size_t left_size = m_left->m_size;
                const std::size_t right_size = m_right->m_size;

                if (left_size <= i) {
                    if (size == right_size)
                        return m_right;
                    else
                        return m_right->substr(i - left_size, size);
                } else if (i + size <= left_size) {
                    if (size == left_size)
                        return m_left;
                    else
                        return m_left->substr(i, size);
                } else { // the substring straddles this concatenation node
                    const std::size_t left_substring_size = left_size - i;
                    rope_node_ptr left;
                    rope_node_ptr right;
                    if (i == 0)
                        left = m_left;
                    else
                        left = m_left->substr(i, left_substring_size);
                    if (i + size == m_size)
                        right = m_right;
                    else
                        right = m_right->substr(0, size - left_substring_size);
                    return new concatenation(left, right);
                }
            }

        rope_node_ptr m_left;
        rope_node_ptr m_right;

    private:
        concatenation& operator=(const concatenation& rhs); // = delete;
    };
    typedef boost::intrusive_ptr<concatenation> concatenation_ptr;

    struct function_base :
        rope_node
    {
        virtual rope_node_ptr append(const char* c_str, std::size_t c_str_size)
            {
                rope_node_ptr right(new string(c_str, c_str_size));
                return new concatenation(this, right);
            }

        virtual rope_node_ptr substr(std::size_t i, std::size_t size);

    protected:
        function_base(std::size_t size,
                      unsigned char depth,
                      unsigned char type_code,
                      bool is_substring) :
            rope_node(size, depth, type_code, is_substring)
            {}
    };

    template <typename Functor>
    struct function :
        function_base
    {
        function(const Functor& function, std::size_t size) :
            function_base(size, 0, function_, false),
            m_rope_fn(size ? function : Functor())
            {}

        virtual char get_char(std::size_t i) const
            { return m_rope_fn(i); }

        Functor m_rope_fn;

    private:
        function& operator=(const function& rhs); // = delete;
    };
    typedef boost::intrusive_ptr<function_base> function_ptr;

    struct substring_reference
    {
        substring_reference() :
            m_node(),
            m_offset(0)
            {}
        substring_reference(const string_ptr& str, std::size_t offset) :
            m_node(str),
            m_offset(offset)
            {}
        substring_reference(const function_ptr& f, std::size_t offset) :
            m_node(f),
            m_offset(offset)
            {}
        char operator()(std::size_t i) const
            { return m_node->get_char(m_offset + i); }
        rope_node_ptr m_node;
        std::size_t m_offset;
    };

    template <>
    struct function<substring_reference> :
        function_base
    {
        function(const substring_reference& function, std::size_t size) :
            function_base(size, 0, function_, true),
            m_rope_fn(size ? function : substring_reference())
            {}

        virtual char get_char(std::size_t i) const
            { return m_rope_fn(i); }

        virtual void fill_buffer(const char*& buffer_first,
                                 const char*& buffer_last,
                                 char internal_buffer[],
                                 std::size_t internal_buffer_size,
                                 std::size_t offset) const
            {
                if (m_rope_fn.m_node && m_rope_fn.m_node->is_string()) {
                    string_ptr str =
                        boost::static_pointer_cast<string>(m_rope_fn.m_node);
                    const char* data = str->m_str.data();
                    buffer_first = data + m_rope_fn.m_offset + offset;
                    buffer_last = data + m_rope_fn.m_offset + m_size;
                } else {
                    rope_node::fill_buffer(buffer_first,
                                           buffer_last,
                                           internal_buffer,
                                           internal_buffer_size,
                                           offset);
                }
            }

        substring_reference m_rope_fn;

    private:
        function& operator=(const function& rhs); // = delete;
    };

    inline rope_node_ptr string::append(const char* c_str, std::size_t c_str_size)
    {
        rope_node_ptr retval;
        if (m_size + c_str_size <= c_short_string_max) {
            if (1u < m_ref_count) {
                retval = new string(*this, c_str, c_str_size);
            } else {
                append_c_str(c_str, c_str_size);
                retval = this;
            }
        } else {
            rope_node_ptr right(new string(c_str, c_str_size));
            retval = new concatenation(this, right);
        }
        return retval;
    }

    inline rope_node_ptr string::substr(std::size_t i, std::size_t size)
    { return new function<substring_reference>(substring_reference(this, i), size); }

    inline rope_node_ptr function_base::substr(std::size_t i, std::size_t size)
    {
        if (m_is_substring) {
            function<substring_reference>& substring_ref =
                static_cast<function<substring_reference>&>(*this);
            substring_ref.m_rope_fn.m_offset += i;
            return this;
        } else {
            return new function<substring_reference>(substring_reference(this, i), size);
        }
    }

    inline rope_node_ptr concatenate(rope_node_ptr lhs, rope_node_ptr rhs)
    {
        if (!lhs)
            return rhs;
        else if (!rhs)
            return lhs;
        return new concatenation(lhs, rhs);
    }

    inline bool is_balanced(rope_node_ptr node)
    { return min_size_of_balanced_tree(node->m_depth) <= node->m_size; }

    inline void add_leaf_to_forest(rope_node_ptr node, rope_node_ptr forest[])
    {
        std::size_t i;
        rope_node_ptr insertee;

        {
            rope_node_ptr too_small;

            std::size_t size = node->m_size;
            for (i = 0; min_size_of_balanced_tree(i + 1) <= size; ++i) {
                if (forest[i]) {
                    too_small = concatenate(forest[i], too_small);
                    forest[i] = 0;
                }
            }

            insertee = concatenate(too_small, node);
        }

        assert(insertee->m_depth <= node->m_depth + 1);
        for (;; ++i) {
            if (forest[i]) {
                insertee = concatenate(forest[i], insertee);
                forest[i] = 0;
            }
            assert(min_size_of_balanced_tree(i) <= insertee->m_size);
            assert(!forest[i]);
            if (i == rope::c_max_depth ||
                insertee->m_size < min_size_of_balanced_tree(i + 1)) {
                forest[i] = insertee;
                return;
            }
        }
    }

    inline void add_to_forest(rope_node_ptr node, rope_node_ptr forest[])
    {
        if (is_balanced(node)) {
            add_leaf_to_forest(node, forest);
        } else {
            assert(node->is_concatenation());
            concatenation_ptr concat = boost::static_pointer_cast<concatenation>(node);
            add_to_forest(concat->m_left, forest);
            add_to_forest(concat->m_right, forest);
        }
    }

#ifndef NDEBUG
    inline void rope_node::dump(std::ostream& os, std::size_t indent) const
    {
        os << std::string(2 * indent, ' ');

        if (is_string()) {
            os << static_cast<const string* const>(this)->m_str;
        } else if (is_concatenation()) {
            os << "concatenation (size=" << m_size
               << ", depth=" << static_cast<int>(m_depth)
               << "):\n";
            const concatenation* const concat =
                static_cast<const concatenation* const>(this);
            concat->m_left->dump(os, indent + 1);
            concat->m_right->dump(os, indent + 1);
        } else {
            os << "function (size=" << m_size << "): ";
            std::size_t i = 0;
            const function_base* const fn =
                static_cast<const function_base* const>(this);
            for (; i < 20 && i < m_size; ++i) {
                os << fn->get_char(i);
            }
            if (i < m_size)
                os << "...";
            os << '\n';
        }
    }
#endif

    inline void rope_node::append_c_str(const char* c_str, std::size_t size)
    {
        assert(is_string());
        rope_detail::string& str = static_cast<rope_detail::string&>(*this);
        str.m_str.append(c_str, size);
        str.m_size += size;
    }

}


////////////////////////////////////////////////////////////
// rope::const_iterator
////////////////////////////////////////////////////////////
inline rope::const_iterator::const_iterator()
{}

inline rope::const_iterator::const_iterator(rope_detail::rope_node* head, std::size_t position) :
    m_rope_position(position),
    m_head(head),
    m_buffer_it(0)
{}

inline rope::const_iterator::const_iterator(const const_iterator& rhs) : 
    m_rope_position(rhs.m_rope_position),
    m_head(rhs.m_head),
    m_current_node_start(rhs.m_current_node_start),
    m_buffer_first(rhs.m_buffer_first),
    m_buffer_it(rhs.m_buffer_it),
    m_last_valid_index_in_path_cache(rhs.m_last_valid_index_in_path_cache),
    m_right_turns_in_path(rhs.m_right_turns_in_path)
{
    memcpy(m_path_cache, rhs.m_path_cache, sizeof(m_path_cache));
    if (rhs.m_buffer_it)
        copy_internal_buffer(rhs);
}

inline rope::const_iterator& rope::const_iterator::operator=(const const_iterator& rhs)
{
    m_rope_position = rhs.m_rope_position;
    m_head = rhs.m_head;
    m_buffer_first = rhs.m_buffer_first;
    m_buffer_it = rhs.m_buffer_it;
    memcpy(m_path_cache, rhs.m_path_cache, sizeof(m_path_cache));
    m_last_valid_index_in_path_cache = rhs.m_last_valid_index_in_path_cache;
    m_right_turns_in_path = rhs.m_right_turns_in_path;
    m_current_node_start = rhs.m_current_node_start;
    if (rhs.m_buffer_it)
        copy_internal_buffer(rhs);
    return *this;
}

inline bool rope::const_iterator::equal(const const_iterator& it) const
{ return m_rope_position == it.m_rope_position && m_head == it.m_head; }

inline std::ptrdiff_t rope::const_iterator::distance_to(const const_iterator& it) const
{ return it.m_rope_position - m_rope_position; }

inline const char& rope::const_iterator::dereference() const
{
    if (!m_buffer_it)
        fill_path_and_buffer(const_cast<const_iterator&>(*this));
    return *m_buffer_it;
}

inline void rope::const_iterator::increment()
{
    if (m_buffer_it) {
        const char *next = m_buffer_it + 1;
        if (next < m_buffer_last) {
            m_buffer_it = next;
            ++m_rope_position;
            return;
        }
    }
    increment(1);
}

inline void rope::const_iterator::decrement()
{ decrement(1); }

inline void rope::const_iterator::advance(std::ptrdiff_t n)
{
    if (0 <= n)
        increment(n);
    else
        decrement(-n);
}

inline void rope::const_iterator::increment(std::size_t n)
{
    m_rope_position += n;
    if (m_buffer_it) {
        std::size_t chars_left = m_buffer_last - m_buffer_it;
        if (n < chars_left) {
            m_buffer_it += n;
        } else if (chars_left == n) {
            m_buffer_it += n;
            fill_path_and_buffer_for_increment(*this);
        } else {
            m_buffer_it = 0;
        }
    }
}

inline void rope::const_iterator::decrement(std::size_t n)
{
    if (m_buffer_it) {
        std::size_t chars_left = m_buffer_it - m_buffer_first;
        if (n <= chars_left)
            m_buffer_it -= n;
        else
            m_buffer_it = 0;
    }
    m_rope_position -= n;
}

inline void rope::const_iterator::copy_internal_buffer(const const_iterator& rhs)
{
    if (rhs.m_buffer_first == rhs.m_internal_buffer) {
        m_buffer_first = m_internal_buffer;
        m_buffer_last = m_buffer_first + (rhs.m_buffer_last - rhs.m_buffer_first);
        m_buffer_it = m_buffer_first + (rhs.m_buffer_it - rhs.m_buffer_first);
        memcpy(m_internal_buffer, rhs.m_internal_buffer, sizeof(m_internal_buffer));
    } else {
        m_buffer_last = rhs.m_buffer_last;
    }
}

inline void rope::const_iterator::fill_buffer(const_iterator& it)
{
    rope_detail::rope_node* current_node =
        it.m_path_cache[it.m_last_valid_index_in_path_cache];

    current_node->fill_buffer(it.m_buffer_first,
                              it.m_buffer_last,
                              it.m_internal_buffer,
                              c_internal_buffer_size,
                              it.m_rope_position - it.m_current_node_start);
    it.m_buffer_it = it.m_buffer_first;
}

inline void rope::const_iterator::fill_path_and_buffer(const_iterator& it)
{
    rope_detail::rope_node* path[rope::c_max_depth + 1];
    rope_detail::rope_node* current_node = it.m_head;
    int current_depth = -1;
    std::size_t current_start_pos = 0;
    std::size_t pos = it.m_rope_position;
    unsigned char right_turns_in_path = 0;

    if (it.m_head->m_size <= pos) {
        it.m_buffer_it = 0;
        return;
    }

    bool done = false;
    while (!done) {
        ++current_depth;
        path[current_depth] = current_node;
        if (current_node->is_concatenation()) {
            rope_detail::concatenation* concat =
                static_cast<rope_detail::concatenation*>(current_node);
            rope_detail::rope_node* left = concat->m_left.get();
            std::size_t left_size = left->m_size;

            right_turns_in_path <<= 1;
            if (current_start_pos + left_size <= pos) {
                right_turns_in_path |= 1;
                current_node = concat->m_right.get();
                current_start_pos += left_size;
            } else {
                current_node = left;
            }
        } else {
            it.m_current_node_start = current_start_pos;
            done = true;
        }
    }

    int i = -1;
    int j = (std::max<int>)(0, current_depth + 1 - c_path_cache_size);

    while (j <= current_depth) {
        it.m_path_cache[++i] = path[j++];
    }

    it.m_last_valid_index_in_path_cache = i;

    it.m_right_turns_in_path = right_turns_in_path;

    fill_buffer(it);
}

inline void rope::const_iterator::fill_path_and_buffer_for_increment(const_iterator& it)
{
    int current_index = it.m_last_valid_index_in_path_cache;
    rope_detail::rope_node* current_node = it.m_path_cache[current_index];
    std::size_t size = current_node->m_size;
    std::size_t node_start_pos = it.m_current_node_start;
    unsigned char right_turns_in_path = it.m_right_turns_in_path;

    if (it.m_rope_position - node_start_pos < size) {
        fill_buffer(it);
        return;
    }

    // node_start_pos is starting position of last_node.
    while (0 <= --current_index) {
        if (!(right_turns_in_path & 1))
            break;

        current_node = it.m_path_cache[current_index];
        rope_detail::concatenation* concat =
            static_cast<rope_detail::concatenation*>(current_node);
        // Otherwise we were in the right child.  Thus we should pop the
        // concatenation node.
        node_start_pos -= concat->m_left->m_size;
        right_turns_in_path >>= 1;
    }

    if (current_index < 0) {
        fill_path_and_buffer(it);
        return;
    }

    current_node = it.m_path_cache[current_index];
    rope_detail::concatenation* concat =
        static_cast<rope_detail::concatenation*>(current_node);
    // current_node is a concatenation node.  We are positioned on the first
    // character in its right child.  node_start_pos is starting position of
    // current_node.
    node_start_pos += concat->m_left->m_size;
    current_node = concat->m_right.get();
    it.m_path_cache[++current_index] = current_node;
    right_turns_in_path |= 1;

    while (current_node->is_concatenation()) {
        ++current_index;
        if (static_cast<int>(c_path_cache_size) == current_index) {
            for (std::size_t i = 0; i < c_path_cache_size - 1; ++i) {
                it.m_path_cache[i] = it.m_path_cache[i + 1];
            }
            --current_index;
        }
        current_node =
            static_cast<rope_detail::concatenation*>(current_node)->m_left.get();
        it.m_path_cache[current_index] = current_node;
        right_turns_in_path <<= 1;
    }

    it.m_last_valid_index_in_path_cache = current_index;
    it.m_current_node_start = node_start_pos;
    it.m_right_turns_in_path = right_turns_in_path;

    fill_buffer(it);
}


////////////////////////////////////////////////////////////
// rope
////////////////////////////////////////////////////////////
inline rope::rope() :
    m_head(0)
{}

inline rope::rope(const char* c_str) :
    m_head(c_str && c_str[0] ? new rope_detail::string(c_str) : 0)
{}

inline rope::rope(const std::string& str) :
    m_head(str.empty() ? 0 : new rope_detail::string(str.data(), str.size()))
{}

inline rope::rope(std::size_t size, char c) :
    m_head(size ?
           new rope_detail::function<rope_detail::repeat>(rope_detail::repeat(c),
                                                          size) :
           0)
{}

inline rope& rope::operator=(const char* c_str)
{
    rope r(c_str);
    swap(r);
    return *this;
}

inline rope& rope::operator=(const std::string& str)
{
    rope r(str);
    swap(r);
    return *this;
}

inline bool rope::empty() const
{ return !m_head; }

inline std::size_t rope::size() const
{ return m_head ? m_head->m_size : 0; }

inline rope::const_iterator rope::begin() const
{ return const_iterator(m_head.get(), 0); }

inline rope::const_iterator rope::end() const
{ return const_iterator(m_head.get(), size()); }

inline rope::const_reverse_iterator rope::rbegin() const
{ return const_reverse_iterator(end()); }

inline rope::const_reverse_iterator rope::rend() const
{ return const_reverse_iterator(begin()); }

inline char rope::operator[](std::size_t i) const
{
    assert(i < size());
    return *const_iterator(m_head.get(), i);
}

inline rope rope::substr(std::size_t i, std::size_t substr_size) const
{
    std::size_t size = this->size();

    assert(substr_size == 0 || i == 0 && substr_size == npos || i < size);
    assert(substr_size == 0 || substr_size == npos || i + substr_size <= size);

    if (substr_size == npos)
        substr_size = size - i;

    rope retval;
    if (!substr_size) {
        return retval;
    } else {
        retval.m_head = m_head->substr(i, substr_size);
        return retval;
    }
}

#ifndef NDEBUG
inline void rope::dump(std::ostream& os) const
{
    if (!m_head)
        os << "[empty]\n";
    else
        m_head->dump(os, 0);
    os << std::endl;
}
#endif

inline void rope::clear()
{ m_head = 0; }

inline void rope::push_back(char c)
{ append(c); }

inline void rope::append(const rope& r)
{
    if (r.empty()) {
        return;
    } else if (empty()) {
        m_head = r.m_head;
        return;
    }

    m_head = new rope_detail::concatenation(m_head, r.m_head);
    if (c_max_depth <= m_head->m_depth)
        rebalance();
}

inline void rope::append(char c)
{
    const char str[] = { c, '\0' };
    append(str);
}

inline void rope::append(const char* c_str)
{
    if (c_str)
        append(c_str, strlen(c_str));
}

inline void rope::append(const char* c_str, std::size_t count)
{
    if (!c_str || !c_str[0]) {
        return;
    } else if (empty()) {
        m_head = new rope_detail::string(c_str);
        return;
    }

    m_head = m_head->append(c_str, count);

    if (c_max_depth <= m_head->m_depth)
        rebalance();
}

inline void rope::append(const std::string& str)
{ append(str.data(), str.size()); }

inline void rope::append(const std::string& str,
                         std::size_t offset,
                         std::size_t count)
{
    assert(!count || offset + count < str.size());
    append(str.data() + offset, count);
}

inline rope& rope::operator+=(const rope& r)
{
    append(r);
    return *this;
}

inline rope& rope::operator+=(char c)
{
    append(c);
    return *this;
}

inline rope& rope::operator+=(const char* c_str)
{
    append(c_str);
    return *this;
}

inline rope& rope::operator+=(const std::string& str)
{
    append(str);
    return *this;
}

inline void rope::rebalance()
{
    if (empty() || m_head->is_string())
        return;

    rope_detail::rope_node_ptr forest[c_max_depth + 1] = {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    };

    rope_detail::add_to_forest(m_head, forest);
    m_head = 0;
    for (std::size_t i = 0; i <= c_max_depth; ++i) {
        if (forest[i])
            prepend(forest[i]);
    }

    assert(m_head->m_depth < c_max_depth);
}

inline void rope::swap(rope& rhs)
{ m_head.swap(rhs.m_head); }

inline void rope::concatenate_forest(rope_detail::rope_node_ptr forest[],
                                     std::size_t total_size)
{
    std::size_t i = 0;
    while (size() != total_size) {
    	if (forest[i])
    	    prepend(forest[i]);
        ++i;
    }
}

inline void rope::prepend(rope_detail::rope_node_ptr& n)
{
    if (empty()) {
        m_head = n;
        return;
    }
    m_head = new rope_detail::concatenation(n, m_head);
}


////////////////////////////////////////////////////////////
// free functions
////////////////////////////////////////////////////////////
inline void swap(rope& lhs, rope& rhs)
{ lhs.swap(rhs); }

inline int compare(const rope& lhs, const rope& rhs)
{
    if (rhs.empty()) {
        return !lhs.empty();
    } else if (lhs.empty()) {
        return -1;
    } else {
        rope::const_iterator l = lhs.begin();
        rope::const_iterator r = rhs.begin();
        const std::size_t l_size = lhs.size();
        const std::size_t r_size = rhs.size();
        const std::size_t n = (std::min)(l_size, r_size);
        for (std::size_t i = 0; i < n; ++i, ++l, ++r) {
            char l_char = *l;
            char r_char = *r;
            if (l_char < r_char)
                return -1;
            else if (r_char < l_char)
                return 1;
        }
        if (l_size == r_size)
            return 0;
        else
            return l_size < r_size ? -1 : 1;
    }
}

namespace rope_detail {

    inline int compare_impl(const rope& lhs, const char* rhs, std::size_t rhs_size)
    {
        assert(rhs);
        if (!rhs[0]) {
            return !lhs.empty();
        } else if (lhs.empty()) {
            return -1;
        } else {
            rope::const_iterator l = lhs.begin();
            const char* r = rhs;
            const std::size_t l_size = lhs.size();
            const std::size_t r_size = rhs_size;
            const std::size_t n = (std::min)(l_size, r_size);
            for (std::size_t i = 0; i < n; ++i, ++l, ++r) {
                char l_char = *l;
                char r_char = *r;
                if (l_char < r_char)
                    return -1;
                else if (r_char < l_char)
                    return 1;
            }
            if (l_size == r_size)
                return 0;
            else
                return l_size < r_size ? -1 : 1;
        }
    }

}

inline int compare(const rope& lhs, const char* rhs)
{ return rope_detail::compare_impl(lhs, rhs, strlen(rhs)); }

inline int compare(const rope& lhs, const std::string& rhs)
{ return rope_detail::compare_impl(lhs, rhs.data(), rhs.size()); }

inline rope insert(const rope& r, std::size_t i, const rope& other_rope)
{
    if (other_rope.empty())
        return r;

    assert(i <= r.size());
    rope retval = r.substr(0, i);
    retval.append(other_rope);
    if (i < r.size())
        retval.append(r.substr(i, rope::npos));
    return retval;
}

inline rope insert(const rope& r, std::size_t i, const char* c_str)
{ return insert(r, i, c_str, strlen(c_str)); }

inline rope insert(const rope& r, std::size_t i, const char* c_str, std::size_t count)
{
    if (!count)
        return r;

    assert(i <= r.size());
    rope retval = r.substr(0, i);
    retval.append(c_str, count);
    if (i < r.size())
        retval.append(r.substr(i, rope::npos));
    return retval;
}

inline rope insert(const rope& r, std::size_t i, const std::string& str)
{ return insert(r, i, str.data(), str.size()); }

inline rope insert(const rope& r,
                   std::size_t i,
                   const std::string& str,
                   std::size_t offset,
                   std::size_t count)
{
    assert(!count || offset + count < str.size());
    return insert(r, i, str.data() + offset, count);
}

inline rope erase(const rope& r, std::size_t i, std::size_t count)
{
    assert(!count || i < r.size());

    if (!count)
        return r;

    std::size_t size = r.size();
    if (count == rope::npos)
        count = size - i;

    rope retval = r.substr(0, i);
    if (i + count < size)
        retval.append(r.substr(i + count, rope::npos));

    return retval;
}

inline rope replace(const rope& r,
                    std::size_t i,
                    std::size_t count,
                    const rope& other_rope)
{
    assert(!count || i < r.size());

    std::size_t size = r.size();
    if (count == rope::npos)
        count = size - i;

    rope retval = r.substr(0, i);
    retval.append(other_rope);
    if (i + count < size)
        retval.append(r.substr(i + count, rope::npos));

    return retval;
}

inline rope replace(const rope& r, std::size_t i, std::size_t count, const char* c_str)
{ return replace(r, i, count, c_str, strlen(c_str)); }

inline rope replace(const rope& r,
                    std::size_t i,
                    std::size_t count,
                    const char* c_str,
                    std::size_t c_str_count)
{
    assert(!count || i < r.size());

    std::size_t size = r.size();
    if (count == rope::npos)
        count = size - i;

    rope retval = r.substr(0, i);
    retval.append(c_str, c_str_count);
    if (i + count < size)
        retval.append(r.substr(i + count, rope::npos));

    return retval;
}

inline rope replace(const rope& r, std::size_t i, std::size_t count, const std::string& str)
{ return replace(r, i, count, str.data(), str.size()); }

inline rope replace(const rope& r, std::size_t i, std::size_t count,
             const std::string& str, std::size_t offset, std::size_t str_count)
{ return replace(r, i, count, str.data() + offset, str_count); }

inline std::ostream& operator<<(std::ostream& os, const rope& r)
{
    rope::const_iterator it = r.begin();
    rope::const_iterator end_it = r.end();
    while (it != end_it) {
        os << *it;
        ++it;
    }
    return os;
}
