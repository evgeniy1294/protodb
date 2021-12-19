#pragma once
#include <QByteArray>

class SolByteArrayWrapper {
public:
    // QByteArray has char value type, but we can't use it in sol
    // We use qt-like declarations
    using value_type = unsigned char;
    using pointer    = unsigned char*;
    using const_pointer = const unsigned char*;
    using reference  = unsigned char&;
    using rvalue_reference  = unsigned char&&;
    using const_reference = const unsigned char&;
    using iterator = unsigned char*;
    using const_iterator = const unsigned char*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using size_type = QByteArray::size_type;
    using difference_type = QByteArray::difference_type;

public:
    SolByteArrayWrapper(QByteArray& byteArray);

    iterator begin();
    const_iterator begin() const;

    iterator end();
    const_iterator end() const;

    void clear() noexcept;

    inline void push_back(value_type c);
    inline void push_back(const_pointer c);
    inline void push_back(const_reference a);
    inline void push_front(value_type c);
    inline void push_front(const_pointer c);
    inline void push_front(const_reference a);

    iterator insert(iterator pos, reference value);
    iterator insert(const_iterator pos, rvalue_reference value);
    iterator insert(const_iterator pos, size_type count, reference value);

    inline value_type at(int i) const;


private:
    QByteArray* m_byte_array;
};
