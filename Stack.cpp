#include "Stack.h"

CPP::Rules::Stack::Stack() : baseRule(nullptr) {}

void CPP::Rules::Stack::setBase(Rule *rule, Action action) {
    baseRule = rule;
    baseAction = action;
}

void CPP::Rules::Stack::push(Rule *rule, Action action) {
    ruleStack.push(rule);
    actionStack.push(action);
}

void CPP::Rules::Stack::pop() {
    if (!ruleStack.empty()) ruleStack.pop();
    if (!actionStack.empty()) actionStack.pop();
}

void CPP::Rules::Stack::popAll() {
    while (!ruleStack.empty()) ruleStack.pop();
    while (!actionStack.empty()) actionStack.pop();
}

std::optional<CPP::IteratorMatcher::MatchData> CPP::Rules::Stack::match(Iterator &iterator, UndoRedo *undo, bool doAction) {
    auto match_ = (ruleStack.empty() ? baseRule : ruleStack.top()).rule->match(iterator, undo, doAction);
    if (!match_.has_value()) return std::nullopt;
    auto match = *match_;
    if (match && doAction) {
        (actionStack.empty() ? baseAction : actionStack.top())(Input(iterator, match, undo, match.matches));
    }
    return match;
}
