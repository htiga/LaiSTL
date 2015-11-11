#pragma once

#include "CommonTypes.h"
#include <iterator>
#include <utility>
#include <type_traits>
#include <initializer_list>

namespace lai
{
    namespace details
    {
        enum class TreeColor : char
        {
            RED = 0,
            BLACK = 1
        };


        template<typename TValue>
        struct TreeNode
        {
            using NodePtr = TreeNode *;
            NodePtr parent = nullptr;
            NodePtr left = nullptr;
            NodePtr right = nullptr;
            TreeColor color;
            bool isNil;
            TValue value;
        };


        template<typename TTree, typename TReference, typename TPointer>
        class TreeIterator
        {
            friend TTree;
        public:
            using value_type = typename TTree::value_type;
            using reference = TReference;
            using pointer = TPointer;
            using iterator_category = std::bidirectional_iterator_tag;
            using difference_type = std::ptrdiff_t;
        private:
            using Node = TreeNode<value_type>;
            using NodePtr = typename Node::NodePtr;
            NodePtr myNode = nullptr;
        public:
            constexpr TreeIterator() noexcept = default;

            TreeIterator(NodePtr node) :
                myNode(node) { }

            // Convertion from iterator to const_iterator
            operator TreeIterator<TTree,
                typename TTree::const_reference,
                typename TTree::const_pointer>() const
            {
                return TreeIterator<TTree,
                    typename TTree::const_reference,
                    typename TTree::const_pointer>(myNode);
            }

            reference operator*() const
            {
                return myNode->value;
            }

            pointer operator->() const
            {
                return std::addressof(operator*());
            }

            TreeIterator & operator++()
            {
                // promote myNode to its ceiling

                if (TTree::isNil(myNode))
                {
                    ;   // end(), don't move
                }
                else if (!TTree::isNil(myNode->right))
                {
                    myNode = TTree::getMin(myNode->right);

                }
                else
                {
                    NodePtr parent = myNode->parent;
                    while (!TTree::isNil(parent) && myNode == parent->right)
                    {
                        myNode = parent;
                        parent = parent->parent;
                    }
                    myNode = parent;
                }
                return *this;
            }

            TreeIterator operator++(int)
            {
                auto old = *this;
                ++*this;
                return old;
            }

            TreeIterator & operator--()
            {
                // down myNode to its floor

                if (TTree::isNil(myNode))
                {
                    myNode = myNode->right;     // end(), return right most
                }
                else if (!TTree::isNil(myNode->left))
                {
                    myNode = TTree::getMax(myNode->left);
                }
                else
                {
                    NodePtr parent = myNode->parent;
                    while (!TTree::isNil(parent) && myNode == parent->left)
                    {
                        myNode = parent;
                        parent = parent->parent;
                    }
                    myNode = parent;
                }
                return *this;
            }

            TreeIterator operator--(int)
            {
                auto old = *this;
                --*this;
                return old;
            }

            friend bool operator==(const TreeIterator & lhs, const TreeIterator & rhs)
            {
                return lhs.myNode == rhs.myNode;
            }

            friend bool operator!=(const TreeIterator & lhs, const TreeIterator & rhs)
            {
                return !(lhs == rhs);
            }
        };

        template<bool, typename T1, typename T2>
        struct Select
        {
            using type = T2;
        };

        template<typename T1, typename T2>
        struct Select<true, T1, T2>
        {
            using type = T1;
        };

        template<bool pred, typename T1, typename T2>
        using SelectType = typename Select<pred, T1, T2>::type;

        // TreeTraits must contain following public members :
        //      typedefs : key_type
        //                 key_compare
        //                 value_type 
        //                 value_compare
        //                 allocator_type
        //      static bool isMulti;
        //      static const key_type & getKey(const value_type & value);
        template<typename TreeTraits>
        class Tree
        {
        private:
            using MyTraits = TreeTraits;
        public:
            using allocator_type = typename MyTraits::allocator_type;
            using key_type = typename MyTraits::key_type;
            using value_type = typename MyTraits::value_type;
            using size_type = typename allocator_type::size_type;
            using difference_type = typename allocator_type::difference_type;
            using key_compare = typename MyTraits::key_compare;
            using value_compare = typename MyTraits::value_compare;
            using reference = typename allocator_type::reference;
            using const_reference = typename allocator_type::const_reference;
            using pointer = typename allocator_type::pointer;
            using const_pointer = typename allocator_type::const_pointer;
            using const_iterator = TreeIterator<Tree, const_reference, const_pointer>;
            using iterator = SelectType<std::is_same<key_type, value_type>::value,
                const_iterator,
                TreeIterator<Tree, reference, pointer >> ;
            using reverse_iterator = std::reverse_iterator<iterator>;
            using const_reverse_iterator = std::reverse_iterator<const_iterator>;
            using PairIb = std::pair<iterator, bool>;
            using PairIi = std::pair<iterator, iterator>;
            using PairCc = std::pair<const_iterator, const_iterator>;
        public:
            using Node = TreeNode<value_type>;
            using NodeAlloc = typename allocator_type::template rebind<Node>::other;
            using NodePtr = Node *;
            static NodeAlloc nodeAlloc;
        public:
            explicit Tree(const key_compare & comp) :
                myHead(createHead()),
                myKeyCompare(comp) { }

            Tree() :
                Tree(key_compare()) { }

            template<typename InputIt,
                typename = std::enable_if_t<!std::is_integral<InputIt>::value >>
                Tree(InputIt first, InputIt last, const key_compare & comp = key_compare()) :
                Tree(comp)
            {
                insert(first, last);
            }

            Tree(std::initializer_list<value_type> iList, const key_compare & comp = key_compare()) :
                Tree(comp)
            {
                insert(iList.begin(), iList.end());
            }

            Tree(const Tree & rhs) :
                Tree(rhs.key_comp())
            {
                // todo
            }

            Tree(Tree && rhs)
            {
                // todo
            }

            ~Tree()
            {
                if (myHead)
                {
                    deallocateTree(root());
                    deallocateNode(myHead);
                    mySize = 0;
                    myHead = nullptr;
                }
            }

            // iterators

            iterator begin() noexcept
            {
                return iterator(leftMost());
            }

            const_iterator begin() const noexcept
            {
                return const_iterator(leftMost());
            }

            const_iterator cbegin() const noexcept
            {
                return begin();
            }

            iterator end() noexcept
            {
                return iterator(nil());
            }

            const_iterator end() const noexcept
            {
                return const_iterator(nil());
            }

            const_iterator cend() const noexcept
            {
                return end();
            }

            reverse_iterator rbegin() noexcept
            {
                return reverse_iterator(end());
            }

            const_reverse_iterator rbegin() const noexcept
            {
                return const_reverse_iterator(end());
            }

            const_reverse_iterator crbegin() const noexcept
            {
                return rbegin();
            }

            reverse_iterator rend() noexcept
            {
                return reverse_iterator(begin());
            }

            const_reverse_iterator rend() const noexcept
            {
                return const_reverse_iterator(begin());
            }

            const_reverse_iterator crend() const noexcept
            {
                return rend();
            }

            // comparers

            key_compare key_comp() const
            {
                return myKeyCompare;
            }

            value_compare value_comp() const
            {
                return value_compare(myKeyCompare);
            }

            // capacity

            bool empty() const noexcept
            {
                return size() == 0;
            }

            size_type size() const noexcept
            {
                return mySize;
            }

            // modifiers

            PairIb insert(const value_type & value)
            {
                return insertNoHint(value, Nil());
            }

            PairIb insert(value_type && value)
            {
                return insertNoHint(std::move(value), Nil());
            }

            iterator insert(const_iterator pos, value_type && value)
            {
                return insertHint(pos, std::move(value), Nil());
            }

            iterator insert(const_iterator pos, const value_type & value)
            {
                return insertHint(pos, value, Nil());
            }

            template<typename InputIt,
                typename = std::enable_if_t<!std::is_integral<InputIt>::value >>
                void insert(InputIt first, InputIt last)
            {
                while (first != last)
                {
                    insertHint(end(), *first++, Nil());
                }
            }

            void insert(std::initializer_list<value_type> iList)
            {
                insert(iList.begin(), iList.end());
            }

            template<typename ... Args>
            PairIb emplace(Args && ... args)
            {
                NodePtr newNode = createNode(std::forward<Args>(args)...);
                return insertNoHint(newNode->value, newNode);
            }

            template<typename ... Args>
            iterator emplace_hint(const_iterator hint, Args && ... args)
            {
                NodePtr newNode = createNode(std::forward<Args>(args)...);
                return insertHint(hint, newNode->value, newNode);
            }

            void clear()
            {
                deallocateTree(root());
                mySize = 0;
                root() = nil();
                leftMost() = nil();
                rightMost() = nil();
            }

            static bool isNil(NodePtr node) noexcept
            {
                return node->isNil;
            }

            static NodePtr getMin(NodePtr node) noexcept
            {
                while (!isNil(node->left))
                {
                    node = node->left;
                }
                return node;
            }

            static NodePtr getMax(NodePtr node) noexcept
            {
                while (!isNil(node->right))
                {
                    node = node->right;
                }
                return node;
            }

        private:
            NodePtr allocateNode(size_type count)
            {
                return nodeAlloc.allocate(count);
            }

            void deallocateNode(NodePtr node)
            {
                nodeAlloc.destroy(std::addressof(node->value));
                nodeAlloc.deallocate(node, 1);
            }

            void deallocateTree(NodePtr aRoot)
            {
                while (!isNil(aRoot))
                {
                    deallocateTree(aRoot->left);
                    NodePtr right = aRoot->right;
                    deallocateNode(aRoot);
                    aRoot = right;
                }
            }

            void deallocateNodeIfNotNil(NodePtr node)
            {
                deallocateNode(node);
            }

            void deallocateNodeIfNotNil(Nil)
            {
                /*empty*/
            }

            NodePtr createHead()
            {
                NodePtr newHead = allocateNode(1);
                newHead->parent = newHead;
                newHead->left = newHead;
                newHead->right = newHead;
                newHead->color = TreeColor::BLACK;
                newHead->isNil = true;
                return newHead;
            }

            template<typename ... Args>
            NodePtr createNode(Args && ... args)
            {
                NodePtr newNode = allocateNode(1);
                try
                {
                    nodeAlloc.construct(std::addressof(newNode->value), std::forward<Args>(args)...);
                }
                catch (...)
                {
                    nodeAlloc.deallocate(newNode, 1);
                    throw;
                }
                newNode->left = nil();
                newNode->right = nil();
                newNode->parent = nil();
                newNode->color = TreeColor::RED;
                newNode->isNil = false;
                return newNode;
            }


            template<typename TValue>
            NodePtr createNodeIfNil(NodePtr node, TValue && value)
            {
                return node;
            }

            template<typename TValue>
            NodePtr createNodeIfNil(Nil, TValue && value)
            {
                return createNode(std::forward<TValue>(value));
            }

            // substitute subtree rooted at <oldRoot> with subtree rooted at <newRoot>
            void transplant(NodePtr oldRoot, NodePtr newRoot)
            {
                newRoot->parent = oldRoot->parent;
                if (oldRoot == root())
                {
                    root() = newRoot;
                }
                else if (oldRoot == oldRoot->parent->left)
                {
                    oldRoot->parent->left = newRoot;
                }
                else
                {
                    oldRoot->parent->right = newRoot;
                }
            }

            void LeftRotate(NodePtr node)
            { // promote node->right to the root of the subtree
                NodePtr nRight = node->right;
                node->right = nRight->left;
                if (!isNil(nRight->left))
                {
                    nRight->left->parent = node;
                }
                transplant(node, nRight);
                nRight->left = node;
                node->parent = nRight;
            }

            void RightRotate(NodePtr node)
            { // promote node->left to the root of the subtree
                NodePtr nLeft = node->left;
                node->left = nLeft->right;
                if (!isNil(nLeft->right))
                {
                    nLeft->right->parent = node;
                }
                transplant(node, nLeft);
                nLeft->right = node;
                node->parent = nLeft;
            }

            // try insert before pos, if fail insertNoHint will be called
            template<typename TValue, typename TNode>
            iterator insertHint(const_iterator pos, TValue && value, TNode node)
            {
                if (empty())
                {
                    return insertAt(true, myHead, std::forward<TValue>(value), node);
                }

                if (MyTraits::isMulti)
                {
                    const_iterator before;
                    // try insert before begin()
                    if (pos == begin() && !compareValues(*pos, value))
                    {
                        return insertAt(true, pos.myNode, std::forward<TValue>(value), node);
                    }
                    // try insert before end()
                    else if (pos == end() && !compareValues(value, rightMost()->value))
                    {
                        return insertAt(false, rightMost(), std::forward<TValue>(value), node);
                    }
                    // try insert before <pos>
                    else if (!compareValues(value, *--(before = pos)) && !compareValues(*pos, value))
                    {
                        if (isNil(before.myNode->right))
                            return insertAt(false, before.myNode, std::forward<TValue>(value), node);
                        else
                            return insertAt(true, pos.myNode, std::forward<TValue>(value), node);
                    }
                    else
                    { // bad hint <pos>, call insertNoHint
                        return insertNoHint(std::forward<TValue>(value), node).first;
                    }
                }
                else
                {   // insert only if unique
                    const_iterator before;
                    // try insert before begin()
                    if (pos == begin() && compareValues(value, *pos))
                    {
                        return insertAt(true, pos.myNode, std::forward<TValue>(value), node);
                    }
                    // try insert before end()
                    else if (pos == end() && compareValues(rightMost()->value, value))
                    {
                        return insertAt(false, rightMost(), std::forward<TValue>(value), node);
                    }
                    // try insert before <pos>
                    else if (compareValues(*--(before = pos), value) && compareValues(value, *pos))
                    {
                        if (isNil(before.myNode->right))
                            return insertAt(false, before.myNode, std::forward<TValue>(value), node);
                        else
                            return insertAt(true, pos.myNode, std::forward<TValue>(value), node);
                    }
                    else
                    { // bad hint <pos>, call insertNoHint
                        return insertNoHint(std::forward<TValue>(value), node).first;
                    }
                }
            }

            // insert <node>
            // if <node> is nil then construct it with <value>
            template<typename TValue, typename TNode>
            PairIb insertNoHint(TValue && value, TNode node)
            {
                NodePtr parentNode = nil();
                NodePtr curr = root();
                bool addToLeft = true;  // add to left if empty()
                while (!isNil(curr))
                {   // search the correct parent node of <node>
                    parentNode = curr;
                    addToLeft = compareValues(value, curr->value);
                    curr = addToLeft ? curr->left : curr->right;
                }

                if (MyTraits::isMulti)
                {
                    return PairIb(
                        insertAt(addToLeft, parentNode, std::forward<TValue>(value), node), true);
                }
                else
                {   // insert only if unique
                    iterator parent(parentNode);
                    if (!addToLeft)
                    { // insert after parent, check if compareValues(parent.myNode->value, value)
                        ;
                    }
                    else if (parent == begin())
                    { // insert before parent and parent is begin(), insertion must be unique
                        return PairIb(
                            insertAt(addToLeft, parentNode, std::forward<TValue>(value), node), true);
                    }
                    else
                    { // insert before parent, check if compareValues((--parent.myNode)->value, value)
                        --parent;
                    }

                    if (compareValues(parent.myNode->value, value))
                    {
                        return PairIb(
                            insertAt(addToLeft, parentNode, std::forward<TValue>(value), node), true);
                    }
                    else
                    {
                        deallocateNodeIfNotNil(node);
                        return PairIb(parent, false);
                    }
                }
            }

            // add <node> next to <parentNode>, to left if <addToLeft>
            // construct <node> with <value> if <node> is nil
            // return an iterator points to the <node>
            template<typename TValue, typename TNode>
            iterator insertAt(bool addToLeft, NodePtr parentNode, TValue && value, TNode node)
            {
                NodePtr newNode = createNodeIfNil(node, std::forward<TValue>(value));
                ++mySize;
                // add newNode to its position
                newNode->parent = parentNode;
                if (isNil(parentNode))
                {
                    root() = newNode;
                    leftMost() = newNode;
                    rightMost() = newNode;
                }
                else if (addToLeft)
                {
                    parentNode->left = newNode;
                    if (parentNode == leftMost())
                    {
                        leftMost() = newNode;
                    }
                }
                else
                {
                    parentNode->right = newNode;
                    if (parentNode == rightMost())
                    {
                        rightMost() = newNode;
                    }
                }
                // rebalance and recolor
                while (newNode->parent->color == TreeColor::RED)
                {
                    if (newNode->parent == newNode->parent->parent->left)
                    {
                        NodePtr tmp = newNode->parent->parent->right;
                        if (tmp->color == TreeColor::RED)
                        {
                            newNode->parent->color = TreeColor::BLACK;
                            tmp->color = TreeColor::BLACK;
                            newNode->parent->parent->color = TreeColor::RED;
                            newNode = newNode->parent->parent;
                        }
                        else
                        {
                            if (newNode == newNode->parent->right)
                            {
                                newNode = newNode->parent;
                                LeftRotate(newNode);
                            }
                            newNode->parent->color = TreeColor::BLACK;
                            newNode->parent->parent->color = TreeColor::RED;
                            RightRotate(newNode->parent->parent);
                        }
                    }
                    else
                    {
                        NodePtr tmp = newNode->parent->parent->left;
                        if (tmp->color == TreeColor::RED)
                        {
                            newNode->parent->color = TreeColor::BLACK;
                            tmp->color = TreeColor::BLACK;
                            newNode->parent->parent->color = TreeColor::RED;
                            newNode = newNode->parent->parent;
                        }
                        else
                        {
                            if (newNode == newNode->parent->left)
                            {
                                newNode = newNode->parent;
                                RightRotate(newNode);
                            }
                            newNode->parent->color = TreeColor::BLACK;
                            newNode->parent->parent->color = TreeColor::RED;
                            LeftRotate(newNode->parent->parent);
                        }
                    }
                }
                root()->color = TreeColor::BLACK;
                return iterator(newNode);
            }

            bool compareValues(const value_type & lhs, const value_type & rhs) const
            {
                return key_comp()(MyTraits::getKey(lhs), MyTraits::getKey(rhs));
            }

            const NodePtr & nil() const noexcept
            {
                return myHead;
            }

            NodePtr & root() const noexcept
            {
                return myHead->parent;
            }

            NodePtr & leftMost() const noexcept
            {
                return myHead->left;
            }

            NodePtr & rightMost() const noexcept
            {
                return myHead->right;
            }
        private:
            // myHead->parent points to the root, or itself if empty()
            // myHead->left points to the minimum node, or itself if empty()
            // myHead->right points to the maximum node, or itself if empty()
            // myHead->isNil is true, and is served as the Nil node.
            // myHead->color is black
            NodePtr myHead = nullptr;
            size_type mySize = 0;
            key_compare myKeyCompare;   // todo : empty base optimization
        }; // template class Tree

           // static definitions

        template<typename TreeTraits>
        typename Tree<TreeTraits>::NodeAlloc
            Tree<TreeTraits>::nodeAlloc;
    }
}