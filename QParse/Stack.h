#ifndef STACK_H
#define STACK_H

#include <stack>
#include "Rules.h"

namespace QParse {
    namespace Rules {
    class Stack : public Rule {
#ifdef GTEST_API_
        public:
#endif
        std::stack<RuleHolder> ruleStack;
        std::stack<Action> actionStack;

        RuleHolder baseRule;
        Action baseAction;

    public:

        Stack();

        void setBase(Rule * rule, Action action = NO_ACTION);

        void push(Rule * rule, Action action = NO_ACTION);

        void pop();

        void popAll();

        using Rule::match;

        virtual std::optional<IteratorMatcher::MatchData> match(Iterator &iterator, UndoRedo *undo, bool doAction = true, bool logErrors = true) override;
    };
    }
}
#endif // STACK_H
