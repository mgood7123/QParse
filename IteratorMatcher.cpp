#include "IteratorMatcher.h"


QParse::IteratorMatcher::MatchData::MatchData() : matched(false) {}

QParse::IteratorMatcher::MatchData::MatchData(bool matched) : matched(matched) {}

QParse::IteratorMatcher::MatchData::operator bool() const noexcept {
    return matched;
}

QParse::IteratorMatcher::MatchData QParse::IteratorMatcher::match(Iterator &i) {
    MatchData matchData;
    matchData.begin = i.current();
    matchData.end = i.current();
    matchData.matched = false;
    if (!i.has_next()) {
        // unexpected EOF
        return matchData;
    }
    i.pushInfo();
    i.advance();
    matchData.end = i.current();
    matchData.matched = true;
    matchData.matches++;
    return matchData;
}

QParse::IteratorMatcher::MatchData QParse::IteratorMatcher::match(Iterator &i, char value) {
    MatchData matchData;
    matchData.begin = i.current();
    matchData.end = i.current();
    matchData.matched = false;
    if (!i.has_next()) {
        // unexpected EOF
        return matchData;
    }
    i.pushInfo();
    if (i.next() == value) {
        matchData.end = i.current();
        matchData.matched = true;
        matchData.matches++;
        return matchData;
    }
    // input does not match
    i.popInfo();
    return matchData;
}

QParse::IteratorMatcher::MatchData QParse::IteratorMatcher::match(Iterator &i, QParse_RULES____STRING &value) {
    MatchData matchData;
    matchData.begin = i.current();
    matchData.end = i.current();
    matchData.matched = false;
    if (value.size() == 0) {
        // match EOF if input is empty
        matchData.matched = !i.has_next();
        return matchData;
    }
    if (!i.has_next()) {
        // unexpected EOF
        return matchData;
    }
    i.pushInfo();
    // optimize for single character matches and double character matches
    if (value.size() == 1) {
        if (i.next() == value[0]) {
            matchData.end = i.current();
            matchData.matched = true;
            matchData.matches++;
            return matchData;
        }
        // input does not match
        i.popInfo();
        return matchData;
    }
    if (value.size() == 2) {
        if (i.next() == value[0]) {
            if (i.next() == value[1]) {
                matchData.end = i.current();
                matchData.matched = true;
                matchData.matches++;
                return matchData;
            }
        }
        // input does not match
        i.popInfo();
        return matchData;
    }

    // value is longer than 2

    Iterator tmp(value);
    while (true) {
        if (!tmp.has_next()) {
            // we have matched all input
            matchData.end = i.current();
            matchData.matched = true;
            matchData.matches++;
            return matchData;
        }
        if (!i.has_next() && tmp.has_next()) {
            // unexpected EOF
            i.popInfo();
            return matchData;
        }
        if (i.next() != tmp.next()) {
            // input does not match
            i.popInfo();
            return matchData;
        }
    }
}

QParse::IteratorMatcher::MatchData QParse::IteratorMatcher::match(Iterator &i, const QParse_RULES____STRING &value) {
    QParse_RULES____STRING tmp = value;
    return match(i, tmp);
}
