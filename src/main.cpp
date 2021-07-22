#include "Vector.hpp"
#include <iostream>

struct Vec3
{
	float x;
	float y;
	float z;
	int *memoryBlock = nullptr;

	Vec3()
	{
		memoryBlock = new int[5];
	}

	Vec3( float scalar ) : x( scalar ), y( scalar ), z( scalar )
	{
		memoryBlock = new int[5];
	}
	Vec3( float x_, float y_, float z_ ) : x( x_ ), y( y_ ), z( z_ )
	{
		memoryBlock = new int[5];
	}
	Vec3( Vec3 &&other ) : x( other.x ), y( other.y ), z( other.z )
	{
		memoryBlock = other.memoryBlock;
		other.memoryBlock = nullptr;
		std::cout << "Move\n";
	}

	Vec3 &operator=( Vec3 &&other )
	{
		std::cout << "Move\n";
		memoryBlock = other.memoryBlock;
		other.memoryBlock = nullptr;
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	Vec3( const Vec3 &other ) = delete;
	Vec3 &operator=( const Vec3 &other ) = delete;

	~Vec3() { std::cout << "Destroy\n"; delete[] memoryBlock; }
};

int main( int argc, const char **argv )
{
	Vector<std::string> values;
	values.EmplaceBack( "1" );
	values.EmplaceBack( "2" );
	values.EmplaceBack( "3" );
	values.EmplaceBack( "4" );
	values.EmplaceBack( "5" );
	std::cout << "Not using iterators:\n";
	for (int i = 0; i < values.Size(); ++i)
	{
		std::cout << values[i] << '\n';
	}

	std::cout << "Range-based for loop:\n";
	for (std::string value : values)
	{
		std::cout << value << '\n';
	}

	std::cout << "Iterator:\n";

	for (
		Vector<std::string>::Iterator it = values.begin();
		it != values.end();
		++it
		)
	{
		std::cout << *it << '\n';
	}

	return 0;
}