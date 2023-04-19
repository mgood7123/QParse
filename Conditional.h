#ifndef QParse_RULES_CONDITIONAL_H
#define QParse_RULES_CONDITIONAL_H

#include "Rules.h"

namespace QParse {
    namespace Rules {

    using Condition = std::function<bool()>;

    class If : public Rule {
        Condition condition;
        RuleHolder rule_if_true;
        RuleHolder rule_if_false;
    public:

        If(Condition cond, Rule * rule_if_true) : condition(cond), rule_if_true(rule_if_true), rule_if_false(nullptr) {}

        If(Condition cond, Rule * rule_if_true, Rule* rule_if_false) : condition(cond), rule_if_true(rule_if_true), rule_if_false(rule_if_false) {}

        using Rule::match;

        virtual std::optional<IteratorMatcher::MatchData> match(Iterator &iterator, UndoRedo *undo, bool doAction, bool logErrors = true) override {
            if (condition()) {
                auto tmp_ = rule_if_true.match(iterator, undo, doAction, logErrors);
                if (!tmp_.has_value()) return std::nullopt;
                auto match = *tmp_;
                if (!match) {
                    iterator.popInfo(match.matches);
                    match.matches = 0;
                }
                return match;
            } else {
                auto tmp_ = rule_if_false.match(iterator, undo, doAction, logErrors);
                if (!tmp_.has_value()) return std::nullopt;
                auto match = *tmp_;
                if (!match) {
                    iterator.popInfo(match.matches);
                    match.matches = 0;
                }
                return match;
            }
        }
    };
    }
}

#endif // QParse_RULES_CONDITIONAL_H
