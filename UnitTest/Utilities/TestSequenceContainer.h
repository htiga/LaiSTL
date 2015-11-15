#pragma once
#include "stdafx.h"
#include <functional>
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
    ContainerTemplate<Uncopyable> c; \
    ContainerTemplate<Uncopyable> moved(std::move(c)); \
    AssertContainerEqual(moved, STD_UVEC()); \
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


// ---- Test for member assign ----

template<typename Container, typename DataSet>
void TSC_AssignCountAux(const DataSet & dataset)
{
    I_IL cases = { 0,1,2,10 };
    for (const auto & data : dataset)
    {
        for (const auto & count: cases)
        {
            Container c;
            c.assign(count, data);
            IS_TRUE(c.size() == count);
            AssertContainerFillWith(c, data);
        }
    }
}


#define TSC_AssignCount(ContainerTemplate) \
do { \
    I_IL intData = { 0, 1, -1, 10 }; \
    TSC_AssignCountAux< ContainerTemplate<int> >(intData); \
\
    S_IL strData = {"", "b", "c", "dd", "eee", "ffff"}; \
    TSC_AssignCountAux< ContainerTemplate<std::string> >(strData); \
} while (false)


template<typename Container, typename DataSet>
void TSC_AssignRangeAux(const DataSet & dataset)
{
    for (const auto & data : dataset)
    {
        Container c;
        c.assign(data.begin(), data.end());
        AssertContainerEqual(c, data);
    }
}


#define TSC_AssignRange(ContainerTemplate) \
do { \
    std::initializer_list<I_IL> intData = \
    {                                     \
        {}, {1}, {1, 2}, {1,2,3,4,5,6,7,8,9} \
    };                                      \
    TSC_AssignRangeAux< ContainerTemplate<int> >(intData); \
\
    std::initializer_list<S_IL> strData =                   \
    {                                                       \
        {}, {"laistl"}, {"lai", "stl"}, {"a", "b", "c", "dd", "eee", "fff"} \
    };                                                      \
    TSC_AssignRangeAux< ContainerTemplate<std::string> >(strData); \
} while (false)


template<typename Container, typename DataSet>
void TSC_AssignInitListAux(const DataSet & dataset)
{
    for (const auto & data : dataset)
    {
        Container c;
        c.assign(data);
        AssertContainerEqual(c, data);
    }
}


#define TSC_AssignInitList(ContainerTemplate) \
do { \
    std::initializer_list<I_IL> intData = \
    {                                     \
        {}, {1}, {1, 2}, {1,2,3,4,5,6,7,8,9} \
    };                                      \
    TSC_AssignInitListAux< ContainerTemplate<int> >(intData); \
\
    std::initializer_list<S_IL> strData =                   \
    {                                                       \
        {}, {"laistl"}, {"lai", "stl"}, {"a", "b", "c", "dd", "eee", "fff"} \
    };                                                      \
    TSC_AssignInitListAux< ContainerTemplate<std::string> >(strData); \
} while (false)


// ---- Test for member at ----

#define TSC_At(ContainerTemplate) \
do { \
    I_IL cases = {0, 1, -1, 10, -10}; \
    ContainerTemplate<int> c; \
    const ContainerTemplate<int> constC; \
    for (const auto & i : cases)                                      \
    {                                                                 \
        EXPECT_EXPCETION<std::out_of_range>( [&](){ c.at(i); });      \
        EXPECT_EXPCETION<std::out_of_range>( [&](){ constC.at(i); }); \
    }                                                                 \
\
    I_IL data = {0,1,2,3,4,5}; \
    ContainerTemplate<int> c1(data); \
    const ContainerTemplate<int> constC1(data); \
    for (const auto & i : data)                 \
    {                                           \
        IS_TRUE(c1.at(i) == i);                 \
        IS_TRUE(constC1.at(i) == i);            \
                                                \
        int j = i + 1;                          \
        c1.at(i) = j;                           \
        IS_TRUE(c1.at(i) == j);                 \
    }                                           \
} while (false)


// ---- Test for subscript operator ----

#define TSC_Subscript(ContainerTemplate) \
do { \
    I_IL data = {0,1,2,3,4,5}; \
    ContainerTemplate<int> c(data); \
    const ContainerTemplate<int> constC(data); \
    for (const auto & i : data)                \
    {                                          \
        IS_TRUE(c[i] == i);                    \
        IS_TRUE(constC[i] == i);               \
                                               \
        int j = i + 10;                        \
        c[i] = j;                              \
        IS_TRUE(c[i] == j);                    \
    }                                          \
} while (false)


// ---- Test for front and back ----

#define TSC_Front(ContainerTemplate) \
do { \
    I_IL data = {0,1,2,3,4,5}; \
    ContainerTemplate<int> c(data); \
    const ContainerTemplate<int> constC(data); \
\
    IS_TRUE(c.front() == 0); \
    IS_TRUE(constC.front() == 0); \
\
    for (const auto & i : data) \
    {                           \
        int j = i + 10;         \
        c.front() = j;          \
        IS_TRUE(c.front() == j);\
    }                           \
} while (false)


#define TSC_Back(ContainerTemplate) \
do { \
    I_IL data = {0,1,2,3,4,5}; \
    ContainerTemplate<int> c(data); \
    const ContainerTemplate<int> constC(data); \
\
    IS_TRUE(c.back() == 5); \
    IS_TRUE(constC.back() == 5); \
\
    for (const auto & i : data) \
    {                           \
        int j = i + 10;         \
        c.back() = j;           \
        IS_TRUE(c.back() == j); \
    }                           \
} while (false)


// ---- Test for insert ----

template<typename Container, typename DataSet>
void TSC_InsertLvalueAux(const DataSet & dataset)
{
    Container c, c1;

    int i = 0;
    for (const auto & val : dataset)
    {
        auto iter = c.insert(std::next(c.begin() , i), val);
        IS_TRUE(iter == std::next(c.begin() , i));
        IS_TRUE(*iter == val);

        auto iter1 = c1.insert(std::next(c1.end(), -i), val);
        IS_TRUE(iter1 == std::next(c1.end(), - (i + 1)));
        IS_TRUE(*iter1 == val);

        i += 1;
    }
}


#define TSC_InsertLvalue(ContainerTemplate) \
do { \
    I_IL intData = {9, 8, 6, 5, 4, 2, 1, 0}; \
    TSC_InsertLvalueAux< ContainerTemplate<int> >(intData); \
\
    S_IL strData = { "a", "bc", "def", "ghij", ""}; \
    TSC_InsertLvalueAux< ContainerTemplate<std::string> >(strData); \
} while (false)


// Elements in DataSet must be copyable as well as moveable.
template<typename Container, typename DataSet>
void TSC_InsertRvalueAux(DataSet & dataset)
{
    Container c, c1;
    DataSet data = dataset;
    DataSet data1 = dataset;

    DataSet::iterator valIt = data.begin();
    DataSet::iterator valIt1 = data1.begin();
    DataSet::iterator expectIt = dataset.begin();
    int i = 0;
    while (expectIt != dataset.end())
    {
        auto iter = c.insert(std::next(c.begin(), i), std::move(*valIt));
        IS_TRUE(iter == std::next(c.begin(), i));
        IS_TRUE(*iter == *expectIt);

        auto iter1 = c1.insert(std::next(c1.end(), -i), std::move(*valIt1));
        IS_TRUE(iter1 == std::next(c1.end(), -(i + 1)));
        IS_TRUE(*iter1 == *expectIt);

        ++valIt;
        ++valIt1;
        ++expectIt;
        ++i;
    }
}


#define TSC_InsertRvalue(ContainerTemplate) \
do { \
    STD_IVEC intData = {9, 8, 6, 5, 4, 2, 1, 0}; \
    TSC_InsertRvalueAux< ContainerTemplate<int> >(intData); \
\
    STD_SVEC strData = { "a", "bc", "def", "ghij", ""}; \
    TSC_InsertRvalueAux< ContainerTemplate<std::string> >(strData); \
\
    ContainerTemplate<Uncopyable> c, c1; \
    for (int i = 0; i != 10; ++i) \
    { \
        auto iter = c.insert(std::next(c.begin(), i), Uncopyable(i)); \
        IS_TRUE(iter == std::next(c.begin(), i)); \
        IS_TRUE(*iter == Uncopyable(i)); \
\
        auto iter1 = c1.insert(std::next(c1.end(), -i), Uncopyable(i)); \
        IS_TRUE(iter1 == std::next(c1.end(), -(i + 1))); \
        IS_TRUE(*iter1 == Uncopyable(i)); \
    } \
} while (false)


template<typename Container, typename ValueType>
void TSC_InsertCountAux(const ValueType & val, const std::size_t count)
{
    I_IL il = { 0, 1, 3 };
    int minSize = 4;
    Container c(minSize), c1(minSize);
    std::vector<ValueType> stdC(minSize), stdC1(minSize);
    int intCount = static_cast<int>(count);

    for (const auto & i : il)
    {
        auto iter = c.insert(std::next(c.begin(), i), count, val);
        IS_TRUE(iter == std::next(c.begin(), i));

        stdC.insert(stdC.begin() + i, count, val);
        AssertContainerEqual(c, stdC);

        auto iter1 = c1.insert(std::next(c1.end(), -i), count, val);
        IS_TRUE(iter1 == std::next(c1.end(), - (intCount + i)));

        stdC1.insert(stdC1.end() - i, count, val);
        AssertContainerEqual(c1, stdC1);
    }
}


#define TSC_InsertCount(ContainerTemplate) \
do { \
    I_IL counts = { 0, 1, 5 }; \
\
    I_IL intData = { 0, 1, 2, 3}; \
    for (const auto & val : intData) \
        for (const auto & count : counts) \
            TSC_InsertCountAux< ContainerTemplate<int> >(val, count); \
\
    S_IL strData = {"", "a", "bc", "def"}; \
    for (const auto & val : strData) \
        for (const auto & count : counts) \
            TSC_InsertCountAux< ContainerTemplate<std::string> >(val, count); \
} while (false)


template<typename Container, typename DataSet>
void TSC_InsertRangeAux(const DataSet & dataset, const int offset)
{
    // test at begin()

    Container c;
    std::vector<typename Container::value_type> stdC;

    auto iter = c.insert(c.begin(), dataset.begin(), dataset.begin());
    IS_TRUE(iter == c.begin());
    stdC.insert(stdC.begin(), dataset.begin(), dataset.begin());
    AssertContainerEqual(c, stdC);
   
    iter = c.insert(c.begin(), dataset.begin(), dataset.end());
    IS_TRUE(iter == c.begin());
    stdC.insert(stdC.begin(), dataset.begin(), dataset.end());
    AssertContainerEqual(c, stdC);
   
    iter = c.insert(std::next(c.begin(), offset), dataset.begin(), dataset.end());
    IS_TRUE(iter == std::next(c.begin(), offset));
    stdC.insert(stdC.begin() + offset, dataset.begin(), dataset.end());
    AssertContainerEqual(c, stdC);

    // test at end()

    Container c1;
    std::vector<typename Container::value_type> stdC1;
    int length = static_cast<int>(dataset.size());

    auto iter1 = c1.insert(c1.end(), dataset.begin(), dataset.begin());
    IS_TRUE(iter1 == c1.end());
    stdC1.insert(stdC1.end(), dataset.begin(), dataset.begin());
    AssertContainerEqual(c1, stdC1);

    iter1 = c1.insert(c1.end(), dataset.begin(), dataset.end());
    IS_TRUE(iter1 == std::next(c1.end(), -length));
    stdC1.insert(stdC1.end(), dataset.begin(), dataset.end());
    AssertContainerEqual(c1, stdC1);

    iter1 = c1.insert(std::next(c1.end(), -offset), dataset.begin(), dataset.end());
    IS_TRUE(iter1 == std::next(c1.end(), -offset-length));
    stdC1.insert(stdC1.end() - offset, dataset.begin(), dataset.end());
    AssertContainerEqual(c1, stdC1);
}


#define TSC_InsertRange(ContainerTemplate) \
do { \
    int offset = 5; \
\
    I_IL intData = {0,1,2,3,4,5,6,7,8,9}; \
    TSC_InsertRangeAux< ContainerTemplate<int> >(intData, offset); \
\
    S_IL strData = {"", "a", "bc", "def", "ghij", "klmn"}; \
    TSC_InsertRangeAux< ContainerTemplate<std::string> > (strData, offset); \
} while (false)


template<typename Container, typename DataSet>
void TSC_InsertInitListAux(const DataSet & dataset)
{
    for (const auto & data : dataset)
    {
        int length = static_cast<int>(data.size());
        // test at begin()

        Container c;
        std::vector<typename Container::value_type> stdC;

        auto iter = c.insert(c.begin(), data);
        IS_TRUE(iter == c.begin());
        stdC.insert(stdC.begin(), data);
        AssertContainerEqual(c, stdC);

        iter = c.insert(std::next(c.begin(), length), data);
        IS_TRUE(iter == std::next(c.begin(), length));
        stdC.insert(stdC.begin() + length, data);
        AssertContainerEqual(c, stdC);

        // test at end()

        Container c1;
        std::vector<typename Container::value_type> stdC1;

        auto iter1 = c1.insert(c1.end(), data);
        IS_TRUE(iter1 == std::next(c1.end(), -length));
        stdC1.insert(stdC1.end(), data);
        AssertContainerEqual(c1, stdC1);

        iter1 = c1.insert(std::next(c1.end(), -length), data);
        IS_TRUE(iter1 == std::next(c1.end(), -length-length));
        stdC1.insert(stdC1.end() - length, data);
        AssertContainerEqual(c1, stdC1);
    }
}


#define TSC_InsertInitList(ContainerTemplate) \
do { \
    std::initializer_list<I_IL> intData = \
    { \
        {}, {0}, {0, 0}, {0, 1, 2, 3} \
    };\
    TSC_InsertInitListAux< ContainerTemplate<int> >(intData); \
\
   std::initializer_list<S_IL> strData = \
   { \
        {}, {""}, {"a", "bc"}, {"def", "ghij", "klmn"} \
   }; \
   TSC_InsertInitListAux< ContainerTemplate<std::string> > (strData); \
} while (false)


// ---- Test for emplace ----

#define TSC_Emplace(ContainerTemplate) \
do { \
    ContainerTemplate<Uncopyable> c; \
    STD_UVEC stdC; \
\
    auto iter = c.emplace(c.begin()); \
    IS_TRUE(iter == c.begin()); \
    stdC.emplace(stdC.begin()); \
    AssertContainerEqual(c, stdC); \
\
    iter = c.emplace(c.end()); \
    IS_TRUE(iter == std::next(c.end(), -1)); \
    stdC.emplace(stdC.end()); \
    AssertContainerEqual(c, stdC); \
\
    ContainerTemplate<std::string> c1; \
    STD_SVEC stdC1; \
\
    auto iter1 = c1.emplace(c1.begin()); \
    IS_TRUE(iter1 == c1.begin()); \
    stdC1.emplace(stdC1.begin()); \
    AssertContainerEqual(c1, stdC1); \
\
    iter1 = c1.emplace(c1.end(), "lai"); \
    IS_TRUE(iter1 == std::next(c1.end(), -1)); \
    stdC1.emplace(stdC1.end(), "lai"); \
    AssertContainerEqual(c1, stdC1); \
\
    iter1 = c1.emplace(std::next(c1.begin(), 1), 3, 'a'); \
    IS_TRUE(iter1 == std::next(c1.begin(), 1)); \
    stdC1.emplace(stdC1.begin() + 1, 3, 'a'); \
    AssertContainerEqual(c1, stdC1); \
\
    std::string str = "tiga"; \
    iter1 = c1.emplace( std::next(c1.end(), -1), str); \
    IS_TRUE(iter1 == std::next(c1.end(), -2)); \
    stdC1.emplace(stdC1.end() - 1, str); \
    AssertContainerEqual(c1, stdC1); \
\
    iter1= c1.emplace(std::next(c1.begin(), 2), std::move(str)); \
    IS_TRUE(iter1 == std::next(c1.begin(), 2)); \
    stdC1.emplace(stdC1.begin() + 2, "tiga"); \
    AssertContainerEqual(c1, stdC1); \
\
    std::initializer_list<char> il = {'l', 'a', 'i', 's', 't', 'l'}; \
    iter1 = c1.emplace(std::next(c1.end(), -2), il); \
    IS_TRUE(iter1 == std::next(c1.end(), -3)); \
    stdC1.emplace(stdC1.end() - 2, il); \
    AssertContainerEqual(c1, stdC1); \
} while (false)


// ---- Test for erase ----

#define TSC_Erase(ContainerTemplate) \
do { \
    I_IL intData = {0,1,2,3,4,5,6,7,8,9};\
    ContainerTemplate<int> c(intData);   \
    STD_IVEC stdC(intData);              \
                                         \
    auto iter = c.erase(c.begin());      \
    IS_TRUE(iter == c.begin());          \
    stdC.erase(stdC.begin());            \
    AssertContainerEqual(c, stdC);       \
                                         \
    iter = c.erase(std::next(c.end(), -1));\
    IS_TRUE(iter == c.end());            \
    stdC.erase(stdC.end() - 1);          \
    AssertContainerEqual(c, stdC);       \
                                         \
    iter = c.erase(std::next(c.begin(), 3)); \
    IS_TRUE(iter == std::next(c.begin(), 3));\
    stdC.erase(stdC.begin() + 3);        \
    AssertContainerEqual(c, stdC);       \
                                         \
    iter = c.erase(std::next(c.end(), -3)); \
    IS_TRUE(iter == std::next(c.end(), -2));\
    stdC.erase(stdC.end() - 3);          \
    AssertContainerEqual(c, stdC);       \
} while (false)


#define TSC_EraseRange(ContainerTemplate) \
do { \
    I_IL intData = {0,1,2,3,4,5,6,7,8,9};              \
    ContainerTemplate<int> c(intData);                 \
    STD_IVEC stdC(intData);                            \
                                                       \
    auto iter = c.erase(c.begin(), c.begin());         \
    IS_TRUE(iter == c.begin());                        \
    stdC.erase(stdC.begin(), stdC.begin());            \
    AssertContainerEqual(c, stdC);                     \
                                                       \
    iter = c.erase(c.begin(), std::next(c.begin(), 1));  \
    IS_TRUE(iter == c.begin());                        \
    stdC.erase(stdC.begin(), stdC.begin() + 1);        \
    AssertContainerEqual(c, stdC);                     \
                                                       \
    iter = c.erase(std::next(c.end(), -1), c.end());     \
    IS_TRUE(iter == c.end());                          \
    stdC.erase(stdC.end() - 1, stdC.end());            \
    AssertContainerEqual(c, stdC);                     \
                                                       \
    iter = c.erase(std::next(c.begin(), 1), std::next(c.begin(), 3)); \
    IS_TRUE(iter == std::next(c.begin(), 1));            \
    stdC.erase(stdC.begin() + 1, stdC.begin() + 3);    \
    AssertContainerEqual(c, stdC);                     \
                                                       \
    iter = c.erase(std::next(c.end(), -3), std::next(c.end(), -1)); \
    IS_TRUE(iter == std::next(c.end(), -1));             \
    stdC.erase(stdC.end() - 3, stdC.end() - 1);        \
    AssertContainerEqual(c, stdC);                     \
                                                       \
    iter = c.erase(c.begin(), c.end());                \
    IS_TRUE(iter == c.begin());                        \
    stdC.erase(stdC.begin(), stdC.end());              \
    AssertContainerEqual(c, stdC);                     \
} while (false)


// ---- Test for push_back ----

template<typename Container,
         typename DataSet,
         typename TestPushOp,
         typename StdPushOp>
void TSC_PushLvalueAux(const DataSet & dataset, TestPushOp testPush, StdPushOp stdPush)
{
    Container c;
    std::deque<typename Container::value_type> stdC;
    for (const auto & val : dataset)
    {
        testPush(c, val);
        stdPush(stdC, val);
        AssertContainerEqual(c, stdC);
    }
}


#define TSC_PushBackLvalue(ContainerTemplate) \
do { \
    auto pushBack = [](auto & c, const auto & val) { c.push_back(val); }; \
\
    I_IL intData = {0,1,2,3,4,5,6,7,8,9}; \
    TSC_PushLvalueAux< ContainerTemplate<int> >(intData, pushBack, pushBack); \
\
    S_IL strData = { "", "a", "bc", "def", "hijk", "lmnop", "lai", "stl", "" }; \
    TSC_PushLvalueAux< ContainerTemplate<std::string> >(strData, pushBack, pushBack); \
} while (false)


#define TSC_PushBackRvalue(ContainerTemplate) \
do { \
    ContainerTemplate<Uncopyable> c; \
    STD_UVEC stdC; \
\
    for (int i = 0; i != 10; ++i) \
    { \
        c.push_back(Uncopyable(i)); \
        stdC.push_back(Uncopyable(i)); \
        AssertContainerEqual(c, stdC); \
    } \
} while (false)


// --- Test for push_front ----


#define TSC_PushFrontLvalue(ContainerTemplate) \
do { \
    auto pushFront = [](auto & c, const auto & val) { c.push_front(val); }; \
    I_IL intData = {0,1,2,3,4,5,6,7,8,9}; \
    TSC_PushLvalueAux< ContainerTemplate<int> >(intData, pushFront, pushFront); \
\
    S_IL strData = { "", "a", "bc", "def", "hijk", "lmnop", "lai", "stl", "" }; \
    TSC_PushLvalueAux< ContainerTemplate<std::string> >(strData, pushFront, pushFront); \
} while (false)


#define TSC_PushFrontRvalue(ContainerTemplate) \
do { \
    ContainerTemplate<Uncopyable> c; \
    std::deque<Uncopyable> stdC; \
\
    for (int i = 0; i != 10; ++i) \
    { \
        c.push_front(Uncopyable(i)); \
        stdC.push_front(Uncopyable(i)); \
        AssertContainerEqual(c, stdC); \
    } \
} while (false)


// ---- Test for emplace_back ----

#define TSC_EmplaceBack(ContainerTemplate) \
do { \
    ContainerTemplate<std::string> c; \
    STD_SVEC stdC; \
\
    c.emplace_back(); \
    stdC.emplace_back(); \
    AssertContainerEqual(c, stdC); \
\
    c.emplace_back("lai"); \
    stdC.emplace_back("lai"); \
    AssertContainerEqual(c, stdC); \
\
    c.emplace_back(3, 'a'); \
    stdC.emplace_back(3, 'a'); \
    AssertContainerEqual(c, stdC); \
\
    std::string str = "tiga"; \
    c.emplace_back(str); \
    stdC.emplace_back(str); \
    AssertContainerEqual(c, stdC); \
\
    c.emplace_back(std::move(str)); \
    stdC.emplace_back("tiga"); \
    AssertContainerEqual(c, stdC); \
\
    std::initializer_list<char> il = { 'l', 'a', 'i', 's', 't', 'l' }; \
    c.emplace_back(il); \
    stdC.emplace_back(il); \
    AssertContainerEqual(c, stdC); \
} while (false)


// ---- Test for emplace_front ----

#define TSC_EmplaceFront(ContainerTemplate) \
do { \
    ContainerTemplate<std::string> c; \
    std::deque<std::string> stdC; \
\
    c.emplace_front(); \
    stdC.emplace_front(); \
    AssertContainerEqual(c, stdC); \
\
    c.emplace_front("lai"); \
    stdC.emplace_front("lai"); \
    AssertContainerEqual(c, stdC); \
\
    c.emplace_front(3, 'a'); \
    stdC.emplace_front(3, 'a'); \
    AssertContainerEqual(c, stdC); \
\
    std::string str = "tiga"; \
    c.emplace_front(str); \
    stdC.emplace_front(str); \
    AssertContainerEqual(c, stdC); \
\
    c.emplace_front(std::move(str)); \
    stdC.emplace_front("tiga"); \
    AssertContainerEqual(c, stdC); \
\
    std::initializer_list<char> il = { 'l', 'a', 'i', 's', 't', 'l' }; \
    c.emplace_front(il); \
    stdC.emplace_front(il); \
    AssertContainerEqual(c, stdC); \
} while (false)


// ---- Test for pop_back ----

template<typename Container,
         typename StdContainer,
         typename TestPopOp,
         typename StdPopOp>
void TSC_PopAux(Container & c, StdContainer & stdC, TestPopOp testPop, StdPopOp stdPop)
{
    while (!stdC.empty())
    {
        testPop(c);
        stdPop(stdC);
        AssertContainerEqual(c, stdC);
    }
}


#define TSC_PopBack(ContainerTemplate) \
do { \
    auto popBack = [](auto & c) { c.pop_back(); }; \
    I_IL intData = {0,1,2,3,4,5,6,7,8,9}; \
    ContainerTemplate<int> c(intData); \
    STD_IVEC stdC(intData); \
    TSC_PopAux(c, stdC, popBack, popBack); \
\
    S_IL strData = { "", "a", "bc", "def", "hijk", "lmnop", "lai", "stl", "" }; \
    ContainerTemplate<std::string> c1(strData); \
    STD_SVEC stdC1(strData); \
    TSC_PopAux< ContainerTemplate<std::string> >(c1, stdC1, popBack, popBack); \
} while (false)


#define TSC_PopFront(ContainerTemplate) \
do { \
    auto popFront = [](auto & c) { c.pop_front(); }; \
    I_IL intData = {0,1,2,3,4,5,6,7,8,9}; \
    ContainerTemplate<int> c(intData); \
    std::deque<int> stdC(intData); \
    TSC_PopAux(c, stdC, popFront, popFront); \
\
    S_IL strData = { "", "a", "bc", "def", "hijk", "lmnop", "lai", "stl", "" }; \
    ContainerTemplate<std::string> c1(strData); \
    std::deque<std::string> stdC1(strData); \
    TSC_PopAux< ContainerTemplate<std::string> >(c1, stdC1, popFront, popFront); \
} while (false)

// ---- Test for resize ----

#define TSC_Resize(ContainerTemplate) \
do { \
    ContainerTemplate<Uncopyable> c; \
    c.resize(10); \
    AssertContainerEqual(c, STD_UVEC(10)); \
    c.resize(5); \
    AssertContainerEqual(c, STD_UVEC(5)); \
    c.resize(0); \
    AssertContainerEqual(c, STD_UVEC(0)); \
\
    S_IL strData = { "", "a", "bc", "def", "hijk", "lmnop", "lai", "stl", "" }; \
    ContainerTemplate<std::string> c1(strData); \
    STD_SVEC stdC1(strData); \
\
    c1.resize(c1.size()); \
    stdC1.resize(stdC1.size()); \
    AssertContainerEqual(c1, stdC1); \
\
    c1.resize(c1.size() - 3); \
    stdC1.resize(stdC1.size() - 3); \
    AssertContainerEqual(c1, stdC1); \
\
    c1.resize(c1.size() + 3); \
    stdC1.resize(stdC1.size() + 3); \
    AssertContainerEqual(c1, stdC1); \
\
    c1.resize(0); \
    stdC1.resize(0); \
    AssertContainerEqual(c1, stdC1); \
} while (false)


#define TSC_ResizeVal(ContainerTemplate) \
do { \
    S_IL strData = { "", "a", "bc", "def", "hijk", "lmnop", "lai", "stl", "" }; \
    ContainerTemplate<std::string> c(strData); \
    STD_SVEC stdC(strData); \
\
    c.resize(c.size(), "a"); \
    stdC.resize(stdC.size(), "a"); \
    AssertContainerEqual(c, stdC); \
\
    c.resize(c.size() - 3, "a"); \
    stdC.resize(stdC.size() - 3, "a"); \
    AssertContainerEqual(c, stdC); \
\
    c.resize(c.size() + 3, "b"); \
    stdC.resize(stdC.size() + 3, "b"); \
    AssertContainerEqual(c, stdC); \
\
    c.resize(0, "c"); \
    stdC.resize(0, "c"); \
    AssertContainerEqual(c, stdC); \
} while (false)


// ---- Test for swap ----

template<typename StrContainer,
         typename SwapOp>
void TSC_SwapStringAux(SwapOp swapOp)
{
    StrContainer c, c1;
    S_IL strData = { "", "a", "bc", "def", "hijk", "lmnop", "lai", "stl", "" };
       
    swapOp(c, c1);
    IS_TRUE(c.empty());
    IS_TRUE(c1.empty());
       
    c = strData;
    swapOp(c1, c);
    AssertContainerEqual(c1, strData);
    IS_TRUE(c.empty());
       
    swapOp(c1, c);
    AssertContainerEqual(c, strData);
    IS_TRUE(c1.empty());
       
    c1 = strData;
    swapOp(c, c1);
    AssertContainerEqual(c, strData);
    AssertContainerEqual(c1, strData);
       
    S_IL data = { "a", "b", "c" };
    c1 = data;
    swapOp(c1, c);
    AssertContainerEqual(c1, strData);
    AssertContainerEqual(c, data);
}


#define TSC_MemberSwap(ContainerTemplate) \
do { \
    auto swapOp = [](auto & c, auto & c1) { c.swap(c1); }; \
    TSC_SwapStringAux< ContainerTemplate<std::string> >(swapOp); \
} while (false)


#define TSC_NonMemberSwap(ContainerTemplate, Swap) \
do { \
    auto swapOp = [](auto & c, auto & c1) { Swap(c, c1); }; \
    TSC_SwapStringAux< ContainerTemplate<std::string> >(swapOp); \
} while (false)


// ---- Test for relational operator ----

#define TSC_RelationalOperator(ContainerTemplate) \
do { \
    ContainerTemplate<int> c, c1; \
    IS_TRUE(c == c1); \
    IS_FALSE(c != c1);\
    IS_FALSE(c < c1); \
    IS_TRUE(c <= c1); \
    IS_FALSE(c > c1); \
    IS_TRUE(c >= c1); \
\
    c = { 0 }; \
    IS_FALSE(c == c1); \
    IS_TRUE(c != c1);\
    IS_FALSE(c < c1); \
    IS_FALSE(c <= c1); \
    IS_TRUE(c > c1); \
    IS_TRUE(c >= c1); \
\
    c1 = { 0 }; \
    IS_TRUE(c == c1); \
    IS_FALSE(c != c1);\
    IS_FALSE(c < c1); \
    IS_TRUE(c <= c1); \
    IS_FALSE(c > c1); \
    IS_TRUE(c >= c1); \
\
    c = {0, 1}; \
    IS_FALSE(c == c1); \
    IS_TRUE(c != c1);\
    IS_FALSE(c < c1); \
    IS_FALSE(c <= c1); \
    IS_TRUE(c > c1); \
    IS_TRUE(c >= c1); \
\
    c1 = {0, 1}; \
    IS_TRUE(c == c1); \
    IS_FALSE(c != c1);\
    IS_FALSE(c < c1); \
    IS_TRUE(c <= c1); \
    IS_FALSE(c > c1); \
    IS_TRUE(c >= c1); \
} while (false)


// Test for sort


static std::initializer_list<I_IL> G_TestedDataForSort =
{
    {},
    { 0 },
    { 0, 0 },
    { 0, 0, 0 },
    { 0, 1 },
    { 1, 0 },
    { 0, 1, 2 },
    { 0, 2, 1 },
    { 1, 0, 2 },
    { 1, 2, 0 },
    { 2, 0, 1 },
    { 2, 1, 0 },
    { 0, 1, 1 },
    { 1, 0, 1 },
    { 1, 1, 0 },
    { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
    { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 },
    { 42, 9, 17, 54, 602, -3, 54, 999, -11 },
    { -11, -3, 9, 17, 42, 54, 54, 602, 999 },
};


template<typename ContainerForInt>
void TSC_MemberSortAux()
{
    static std::initializer_list<I_IL> SortedData =
    {
        {},
        { 0 },
        { 0, 0 },
        { 0, 0, 0 },
        { 0, 1 },
        { 0, 1 },
        { 0, 1, 2 },
        { 0, 1, 2 },
        { 0, 1, 2 },
        { 0, 1, 2 },
        { 0, 1, 2 },
        { 0, 1, 2 },
        { 0, 1, 1 },
        { 0, 1, 1 },
        { 0, 1, 1 },
        { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
        { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
        { -11, -3, 9, 17, 42, 54, 54, 602, 999 },
        { -11, -3, 9, 17, 42, 54, 54, 602, 999 },
    };

    auto testIter = G_TestedDataForSort.begin();
    auto stdIter = SortedData.begin();
    while (testIter != G_TestedDataForSort.end())
    {
        ContainerForInt il = *testIter++;
        il.sort();
        AssertContainerEqual(il, *stdIter++);
    }
}


#define TSC_MemberSort(ContainerTemplate) \
do { \
    TSC_MemberSortAux< ContainerTemplate<int> >(); \
} while (false)


template<typename ContainerForInt>
void TSC_MemberSortByAux()
{
    std::initializer_list<I_IL> SortedData =
    {
        {},
        { 0 },
        { 0, 0 },
        { 0, 0, 0 },
        { 1, 0 },
        { 1, 0 },
        { 2, 1, 0 },
        { 2, 1, 0 },
        { 2, 1, 0 },
        { 2, 1, 0 },
        { 2, 1, 0 },
        { 2, 1, 0 },
        { 1, 1, 0 },
        { 1, 1, 0 },
        { 1, 1, 0 },
        { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 },
        { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 },
        { 999, 602, 54, 54, 42, 17, 9, -3, -11 },
        { 999, 602, 54, 54, 42, 17, 9, -3, -11 },
    };

    auto testIter = G_TestedDataForSort.begin();
    auto stdIter = SortedData.begin();
    while (testIter != G_TestedDataForSort.end())
    {
        ContainerForInt il = *testIter++;
        il.sort(std::greater_equal<>());
        AssertContainerEqual(il, *stdIter++);
    }
}


#define TSC_MemberSortBy(ContainerTemplate) \
do { \
    TSC_MemberSortByAux< ContainerTemplate<int> >(); \
} while (false)


// Test for merge

#define TSC_Merge(ContainerTemplate) \
do { \
    ContainerTemplate<int> a, b; \
                                                               \
    a.merge(b);                                                \
    AssertContainerEqual(a, I_IL{});                           \
    AssertContainerEqual(b, I_IL{});                           \
                                                               \
    a = { 1,2,3,7,8,9 };                                       \
    a.merge(b);                                                \
    AssertContainerEqual(a, I_IL{ 1,2,3,7,8,9 });              \
    AssertContainerEqual(b, I_IL{});                           \
                                                               \
    a = {};                                                    \
    b = { 1,2,3,7,8,9 };                                       \
    a.merge(b);                                                \
    AssertContainerEqual(a, I_IL{ 1,2,3,7,8,9 });              \
    AssertContainerEqual(b, I_IL{});                           \
                                                               \
    b = { 4,5 };                                               \
    a.merge(b);                                                \
    AssertContainerEqual(a, I_IL{ 1,2,3,4,5,7,8,9 });          \
    AssertContainerEqual(b, I_IL{});                           \
                                                               \
    b = { 0, 6, 10 };                                          \
    a.merge(b);                                                \
    AssertContainerEqual(a, I_IL{ 0,1,2,3,4,5,6,7,8,9,10 });   \
    AssertContainerEqual(b, I_IL{});                           \
                                                               \
    b = { 0,1,2,3,4,5,6,7,8,9,10 }; \
    a.merge(b); \
    AssertContainerEqual(a, I_IL{ 0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10 }); \
    AssertContainerEqual(b, I_IL{}); \
} while (false)


#define TSC_MergeBy(ContainerTemplate) \
do { \
    ContainerTemplate<int> a, b;                            \
                                                            \
    auto comp = [](int i, int j) { return i >= j; };        \
                                                            \
    a.merge(b, comp);                                       \
    AssertContainerEqual(a, I_IL{});                        \
    AssertContainerEqual(b, I_IL{});                        \
                                                            \
    a = { 9,8,7,3,2,1 };                                    \
    a.merge(b, comp);                                       \
    AssertContainerEqual(a, I_IL{ 9,8,7,3,2,1 });           \
    AssertContainerEqual(b, I_IL{});                        \
                                                            \
    a = {};                                                 \
    b = { 9,8,7,3,2,1 };                                    \
    a.merge(b, comp);                                       \
    AssertContainerEqual(a, I_IL{ 9,8,7,3,2,1 });           \
    AssertContainerEqual(b, I_IL{});                        \
                                                            \
    b = { 5,4 };                                            \
    a.merge(b, comp);                                       \
    AssertContainerEqual(a, I_IL{ 9,8,7,5,4,3,2,1 });       \
    AssertContainerEqual(b, I_IL{});                        \
                                                            \
    b = { 10, 6, 0 };                                       \
    a.merge(b, comp);                                       \
    AssertContainerEqual(a, I_IL{ 10,9,8,7,6,5,4,3,2,1,0 });\
    AssertContainerEqual(b, I_IL{});                        \
\
    b = { 10,9,8,7,6,5,4,3,2,1,0 }; \
    a.merge(b, comp); \
    AssertContainerEqual(a, I_IL{ 10,10,9,9,8,8,7,7,6,6,5,5,4,4,3,3,2,2,1,1,0,0 }); \
    AssertContainerEqual(b, I_IL{}); \
} while (false)


// Test for remove

#define TSC_Remove(ContainerTemplate) \
do { \
    ContainerTemplate<int> a;                                  \
                                                               \
    a.remove(0);                                               \
    AssertContainerEqual(a, I_IL{});                           \
                                                               \
    a = { 0,0,0,0 };                                           \
    a.remove(1);                                               \
    AssertContainerEqual(a, I_IL{ 0,0,0,0 });                  \
                                                               \
    a.remove(0);                                               \
    AssertContainerEqual(a, I_IL{});                           \
                                                               \
    a = { 0, 1, 1, 2, 3, 0, 4, 5, 0, 5, 5, 5 };                \
    a.remove(0);                                               \
    AssertContainerEqual(a, I_IL{ 1, 1, 2, 3, 4, 5, 5, 5, 5 });\
                                                               \
    a.remove(1);                                               \
    AssertContainerEqual(a, I_IL{ 2, 3, 4, 5, 5, 5, 5 });      \
                                                               \
    a.remove(5);                                               \
    AssertContainerEqual(a, I_IL{ 2, 3, 4 });                  \
} while (false)


#define TSC_RemoveIf(ContainerTemplate) \
do { \
    ContainerTemplate<int> a;                          \
                                                       \
    a.remove_if([](int i) {return true; });            \
    AssertContainerEqual(a, I_IL{});                   \
                                                       \
    a = { 0,1,2,3,4,5,6,7,8,9 };                       \
                                                       \
    a.remove_if([](int i) {return i > 7; });           \
    AssertContainerEqual(a, I_IL{ 0,1,2,3,4,5,6,7 });  \
                                                       \
    a.remove_if([](int i) { return i < 2; });          \
    AssertContainerEqual(a, I_IL{ 2,3,4,5,6,7 });      \
                                                       \
    a.remove_if([](int i) { return false; });          \
    AssertContainerEqual(a, I_IL{ 2,3,4,5,6,7 });      \
                                                       \
    a.remove_if([](int i) { return i % 2 == 0; });     \
    AssertContainerEqual(a, I_IL{ 3,5,7 });            \
                                                       \
    a.remove_if([](int i) { return true; });           \
    AssertContainerEqual(a, I_IL{});                   \
} while (false)


// Test for reverse

#define TSC_Reverse(ContainerTemplate) \
do { \
    ContainerTemplate<int> il = {};                         \
                                                            \
    il.reverse();                                           \
    AssertContainerEqual(il, I_IL{});                       \
                                                            \
    il = { 0 };                                             \
    il.reverse();                                           \
    AssertContainerEqual(il, I_IL{ 0 });                    \
                                                            \
    il = { 0, 0 };                                          \
    il.reverse();                                           \
    AssertContainerEqual(il, I_IL{ 0, 0 });                 \
                                                            \
    il = { 1, 2, 3, 0, 3, 2, 1 };                           \
    il.reverse();                                           \
    AssertContainerEqual(il, I_IL{ 1, 2, 3, 0, 3, 2, 1 });  \
    il.reverse();                                           \
    AssertContainerEqual(il, I_IL{ 1, 2, 3, 0, 3, 2, 1 });  \
                                                            \
    il = { 1,2,3,4,5,6,7,8,9 };                             \
    il.reverse();                                           \
    AssertContainerEqual(il, I_IL{ 9,8,7,6,5,4,3,2,1 });    \
    il.reverse();                                           \
    AssertContainerEqual(il, I_IL{ 1,2,3,4,5,6,7,8,9 });    \
} while (false)


// Test for unique

#define TSC_Unique(ContainerTemplate) \
do {                                                                    \
    ContainerTemplate<int> a;                                           \
                                                                        \
    a.unique();                                                         \
    AssertContainerEqual(a, I_IL{});                                    \
                                                                        \
    a = { 1 };                                                          \
    a.unique();                                                         \
    AssertContainerEqual(a, I_IL{ 1 });                                 \
                                                                        \
    a = { 1, 1 };                                                       \
    a.unique();                                                         \
    AssertContainerEqual(a, I_IL{ 1 });                                 \
                                                                        \
    a = { 1,1,1,1,1,1 };                                                \
    a.unique();                                                         \
    AssertContainerEqual(a, I_IL{ 1 });                                 \
                                                                        \
    a = { 1, 1, 0, 1 };                                                 \
    a.unique();                                                         \
    AssertContainerEqual(a, I_IL{ 1,0,1 });                             \
                                                                        \
    a = { 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 3, 0, 0, 1, 1, 2, 2, 3, 3 }; \
    a.unique();                                                         \
    AssertContainerEqual(a, I_IL{ 0,1,2,3,0,1,2,3 });                   \
} while (false)


#define TSC_UniqueIf(ContainerTemplate) \
do {                                                      \
    ContainerTemplate<int> a;                             \
                                                          \
    a.unique([](int i, int j) { return true; });          \
    AssertContainerEqual(a, I_IL{});                      \
                                                          \
    a = { 0, 1,2, 4,3, 5,6, 8,7, 9 };                     \
    a.unique([](int i, int j) { return i > j; });         \
    AssertContainerEqual(a, I_IL{ 0, 1,2, 4, 5,6, 8, 9 });\
                                                          \
    a.unique([](int i, int j) { return i == j - 1; });    \
    AssertContainerEqual(a, I_IL{ 0, 2, 4, 6, 8 });       \
                                                          \
    a.unique([](int i, int j) { return false; });         \
    AssertContainerEqual(a, I_IL{ 0, 2, 4, 6, 8 });       \
                                                          \
    a.unique([](int i, int j) { return true; });          \
    AssertContainerEqual(a, I_IL{ 0 });                   \
} while (false)