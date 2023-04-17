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

        virtual std::optional<IteratorMatcher::MatchData> match(Iterator &iterator, UndoRedo *undo, bool doAction = true) override {
            IteratorMatcher::MatchData match;
            match.begin = iterator.current();
            match.end = iterator.current();
            match.matched = false;

            if (condition()) {
                    auto tmp_ = rule_if_true.match(iterator, undo, doAction);
                    if (!tmp_.has_value()) return std::nullopt;
                    auto tmp = *tmp_;
                    match.matched = tmp.matched;
                    match.end = tmp.end;
                    match.matches += tmp.matches;
            } else {
                auto tmp_ = rule_if_false.match(iterator, undo, doAction);
                if (!tmp_.has_value()) return std::nullopt;
                auto tmp = *tmp_;
                match.matched = tmp.matched;
                match.end = tmp.end;
                match.matches += tmp.matches;
            }

            return match;
        }

    };
    }
}

#endif // QParse_RULES_CONDITIONAL_H
