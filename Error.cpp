#include "Rules.h"
#include CPP_RULES____COUT_INCLUDE

void print_error(CPP::Iterator & iterator) {
    CPP_RULES____COUT_NO_SPACE << "  at source:" << iterator.line() << ":" << iterator.column() << " (index " << iterator.currentPosition() << ") :" CPP_RULES____COUT_ENDL;
    CPP_RULES____COUT_NO_SPACE_NO_QUOTE << "    input: \"" << iterator.lineString() << "\"" CPP_RULES____COUT_ENDL;

    CPP_RULES____STRING pre = "           ";
    CPP_RULES____STRING_APPEND(spacing, " ", iterator.column());
    CPP_RULES____STRING cursor = spacing + "^";
    CPP_RULES____STRING full_cursor = pre + spacing + "^";

    CPP_RULES____COUT_NO_SPACE_NO_QUOTE << full_cursor CPP_RULES____COUT_ENDL;
    CPP_RULES____COUT_NO_SPACE << "" CPP_RULES____COUT_ENDL;
}

void CPP::Rules::printError(const CPP_RULES____STRING & message, Iterator & iterator, UndoRedo & undo)
{
    CPP_RULES____COUT_NO_SPACE << "ERROR: " << message CPP_RULES____COUT_ENDL;
    print_error(iterator);
    undo.print([&](auto description) {
        CPP_RULES____COUT_NO_SPACE << description CPP_RULES____COUT_ENDL;
        undo.undo();
        print_error(iterator);
    });
}


CPP::Rules::Error::Error(const CPP_RULES____STRING &message, Action action) : message(message), Rule(action) {}

std::optional<CPP::IteratorMatcher::MatchData> CPP::Rules::Error::match(Iterator &iterator, UndoRedo *undo, bool doAction) {
    IteratorMatcher::MatchData match;
    match.begin = iterator.current();
    match.end = iterator.current();
    match.matched = false;
    if (doAction) action(Input(iterator, match, undo, 0));
    //iterator.popInfo(match.matches);
    printError(message, iterator, *undo);
    return std::nullopt;
}

CPP::Rules::ErrorIfMatch::ErrorIfMatch(Rule *rule, const CPP_RULES____STRING &message, Action action) : message(message), RuleHolder(rule, action) {}

std::optional<CPP::IteratorMatcher::MatchData> CPP::Rules::ErrorIfMatch::match(Iterator &iterator, UndoRedo *undo, bool doAction) {
    auto match_ = rule->match(iterator, undo, doAction);
    if (!match_.has_value()) return std::nullopt;
    auto match = *match_;
    if (match) {
        if (doAction) action(Input(iterator, match, undo, 0));
        //iterator.popInfo(match.matches);
        printError(message, iterator, *undo);
        return std::nullopt;
    }
    return match;
}

CPP::Rules::ErrorIfNotMatch::ErrorIfNotMatch(Rule *rule, const CPP_RULES____STRING &message, Action action) : message(message), RuleHolder(rule, action) {}

std::optional<CPP::IteratorMatcher::MatchData> CPP::Rules::ErrorIfNotMatch::match(Iterator &iterator, UndoRedo *undo, bool doAction) {
    auto match_ = rule->match(iterator, undo, doAction);
    if (!match_.has_value()) return std::nullopt;
    auto match = *match_;
    if (!match) {
        if (doAction) action(Input(iterator, match, undo, 0));
        //iterator.popInfo(match.matches);
        printError(message, iterator, *undo);
        return std::nullopt;
    }
    return match;
}
