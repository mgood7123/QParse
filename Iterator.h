#ifndef CPP_ITERATOR
#define CPP_ITERATOR

#include "framework_defines.h"

#include <functional>
#include CPP_RULES____STRING_INCLUDE
#include CPP_RULES____VECTOR_INCLUDE
#include CPP_RULES____COUT_INCLUDE
#include <optional>
#include <stdexcept>

// sub iterators
//
// parent: 12345
// child : 987
//
// child sub iterator of parent from '1' to '3'
// parent:                    1          3 4 5
//                            ^          ^ ^
// child :                    ^  9 8 7   ^ ^
//                            ^  ^ ^ ^ ^ ^ ^
// child begin----------------^--^ ^ ^ ^ ^ ^ // '9' from itself, begin returns the same as if next at begin
// child end------------------^--^-^-^-^ ^ ^ // END
// child previous at begin----^  ^ ^ ^   ^ ^ // '1' from parent
// child next at begin-----------^ ^ ^   ^ ^ // '9' from itself, next at begin must return begin
// child next----------------------^ ^   ^ ^ // '8' from itself
// child next------------------------^   ^ ^ // '7' from itself
// child next at end-----------------^---^ ^ // '3' from parent
// child next------------------------^---^-^ // '4' from parent, returns iterator and increases it
// child previous--------------------^---^-^ // '4' from parent, decreases iterator and returns it
// child previous--------------------^---^   // '3' from parent, decreases iterator and returns it
// child previous--------------------^       // '7' from itself, decreases iterator and returns it

namespace CPP {
    class Iterator {
#ifdef GTEST_API_
    public:
#endif
        struct SaveInfo {
            CPP_RULES____STRING::difference_type iteratorCurrent;
            CPP_RULES____STRING::difference_type line_start;
            CPP_RULES____STRING::difference_type line_end;
            uint64_t current_line = 1;
            uint64_t current_column = 1;
            CPP_RULES____CHAR current_char = '\0';
        };
        struct Info {
            CPP_RULES____STRING::const_iterator iteratorCurrent;
            CPP_RULES____STRING::const_iterator line_start;
            CPP_RULES____STRING::const_iterator line_end;
            uint64_t current_line = 1;
            uint64_t current_column = 1;
            CPP_RULES____CHAR current_char = '\0';
        };
        Info info;
        CPP_RULES____VECTOR <Info> infoStack;
        bool allocated = false;

        std::function<void(const char*)> next_prev_callback = [](auto unused){};

        Iterator(CPP_RULES____STRING *allocated_input);

    public:
        CPP_RULES____STRING *input;

        bool enable_logging;

        Iterator(CPP_RULES____STRING &input);
        Iterator(const char * input);
        ~Iterator();

        void set_next_prev_callback(std::function<void(const char*)> next_prev_callback);

        Iterator copy();

        uint64_t line() const;
        uint64_t column() const;

        bool has_next() const;

        CPP_RULES____CHAR next();

        bool has_previous() const;

        CPP_RULES____CHAR previous();

        CPP_RULES____CHAR peekPrevious() const;

        CPP_RULES____CHAR peekNext() const;

        CPP_RULES____STRING::const_iterator cbegin() const;

        CPP_RULES____STRING::const_iterator current() const;

        void setCurrent(CPP_RULES____STRING::const_iterator current);

        CPP_RULES____STRING::difference_type currentPosition() const;

        CPP_RULES____STRING::difference_type currentPosition(CPP_RULES____STRING::const_iterator iterator) const;

        CPP_RULES____STRING::const_iterator peekPreviousCurrent() const;

        CPP_RULES____STRING::const_iterator peekNextCurrent() const;

        CPP_RULES____STRING::const_iterator cend() const;

        void reset();

        CPP_RULES____STRING substr(CPP_RULES____STRING::const_iterator begin, CPP_RULES____STRING::const_iterator end) const;

        CPP_RULES____STRING lineString();

        void pushInfo();

        void pushInfoWithIterator(CPP_RULES____STRING::const_iterator iterator);

        bool popInfo();

        void popInfo(size_t n);

        void compute_line_end();
        void compute_line_start();

        void advance();

        void advance(size_t n);

        struct SaveState {
            SaveInfo info;
            CPP_RULES____VECTOR <SaveInfo> infoStack;
        };

        SaveState save();

        SaveState save(CPP_RULES____STRING::const_iterator iterator);

        void load(SaveState & saveState);

        void load(SaveState & saveState, CPP_RULES____STRING::const_iterator & iterator);

        CPP_RULES____STRING currentString() const;

        CPP_RULES____STRING currentString(CPP_RULES____STRING::const_iterator iterator) const;
    };

#if (false)

    class Iterator2 {
#ifdef GTEST_API_
    public:
#endif
        std::string::const_iterator iteratorCurrent;
        std::vector <std::string::const_iterator> iteratorStack;
        Iterator2 * parent;

        bool current_has_next() const;

        bool parent_has_next() const;

        bool current_has_previous() const;

        bool parent_has_previous() const;

        int parentCount(int count) const;


    public:
        std::string *input;

        Iterator2(std::string &input);

        Iterator2(Iterator2 & parent, std::string &input);

        int parentCount() const;

        bool has_previous() const;

        std::optional<char> previous();

        std::optional<char> peekPrevious() const;

        bool has_next() const;

        std::optional<char> next();

        std::optional<char> peekNext() const;

        std::string::const_iterator cbegin() const;

        std::string::const_iterator current() const;

        void setCurrent(std::string::const_iterator current);

        std::string::difference_type currentPosition() const;

        std::string::difference_type currentPosition(std::string::const_iterator iterator) const;

        std::optional<std::string::const_iterator> peekPreviousCurrent() const;

        std::optional<std::string::const_iterator> peekNextCurrent() const;

        std::string::const_iterator cend() const;

        void reset();

        std::string substr(std::string::const_iterator begin, std::string::const_iterator end) const;

        void pushIterator();

        void pushIterator(std::string::const_iterator iterator);

        void popIterator();

        void popIterator(size_t n);

        void setCurrentToIteratorPops(size_t n);

        void advance();

        void advance(size_t n);

        struct SaveState {
            std::string::difference_type iteratorCurrent = 0;
            std::vector <std::string::difference_type> iteratorStack;
        };

        SaveState save();

        SaveState save(std::string::const_iterator iterator);

        void load(SaveState & saveState);

        void load(SaveState & saveState, std::string::const_iterator & iterator);

        std::string currentString() const;

        std::string currentString(std::string::const_iterator & iterator) const;
    };
#endif

}


#ifdef GTEST_API_
TEST(Initialization_Iterator, test_01) {
    std::string a = "Hello World!";
    CPP::Iterator<std::string> b(a);
    EXPECT_EQ(b.input, a);
    EXPECT_EQ(b.input.data(), a.data());
    EXPECT_EQ(b.input.c_str(), a.c_str());
    EXPECT_STREQ(b.input.c_str(), a.c_str());
    EXPECT_STRCASEEQ(b.input.c_str(), a.c_str());
    EXPECT_EQ(b.current(), a.cbegin());
    EXPECT_EQ(b.cbegin(), b.input.cbegin());
    EXPECT_EQ(b.cbegin(), a.cbegin());
    EXPECT_EQ(b.cend(), b.input.cend());
    EXPECT_EQ(b.cend(), a.cend());
    EXPECT_EQ(b.iteratorCurrent, b.input.cbegin());
    EXPECT_EQ(b.iteratorStack.size(), 0);
    EXPECT_EQ(b.currentPosition(), 0);
    EXPECT_TRUE(b.has_next());
    EXPECT_FALSE(b.has_previous());
}

TEST(Initialization_Iterator, test_02) {
    std::string a;
    CPP::Iterator<std::string> b(a);
    EXPECT_EQ(b.input, a);
    EXPECT_EQ(b.input.data(), a.data());
    EXPECT_EQ(b.input.c_str(), a.c_str());
    EXPECT_STREQ(b.input.c_str(), a.c_str());
    EXPECT_STRCASEEQ(b.input.c_str(), a.c_str());
    EXPECT_EQ(b.current(), a.cbegin());
    EXPECT_EQ(b.cbegin(), b.input.cbegin());
    EXPECT_EQ(b.cbegin(), a.cbegin());
    EXPECT_EQ(b.cend(), b.input.cend());
    EXPECT_EQ(b.cend(), a.cend());
    EXPECT_EQ(b.iteratorCurrent, b.input.cbegin());
    EXPECT_EQ(b.iteratorStack.size(), 0);
    EXPECT_EQ(b.currentPosition(), 0);
    EXPECT_FALSE(b.has_next());
    EXPECT_FALSE(b.has_previous());
}

TEST(Initialization_Iterator, test_03) {
    std::string a = "";
    CPP::Iterator<std::string> b(a);
    EXPECT_EQ(b.input, a);
    EXPECT_EQ(b.input.data(), a.data());
    EXPECT_EQ(b.input.c_str(), a.c_str());
    EXPECT_STREQ(b.input.c_str(), a.c_str());
    EXPECT_STRCASEEQ(b.input.c_str(), a.c_str());
    EXPECT_EQ(b.current(), a.cbegin());
    EXPECT_EQ(b.cbegin(), b.input.cbegin());
    EXPECT_EQ(b.cbegin(), a.cbegin());
    EXPECT_EQ(b.cend(), b.input.cend());
    EXPECT_EQ(b.cend(), a.cend());
    EXPECT_EQ(b.iteratorCurrent, b.input.cbegin());
    EXPECT_EQ(b.iteratorStack.size(), 0);
    EXPECT_EQ(b.currentPosition(), 0);
    EXPECT_FALSE(b.has_next());
    EXPECT_FALSE(b.has_previous());
}

TEST(Iterator, basic_test_01) {
    std::string a = "Hello World!";
    CPP::Iterator<std::string> b(a);
    EXPECT_EQ(b.current(), a.cbegin());
    EXPECT_FALSE(b.has_previous());
    EXPECT_TRUE(b.has_next());
    EXPECT_EQ(b.next(), 'H');
    EXPECT_EQ(b.current(), a.cbegin()+1);
    EXPECT_TRUE(b.has_previous());
    EXPECT_TRUE(b.has_next());
}

TEST(Iterator, basic_test_02) {
    std::string a;
    CPP::Iterator<std::string> b(a);
    EXPECT_FALSE(b.has_next());
    EXPECT_FALSE(b.has_previous());
    EXPECT_EQ(b.next(), '\0');
    EXPECT_EQ(b.current(), a.cbegin()+1);
    EXPECT_TRUE(b.has_previous());
    EXPECT_FALSE(b.has_next());
}

TEST(Iterator, basic_test_03) {
    std::string a = "";
    CPP::Iterator<std::string> b(a);
    EXPECT_FALSE(b.has_next());
    EXPECT_FALSE(b.has_previous());
    EXPECT_EQ(b.next(), '\0');
    EXPECT_EQ(b.current(), a.cbegin()+1);
    EXPECT_TRUE(b.has_previous());
    EXPECT_FALSE(b.has_next());
}

TEST(Iterator, basic_test_04) {
    std::string a = "Hello World!";
    CPP::Iterator<std::string> b(a);
    EXPECT_FALSE(b.has_previous());
    EXPECT_TRUE(b.has_next());
    EXPECT_EQ(b.next(), 'H');
    EXPECT_EQ(b.current(), a.cbegin()+1);
    EXPECT_TRUE(b.has_previous());
    EXPECT_EQ(b.previous(), 'H');
    EXPECT_EQ(b.current(), a.cbegin());
    EXPECT_TRUE(b.has_next());
}

TEST(Iterator, basic_test_05) {
    std::string a;
    CPP::Iterator<std::string> b(a);
    EXPECT_FALSE(b.has_next());
    EXPECT_FALSE(b.has_previous());
    EXPECT_EQ(b.next(), '\0');
    EXPECT_EQ(b.current(), a.cbegin()+1);
    EXPECT_TRUE(b.has_previous());
    EXPECT_EQ(b.previous(), '\0');
    EXPECT_EQ(b.current(), a.cbegin());
    EXPECT_FALSE(b.has_next());
}

TEST(Iterator, basic_test_06) {
    std::string a = "";
    CPP::Iterator<std::string> b(a);
    EXPECT_FALSE(b.has_next());
    EXPECT_FALSE(b.has_previous());
    EXPECT_EQ(b.next(), '\0');
    EXPECT_EQ(b.current(), a.cbegin()+1);
    EXPECT_TRUE(b.has_previous());
    EXPECT_EQ(b.previous(), '\0');
    EXPECT_EQ(b.current(), a.cbegin());
    EXPECT_FALSE(b.has_next());
}

TEST(Iterator, test_01) {
    std::string a = "12";
    CPP::Iterator<std::string> b(a);
    EXPECT_EQ(b.currentPosition(), 0);
    EXPECT_EQ(b.peekNext(), '1');
    EXPECT_EQ(b.currentPosition(), 0);
    EXPECT_EQ(b.next(), '1');

    EXPECT_EQ(b.currentPosition(), 1);
    EXPECT_EQ(b.peekNext(), '2');
    EXPECT_EQ(b.currentPosition(), 1);
    EXPECT_EQ(b.next(), '2');

    EXPECT_EQ(b.currentPosition(), 2);
    EXPECT_EQ(b.peekNext(), '\0');
    EXPECT_EQ(b.currentPosition(), 2);
    EXPECT_EQ(b.peekPrevious(), '2');
    EXPECT_EQ(b.currentPosition(), 2);
    EXPECT_EQ(b.previous(), '2');
    EXPECT_EQ(b.currentPosition(), 1);
    EXPECT_EQ(b.peekNext(), '2');
    EXPECT_EQ(b.currentPosition(), 1);
    EXPECT_EQ(b.peekPrevious(), '1');
    EXPECT_EQ(b.currentPosition(), 1);
    EXPECT_EQ(b.previous(), '1');
    EXPECT_EQ(b.currentPosition(), 0);
    EXPECT_EQ(b.peekNext(), '1');
    EXPECT_EQ(b.currentPosition(), 0);
}

TEST(Iterator_Stack, test_01) {
    std::string a = "12";
    CPP::Iterator<std::string> b(a);
    b.pushIterator();
    EXPECT_EQ(b.iteratorStack.size(), 1);
    EXPECT_EQ(b.currentPosition(), 0);
    b.advance();
    EXPECT_EQ(b.currentPosition(), 1);
    EXPECT_EQ(b.iteratorStack.size(), 1);
    b.popIterator();
    EXPECT_EQ(b.currentPosition(), 0);
    EXPECT_EQ(b.iteratorStack.size(), 0);
}

TEST(Iterator_SaveState, test_initialization_01) {
    CPP::Iterator<std::string>::SaveState saveState;
    EXPECT_EQ(saveState.iteratorCurrent, 0);
    EXPECT_EQ(saveState.iteratorStack.size(), 0);
}

TEST(Iterator_SaveState, test_initialization_02) {
    std::string a;
    CPP::Iterator<std::string> b(a);
    auto saveState = b.save();
    EXPECT_EQ(saveState.iteratorCurrent, 0);
    EXPECT_EQ(saveState.iteratorStack.size(), 0);
}

TEST(Iterator_SaveState, test_save) {
    std::string a;
    CPP::Iterator<std::string> b(a);
    auto saveState = b.save();
    EXPECT_EQ(saveState.iteratorCurrent, 0);
    EXPECT_EQ(saveState.iteratorStack.size(), 0);
    EXPECT_EQ(b.currentPosition(), 0);
    EXPECT_EQ(b.iteratorStack.size(), 0);
    b.pushIterator();
    b.advance();
    EXPECT_EQ(b.currentPosition(), 1);
    EXPECT_EQ(b.iteratorStack.size(), 1);
    EXPECT_EQ(b.iteratorStack[0], a.cbegin());
    b.pushIterator();
    b.advance();
    EXPECT_EQ(b.currentPosition(), 2);
    EXPECT_EQ(b.iteratorStack.size(), 2);
    EXPECT_EQ(b.iteratorStack[0], a.cbegin());
    EXPECT_EQ(b.iteratorStack[1], a.cbegin()+1);
    auto saveState2 = b.save();
    EXPECT_EQ(b.currentPosition(), 2);
    EXPECT_EQ(b.iteratorStack.size(), 2);
    EXPECT_EQ(b.iteratorStack[0], a.cbegin());
    EXPECT_EQ(b.iteratorStack[1], a.cbegin()+1);
    EXPECT_EQ(saveState.iteratorCurrent, 0);
    EXPECT_EQ(saveState.iteratorStack.size(), 0);
    EXPECT_EQ(saveState2.iteratorCurrent, 2);
    EXPECT_EQ(saveState2.iteratorStack.size(), 2);
    EXPECT_EQ(saveState2.iteratorStack[0], 0);
    EXPECT_EQ(saveState2.iteratorStack[1], 1);
    b.load(saveState);
    EXPECT_EQ(b.currentPosition(), 0);
    EXPECT_EQ(b.iteratorStack.size(), 0);
    EXPECT_EQ(saveState.iteratorCurrent, 0);
    EXPECT_EQ(saveState.iteratorStack.size(), 0);
    EXPECT_EQ(saveState2.iteratorCurrent, 2);
    EXPECT_EQ(saveState2.iteratorStack.size(), 2);
    EXPECT_EQ(saveState2.iteratorStack[0], 0);
    EXPECT_EQ(saveState2.iteratorStack[1], 1);
    b.load(saveState2);
    EXPECT_EQ(b.currentPosition(), 2);
    EXPECT_EQ(b.iteratorStack.size(), 2);
    EXPECT_EQ(b.iteratorStack[0], a.cbegin());
    EXPECT_EQ(b.iteratorStack[1], a.cbegin()+1);
    EXPECT_EQ(saveState.iteratorCurrent, 0);
    EXPECT_EQ(saveState.iteratorStack.size(), 0);
    EXPECT_EQ(saveState2.iteratorCurrent, 2);
    EXPECT_EQ(saveState2.iteratorStack.size(), 2);
    EXPECT_EQ(saveState2.iteratorStack[0], 0);
    EXPECT_EQ(saveState2.iteratorStack[1], 1);
    b.load(saveState);
    EXPECT_EQ(b.currentPosition(), 0);
    EXPECT_EQ(b.iteratorStack.size(), 0);
    EXPECT_EQ(saveState.iteratorCurrent, 0);
    EXPECT_EQ(saveState.iteratorStack.size(), 0);
    EXPECT_EQ(saveState2.iteratorCurrent, 2);
    EXPECT_EQ(saveState2.iteratorStack.size(), 2);
    EXPECT_EQ(saveState2.iteratorStack[0], 0);
    EXPECT_EQ(saveState2.iteratorStack[1], 1);
}

#endif

#endif
