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
        TEST_METHOD(TestCategory)
        {
            IS_TRUE(IteratorCategoryTraits<LAI_IDEQ::iterator>::isRandomAccess);
            IS_TRUE(IteratorCategoryTraits<LAI_IDEQ::const_iterator>::isRandomAccess);
            IS_TRUE(IteratorCategoryTraits<LAI_IDEQ::reverse_iterator>::isRandomAccess);
            IS_TRUE(IteratorCategoryTraits<LAI_IDEQ::const_reverse_iterator>::isRandomAccess);
        }

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

        TEST_METHOD(TestFront)
        {
            TSC_Front(lai::deque);
        }

        TEST_METHOD(TestBack)
        {
            TSC_Back(lai::deque);
        }

        TEST_METHOD(TestCapacityOperations)
        {
            LAI_IDEQ iDeq;
            iDeq.clear();
            IS_TRUE(iDeq.empty());
            IS_TRUE(iDeq.size() == 0);

            LAI_SDEQ sDeq(10);
            sDeq.clear();
            IS_TRUE(sDeq.empty());
            IS_TRUE(iDeq.size() == 0);
        }

        template<typename Container, typename PtrContainer, typename InsertEnd>
        void isInsertEndInvalidatePtrs(InsertEnd insertEnd, const int performedTimes = 100)
        {
            Container container;
            PtrContainer ptrs;
            for (int i = 0; i != performedTimes; ++i)
            {
                insertEnd(container);
                ptrs.push_back(&container[i]);
            }
            insertEnd(container);

            for (int i = 0; i != performedTimes; ++i)
            {
                IS_TRUE(*ptrs[i] == container[i]);
            }
        }

        template<typename Container, typename PtrContainer, typename InsertBegin>
        void isInsertBeginInvalidatePtrs(InsertBegin insertBegin, const int performedTimes = 100)
        {
            Container container;
            PtrContainer ptrs;
            for (int i = 0; i != performedTimes; ++i)
            {
                insertBegin(container);
                ptrs.push_front(&container[i]);
            }
            insertBegin(container);

            for (int i = 0; i != performedTimes; ++i)
            {
                IS_TRUE(*ptrs[i] == container[i + 1]);
            }
        }

        TEST_METHOD(TestInsertLvalue)
        {
            TSC_InsertLvalue(lai::deque);


            // test if insert begin() or end() invalidate pointers.

            using PtrContainer = std::deque<LAI_SDEQ::pointer>;

            auto insertEnd = [](LAI_SDEQ & c)
            {
                std::string i = "lai";
                c.insert(c.end(), i);
            };
            isInsertEndInvalidatePtrs<LAI_SDEQ, PtrContainer>(insertEnd, 10);

            auto insertBegin = [](LAI_SDEQ & c)
            {
                std::string i = "lai";
                c.insert(c.begin(), i);
            };
            isInsertBeginInvalidatePtrs<LAI_SDEQ, PtrContainer>(insertBegin, 10);
        }

        TEST_METHOD(TestInsertRvalue)
        {
            TSC_InsertRvalue(lai::deque);

            // test if insert begin() or end() invalidate pointers.

            using PtrContainer = std::deque<LAI_UDEQ::pointer>;

            auto insertEnd = [](LAI_UDEQ & c)
            {
                c.insert(c.end(), Uncopyable(42));
            };
            isInsertEndInvalidatePtrs<LAI_UDEQ, PtrContainer>(insertEnd, 10);

            auto insertBegin = [](LAI_UDEQ & c)
            {
                c.insert(c.begin(), Uncopyable(42));
            };
            isInsertBeginInvalidatePtrs<LAI_UDEQ, PtrContainer>(insertBegin, 10);
        }

        TEST_METHOD(TestInsertCount)
        {
            TSC_InsertCount(lai::deque);

            // test if insert begin() or end() invalidate pointers.

            using PtrContainer = std::deque<LAI_SDEQ::pointer>;

            auto insertEnd = [](LAI_SDEQ & c)
            {
                std::string i = "lai";
                c.insert(c.end(), 2, i);
            };
            isInsertEndInvalidatePtrs<LAI_SDEQ, PtrContainer>(insertEnd, 10);

            auto insertBegin = [](LAI_SDEQ & c)
            {
                std::string i = "lai";
                c.insert(c.begin(), 2, i);
            };
            isInsertBeginInvalidatePtrs<LAI_SDEQ, PtrContainer>(insertBegin, 10);
        }

        TEST_METHOD(TestInsertRange)
        {
            TSC_InsertRange(lai::deque);

            // test if insert begin() or end() invalidate pointers.

            using PtrContainer = std::deque<LAI_SDEQ::pointer>;

            auto insertEnd = [](LAI_SDEQ & c)
            {
                S_IL il = { std::string("lai") };
                c.insert(c.end(), il.begin(), il.end());
            };
            isInsertEndInvalidatePtrs<LAI_SDEQ, PtrContainer>(insertEnd, 10);

            auto insertBegin = [](LAI_SDEQ & c)
            {
                S_IL il = { std::string("lai") };
                c.insert(c.begin(), il.begin(), il.end());
            };
            isInsertBeginInvalidatePtrs<LAI_SDEQ, PtrContainer>(insertBegin, 10);
        }

        TEST_METHOD(TestInsertInitList)
        {
            TSC_InsertInitList(lai::deque);

            // test if insert begin() or end() invalidate pointers.

            using PtrContainer = std::deque<LAI_SDEQ::pointer>;

            auto insertEnd = [](LAI_SDEQ & c)
            {
                c.insert(c.end(), { std::string("laistl") });
            };
            isInsertEndInvalidatePtrs<LAI_SDEQ, PtrContainer>(insertEnd, 10);

            auto insertBegin = [](LAI_SDEQ & c)
            {
                c.insert(c.begin(), { std::string("laistl") });
            };
            isInsertBeginInvalidatePtrs<LAI_SDEQ, PtrContainer>(insertBegin, 10);
        }

        TEST_METHOD(TestEmplace)
        {
            TSC_Emplace(lai::deque);

            // test if insert begin() or end() invalidate pointers.

            using PtrContainer = std::deque<LAI_UDEQ::pointer>;

            auto insertBegin = [](LAI_UDEQ & c)
            {
                c.emplace(c.begin(), Uncopyable(42));
            };
            isInsertBeginInvalidatePtrs<LAI_UDEQ, PtrContainer>(insertBegin, 10);

            auto insertEnd = [](LAI_UDEQ & c)
            {
                c.emplace(c.end(), Uncopyable(42));
            };
            isInsertEndInvalidatePtrs<LAI_UDEQ, PtrContainer>(insertEnd, 10);
        }

        template<typename EraseBegin>
        void isEraseBeginInvalidatePtrs(EraseBegin eraseBegin)
        {
            S_IL data = { "", "a", "bc", "def", "hijk", "lmnop", "lai", "stl", "" };
            STD_SVEC values(data);
            LAI_SDEQ c = (data);
            std::vector<LAI_SDEQ::pointer> pointers;

            for (auto & val : c)
            {
                pointers.push_back(std::addressof(val));
            }

            while (!c.empty())
            {
                eraseBegin(c);
                pointers.erase(pointers.begin());
                values.erase(values.begin());
                for (int i = 0; i != pointers.size(); ++i)
                {
                    IS_TRUE(*pointers[i] == values[i]);
                }
            }
        }

        template<typename EraseEnd>
        void isEraseEndInvalidatePtrs(EraseEnd eraseEnd)
        {
            S_IL data = { "", "a", "bc", "def", "hijk", "lmnop", "lai", "stl", "" };
            STD_SVEC values(data);
            LAI_SDEQ c = (data);
            std::vector<LAI_SDEQ::pointer> pointers;

            for (auto & val : c)
            {
                pointers.push_back(std::addressof(val));
            }

            while (!c.empty())
            {
                eraseEnd(c);
                values.erase(values.end() - 1);
                pointers.erase(pointers.end() - 1);
                for (int i = 0; i != pointers.size(); ++i)
                {
                    IS_TRUE(*pointers[i] == values[i]);
                }
            }
        }

        TEST_METHOD(TestErase)
        {
            TSC_Erase(lai::deque);

            auto eraseBegin = [](LAI_SDEQ & c)
            {
                c.erase(c.begin());
            };
            isEraseBeginInvalidatePtrs(eraseBegin);

            auto eraseEnd = [](LAI_SDEQ & c)
            {
                c.erase(c.end() - 1);
            };
            isEraseEndInvalidatePtrs(eraseEnd);
        }

        TEST_METHOD(TestEraseRange)
        {
            TSC_EraseRange(lai::deque);

            auto eraseBegin = [](LAI_SDEQ & c)
            {
                c.erase(c.begin(), c.begin() + 1);
            };
            isEraseBeginInvalidatePtrs(eraseBegin);

            auto eraseEnd = [](LAI_SDEQ & c)
            {
                c.erase(c.end() - 1, c.end());
            };
            isEraseEndInvalidatePtrs(eraseEnd);
        }

        TEST_METHOD(TestPushBackLvalue)
        {
            TSC_PushBackLvalue(lai::deque);

            using PtrContainer = std::vector<LAI_SDEQ::pointer>;
            auto pushBack = [](LAI_SDEQ & c)
            {
                std::string str = "lai";
                c.push_back(str);
            };
            isInsertEndInvalidatePtrs<LAI_SDEQ, PtrContainer>(pushBack);
        }

        TEST_METHOD(TestPushBackRvalue)
        {
            TSC_PushBackRvalue(lai::deque);

            using PtrContainer = std::vector<LAI_UDEQ::pointer>;
            auto pushBack = [](LAI_UDEQ & c)
            {
                c.push_back(Uncopyable(42));
            };
            isInsertEndInvalidatePtrs<LAI_UDEQ, PtrContainer>(pushBack);
        }

        TEST_METHOD(TestEmplaceBack)
        {
            TSC_EmplaceBack(lai::deque);

            using PtrContainer = std::vector<LAI_UDEQ::pointer>;
            auto emplaceBack = [](LAI_UDEQ & c)
            {
                c.emplace_back(Uncopyable(42));
            };
            isInsertEndInvalidatePtrs<LAI_UDEQ, PtrContainer>(emplaceBack);
        }

        TEST_METHOD(TestEmplaceFront)
        {
            TSC_EmplaceFront(lai::deque);

            using PtrContainer = std::deque<LAI_UDEQ::pointer>;
            auto empalceFront = [](LAI_UDEQ & c)
            {
                c.emplace_front(Uncopyable(42));
            };
            isInsertBeginInvalidatePtrs<LAI_UDEQ, PtrContainer>(empalceFront);
        }

        TEST_METHOD(TestPushFrontLvalue)
        {
            TSC_PushFrontLvalue(lai::deque);

            using PtrContainer = std::deque<LAI_SDEQ::pointer>;
            auto pushFront = [](LAI_SDEQ & c)
            {
                std::string str = "lai";
                c.push_front(str);
            };
            isInsertBeginInvalidatePtrs<LAI_SDEQ, PtrContainer>(pushFront);
        }

        TEST_METHOD(TestPushFrontRvalue)
        {
            TSC_PushFrontRvalue(lai::deque);

            using PtrContainer = std::deque<LAI_UDEQ::pointer>;
            auto pushFront = [](LAI_UDEQ & c)
            {
                c.push_front(Uncopyable(42));
            };
            isInsertBeginInvalidatePtrs<LAI_UDEQ, PtrContainer>(pushFront);
        }

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

    //    TEST_METHOD(TestRelationalOperators)
    //    {
    //        TSC_RelationalOperator(lai::deque);
    //    }
    };
}