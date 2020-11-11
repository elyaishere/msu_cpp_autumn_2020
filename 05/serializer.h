#pragma once
#include <ostream>
#include <cstdint>

enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer
{
    static constexpr char Separator = ' ';
public:
    explicit Serializer(std::ostream& out)
        : out_(out) {}

    template <class T>
    Error save(T& object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&&... args)
    {
        return process(std::forward<ArgsT>(args)...);
    }
    
private:
    std::ostream& out_;

    template <class T>
    Error process(T) {
        return Error::CorruptedArchive;
    }
    Error process(bool);
    Error process(uint64_t);

    template <class T, class... Args>
    Error process(T&& val, Args&&... args) {
        const Error err = process(val);
        switch (err) {
            case Error::NoError:
                out_ << Separator;
                return process(std::forward<Args>(args)...);
            default:
                return err;
        }
    }
};

class Deserializer
{
    static constexpr char Separator = ' ';
public:
    explicit Deserializer(std::istream& in)
        : in_(in) {}

    template <class T>
    Error load(T& object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&&... args)
    {
        return process(std::forward<ArgsT>(args)...);
    }
    
private:
    std::istream& in_;
    
    template <class T>
    Error process(T) {
        return Error::CorruptedArchive;
    }
    Error process(bool&);
    Error process(uint64_t&);

    template <class T, class... Args>
    Error process(T&& val, Args&&... args) {
        const Error err = process(val);
        switch (err) {
            case Error::NoError:
                return process(std::forward<Args>(args)...);
            default:
                return err;
        }
    }
};
