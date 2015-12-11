#pragma once
// Types for argument dispatch

namespace lai
{
    namespace details
    {
        struct Nil
        {
            // empty struct for unused argument
        };

        struct TrueType
        {
            // empty struct for dispatch
        };

        struct FalseType
        {
            // empty struct for dispatch
        };

        template<bool, typename T1, typename T2>
        struct Select
        {
            // false, select T2
            using type = T2;
        };

        template<typename T1, typename T2>
        struct Select<true, T1, T2>
        {
            // true, select T1
            using type = T1;
        };

        // Select T1 if pred else T2
        template<bool pred, typename T1, typename T2>
        using SelectType = typename Select<pred, T1, T2>::type;
    }
}