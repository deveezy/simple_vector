#pragma once

#include <stddef.h>
#include <assert.h>
#include <utility>

template <typename Vector>
class VectorIterator
{
public:
	using ValueType = typename Vector::ValueType;
	using PointerType = ValueType *;
	using ReferenceType = ValueType &;

public:
	VectorIterator( PointerType ptr_ )
		: ptr( ptr_ )
	{}

	VectorIterator &operator++()
	{
		++ptr;
		return *this;
	}

	VectorIterator &operator++( int )
	{
		VectorIterator iterator = *this;
		++( *this );
		return iterator;
	}

	VectorIterator &operator--()
	{
		--ptr;
		return *this;
	}

	VectorIterator &operator--( int )
	{
		VectorIterator iterator = *this;
		--( *this );
		return iterator;
	}

	ReferenceType operator[]( int index )
	{
		return *( ptr + index );
	}

	PointerType operator->()
	{
		return ptr;
	}

	ReferenceType operator*()
	{
		return *ptr;
	}

	bool operator==( const VectorIterator &other ) const
	{
		return ptr == other.ptr;
	}

	bool operator!=( const VectorIterator &other ) const
	{
		return !( *this == other );
	}

private:
	PointerType ptr;
};

template <typename T>
class Vector
{
public:
	using ValueType = T;
	using Iterator = VectorIterator<Vector<T>>;

public:
	Vector()
	{
		Realloc( 20 );
	}

	~Vector()
	{
		Clear();
		::operator delete( data, capacity * sizeof( T ) );
		data = nullptr;
	}

public:
	void PushBack( const T &value )
	{
		if (size >= capacity)
			Realloc( capacity + ( capacity / 2 ) );
		data[size++] = value;
	}

	void PushBack( const T &&value )
	{
		if (size >= capacity)
			Realloc( capacity + ( capacity / 2 ) );
		data[size++] = std::move( value );
	}

	template <typename... Args>
	T &EmplaceBack( Args &&... args )
	{
		if (size >= capacity)
			Realloc( capacity + ( capacity / 2 ) );
		new ( &data[size] ) T( std::forward<Args>( args )... );
		return data[size++];
	}

	void PopBack()
	{
		if (size > 0)
		{
			--size;
			data[size].~T();
		}
	}

	void Clear()
	{
		for (size_t i = 0; i < size; ++i)
			data[i].~T();
		size = 0;
	}

	const T &operator[]( size_t index ) const
	{
		assert( index < size );
		return data[index];
	}

	T &operator[]( size_t index )
	{
		assert( index < size );
		return data[index];
	}

	size_t Size() const { return size; }
	size_t Capacity() const { return capacity; }

	Iterator begin()
	{
		return Iterator( data );
	}

	Iterator end()
	{
		return Iterator( data + size );
	}

private:
	void Realloc( size_t newCapacity )
	{
		T *newBlock = ( T * )::operator new( newCapacity * sizeof( T ) );
		if (newCapacity < size)
			size = newCapacity;

		for (size_t i = 0; i < size; ++i)
		{
			// newBlock[i] = std::move( data[i] );
			new ( &newBlock[i] ) T( std::move( data[i] ) );
		}


		for (size_t i = 0; i < size; ++i)
			data[i].~T(); // if we're using operator delete instead of delete, we need to call dtors explicit.
		::operator delete( data, capacity * sizeof( T ) );
		data = newBlock;
		capacity = newCapacity;
	}
private:
	T *data = nullptr;
	size_t size = 0;
	size_t capacity = 0;
};
