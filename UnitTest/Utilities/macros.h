#pragma once
#include "stdafx.h"
#include "../Utilities/Uncopyable.h"
#include "CppUnitTest.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
#include <vector>
#include <string>
#include <initializer_list>

#define IS_TRUE Assert::IsTrue
#define IS_FALSE Assert::IsFalse

#define STD_IVEC std::vector<int>
#define STD_SVEC std::vector<std::string>
#define STD_UVEC std::vector<Uncopyable>

#define I_IL std::initializer_list<int>
#define S_IL std::initializer_list<std::string>
#define U_IL std::initializer_list<Uncopyable>