// https://github.com/apriorit/gmock-global
// https://github.com/apriorit/gmock-global-sample/blob/master/src/sample/main.cpp
// https://www.codeproject.com/Tips/1262469/Gmock-Enhancements-Mocking-Global-Functions-and-Me

#pragma once
#include <memory>

// --std=c++11 option for compile.
#if __cplusplus >= 201103L
//#warning "You can use gmock-global.h directly!"
#endif

//
// Mock classes for different argument count definitions
//

#define MOCK_GLOBAL_CHECK_INIT(Method)                                                \
    if (!gmock_globalmock_##Method##_instance.get())                                        \
    {                                                                                 \
        throw std::logic_error("You forgot to call EXPECT_GLOBAL_CALL for " #Method); \
    }

//
// Mock class and macroses for 0 arguments global function
//
#define MOCK_GLOBAL_FUNC0_(tn, constness, ct, Method, ...) \
class gmock_globalmock_##Method { \
public:\
  gmock_globalmock_##Method(const char* tag) : m_tag(tag) {}  \
  const char* const m_tag; \
  GMOCK_METHOD0_(tn, constness, ct, Method, __VA_ARGS__);\
}; \
static std::auto_ptr< gmock_globalmock_##Method > gmock_globalmock_##Method##_instance;\
static inline GMOCK_RESULT_(tn, __VA_ARGS__) ct Method() constness { \
       MOCK_GLOBAL_CHECK_INIT(Method); \
       return gmock_globalmock_##Method##_instance->Method();\
      }\

#define MOCK_GLOBAL_FUNC0(m, ...) MOCK_GLOBAL_FUNC0_(, , , m, __VA_ARGS__)
#define MOCK_GLOBAL_FUNC0_WITH_CALLTYPE(ct, m, ...) MOCK_GLOBAL_FUNC0_(, , ct, m, __VA_ARGS__)

//
// Mock class and macroses for 1 arguments global function
//
#define MOCK_GLOBAL_FUNC1_(tn, constness, ct, Method, ...) \
class gmock_globalmock_##Method { \
public:\
  gmock_globalmock_##Method(const char* tag) : m_tag(tag) {}  \
  const char* const m_tag; \
  GMOCK_METHOD1_(tn, constness, ct, Method, __VA_ARGS__);\
}; \
static std::auto_ptr< gmock_globalmock_##Method > gmock_globalmock_##Method##_instance;\
static inline GMOCK_RESULT_(tn, __VA_ARGS__) ct Method( \
      GMOCK_ARG_(tn, 1, __VA_ARGS__) gmock_a1) constness { \
       MOCK_GLOBAL_CHECK_INIT(Method); \
       return gmock_globalmock_##Method##_instance->Method(gmock_a1);\
      }\

#define MOCK_GLOBAL_FUNC1(m, ...) MOCK_GLOBAL_FUNC1_(, , , m, __VA_ARGS__)
#define MOCK_GLOBAL_FUNC1_WITH_CALLTYPE(ct, m, ...) MOCK_GLOBAL_FUNC1_(, , ct, m, __VA_ARGS__)

//
// Mock class and macroses for 2 arguments global function
//
#define MOCK_GLOBAL_FUNC2_(tn, constness, ct, Method, ...) \
class gmock_globalmock_##Method { \
public:\
  gmock_globalmock_##Method(const char* tag) : m_tag(tag) {}  \
  const char* const m_tag; \
  GMOCK_METHOD2_(tn, constness, ct, Method, __VA_ARGS__);\
}; \
static std::auto_ptr< gmock_globalmock_##Method > gmock_globalmock_##Method##_instance;\
static inline GMOCK_RESULT_(tn, __VA_ARGS__) ct Method( \
      GMOCK_ARG_(tn, 1, __VA_ARGS__) gmock_a1, \
      GMOCK_ARG_(tn, 2, __VA_ARGS__) gmock_a2) constness { \
       MOCK_GLOBAL_CHECK_INIT(Method); \
       return gmock_globalmock_##Method##_instance->Method(gmock_a1, gmock_a2);\
      }\

#define MOCK_GLOBAL_FUNC2(m, ...) MOCK_GLOBAL_FUNC2_(, , , m, __VA_ARGS__)
#define MOCK_GLOBAL_FUNC2_WITH_CALLTYPE(ct, m, ...) MOCK_GLOBAL_FUNC2_(, , ct, m, __VA_ARGS__)

//
// Mock class and macroses for 3 arguments global function
//
#define MOCK_GLOBAL_FUNC3_(tn, constness, ct, Method, ...) \
class gmock_globalmock_##Method { \
public:\
  gmock_globalmock_##Method(const char* tag) : m_tag(tag) {}  \
  const char* const m_tag; \
  GMOCK_METHOD3_(tn, constness, ct, Method, __VA_ARGS__);\
}; \
static std::auto_ptr< gmock_globalmock_##Method > gmock_globalmock_##Method##_instance;\
static inline GMOCK_RESULT_(tn, __VA_ARGS__) ct Method( \
      GMOCK_ARG_(tn, 1, __VA_ARGS__) gmock_a1, \
      GMOCK_ARG_(tn, 2, __VA_ARGS__) gmock_a2, \
      GMOCK_ARG_(tn, 3, __VA_ARGS__) gmock_a3) constness { \
       MOCK_GLOBAL_CHECK_INIT(Method); \
       return gmock_globalmock_##Method##_instance->Method(gmock_a1, gmock_a2, gmock_a3);\
      }\

#define MOCK_GLOBAL_FUNC3(m, ...) MOCK_GLOBAL_FUNC3_(, , , m, __VA_ARGS__)
#define MOCK_GLOBAL_FUNC3_WITH_CALLTYPE(ct, m, ...) MOCK_GLOBAL_FUNC3_(, , ct, m, __VA_ARGS__)

//
// Mock class and macroses for 4 arguments global function
//
#define MOCK_GLOBAL_FUNC4_(tn, constness, ct, Method, ...) \
class gmock_globalmock_##Method { \
public:\
  gmock_globalmock_##Method(const char* tag) : m_tag(tag) {}  \
  const char* const m_tag; \
  GMOCK_METHOD4_(tn, constness, ct, Method, __VA_ARGS__);\
}; \
static std::auto_ptr< gmock_globalmock_##Method > gmock_globalmock_##Method##_instance;\
static inline GMOCK_RESULT_(tn, __VA_ARGS__) ct Method( \
      GMOCK_ARG_(tn, 1, __VA_ARGS__) gmock_a1, \
      GMOCK_ARG_(tn, 2, __VA_ARGS__) gmock_a2, \
      GMOCK_ARG_(tn, 3, __VA_ARGS__) gmock_a3, \
      GMOCK_ARG_(tn, 4, __VA_ARGS__) gmock_a4 ) constness { \
       MOCK_GLOBAL_CHECK_INIT(Method); \
       return gmock_globalmock_##Method##_instance->Method(gmock_a1, gmock_a2, gmock_a3, gmock_a4);\
      }\

#define MOCK_GLOBAL_FUNC4(m, ...) MOCK_GLOBAL_FUNC4_(, , , m, __VA_ARGS__)
#define MOCK_GLOBAL_FUNC4_WITH_CALLTYPE(ct, m, ...) MOCK_GLOBAL_FUNC4_(, , ct, m, __VA_ARGS__)

//
// Mock class and macroses for 5 arguments global function
//
#define MOCK_GLOBAL_FUNC5_(tn, constness, ct, Method, ...) \
class gmock_globalmock_##Method { \
public:\
  gmock_globalmock_##Method(const char* tag) : m_tag(tag) {}  \
  const char* const m_tag; \
  GMOCK_METHOD5_(tn, constness, ct, Method, __VA_ARGS__);\
}; \
static std::auto_ptr< gmock_globalmock_##Method > gmock_globalmock_##Method##_instance;\
static inline GMOCK_RESULT_(tn, __VA_ARGS__) ct Method( \
      GMOCK_ARG_(tn, 1, __VA_ARGS__) gmock_a1, \
      GMOCK_ARG_(tn, 2, __VA_ARGS__) gmock_a2, \
      GMOCK_ARG_(tn, 3, __VA_ARGS__) gmock_a3, \
      GMOCK_ARG_(tn, 4, __VA_ARGS__) gmock_a4, \
      GMOCK_ARG_(tn, 5, __VA_ARGS__) gmock_a5) constness { \
       MOCK_GLOBAL_CHECK_INIT(Method); \
       return gmock_globalmock_##Method##_instance->Method(gmock_a1, gmock_a2, gmock_a3, gmock_a4, gmock_a5);\
      }\

#define MOCK_GLOBAL_FUNC5(m, ...) MOCK_GLOBAL_FUNC5_(, , , m, __VA_ARGS__)
#define MOCK_GLOBAL_FUNC5_WITH_CALLTYPE(ct, m, ...) MOCK_GLOBAL_FUNC5_(, , ct, m, __VA_ARGS__)

//
// Mock class and macroses for 6 arguments global function
//
#define MOCK_GLOBAL_FUNC6_(tn, constness, ct, Method, ...) \
class gmock_globalmock_##Method { \
public:\
  gmock_globalmock_##Method(const char* tag) : m_tag(tag) {}  \
  const char* const m_tag; \
  GMOCK_METHOD6_(tn, constness, ct, Method, __VA_ARGS__);\
}; \
static std::auto_ptr< gmock_globalmock_##Method > gmock_globalmock_##Method##_instance;\
static inline GMOCK_RESULT_(tn, __VA_ARGS__) ct Method( \
      GMOCK_ARG_(tn, 1, __VA_ARGS__) gmock_a1, \
      GMOCK_ARG_(tn, 2, __VA_ARGS__) gmock_a2, \
      GMOCK_ARG_(tn, 3, __VA_ARGS__) gmock_a3, \
      GMOCK_ARG_(tn, 4, __VA_ARGS__) gmock_a4, \
      GMOCK_ARG_(tn, 5, __VA_ARGS__) gmock_a5, \
      GMOCK_ARG_(tn, 6, __VA_ARGS__) gmock_a6) constness { \
       MOCK_GLOBAL_CHECK_INIT(Method); \
       return gmock_globalmock_##Method##_instance->Method(gmock_a1, gmock_a2, gmock_a3, gmock_a4, gmock_a5, gmock_a6);\
      }\

#define MOCK_GLOBAL_FUNC6(m, ...) MOCK_GLOBAL_FUNC6_(, , , m, __VA_ARGS__)
#define MOCK_GLOBAL_FUNC6_WITH_CALLTYPE(ct, m, ...) MOCK_GLOBAL_FUNC6_(, , ct, m, __VA_ARGS__)

//
// Mock class and macroses for 7 arguments global function
//
#define MOCK_GLOBAL_FUNC7_(tn, constness, ct, Method, ...) \
class gmock_globalmock_##Method { \
public:\
  gmock_globalmock_##Method(const char* tag) : m_tag(tag) {}  \
  const char* const m_tag; \
  GMOCK_METHOD7_(tn, constness, ct, Method, __VA_ARGS__);\
}; \
static std::auto_ptr< gmock_globalmock_##Method > gmock_globalmock_##Method##_instance;\
static inline GMOCK_RESULT_(tn, __VA_ARGS__) ct Method( \
      GMOCK_ARG_(tn, 1, __VA_ARGS__) gmock_a1, \
      GMOCK_ARG_(tn, 2, __VA_ARGS__) gmock_a2, \
      GMOCK_ARG_(tn, 3, __VA_ARGS__) gmock_a3, \
      GMOCK_ARG_(tn, 4, __VA_ARGS__) gmock_a4, \
      GMOCK_ARG_(tn, 5, __VA_ARGS__) gmock_a5, \
      GMOCK_ARG_(tn, 6, __VA_ARGS__) gmock_a6, \
      GMOCK_ARG_(tn, 7, __VA_ARGS__) gmock_a7) constness { \
       MOCK_GLOBAL_CHECK_INIT(Method); \
       return gmock_globalmock_##Method##_instance->Method(gmock_a1, gmock_a2, gmock_a3, gmock_a4, gmock_a5, gmock_a6, gmock_a7);\
      }\

#define MOCK_GLOBAL_FUNC7(m, ...) MOCK_GLOBAL_FUNC7_(, , , m, __VA_ARGS__)
#define MOCK_GLOBAL_FUNC7_WITH_CALLTYPE(ct, m, ...) MOCK_GLOBAL_FUNC7_(, , ct, m, __VA_ARGS__)

//
// Mock class and macroses for 8 arguments global function
//
#define MOCK_GLOBAL_FUNC8_(tn, constness, ct, Method, ...) \
class gmock_globalmock_##Method { \
public:\
  gmock_globalmock_##Method(const char* tag) : m_tag(tag) {}  \
  const char* const m_tag; \
  GMOCK_METHOD8_(tn, constness, ct, Method, __VA_ARGS__);\
}; \
static std::auto_ptr< gmock_globalmock_##Method > gmock_globalmock_##Method##_instance;\
static inline GMOCK_RESULT_(tn, __VA_ARGS__) ct Method( \
      GMOCK_ARG_(tn, 1, __VA_ARGS__) gmock_a1, \
      GMOCK_ARG_(tn, 2, __VA_ARGS__) gmock_a2, \
      GMOCK_ARG_(tn, 3, __VA_ARGS__) gmock_a3, \
      GMOCK_ARG_(tn, 4, __VA_ARGS__) gmock_a4, \
      GMOCK_ARG_(tn, 5, __VA_ARGS__) gmock_a5, \
      GMOCK_ARG_(tn, 6, __VA_ARGS__) gmock_a6, \
      GMOCK_ARG_(tn, 7, __VA_ARGS__) gmock_a7, \
      GMOCK_ARG_(tn, 8, __VA_ARGS__) gmock_a8) constness { \
       MOCK_GLOBAL_CHECK_INIT(Method); \
       return gmock_globalmock_##Method##_instance->Method(gmock_a1, gmock_a2, gmock_a3, gmock_a4, gmock_a5, gmock_a6, gmock_a7, gmock_a8);\
      }\

#define MOCK_GLOBAL_FUNC8(m, ...) MOCK_GLOBAL_FUNC8_(, , , m, __VA_ARGS__)
#define MOCK_GLOBAL_FUNC8_WITH_CALLTYPE(ct, m, ...) MOCK_GLOBAL_FUNC8_(, , ct, m, __VA_ARGS__)

//
// Mock class and macroses for 9 arguments global function
//
#define MOCK_GLOBAL_FUNC9_(tn, constness, ct, Method, ...) \
class gmock_globalmock_##Method { \
public:\
  gmock_globalmock_##Method(const char* tag) : m_tag(tag) {}  \
  const char* const m_tag; \
  GMOCK_METHOD9_(tn, constness, ct, Method, __VA_ARGS__);\
}; \
static std::auto_ptr< gmock_globalmock_##Method > gmock_globalmock_##Method##_instance;\
static inline GMOCK_RESULT_(tn, __VA_ARGS__) ct Method( \
      GMOCK_ARG_(tn, 1, __VA_ARGS__) gmock_a1, \
      GMOCK_ARG_(tn, 2, __VA_ARGS__) gmock_a2, \
      GMOCK_ARG_(tn, 3, __VA_ARGS__) gmock_a3, \
      GMOCK_ARG_(tn, 4, __VA_ARGS__) gmock_a4, \
      GMOCK_ARG_(tn, 5, __VA_ARGS__) gmock_a5, \
      GMOCK_ARG_(tn, 6, __VA_ARGS__) gmock_a6, \
      GMOCK_ARG_(tn, 7, __VA_ARGS__) gmock_a7, \
      GMOCK_ARG_(tn, 8, __VA_ARGS__) gmock_a8, \
      GMOCK_ARG_(tn, 9, __VA_ARGS__) gmock_a9) constness { \
       MOCK_GLOBAL_CHECK_INIT(Method); \
       return gmock_globalmock_##Method##_instance->Method(gmock_a1, gmock_a2, gmock_a3, gmock_a4, gmock_a5, gmock_a6, gmock_a7, gmock_a8, gmock_a9);\
      }\

#define MOCK_GLOBAL_FUNC9(m, ...) MOCK_GLOBAL_FUNC9_(, , , m, __VA_ARGS__)
#define MOCK_GLOBAL_FUNC9_WITH_CALLTYPE(ct, m, ...) MOCK_GLOBAL_FUNC9_(, , ct, m, __VA_ARGS__)

//
// Mock class and macroses for 10 arguments global function
//
#define MOCK_GLOBAL_FUNC10_(tn, constness, ct, Method, ...) \
class gmock_globalmock_##Method { \
public:\
  gmock_globalmock_##Method(const char* tag) : m_tag(tag) {}  \
  const char* const m_tag; \
  GMOCK_METHOD10_(tn, constness, ct, Method, __VA_ARGS__);\
}; \
static std::auto_ptr< gmock_globalmock_##Method > gmock_globalmock_##Method##_instance;\
static inline GMOCK_RESULT_(tn, __VA_ARGS__) ct Method( \
      GMOCK_ARG_(tn, 1, __VA_ARGS__) gmock_a1, \
      GMOCK_ARG_(tn, 2, __VA_ARGS__) gmock_a2, \
      GMOCK_ARG_(tn, 3, __VA_ARGS__) gmock_a3, \
      GMOCK_ARG_(tn, 4, __VA_ARGS__) gmock_a4, \
      GMOCK_ARG_(tn, 5, __VA_ARGS__) gmock_a5, \
      GMOCK_ARG_(tn, 6, __VA_ARGS__) gmock_a6, \
      GMOCK_ARG_(tn, 7, __VA_ARGS__) gmock_a7, \
      GMOCK_ARG_(tn, 8, __VA_ARGS__) gmock_a8, \
      GMOCK_ARG_(tn, 9, __VA_ARGS__) gmock_a9, \
      GMOCK_ARG_(tn, 10, __VA_ARGS__) gmock_a10) constness { \
       MOCK_GLOBAL_CHECK_INIT(Method); \
       return gmock_globalmock_##Method##_instance->Method(gmock_a1, gmock_a2, gmock_a3, gmock_a4, gmock_a5, gmock_a6, gmock_a7, gmock_a8, gmock_a9, gmock_a10);\
      }\

#define MOCK_GLOBAL_FUNC10(m, ...) MOCK_GLOBAL_FUNC10_(, , , m, __VA_ARGS__)
#define MOCK_GLOBAL_FUNC10_WITH_CALLTYPE(ct, m, ...) MOCK_GLOBAL_FUNC10_(, , ct, m, __VA_ARGS__)

//
// Code for deleting mock objects generated for global function in the end of each test.
//
template <typename T>
class GlobalMockDeleter
{
public:
    GlobalMockDeleter(std::auto_ptr<T>& ref) : m_mockReference(ref) {}

    ~GlobalMockDeleter()
    {
        m_mockReference.reset();
    }

private:
    std::auto_ptr<T>& m_mockReference;
};

//
// Mock call method
//
#define GLOBAL_MOCK_DELETER_NAME3(x, y)     x##y
#define GLOBAL_MOCK_DELETER_NAME2(x, y)     GLOBAL_MOCK_DELETER_NAME3(x, y)
#define GLOBAL_MOCK_DELETER_NAME(x)         GLOBAL_MOCK_DELETER_NAME2(x, __COUNTER__)

#define GLOBAL_MOCK_TYPE(name)              gmock_globalmock_##name
#define GLOBAL_MOCK_INSTANCE(name)          gmock_globalmock_##name##_instance

#define GLOBAL_MOCK_CALL(name, method, callType) \
GlobalMockDeleter<GLOBAL_MOCK_TYPE(name)> GLOBAL_MOCK_DELETER_NAME(mock_deleter)(GLOBAL_MOCK_INSTANCE(name));\
if (!GLOBAL_MOCK_INSTANCE(name).get() ||  0 != strcmp(GLOBAL_MOCK_INSTANCE(name)->m_tag, __FUNCTION__)) {\
    GLOBAL_MOCK_INSTANCE(name).reset(new GLOBAL_MOCK_TYPE(name)(__FUNCTION__));\
}\
callType(*GLOBAL_MOCK_INSTANCE(name), method)

#define EXPECT_GLOBAL_CALL(name, method)    GLOBAL_MOCK_CALL(name, method, EXPECT_CALL)
#define ON_GLOBAL_CALL(name, method)        GLOBAL_MOCK_CALL(name, method, ON_CALL)

