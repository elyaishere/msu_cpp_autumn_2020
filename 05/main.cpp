#include <iostream>
#include "serializer.h"
#include <sstream>
#include <cassert>

struct Data
{
    uint64_t a;
    bool b;
    uint64_t c;

    template <class Serializer>
    Error serialize(Serializer& serializer)
    {
        return serializer(a, b, c);
    }

    template <class Deserializer>
    Error deserialize(Deserializer& deserializer)
    {
        return deserializer(a, b, c);
    }
};

struct ShuffleData
{
    bool a;
    uint64_t b;
    uint64_t c;

    template <class Serializer>
    Error serialize(Serializer& serializer)
    {
        return serializer(a, b, c);
    }

    template <class Deserializer>
    Error deserialize(Deserializer& deserializer)
    {
        return deserializer(a, b, c);
    }
};

struct CorruptedData
{
    uint64_t a;
    bool b;
    float c;

    template <class Serializer>
    Error serialize(Serializer& serializer)
    {
        return serializer(a, b, c);
    }

    template <class Deserializer>
    Error deserialize(Deserializer& deserializer)
    {
        return deserializer(a, b, c);
    }
};


int main()
{
    // Test 1
    {
    Data x { 1, true, 2 };

    std::stringstream stream;

    Serializer serializer(stream);
    assert(serializer.save(x) == Error::NoError);
    Data y { 0, false, 0 };

    Deserializer deserializer(stream);

    assert(deserializer.load(y) == Error::NoError);

    assert(x.a == y.a);
    assert(x.b == y.b);
    assert(x.c == y.c);
    }

    // Test 2
    {
    Data x { 1, true, 2 };

    std::stringstream stream;

    Serializer serializer(stream);
    assert(serializer.save(x) == Error::NoError);
    ShuffleData y { false, 0, 0 };

    Deserializer deserializer(stream);

    assert(deserializer.load(y) == Error::CorruptedArchive);
    }

    // Test 3
    {
    CorruptedData x { 1, true, 2.0 };

    std::stringstream stream;

    Serializer serializer(stream);

    assert(serializer.save(x) == Error::CorruptedArchive);    
    }

    std::cout << "All tests passed" << std::endl;

    return 0;
}