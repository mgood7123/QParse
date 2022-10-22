#ifndef CPP_ITERATOR_MATCHER_H
#define CPP_ITERATOR_MATCHER_H

#include "Iterator.h"

namespace CPP {
    namespace IteratorMatcher {

        struct MatchData {
            bool matched;
            int matches = 0;
            CPP_RULES____STRING::const_iterator begin;
            CPP_RULES____STRING::const_iterator end;
            MatchData();
            MatchData(bool matched);
            operator bool() const noexcept;
        };

        // match anything
        MatchData match(Iterator & i);

        // optimize for single character match
        MatchData match(Iterator & i, char value);

        MatchData match(Iterator & i, CPP_RULES____STRING &value);

        MatchData match(Iterator & i, const CPP_RULES____STRING &value);
    }
}

#ifdef GTEST_API_
TEST(Initialization_IteratorMatcher_MatchData, test_initialization) {
    CPP::IteratorMatcher::MatchData matchData;
    EXPECT_FALSE(matchData.matched);
    EXPECT_FALSE(matchData);
    EXPECT_EQ(matchData.begin, CPP_RULES____STRING::const_iterator());
    EXPECT_EQ(matchData.end, CPP_RULES____STRING::const_iterator());
    EXPECT_EQ(matchData.matches, 0);
}

TEST(IteratorMatcher, test_01) {
    CPP_RULES____STRING a = "Hello World!";
    CPP::Iterator<CPP_RULES____STRING> b(a);
    CPP::IteratorMatcher::MatchData matchData = CPP::IteratorMatcher::match(b);
    EXPECT_TRUE(matchData.matched);
    EXPECT_TRUE(matchData);
    EXPECT_EQ(matchData.begin, b.cbegin());
    EXPECT_EQ(matchData.end, b.cbegin()+1);
    EXPECT_EQ(matchData.matches, 1);
    EXPECT_EQ(b.currentPosition(), 1);
    EXPECT_EQ(b.peekNext(), 'e');
}

TEST(IteratorMatcher, test_02) {
    CPP_RULES____STRING a = "Hello World!";
    CPP::Iterator<CPP_RULES____STRING> b(a);
    CPP::IteratorMatcher::MatchData matchData = CPP::IteratorMatcher::match(b, 'H');
    EXPECT_TRUE(matchData.matched);
    EXPECT_TRUE(matchData);
    EXPECT_EQ(matchData.begin, b.cbegin());
    EXPECT_EQ(matchData.end, b.cbegin()+1);
    EXPECT_EQ(matchData.matches, 1);
    EXPECT_EQ(b.currentPosition(), 1);
    EXPECT_EQ(b.peekNext(), 'e');
}

TEST(IteratorMatcher, test_03) {
    CPP_RULES____STRING a = "Hello World!";
    CPP::Iterator<CPP_RULES____STRING> b(a);
    CPP::IteratorMatcher::MatchData matchData = CPP::IteratorMatcher::match(b, 'H');
    CPP::IteratorMatcher::MatchData matchData2 = CPP::IteratorMatcher::match(b, 'e');
    EXPECT_TRUE(matchData2.matched);
    EXPECT_TRUE(matchData2);
    EXPECT_EQ(matchData2.begin, b.cbegin()+1);
    EXPECT_EQ(matchData2.end, b.cbegin()+2);
    EXPECT_EQ(matchData2.matches, 1);
    EXPECT_EQ(b.currentPosition(), 2);
    EXPECT_EQ(b.peekNext(), 'l');
}

TEST(IteratorMatcher, test_04) {
    CPP_RULES____STRING a = "Hello World!";
    CPP::Iterator<CPP_RULES____STRING> b(a);
    CPP::IteratorMatcher::MatchData matchData = CPP::IteratorMatcher::match(b, 'H');
    CPP::IteratorMatcher::MatchData matchData2 = CPP::IteratorMatcher::match(b, 'r');
    EXPECT_TRUE(matchData.matched);
    EXPECT_TRUE(matchData);
    EXPECT_EQ(matchData.begin, b.cbegin());
    EXPECT_EQ(matchData.end, b.cbegin()+1);
    EXPECT_EQ(matchData.matches, 1);
    EXPECT_FALSE(matchData2.matched);
    EXPECT_FALSE(matchData2);
    EXPECT_EQ(matchData2.begin, b.cbegin()+1);
    EXPECT_EQ(matchData2.end, b.cbegin()+1);
    EXPECT_EQ(matchData2.matches, 0);
    EXPECT_EQ(b.currentPosition(), 1);
    EXPECT_EQ(b.peekNext(), 'e');
}

TEST(IteratorMatcher_backtracking, test_01) {
    CPP_RULES____STRING a = "Hello World!";
    CPP::Iterator<CPP_RULES____STRING> b(a);
    b.pushIterator();
    CPP::IteratorMatcher::MatchData matchData = CPP::IteratorMatcher::match(b, 'H');
    CPP::IteratorMatcher::MatchData matchData2 = CPP::IteratorMatcher::match(b, 'r');
    b.popIterator();
    EXPECT_TRUE(matchData.matched);
    EXPECT_TRUE(matchData);
    EXPECT_EQ(matchData.begin, b.cbegin());
    EXPECT_EQ(matchData.end, b.cbegin()+1);
    EXPECT_EQ(matchData.matches, 1);
    EXPECT_EQ(b.currentPosition(), 0);
    EXPECT_EQ(b.peekNext(), 'H');
}

TEST(IteratorMatcher_backtracking, test_02) {
    CPP_RULES____STRING a = "Hello World!";
    CPP::Iterator<CPP_RULES____STRING> b(a);
    CPP::IteratorMatcher::MatchData matchData = CPP::IteratorMatcher::match(b, 'H');
    b.popIterator(matchData.matches);
    EXPECT_EQ(b.currentPosition(), 0);
    EXPECT_EQ(b.peekNext(), 'H');
}

#endif

#endif
