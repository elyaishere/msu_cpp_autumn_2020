#include "serializer.h"
#include <string>

Error Serializer::process(bool val) {
    val ? out_ << "true" : out_ << "false";
    return Error::NoError;
}

Error Serializer::process(uint64_t val) {
    out_ << val;
    return Error::NoError;
}


Error Deserializer::process(bool& val) {
    std::string token;
    in_ >> token;
    if (token == "true") {
        val = true;
    } else if (token == "false") {
        val = false;
    } else {
        return Error::CorruptedArchive;
    }
    return Error::NoError;
}

Error Deserializer::process(uint64_t& val) {
    std::string token;
    in_ >> token;
    try {
        val = std::stoull(token);
    } catch (std::invalid_argument) {
        return Error::CorruptedArchive;
    } catch (std::out_of_range) {
        return Error::CorruptedArchive;
    }
    return Error::NoError;
}
