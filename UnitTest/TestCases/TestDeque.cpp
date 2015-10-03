#include "stdafx.h"
#include "../../LaiSTL/Deque.h"
#include <deque>
#include <algorithm>
#include <functional>

#define LAI_IDEQ lai::deque<int>
#define LAI_SDEQ lai::deque<std::string>
#define LAI_UDEQ lai::deque<Uncopyable>
#define STD_IDEQ std::deque<int>
#define STD_SDEQ std::deque<std::string>
#define STD_UDEQ std::deque<Uncopyable>

namespace UnitTest
{
    TEST_CLASS(TestDequeIterator)
    {
        // todo
    };

    TEST_CLASS(TestDeque)
    {
        TEST_METHOD(TestDefaultConstructor)
        {
            TSC_DefaultConstructor(lai::deque);
        }

        TEST_METHOD(TestCountConstructor)
        {
            TSC_CountConstructor(lai::deque);
        }

        TEST_METHOD(TestRangeConstructor)
        {
            TSC_RangeConstructor(lai::deque);
        }

        TEST_METHOD(TestBraceListConstructor)
        {
            TSC_BraceListConstructor(lai::deque);
        }

        TEST_METHOD(TestCopyConstructor)
        {
            TSC_CopyConstructor(lai::deque);
        }

        TEST_METHOD(TestMoveConstructor)
        {
            TSC_MoveConstructor(lai::deque);
        }

        TEST_METHOD(TestCopyAssignmentOperator)
        {
            TSC_CopyAssignmentOperator(lai::deque);
        }

        TEST_METHOD(TestMoveAssignmentOperator)
        {
            TSC_MoveAssignmentOperator(lai::deque);
        }

        TEST_METHOD(TestAssignmentOperatorInitList)
        {
            TSC_AssignmentOperatorInitList(lai::deque);
        }

        TEST_METHOD(TestAssignCount)
        {
            TSC_AssignCount(lai::deque);
        }

        TEST_METHOD(TestAssignRange)
        {
            TSC_AssignRange(lai::deque);
        }

        TEST_METHOD(TestAssignInitList)
        {
            TSC_AssignInitList(lai::deque);
        }

        TEST_METHOD(TestAt)
        {
            TSC_At(lai::deque);
        }

        TEST_METHOD(TestSubscript)
        {
            TSC_Subscript(lai::deque);
        }

    //    // operation must insert an element at the end of the container
    //    template<typename Container, typename PtrContainer>
    //    void isBackOperationInvalidPtrs(std::function<void(Container & c)> operation,
    //        const int performedTimes = 100)
    //    {
    //        Container container;
    //        PtrContainer ptrs;
    //        for (int i = 0; i != performedTimes; ++i)
    //        {
    //            operation(container);
    //            ptrs.push_back(&container[i]);
    //        }
    //        operation(container);

    //        for (int i = 0; i != performedTimes; ++i)
    //        {
    //            IS_TRUE(*ptrs[i] == container[i]);
    //        }
    //    }

    //    // operation must insert an element at the begin of the container
    //    template<typename Container, typename PtrContainer>
    //    void isFrontOperationInvalidatePtrs(std::function<void(Container & c)> operation,
    //        const int performedTimes = 100)
    //    {
    //        Container container;
    //        PtrContainer ptrs;
    //        for (int i = 0; i != performedTimes; ++i)
    //        {
    //            operation(container);
    //            ptrs.push_front(&container[i]);
    //        }
    //        operation(container);

    //        for (int i = 0; i != performedTimes; ++i)
    //        {
    //            IS_TRUE(*ptrs[i] == container[i + 1]);
    //        }
    //    }

    //    TEST_METHOD(TestErase)
    //    {
    //        TSC_Erase(lai::deque);
    //    }

    //    TEST_METHOD(TestEraseRange)
    //    {
    //        TSC_EraseRange(lai::deque);
    //    }

    //    TEST_METHOD(TestEmplace)
    //    {
    //        TSC_Emplace(lai::deque);

    //        using PtrContainer = std::deque<LAI_UDEQ::pointer>;
    //        isBackOperationInvalidPtrs<LAI_UDEQ, PtrContainer>(
    //            [](LAI_UDEQ & c) { c.emplace(c.end(), Uncopyable(42)); }, 20);
    //        isFrontOperationInvalidatePtrs<LAI_UDEQ, PtrContainer>(
    //            [](LAI_UDEQ & c) { c.emplace(c.begin(), Uncopyable(41)); }, 20);
    //    }

    //    TEST_METHOD(TestInsert)
    //    {
    //        TSC_Insert(lai::deque);

    //        using PtrContainer = std::deque<LAI_UDEQ::pointer>;
    //        isBackOperationInvalidPtrs<LAI_UDEQ, PtrContainer>(
    //            [](LAI_UDEQ & c) { c.insert(c.end(), Uncopyable(42)); }, 20);
    //        isFrontOperationInvalidatePtrs<LAI_UDEQ, PtrContainer>(
    //            [](LAI_UDEQ & c) { c.insert(c.begin(), Uncopyable(41)); }, 20);

    //        using PtrContainer1 = std::deque<LAI_IDEQ::pointer>;
    //        isBackOperationInvalidPtrs<LAI_IDEQ, PtrContainer1>(
    //            [](LAI_IDEQ & c) { int i = 42; c.insert(c.end(), i); }, 20);
    //        isFrontOperationInvalidatePtrs<LAI_IDEQ, PtrContainer1>(
    //            [](LAI_IDEQ & c) { int i = 42; c.insert(c.begin(), i); }, 20);
    //    }

    //    TEST_METHOD(TestInsertCount)
    //    {
    //        TSC_InsertCount(lai::deque, std::deque);

    //        using PtrContainer = std::deque<LAI_IDEQ::pointer>;
    //        isBackOperationInvalidPtrs<LAI_IDEQ, PtrContainer>(
    //            [](LAI_IDEQ & c) { int i = 42; c.insert(c.end(), 1, i); }, 20);
    //        isFrontOperationInvalidatePtrs<LAI_IDEQ, PtrContainer>(
    //            [](LAI_IDEQ & c) { int i = 42; c.insert(c.begin(), 1, i); }, 20);
    //    }

    //    TEST_METHOD(TestInsertInitList)
    //    {
    //        TSC_InsertInitList(lai::deque);

    //        using PtrContainer = std::deque<LAI_IDEQ::pointer>;
    //        isBackOperationInvalidPtrs<LAI_IDEQ, PtrContainer>(
    //            [](LAI_IDEQ & c) { c.insert(c.end(), { 42 }); }, 20);
    //        isFrontOperationInvalidatePtrs<LAI_IDEQ, PtrContainer>(
    //            [](LAI_IDEQ & c) { c.insert(c.begin(), { 42 }); }, 20);
    //    }

    //    TEST_METHOD(TestInsertRange)
    //    {
    //        TSC_InsertRange(lai::deque, std::deque);

    //        using PtrContainer = std::deque<LAI_IDEQ::pointer>;
    //        isBackOperationInvalidPtrs<LAI_IDEQ, PtrContainer>(
    //            [](LAI_IDEQ & c) { auto il = { 42 }; c.insert(c.end(), il.begin(), il.end()); }, 20);
    //        isFrontOperationInvalidatePtrs<LAI_IDEQ, PtrContainer>(
    //            [](LAI_IDEQ & c) { auto il = { 42 }; c.insert(c.begin(), il.begin(), il.end()); }, 20);
    //    }

    //    TEST_METHOD(TestPushBack)
    //    {
    //        TSC_PushBack(lai::deque);

    //        using PtrContainer = std::vector<LAI_IDEQ::pointer>;

    //        isBackOperationInvalidPtrs<LAI_IDEQ, PtrContainer>(
    //            [](LAI_IDEQ & c) { int elem = 42;  c.push_back(elem); });
    //    }

    //    TEST_METHOD(TestMovePushBack)
    //    {
    //        TSC_MovePushBack(lai::deque);

    //        using PtrContainer = std::vector<LAI_UDEQ::pointer>;

    //        isBackOperationInvalidPtrs<LAI_UDEQ, PtrContainer>(
    //            [](LAI_UDEQ & c) { c.push_back(Uncopyable(42)); });
    //    }

    //    TEST_METHOD(TestEmplaceBack)
    //    {
    //        TSC_EmplaceBack(lai::deque);

    //        using PtrContainer = std::vector<LAI_UDEQ::pointer>;

    //        isBackOperationInvalidPtrs<LAI_UDEQ, PtrContainer>(
    //            [](LAI_UDEQ & c) { c.emplace_back(Uncopyable(42)); });
    //    }

    //    TEST_METHOD(TestEmplaceFront)
    //    {
    //        TSC_EmplaceFront(lai::deque);

    //        using PtrContainer = std::deque<LAI_UDEQ::pointer>;

    //        isFrontOperationInvalidatePtrs<LAI_UDEQ, PtrContainer>(
    //            [](LAI_UDEQ & c) { c.emplace_front(Uncopyable(42)); });
    //    }

    //    TEST_METHOD(TestPushFront)
    //    {
    //        TSC_PushFront(lai::deque);

    //        using PtrContainer = std::deque<LAI_IDEQ::pointer>;

    //        isFrontOperationInvalidatePtrs<LAI_IDEQ, PtrContainer>(
    //            [](LAI_IDEQ & c) { int elem = 42; c.push_front(elem); });
    //    }

    //    TEST_METHOD(TestMovePushFront)
    //    {
    //        TSC_MovePushFront(lai::deque);

    //        using PtrContainer = std::deque<LAI_UDEQ::pointer>;

    //        isFrontOperationInvalidatePtrs<LAI_UDEQ, PtrContainer>(
    //            [](LAI_UDEQ & c) { c.push_front(Uncopyable(42)); });
    //    }

    //    TEST_METHOD(TestPopBack)
    //    {
    //        TSC_PopBack(lai::deque, std::deque);

    //        // test: pop_back can't invalidate pointers or references to the rest of the elements.

    //        LAI_IDEQ iDeq;
    //        std::deque<LAI_IDEQ::pointer> pointers;
    //        int size = 15;
    //        for (int i = 0; i != size; ++i)
    //        {
    //            iDeq.push_back(i);
    //            pointers.push_back(&iDeq.back());
    //        }

    //        while (!iDeq.empty())
    //        {
    //            iDeq.pop_back();
    //            pointers.pop_back();
    //            for (int i = 0; i != iDeq.size(); ++i)
    //                IS_TRUE(*pointers[i] == iDeq[i]);
    //        }
    //    }

    //    TEST_METHOD(TestPopFront)
    //    {
    //        TSC_PopFront(lai::deque, std::deque);

    //        // test: pop_front can't invalidate pointers or references to the rest of the elements.

    //        LAI_IDEQ iDeq;
    //        std::deque<LAI_IDEQ::pointer> pointers;
    //        int size = 15;
    //        for (int i = 0; i != size; ++i)
    //        {
    //            iDeq.push_front(i);
    //            pointers.push_front(&iDeq.front());
    //        }

    //        while (!iDeq.empty())
    //        {
    //            iDeq.pop_front();
    //            pointers.pop_front();
    //            for (int i = 0; i != iDeq.size(); ++i)
    //                IS_TRUE(*pointers[i] == iDeq[i]);
    //        }
    //    }

    //    TEST_METHOD(TestSwap)
    //    {
    //        TSC_Swap(lai::deque);
    //        TSC_NonmemberSwap(lai::deque, lai::swap);
    //    }

    //    TEST_METHOD(TestReize)
    //    {
    //        TSC_Resize(lai::deque);
    //    }

    //    TEST_METHOD(TestResizeVal)
    //    {
    //        TSC_ResizeVal(lai::deque);
    //    }

    //    TEST_METHOD(TestClear)
    //    {
    //        LAI_IDEQ iDeq;
    //        iDeq.clear();
    //        IS_TRUE(iDeq.empty());

    //        LAI_SDEQ sDeq(10);
    //        sDeq.clear();
    //        IS_TRUE(sDeq.empty());
    //    }

    //    TEST_METHOD(TestRelationalOperators)
    //    {
    //        TSC_RelationalOperator(lai::deque);
    //    }
    };
}