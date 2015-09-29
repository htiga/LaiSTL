#pragma once
#include "stdafx.h"
#include "../Utilities/Uncopyable.h"
#include <string>
// TSC means "Test sequence container"

template<typename Container>
void TSC_DefaultConstructorAux()
{
    Container c;
    IS_TRUE(c.empty());
    IS_TRUE(c.size() == 0);
    IS_TRUE(c.begin() == c.end());
    IS_TRUE(c.cbegin() == c.cend());
}


#define TSC_DefaultConstructor(ContainerTemplate) \
do { \
    TSC_DefaultConstructorAux<ContainerTemplate<int>>(); \
    TSC_DefaultConstructorAux<ContainerTemplate<std::string>>(); \
    TSC_DefaultConstructorAux<ContainerTemplate<Uncopyable>>(); \
} while (false)


template<typename Container, typename StdContainer>
void TSC_CountConstructorAux()
{
    I_IL cases = { 0, 1, 10, 20 };
    for (auto & i : cases)
    {
        AssertContainerEqual(Container(i), StdContainer(i));
    }
}


template<typename Container, typename StdContainer, typename ValueType>
void TSC_CountConstructorAux(const ValueType & val)
{
    I_IL cases = { 0, 1, 10, 20 };
    for (auto & i : cases)
    {
        AssertContainerEqual(Container(i, val), StdContainer(i, val));
    }
}


#define TSC_CountConstructor(ContainerTemplate) \
do { \
    TSC_CountConstructorAux<ContainerTemplate<int>, STD_IVEC>(); \
    TSC_CountConstructorAux<ContainerTemplate<std::string>, STD_SVEC>(); \
    TSC_CountConstructorAux<ContainerTemplate<Uncopyable>, STD_UVEC>(); \
\
    TSC_CountConstructorAux<ContainerTemplate<int>, STD_IVEC>(42); \
    TSC_CountConstructorAux<ContainerTemplate<std::string>, STD_SVEC>("hello"); \
} while (false)