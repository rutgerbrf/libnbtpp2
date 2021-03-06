#ifndef NBTPP2_NUMBER_TAG_HPP
#define NBTPP2_NUMBER_TAG_HPP

#include <nbtpp2/tag.hpp>
#include <nbtpp2/util.hpp>

#include <iostream>

namespace nbtpp2
{

template<typename NumberT, typename NumberTUnsigned>
class NumberTag: public Tag
{
    static_assert(sizeof(NumberT) == sizeof(NumberTUnsigned), "NumberT and NumberTUnsigned must have the same size");
public:
    NumberT value = 0;

    /**
     * @param value Value
     * @param type Tag type as TagType (used for identify())
     */
    explicit NumberTag(NumberT value, TagType type)
        : Tag{type}, value{value}
    {}

    /**
     * @brief Write NumberTag
     * @param writer BinaryWriter to write to
     * @param endianness Endianness to write NumberTag in
     */
    void write(BinaryWriter &writer, Endianness endianness) override
    {
        write_number<NumberT, NumberTUnsigned>(value, writer, endianness);
    }

    /**
     * @brief Read tag of type @p ResultT (extending NumberTag)
     * @tparam ResultT Result tag class type
     * @param reader BinaryReader to read from
     * @param endianness Endianness to read the NumberTag in
     * @return The resulting NumberTag as @p ResultT
     */
    template<typename ResultT>
    static auto read(BinaryReader &reader, Endianness endianness)
    {
        return new ResultT{read_number<NumberT, NumberTUnsigned>(reader, endianness)};
    }
};

}

#endif //NBTPP2_NUMBER_TAG_HPP
