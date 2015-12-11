#pragma once

namespace lai
{
    namespace details
    {
        // HashTraits must contain following public members :
        //      typedefs : key_type
        //                 value_type 
        //                 hasher
        //                 key_equal
        //                 allocator_type
        //                 IsMulti (std::true_type or std::false_type)
        //      static const key_type & getKey(const value_type & value);
        template<typename HashTraits>
        class HashTable
        {

        };
    }
}