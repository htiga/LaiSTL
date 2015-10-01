#pragma once
#include "stdafx.h"
// TSC means "Test sequence container"


// ---- Test for constructor ----

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
    TSC_DefaultConstructorAux< ContainerTemplate<int> >(); \
    TSC_DefaultConstructorAux< ContainerTemplate<std::string> >(); \
    TSC_DefaultConstructorAux< ContainerTemplate<Uncopyable> >(); \
} while (false)


template<typename Container, typename StdContainer>
void TSC_CountConstructorAux()
{
    I_IL cases = { 0, 1, 10, 20 };
    for (const auto & i : cases)
    {
        AssertContainerEqual(Container(i), StdContainer(i));
    }
}


template<typename Container, typename StdContainer, typename ValueType>
void TSC_CountConstructorAux(const ValueType & val)
{
    I_IL cases = { 0, 1, 10, 20 };
    for (const auto & i : cases)
    {
        AssertContainerEqual(Container(i, val), StdContainer(i, val));
    }
}


#define TSC_CountConstructor(ContainerTemplate) \
do { \
    TSC_CountConstructorAux< ContainerTemplate<int>, STD_IVEC>(); \
    TSC_CountConstructorAux< ContainerTemplate<std::string>, STD_SVEC>(); \
    TSC_CountConstructorAux< ContainerTemplate<Uncopyable>, STD_UVEC>(); \
\
    TSC_CountConstructorAux< ContainerTemplate<int>, STD_IVEC>(42); \
    TSC_CountConstructorAux< ContainerTemplate<std::string>, STD_SVEC>("hello"); \
} while (false)


template<typename Container, typename DataSet>
void TSC_RangeConstructorAux(const DataSet & dataSet)
{
    for (const auto & data : dataSet)
    {
        Container c(data.begin(), data.end());
        AssertContainerEqual(c, data);
    }
}


#define TSC_RangeConstructor(ContainerTemplate)\
do { \
    std::initializer_list<I_IL> intData =  \
    {                                      \
        {}, { 1 }, { 0,1,2,3,4,5,6,7,8,9 } \
    };                                     \
    TSC_RangeConstructorAux< ContainerTemplate<int> >(intData); \
\
    std::initializer_list<S_IL> strData =                   \
    {                                                       \
        {}, {"laistl"}, {"a", "b", "c", "dd", "eee", "fff"} \
    };                                                      \
    TSC_RangeConstructorAux< ContainerTemplate<std::string> >(strData); \
} while (false)


template<typename Container, typename DataSet>
void TSC_BraceListConstructorAux(const DataSet & dataset)
{
    for (const auto & data : dataset)
    {
        Container c = { data };
        AssertContainerEqual(c, data);
    }
}


#define TSC_BraceListConstructor(ContainerTemplate) \
do { \
    std::initializer_list<I_IL> intData = \
    {                                     \
        {}, {1}, {1, 2}, {1,2,3,4,5,6,7,8,9} \
    };                                      \
    TSC_BraceListConstructorAux< ContainerTemplate<int> >(intData); \
\
    std::initializer_list<S_IL> strData =                   \
    {                                                       \
        {}, {"laistl"}, {"lai", "stl"}, {"a", "b", "c", "dd", "eee", "fff"} \
    };                                                      \
    TSC_BraceListConstructorAux< ContainerTemplate<std::string> >(strData); \
} while (false)


template<typename Container, typename DataSet>
void TSC_CopyConstructorAux(const DataSet & dataset)
{
    for (const auto & data : dataset)
    {
        Container c(data);
        Container copy(c);
        AssertContainerEqual(c, data);
        AssertContainerEqual(c, copy);
        c.~Container();
        AssertContainerEqual(copy, data);
    }
}


#define TSC_CopyConstructor(ContainerTemplate) \
do { \
    std::initializer_list<I_IL> intData = \
    {                                     \
        {}, {1}, {1, 2}, {1,2,3,4,5,6,7,8,9} \
    };                                      \
    TSC_CopyConstructorAux< ContainerTemplate<int> >(intData); \
\
    std::initializer_list<S_IL> strData =                   \
    {                                                       \
        {}, {"laistl"}, {"lai", "stl"}, {"a", "b", "c", "dd", "eee", "fff"} \
    };                                                      \
    TSC_CopyConstructorAux< ContainerTemplate<std::string> >(strData); \
} while (false)


template<typename Container, typename DataSet>
void TSC_MoveConstructorAux(const DataSet & dataset)
{
    for (const auto & data : dataset)
    {
        Container c(data);
        Container copy(std::move(c));
        c.~Container();
        AssertContainerEqual(copy, data);
    }
}


#define TSC_MoveConstructor(ContainerTemplate) \
do { \
    std::initializer_list<I_IL> intData = \
    {                                     \
        {}, {1}, {1, 2}, {1,2,3,4,5,6,7,8,9} \
    };                                      \
    TSC_MoveConstructorAux< ContainerTemplate<int> >(intData); \
\
    std::initializer_list<S_IL> strData =                   \
    {                                                       \
        {}, {"laistl"}, {"lai", "stl"}, {"a", "b", "c", "dd", "eee", "fff"} \
    };                                                      \
    TSC_MoveConstructorAux< ContainerTemplate<std::string> >(strData); \
\
    ContainerTemplate<Uncopyable> c(10); \
    ContainerTemplate<Uncopyable> moved(std::move(c)); \
    AssertContainerEqual(moved, STD_UVEC(10)); \
} while (false)


// ---- Test for assignment operator ----

template<typename Container, typename DataSet>
void TSC_CopyAssignmentOperatorAux(const DataSet & dataset)
{
    for (const auto & data : dataset)
    {
        Container c(data);
        c = c;
        AssertContainerEqual(c, data);

        Container c1, c2;
        c2 = (c1 = c);
        AssertContainerEqual(c, c1);
        AssertContainerEqual(c, c2);
        AssertContainerEqual(c, data);

        c.~Container();
        AssertContainerEqual(c1, data);
        AssertContainerEqual(c2, data);
    }
}


#define TSC_CopyAssignmentOperator(ContainerTemplate) \
do { \
    std::initializer_list<I_IL> intData = \
    {                                     \
        {}, {1}, {1, 2}, {1,2,3,4,5,6,7,8,9} \
    };                                      \
    TSC_CopyAssignmentOperatorAux< ContainerTemplate<int> >(intData); \
\
    std::initializer_list<S_IL> strData =                   \
    {                                                       \
        {}, {"laistl"}, {"lai", "stl"}, {"a", "b", "c", "dd", "eee", "fff"} \
    };                                                      \
    TSC_CopyAssignmentOperatorAux< ContainerTemplate<std::string> >(strData); \
} while (false)


template<typename Container, typename DataSet>
void TSC_MoveAssignmentOperatorAux(const DataSet & dataset)
{
    for (const auto & data : dataset)
    {
        Container c(data);
        c = std::move(c);
        AssertContainerEqual(c, data);

        Container c1, c2;
        c1 = std::move(c2 = std::move(c));
        c.~Container();
        c2.~Container();
        AssertContainerEqual(c1, data);
    }
}


#define TSC_MoveAssignmentOperator(ContainerTemplate) \
do { \
    std::initializer_list<I_IL> intData = \
    {                                     \
        {}, {1}, {1, 2}, {1,2,3,4,5,6,7,8,9} \
    };                                      \
    TSC_MoveAssignmentOperatorAux< ContainerTemplate<int> >(intData); \
\
    std::initializer_list<S_IL> strData =                   \
    {                                                       \
        {}, {"laistl"}, {"lai", "stl"}, {"a", "b", "c", "dd", "eee", "fff"} \
    };                                                      \
    TSC_MoveAssignmentOperatorAux< ContainerTemplate<std::string> >(strData); \
\
    ContainerTemplate<Uncopyable> c(10); \
    ContainerTemplate<Uncopyable> moved; \
    moved = std::move(c);                \
    AssertContainerEqual(moved, STD_UVEC(10)); \
} while (false)


template<typename Container, typename DataSet>
void TSC_AssignmentOperatorInitListAux(const DataSet & dataset)
{
    for (const auto & data : dataset)
    {
        Container c;
        c = { data };
        AssertContainerEqual(c, data);
    }
}


#define TSC_AssignmentOperatorInitList(ContainerTemplate) \
do { \
    std::initializer_list<I_IL> intData = \
    {                                     \
        {}, {1}, {1, 2}, {1,2,3,4,5,6,7,8,9} \
    };                                      \
    TSC_AssignmentOperatorInitListAux< ContainerTemplate<int> >(intData); \
\
    std::initializer_list<S_IL> strData =                   \
    {                                                       \
        {}, {"laistl"}, {"lai", "stl"}, {"a", "b", "c", "dd", "eee", "fff"} \
    };                                                      \
    TSC_AssignmentOperatorInitListAux< ContainerTemplate<std::string> >(strData); \
} while (false)