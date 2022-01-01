#include "SolByteArrayWrapper.h"

SolByteArrayWrapper::SolByteArrayWrapper(QByteArray& byteArray)
    : m_byte_array(&byteArray) { }

auto SolByteArrayWrapper::begin() -> iterator
{
    return reinterpret_cast<iterator>(m_byte_array->begin());
}

auto SolByteArrayWrapper::begin() const -> const_iterator
{
    return reinterpret_cast<const_iterator>(m_byte_array->begin());
}

auto SolByteArrayWrapper::end() -> iterator
{
    return reinterpret_cast<iterator>(m_byte_array->end());
}

auto SolByteArrayWrapper::end() const -> const_iterator
{
    return reinterpret_cast<const_iterator>(m_byte_array->end());
}

void SolByteArrayWrapper::clear() noexcept
{
    m_byte_array->clear();
}

void SolByteArrayWrapper::push_back(value_type c)
{
    m_byte_array->push_back(c);
}

void SolByteArrayWrapper::push_back(const_pointer c)
{
    m_byte_array->push_back(reinterpret_cast<QByteArray::const_pointer>(c));
}

void SolByteArrayWrapper::push_back(const_reference a)
{
    m_byte_array->push_back(a);
}

void SolByteArrayWrapper::push_front(value_type c)
{
    m_byte_array->push_front(c);
}

void SolByteArrayWrapper::push_front(const_pointer c)
{
    m_byte_array->push_front(reinterpret_cast<QByteArray::const_pointer>(c));
}

void SolByteArrayWrapper::push_front(const_reference a)
{
    m_byte_array->push_back(a);
}

auto SolByteArrayWrapper::insert(iterator pos, reference value) -> iterator
{
    difference_type diff = pos - reinterpret_cast<pointer>(m_byte_array->data());
    m_byte_array->insert(diff, value);

    return pos;
}

auto SolByteArrayWrapper::insert(const_iterator pos, rvalue_reference value) -> iterator
{
    difference_type diff = pos - reinterpret_cast<pointer>(m_byte_array->data());
    m_byte_array->insert(diff, value);

    return iterator(pos);
}

auto SolByteArrayWrapper::insert(const_iterator pos, size_type count, reference value) -> iterator
{
    difference_type diff = pos - reinterpret_cast<pointer>(m_byte_array->data());
    m_byte_array->insert(diff, count, value);

    return iterator(pos);
}

auto SolByteArrayWrapper::at(int i) const -> value_type
{
    return static_cast<value_type>(m_byte_array->at(i));
}
