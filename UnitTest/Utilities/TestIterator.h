#pragma once
// Test utilities for iterator

#include <iterator>

template<typename IteratorCategoryTag>
struct IteratorCategoryTraitsAux
{
    constexpr static bool isInput = false;
    constexpr static bool isOutput = false;
    constexpr static bool isForward = false;
    constexpr static bool isBidirectional = false;
    constexpr static bool isRandomAccess = false;
};


template<>
struct IteratorCategoryTraitsAux<std::input_iterator_tag>
{
    constexpr static bool isInput = true;
    constexpr static bool isOutput = false;
    constexpr static bool isForward = false;
    constexpr static bool isBidirectional = false;
    constexpr static bool isRandomAccess = false;
};


template<>
struct IteratorCategoryTraitsAux<std::output_iterator_tag>
{
    constexpr static bool isInput = false;
    constexpr static bool isOutput = true;
    constexpr static bool isForward = false;
    constexpr static bool isBidirectional = false;
    constexpr static bool isRandomAccess = false;
};


template<>
struct IteratorCategoryTraitsAux<std::forward_iterator_tag>
{
    constexpr static bool isInput = false;
    constexpr static bool isOutput = false;
    constexpr static bool isForward = true;
    constexpr static bool isBidirectional = false;
    constexpr static bool isRandomAccess = false;
};


template<>
struct IteratorCategoryTraitsAux<std::bidirectional_iterator_tag>
{
    constexpr static bool isInput = false;
    constexpr static bool isOutput = false;
    constexpr static bool isForward = false;
    constexpr static bool isBidirectional = true;
    constexpr static bool isRandomAccess = false;
};


template<>
struct IteratorCategoryTraitsAux<std::random_access_iterator_tag>
{
    constexpr static bool isInput = false;
    constexpr static bool isOutput = false;
    constexpr static bool isForward = false;
    constexpr static bool isBidirectional = false;
    constexpr static bool isRandomAccess = true;
};


template<typename Iterator>
struct IteratorCategoryTraits :
    public IteratorCategoryTraitsAux<typename std::iterator_traits<Iterator>::iterator_category>
{
    // empty
};