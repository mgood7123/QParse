#ifndef CPP_RULES_H
#define CPP_RULES_H

#include "IteratorMatcher.h"
#include "Input.h"

#define CPP_Rules_LogCapture1(rule, custom_name) new CPP::Rules::LogCapture(rule, custom_name)
#define CPP_Rules_LogCapture(rule) CPP_Rules_LogCapture1(rule, #rule)
#define Rules_NS_LogCapture1(rule, custom_name) new Rules::LogCapture(rule, custom_name)
#define Rules_NS_LogCapture(rule) Rules_NS_LogCapture1(rule, #rule)

#define CPP_Rules_LogCapture1_A(rule, custom_name, action) new CPP::Rules::LogCapture(rule, custom_name, action)
#define CPP_Rules_LogCapture_A(rule, action) CPP_Rules_LogCapture1(rule, #rule, action)
#define Rules_NS_LogCapture1_A(rule, custom_name, action) new Rules::LogCapture(rule, custom_name, action)
#define Rules_NS_LogCapture_A(rule, action) Rules_NS_LogCapture1(rule, #rule, action)

#define CPP_Rules_LogCapture1_NO_ALLOC(rule, custom_name) CPP::Rules::LogCapture(rule, custom_name)
#define CPP_Rules_LogCapture_NO_ALLOC(rule) CPP_Rules_LogCapture1_NO_ALLOC(rule, #rule)
#define Rules_NS_LogCapture1_NO_ALLOC(rule, custom_name) Rules::LogCapture(rule, custom_name)
#define Rules_NS_LogCapture_NO_ALLOC(rule) Rules_NS_LogCapture1_NO_ALLOC(rule, #rule)

#define CPP_Rules_LogCapture1_A_NO_ALLOC(rule, custom_name, action) CPP::Rules::LogCapture(rule, custom_name, action)
#define CPP_Rules_LogCapture_A_NO_ALLOC(rule, action) CPP_Rules_LogCapture1_NO_ALLOC(rule, #rule, action)
#define Rules_NS_LogCapture1_A_NO_ALLOC(rule, custom_name, action) Rules::LogCapture(rule, custom_name, action)
#define Rules_NS_LogCapture_A_NO_ALLOC(rule, action) Rules_NS_LogCapture1_NO_ALLOC(rule, #rule, action)



#define CPP_Rules_LogInput1(rule, custom_name) new CPP::Rules::LogInput(rule, custom_name)
#define CPP_Rules_LogInput(rule) CPP_Rules_LogInput1(rule, #rule)
#define Rules_NS_LogInput1(rule, custom_name) new Rules::LogInput(rule, custom_name)
#define Rules_NS_LogInput(rule) Rules_NS_LogInput1(rule, #rule)

#define CPP_Rules_LogInput1_A(rule, custom_name, action) new CPP::Rules::LogInput(rule, custom_name, action)
#define CPP_Rules_LogInput_A(rule, action) CPP_Rules_LogInput1_A(rule, #rule, action)
#define Rules_NS_LogInput1_A(rule, custom_name, action) new Rules::LogInput(rule, custom_name, action)
#define Rules_NS_LogInput_A(rule, action) Rules_NS_LogInput1_A(rule, #rule, action)

#define CPP_Rules_LogInput1_NO_ALLOC(rule, custom_name) CPP::Rules::LogInput(rule, custom_name)
#define CPP_Rules_LogInput_NO_ALLOC(rule) CPP_Rules_LogInput1_NO_ALLOC(rule, #rule)
#define Rules_NS_LogInput1_NO_ALLOC(rule, custom_name) Rules::LogInput(rule, custom_name)
#define Rules_NS_LogInput_NO_ALLOC(rule) Rules_NS_LogInput1_NO_ALLOC(rule, #rule)

#define CPP_Rules_LogInput1_A_NO_ALLOC(rule, custom_name, action) CPP::Rules::LogInput(rule, custom_name, action)
#define CPP_Rules_LogInput_A_NO_ALLOC(rule, action) CPP_Rules_LogInput1_A_NO_ALLOC(rule, #rule, action)
#define Rules_NS_LogInput1_A_NO_ALLOC(rule, custom_name, action) Rules::LogInput(rule, custom_name, action)
#define Rules_NS_LogInput_A_NO_ALLOC(rule, action) Rules_NS_LogInput1_A_NO_ALLOC(rule, #rule, action)



#define CPP_Rules_LogMatchStatus1(rule, custom_name) new CPP::Rules::LogMatchStatus(rule, custom_name)
#define CPP_Rules_LogMatchStatus(rule) CPP_Rules_LogMatchStatus1(rule, #rule)
#define Rules_NS_LogMatchStatus1(rule, custom_name) new Rules::LogMatchStatus(rule, custom_name)
#define Rules_NS_LogMatchStatus(rule) Rules_NS_LogMatchStatus1(rule, #rule)

#define CPP_Rules_LogMatchStatus1_A(rule, custom_name, action) new CPP::Rules::LogMatchStatus(rule, custom_name, action)
#define CPP_Rules_LogMatchStatus_A(rule, action) CPP_Rules_LogMatchStatus1(rule, #rule, action)
#define Rules_NS_LogMatchStatus1_A(rule, custom_name, action) new Rules::LogMatchStatus(rule, custom_name, action)
#define Rules_NS_LogMatchStatus_A(rule, action) Rules_NS_LogMatchStatus1(rule, #rule, action)

#define CPP_Rules_LogMatchStatus1_NO_ALLOC(rule, custom_name) CPP::Rules::LogMatchStatus(rule, custom_name)
#define CPP_Rules_LogMatchStatus_NO_ALLOC(rule) CPP_Rules_LogMatchStatus1_NO_ALLOC(rule, #rule)
#define Rules_NS_LogMatchStatus1_NO_ALLOC(rule, custom_name) Rules::LogMatchStatus(rule, custom_name)
#define Rules_NS_LogMatchStatus_NO_ALLOC(rule) Rules_NS_LogMatchStatus1_NO_ALLOC(rule, #rule)

#define CPP_Rules_LogMatchStatus1_A_NO_ALLOC(rule, custom_name, action) CPP::Rules::LogMatchStatus(rule, custom_name, action)
#define CPP_Rules_LogMatchStatus_A_NO_ALLOC(rule, action) CPP_Rules_LogMatchStatus1_NO_ALLOC(rule, #rule, action)
#define Rules_NS_LogMatchStatus1_A_NO_ALLOC(rule, custom_name, action) Rules::LogMatchStatus(rule, custom_name, action)
#define Rules_NS_LogMatchStatus_A_NO_ALLOC(rule, action) Rules_NS_LogMatchStatus1_NO_ALLOC(rule, #rule, action)



#define CPP_Rules_LogTrace1(rule, custom_name) new CPP::Rules::LogTrace(rule, custom_name)
#define CPP_Rules_LogTrace(rule) CPP_Rules_LogTrace1(rule, #rule)
#define Rules_NS_LogTrace1(rule, custom_name) new Rules::LogTrace(rule, custom_name)
#define Rules_NS_LogTrace(rule) Rules_NS_LogTrace1(rule, #rule)

#define CPP_Rules_LogTrace1_A(rule, custom_name, action) new CPP::Rules::LogTrace(rule, custom_name, action)
#define CPP_Rules_LogTrace_A(rule, action) CPP_Rules_LogTrace1(rule, #rule, action)
#define Rules_NS_LogTrace1_A(rule, custom_name, action) new Rules::LogTrace(rule, custom_name, action)
#define Rules_NS_LogTrace_A(rule, action) Rules_NS_LogTrace1(rule, #rule, action)

#define CPP_Rules_LogTrace1_NO_ALLOC(rule, custom_name) CPP::Rules::LogTrace(rule, custom_name)
#define CPP_Rules_LogTrace_NO_ALLOC(rule) CPP_Rules_LogTrace1_NO_ALLOC(rule, #rule)
#define Rules_NS_LogTrace1_NO_ALLOC(rule, custom_name) Rules::LogTrace(rule, custom_name)
#define Rules_NS_LogTrace_NO_ALLOC(rule) Rules_NS_LogTrace1_NO_ALLOC(rule, #rule)

#define CPP_Rules_LogTrace1_A_NO_ALLOC(rule, custom_name, action) CPP::Rules::LogTrace(rule, custom_name, action)
#define CPP_Rules_LogTrace_A_NO_ALLOC(rule, action) CPP_Rules_LogTrace1_NO_ALLOC(rule, #rule, action)
#define Rules_NS_LogTrace1_A_NO_ALLOC(rule, custom_name, action) Rules::LogTrace(rule, custom_name, action)
#define Rules_NS_LogTrace_A_NO_ALLOC(rule, action) Rules_NS_LogTrace1_NO_ALLOC(rule, #rule, action)



namespace CPP {
    namespace Rules {
        using Action = std::function<void(Input input)>;

        extern Action NO_ACTION;

        // an empty rule, this matches nothing
        struct Rule {
            Action action;
            UndoRedo * undo = nullptr;
            bool allocated = false;

            Rule(Action action = NO_ACTION);

            std::optional<IteratorMatcher::MatchData> match(const char * string, bool doAction = true);
            std::optional<IteratorMatcher::MatchData> match(CPP_RULES____STRING & string, bool doAction = true);
            std::optional<IteratorMatcher::MatchData> match(Iterator &iterator, bool doAction = true);

            std::optional<IteratorMatcher::MatchData> match(const char * string, UndoRedo *undo, bool doAction = true);
            std::optional<IteratorMatcher::MatchData> match(CPP_RULES____STRING & string, UndoRedo *undo, bool doAction = true);
            virtual std::optional<IteratorMatcher::MatchData> match(Iterator &iterator, UndoRedo *undo, bool doAction = true);

            virtual ~Rule();
        };

        struct Success : Rule {

            Success(Action action = NO_ACTION);

            using Rule::match;

            virtual std::optional<IteratorMatcher::MatchData> match(Iterator &iterator, UndoRedo *undo, bool doAction = true) override;
        };

        class AdvanceInputBy : public Rule {
#ifdef GTEST_API_
            public:
#endif
            int n;

        public:
            AdvanceInputBy(Action action = NO_ACTION);
            AdvanceInputBy(int n, Action action = NO_ACTION);

            using Rule::match;

            virtual std::optional<IteratorMatcher::MatchData> match(Iterator &iterator, UndoRedo *undo, bool doAction = true) override;
        };

        struct Fail : Rule {

            Fail(Action action = NO_ACTION);

            using Rule::match;

            virtual std::optional<IteratorMatcher::MatchData> match(Iterator &iterator, UndoRedo *undo, bool doAction = true) override;
        };

        struct Any : Rule {
            Any(Action action = NO_ACTION);

            using Rule::match;

            virtual std::optional<IteratorMatcher::MatchData> match(Iterator &iterator, UndoRedo *undo, bool doAction = true) override;
        };

        struct Char : Rule {
            char character;

            Char(char character, Action action = NO_ACTION);

            using Rule::match;

            virtual std::optional<IteratorMatcher::MatchData> match(Iterator &iterator, UndoRedo *undo, bool doAction = true) override;
        };

        struct EndOfFile : Rule {
            EndOfFile(Action action = NO_ACTION);

            using Rule::match;

            virtual std::optional<IteratorMatcher::MatchData> match(Iterator &iterator, UndoRedo *undo, bool doAction = true) override;
        };

        struct NewlineOrEOF : Rule {
            NewlineOrEOF(Action action = NO_ACTION);

            using Rule::match;

            virtual std::optional<IteratorMatcher::MatchData> match(Iterator &iterator, UndoRedo *undo, bool doAction = true) override;
        };

        struct Newline : Rule {
            Newline(Action action = NO_ACTION);

            using Rule::match;

            virtual std::optional<IteratorMatcher::MatchData> match(Iterator &iterator, UndoRedo *undo, bool doAction = true) override;
        };

        struct String : Rule {
            CPP_RULES____STRING string;

            String(const CPP_RULES____STRING &string, Action action = NO_ACTION);

            using Rule::match;

            virtual std::optional<IteratorMatcher::MatchData> match(Iterator &iterator, UndoRedo *undo, bool doAction = true) override;
        };

        struct RuleHolder : Rule {
            struct Reference {
                Rule * rule = nullptr;
                int reference = 0;
            };
            static CPP_RULES____VECTOR<Reference*> rules;
            Reference * ref = nullptr;
            Rule * rule = nullptr;

            RuleHolder(Rule *rule, Action action = NO_ACTION);

            RuleHolder(const RuleHolder & other);

            RuleHolder & operator=(const RuleHolder & other);

            RuleHolder(RuleHolder && other) noexcept;

            RuleHolder & operator=(RuleHolder && other)  noexcept;

            void setRule(Rule * rule);

            using Rule::match;

            virtual std::optional<IteratorMatcher::MatchData> match(Iterator &iterator, UndoRedo *undo, bool doAction = true) override;

            virtual ~RuleHolder();
        };

        struct TemporaryAction : RuleHolder {
            using RuleHolder::RuleHolder;

            using Rule::match;

            virtual std::optional<IteratorMatcher::MatchData> match(Iterator &iterator, UndoRedo *undo, bool doAction = true) override;

        };

        struct LogCurrentCharacter : Rule {

            LogCurrentCharacter(Action action = NO_ACTION);

            using Rule::match;

            virtual std::optional<IteratorMatcher::MatchData> match(Iterator &iterator, UndoRedo *undo, bool doAction = true) override;
        };

        class LogMatchStatus : public RuleHolder {
#ifdef GTEST_API_
            public:
#endif
            CPP_RULES____STRING ruleName;
        public:
            LogMatchStatus(Rule *rule, const CPP_RULES____STRING & ruleName, Action action = NO_ACTION);

            using Rule::match;

            virtual std::optional<IteratorMatcher::MatchData> match(Iterator &iterator, UndoRedo *undo, bool doAction = true) override;

        };

        class LogCapture : public RuleHolder {
#ifdef GTEST_API_
            public:
#endif
            CPP_RULES____STRING ruleName;
        public:
            LogCapture(Rule *rule, const CPP_RULES____STRING & ruleName, Action action = NO_ACTION);

            using Rule::match;

            virtual std::optional<IteratorMatcher::MatchData> match(Iterator &iterator, UndoRedo *undo, bool doAction = true) override;

        };

        class LogInput : public RuleHolder {
#ifdef GTEST_API_
            public:
#endif
            CPP_RULES____STRING ruleName;
        public:
            LogInput(Rule *rule, const CPP_RULES____STRING & ruleName, Action action = NO_ACTION);

            using Rule::match;

            virtual std::optional<IteratorMatcher::MatchData> match(Iterator &iterator, UndoRedo *undo, bool doAction = true) override;

        };

        class LogTrace : public RuleHolder {
#ifdef GTEST_API_
            public:
#endif
            CPP_RULES____STRING ruleName;
        public:
            LogTrace(Rule *rule, const CPP_RULES____STRING & ruleName, Action action = NO_ACTION);

            using Rule::match;

            virtual std::optional<IteratorMatcher::MatchData> match(Iterator &iterator, UndoRedo *undo, bool doAction = true) override;

        };

        struct Optional : RuleHolder {

            Optional(Rule * rule, Action action = NO_ACTION);

            using Rule::match;

            virtual std::optional<IteratorMatcher::MatchData> match(Iterator &iterator, UndoRedo *undo, bool doAction = true) override;
        };

        struct OneOrMore : RuleHolder {

            OneOrMore(Rule * rule, Action action = NO_ACTION);

            using Rule::match;

            virtual std::optional<IteratorMatcher::MatchData> match(Iterator &iterator, UndoRedo *undo, bool doAction = true) override;
        };

        struct ZeroOrMore : RuleHolder {

            ZeroOrMore(Rule * rule, Action action = NO_ACTION);

            using Rule::match;

            virtual std::optional<IteratorMatcher::MatchData> match(Iterator &iterator, UndoRedo *undo, bool doAction = true) override;
        };

        struct MatchBUntilA : Rule {
            RuleHolder A;
            RuleHolder B;

            MatchBUntilA(Rule * A, Rule * B, Action action = NO_ACTION);

            using Rule::match;

            virtual std::optional<IteratorMatcher::MatchData> match(Iterator &iterator, UndoRedo *undo, bool doAction = true) override;
        };

        struct Or : Rule {
            CPP_RULES____VECTOR<RuleHolder> rules;

            Or(std::initializer_list<Rule*> rules, Action action = NO_ACTION);

            using Rule::match;

            virtual std::optional<IteratorMatcher::MatchData> match(Iterator &iterator, UndoRedo *undo, bool doAction = true) override;
        };

        struct Sequence : Rule {
            CPP_RULES____VECTOR<RuleHolder> rules;

            Sequence(std::initializer_list<Rule*> rules, Action action = NO_ACTION);

            using Rule::match;

            virtual std::optional<IteratorMatcher::MatchData> match(Iterator &iterator, UndoRedo *undo, bool doAction = true) override;
        };

        struct Until : RuleHolder {

            Until(Rule * rule, Action action = NO_ACTION);

            using Rule::match;

            virtual std::optional<IteratorMatcher::MatchData> match(Iterator &iterator, UndoRedo *undo, bool doAction = true) override;
        };

        struct Range : Rule {
            CPP_RULES____STRING letters;

            Range(std::initializer_list<char> letters, Action action = NO_ACTION);

            using Rule::match;

            virtual std::optional<IteratorMatcher::MatchData> match(Iterator &iterator, UndoRedo *undo, bool doAction = true) override;
        };

        struct At : RuleHolder {

            At(Rule * rule, Action action = NO_ACTION);

            using Rule::match;

            virtual std::optional<IteratorMatcher::MatchData> match(Iterator &iterator, UndoRedo *undo, bool doAction = true) override;
        };

        struct NotAt : RuleHolder {

            NotAt(Rule * rule, Action action = NO_ACTION);

            using Rule::match;

            virtual std::optional<IteratorMatcher::MatchData> match(Iterator &iterator, UndoRedo *undo, bool doAction = true) override;
        };
    }
}

#define CPP_RULES_ERROR2
#include "Error.h"
#undef CPP_RULES_ERROR2

#endif
