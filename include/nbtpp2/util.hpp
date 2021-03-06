#ifndef NBTPP2_UTIL_HPP
#define NBTPP2_UTIL_HPP

#include <nbtpp2/converters.hpp>
#include <nbtpp2/endianness.hpp>
#include <nbtpp2/tag_type.hpp>
#include <nbtpp2/tag.hpp>
#include <nbtpp2/io.hpp>

#include <cstdint>
#include <ostream>
#include <istream>
#include <string>

namespace nbtpp2
{

/**
 * @brief Reverse an unsigned integer
 * @tparam UintT Type of the unsigned integer
 * @param value Value of the unsigned integer
 * @return Reversed unsigned integer
 */
template<typename UintT>
auto reverse_uint(UintT value)
{
    UintT result = 0;
    for (std::size_t i = 0; i < sizeof(value); ++i) {
        result |= ((value >> (i * 8)) & 0xFF) << ((sizeof(value) - i) * 8 - 8);
    }
    return result;
}

/**
 * @brief Reverse an unsigned integer if the system endianness does not equal the target endianness
 * @tparam UintT Type of the unsigned integer
 * @param value Value of the unsigned integer
 * @param target_endianness Target endianness
 * @return Reversed unsigned integer
 */
template<typename UintT>
auto optional_reverse_uint(UintT value, Endianness target_endianness)
{
    return target_endianness != SYSTEM_ENDIANNESS ? reverse_uint(value) : value;
}

/**
 * @brief Write a number to a BinaryWriter
 * @tparam NumberT (signed) input number type
 * @tparam NumberTUnsigned Unsigned version of NumberT (can also be the same if NumberT is already unsigned)
 * @param value Value of the number in NumberT
 * @param out BinaryWriter to write to
 * @param endianness Endianness to write the number in
 */
template<typename NumberT, typename NumberTUnsigned>
void write_number(NumberT value, BinaryWriter &writer, Endianness endianness)
{
    static_assert(sizeof(NumberT) == sizeof(NumberTUnsigned), "NumberT and NumberTUnsigned must have the same size");

    writer.write(
        ConvertToChars<NumberTUnsigned>{
            optional_reverse_uint(
                Convert<NumberT, NumberTUnsigned>{
                    value
                }.b,
                endianness
            )
        }.chars,
        sizeof(NumberTUnsigned)
    );
}

/**
 * @brief Write a string to a BinaryWriter
 * @param str String to write
 * @param out BinaryWriter to write to
 * @param endianness Endianness to write the string's length in
 */
void write_string(const std::string &str, BinaryWriter &writer, Endianness endianness);

/**
 * @brief Read a number from a BinaryReader
 * @tparam NumberT (signed) number type
 * @tparam NumberTUnsigned Unsigned version of NumberT (can also be the same if NumberT is already unsigned)
 * @param in BinaryReader to read from
 * @param endianness Endianness to read the number in
 * @return Read number
 */
template<typename NumberT, typename NumberTUnsigned>
auto read_number(BinaryReader &reader, Endianness endianness)
{
    static_assert(sizeof(NumberT) == sizeof(NumberTUnsigned), "NumberT and NumberTUnsigned must have the same size");

    auto converter = ConvertToChars<NumberTUnsigned>{0};
    reader.read(converter.chars, sizeof(NumberTUnsigned));
    auto result = Convert<NumberTUnsigned, NumberT>{
        optional_reverse_uint(
            converter.int_type,
            endianness
        )
    };

    return result.b;
}

/**
 * @brief Read a string from a BinaryReader
 * @param in BinaryReader to read from
 * @param endianness Endianness to read the string's length in
 * @return Read string
 */
std::string read_string(BinaryReader &reader, Endianness endianness);

/**
 * @brief Write a TagType (tag id) to a BinaryWriter
 * @param type TagType to write (tag id)
 * @param out BinaryWriter to write to
 */
void write_tag_id(TagType type, BinaryWriter &writer);

/**
 * @brief Read a TagType (tag id) from a BinaryReader
 * @param in BinaryReader to read from
 * @return Tag id (TagType)
 */
TagType read_tag_id(BinaryReader &reader);

/**
 * @brief Read a tag
 * @param type Tag type (tag id) of the tag to read
 * @param in BinaryReader to read the tag from
 * @param endianness Endianness to read the tag in
 * @return Resulting tag as Tag *
 */
Tag *read_tag(TagType type, BinaryReader &reader, Endianness endianness);

/**
 * @fn tag_type_to_string
 * @brief Converts a TagType to the std::string representation
 * @param tag_type The TagType
 * @return tag_type as std::string
 */
std::string tag_type_to_string(TagType tag_type);

}

#endif //NBTPP2_UTIL_HPP
