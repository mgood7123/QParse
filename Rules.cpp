#include "Rules.h"
#include CPP_RULES____COUT_INCLUDE

CPP::Rules::Action CPP::Rules::NO_ACTION = [](CPP::Rules::Input) {};
CPP_RULES____VECTOR<CPP::Rules::RuleHolder::Reference*> CPP::Rules::RuleHolder::rules;

CPP::Rules::Rule::Rule(Action action) : action(action) {}

std::optional<CPP::IteratorMatcher::MatchData> CPP::Rules::Rule::match(const char *string, bool doAction)
{
    if (undo != nullptr && allocated) {
        delete undo;
    }
    undo = new UndoRedo();
    allocated = true;
    return match(string, undo, doAction);
}

std::optional<CPP::IteratorMatcher::MatchData> CPP::Rules::Rule::match(CPP_RULES____STRING &string, bool doAction) {
    if (undo != nullptr && allocated) {
        delete undo;
    }
    undo = new UndoRedo();
    allocated = true;
    return match(string, undo, doAction);
}

std::optional<CPP::IteratorMatcher::MatchData> CPP::Rules::Rule::match(Iterator &iterator, bool doAction) {
    if (undo != nullptr && allocated) {
        delete undo;
    }
    undo = new UndoRedo();
    allocated = true;
    return match(iterator, undo, doAction);
}

std::optional<CPP::IteratorMatcher::MatchData> CPP::Rules::Rule::match(const char *string, UndoRedo *undo, bool doAction)
{
    Iterator iterator(string);
    return match(iterator, undo, doAction);
}

std::optional<CPP::IteratorMatcher::MatchData> CPP::Rules::Rule::match(CPP_RULES____STRING &string, UndoRedo *undo, bool doAction) {
    Iterator iterator(string);
    return match(iterator, undo, doAction);
}

std::optional<CPP::IteratorMatcher::MatchData> CPP::Rules::Rule::match(Iterator &iterator, UndoRedo *undo, bool doAction) {
    IteratorMatcher::MatchData match;
    match.matched = false;
    match.begin = iterator.current();
    match.end = iterator.current();
    return match;
}

CPP::Rules::Rule::~Rule() {
    if (allocated) delete undo;
}

CPP::Rules::Success::Success(Action action) : Rule(action) {}

std::optional<CPP::IteratorMatcher::MatchData> CPP::Rules::Success::match(Iterator &iterator, UndoRedo *undo, bool doAction) {
    IteratorMatcher::MatchData match;
    match.matched = true;
    match.begin = iterator.current();
    iterator.pushInfo();
    match.matches++;
    match.end = iterator.current();
    if (doAction) action(Input(iterator, match, undo, match.matches));
    return match;
}

CPP::Rules::AdvanceInputBy::AdvanceInputBy(Action action) : n(1), Rule(action) {}

CPP::Rules::AdvanceInputBy::AdvanceInputBy(int n, Action action) : n(n < 0 ? 0 : n), Rule(action) {}

std::optional<CPP::IteratorMatcher::MatchData> CPP::Rules::AdvanceInputBy::match(Iterator &iterator, UndoRedo *undo, bool doAction) {
    IteratorMatcher::MatchData match;
    match.matched = true;
    match.begin = iterator.current();
    iterator.pushInfo();
    match.matches++;
    iterator.advance(n);
    match.end = iterator.current();
    if (doAction) action(Input(iterator, match, undo, match.matches));
    return match;
}

CPP::Rules::Fail::Fail(Action action) : Rule(action) {}

std::optional<CPP::IteratorMatcher::MatchData> CPP::Rules::Fail::match(Iterator &iterator, UndoRedo *undo, bool doAction) {
    IteratorMatcher::MatchData match;
    match.matched = false;
    match.begin = iterator.current();
    match.end = iterator.current();
    if (doAction) action(Input(iterator, match, undo, match.matches));
    return match;
}

CPP::Rules::Any::Any(Action action) : Rule(action) {}

std::optional<CPP::IteratorMatcher::MatchData> CPP::Rules::Any::match(Iterator &iterator, UndoRedo *undo, bool doAction) {
    IteratorMatcher::MatchData match = IteratorMatcher::match(iterator);
    if (match && doAction) {
        action(Input(iterator, match, undo, match.matches));
    }
    return match;
}

CPP::Rules::Char::Char(char character, Action action) : character(character), Rule(action) {}

std::optional<CPP::IteratorMatcher::MatchData> CPP::Rules::Char::match(Iterator &iterator, UndoRedo *undo, bool doAction) {
    auto match = IteratorMatcher::match(iterator, character);
    if (match && doAction) {
        action(Input(iterator, match, undo, match.matches));
    }
    return match;
}

CPP::Rules::EndOfFile::EndOfFile(Action action) : Rule(action) {}

std::optional<CPP::IteratorMatcher::MatchData> CPP::Rules::EndOfFile::match(Iterator &iterator, UndoRedo *undo, bool doAction) {
    IteratorMatcher::MatchData match;
    match.begin = iterator.current();
    match.end = iterator.current();
    match.matched = false;
    if (!iterator.has_next()) {
        match.matched = true;
        iterator.pushInfo();
        match.matches++;
        if (doAction) action(Input(iterator, match, undo, match.matches));
        return match;
    }

    return match;
}

CPP::Rules::NewlineOrEOF::NewlineOrEOF(Action action) : Rule(action) {}

std::optional<CPP::IteratorMatcher::MatchData> CPP::Rules::NewlineOrEOF::match(Iterator &iterator, UndoRedo *undo, bool doAction) {
    auto match_ = Or({new String("\r\n"), new Char('\n')}).match(iterator, undo);
    if (!match_.has_value()) return std::nullopt;
    auto match = *match_;

    if (match.matched) {
        if (doAction) action(Input(iterator, match, undo, match.matches));
    } else if (!iterator.has_next()) {
        match.matched = true;
        iterator.pushInfo();
        match.matches++;
        if (doAction) action(Input(iterator, match, undo, match.matches));
    }

    return match;
}

CPP::Rules::Newline::Newline(Action action) : Rule(action) {}

std::optional<CPP::IteratorMatcher::MatchData> CPP::Rules::Newline::match(Iterator &iterator, UndoRedo *undo, bool doAction) {
    auto match_ = Or({new String("\r\n"), new Char('\n')}).match(iterator, undo);
    if (!match_.has_value()) return std::nullopt;
    auto match = *match_;

    if (match.matched && doAction) {
        action(Input(iterator, match, undo, match.matches));
    }
    return match;
}

CPP::Rules::String::String(const CPP_RULES____STRING &string, Action action) : string(string), Rule(action) {}

std::optional<CPP::IteratorMatcher::MatchData> CPP::Rules::String::match(Iterator &iterator, UndoRedo *undo, bool doAction) {
    auto match = IteratorMatcher::match(iterator, string);

    if (match && doAction) {
        action(Input(iterator, match, undo, match.matches));
    }
    return match;
}

CPP::Rules::RuleHolder::RuleHolder(Rule *rule, Action action) : Rule(action) {
    if (rule == nullptr) return;
    setRule(rule);
}

CPP::Rules::RuleHolder::RuleHolder(const RuleHolder &other) {
    ref = other.ref;
    rule = other.rule;
    if (ref != nullptr) {
        ref->reference++;
    }
}

CPP::Rules::RuleHolder &CPP::Rules::RuleHolder::operator=(const RuleHolder &other) {
    ref = other.ref;
    rule = other.rule;
    if (ref != nullptr) {
        ref->reference++;
    }
    return *this;
}

void CPP::Rules::RuleHolder::setRule(Rule *rule)
{
    if (rule == nullptr) {
        if (ref != nullptr) {
            if (ref->reference > 0) {
                ref->reference--;
                if (ref->reference == 0) {
                    CPP_RULES____VECTOR<Reference*>::iterator r = rules.begin();
                    CPP_RULES____VECTOR<Reference*>::const_iterator e = rules.cend();
                    while (r != e) {
                        if ((*r)->rule == ref->rule) {
                            break;
                        }
                        r++;
                    }
                    rules.erase(r);
                    delete ref->rule;
                    ref->rule = nullptr;
                    delete ref;
                    ref = nullptr;
                }
            }
        }
    } else {
        if (this->rule == nullptr) {
            bool exists = false;
            for (Reference * reference : rules) {
                if (reference->rule == rule) {
                    ref = reference;
                    exists = true;
                    break;
                }
            }
            if (!exists) {
                ref = new Reference();
                rules.push_back(ref);
            }
            ref->rule = rule;
            this->rule = ref->rule;
            ref->reference++;
        } else {
            if (this->rule != rule) {
                setRule(nullptr);
                setRule(rule);
            }
        }
    }
}

CPP::Rules::RuleHolder::RuleHolder(RuleHolder &&other) noexcept {
    ref = other.ref;
    rule = other.rule;
    if (ref != nullptr) {
        ref->reference++;
    }
}

CPP::Rules::RuleHolder &CPP::Rules::RuleHolder::operator=(RuleHolder &&other) noexcept {
    ref = other.ref;
    rule = other.rule;
    if (ref != nullptr) {
        ref->reference++;
    }
    return *this;
}

std::optional<CPP::IteratorMatcher::MatchData> CPP::Rules::RuleHolder::match(Iterator &iterator, UndoRedo *undo, bool doAction) {
    if (ref == nullptr) { // we have no rule currently set
        IteratorMatcher::MatchData match;
        match.begin = iterator.current();
        match.end = iterator.current();
        match.matched = true;
        iterator.pushInfo();
        match.matches++;
        if (doAction) action(Input(iterator, match, undo, match.matches));
        return match;
    }
    auto match_ = rule->match(iterator, undo, doAction);
    if (!match_.has_value()) return std::nullopt;
    auto match = *match_;
    if (match && doAction) {
        action(Input(iterator, match, undo, match.matches));
    }
    return match;
}

CPP::Rules::RuleHolder::~RuleHolder() {
    setRule(nullptr);
}

std::optional<CPP::IteratorMatcher::MatchData> CPP::Rules::TemporaryAction::match(Iterator &iterator, UndoRedo *undo, bool doAction) {
    auto match_ = rule->match(iterator, undo, false);
    if (!match_.has_value()) return std::nullopt;
    auto match = *match_;
    if (match && doAction) {
        action(Input(iterator, match, undo, match.matches));
    }
    return match;
}

CPP::Rules::LogCurrentCharacter::LogCurrentCharacter(Action action) : Rule(action) {}

std::optional<CPP::IteratorMatcher::MatchData> CPP::Rules::LogCurrentCharacter::match(Iterator &iterator, UndoRedo *undo, bool doAction) {
    IteratorMatcher::MatchData match;
    match.begin = iterator.current();
    match.matched = true;
    iterator.pushInfo();
    match.matches++;
    match.end = iterator.current();
    CPP_RULES____COUT << "current character: " << Input::quote(iterator.peekNext()) CPP_RULES____COUT_ENDL;
    if (doAction) action(Input(iterator, match, undo, match.matches));
    return match;
}

CPP::Rules::LogMatchStatus::LogMatchStatus(Rule *rule, const CPP_RULES____STRING &ruleName, Action action) : ruleName(ruleName), RuleHolder(rule, action) {}

std::optional<CPP::IteratorMatcher::MatchData> CPP::Rules::LogMatchStatus::match(Iterator &iterator, UndoRedo *undo, bool doAction) {
    auto match_ = rule->match(iterator, undo, doAction);
    if (!match_.has_value()) return std::nullopt;
    auto match = *match_;
    CPP_RULES____COUT << "rule '" << ruleName << "' was " << (match ? "matched" : "not matched") CPP_RULES____COUT_ENDL;
    if (doAction) action(Input(iterator, match, undo, match.matches));
    return match;
}

CPP::Rules::LogCapture::LogCapture(Rule *rule, const CPP_RULES____STRING &ruleName, Action action) : ruleName(ruleName), RuleHolder(rule, action) {}

std::optional<CPP::IteratorMatcher::MatchData> CPP::Rules::LogCapture::match(Iterator &iterator, UndoRedo *undo, bool doAction) {
    auto match_ = rule->match(iterator, undo, doAction);
    if (!match_.has_value()) return std::nullopt;
    auto match = *match_;
    if (match) {
        CPP_RULES____COUT << "rule '" << ruleName << "' captured " << Input(iterator, match, undo, 0).quotedString() CPP_RULES____COUT_ENDL;
    } else {
        CPP_RULES____COUT << "rule '" << ruleName << "' did not capture anything because it did not match" CPP_RULES____COUT_ENDL;
    }
    if (doAction) action(Input(iterator, match, undo, match.matches));
    return match;
}

CPP::Rules::LogInput::LogInput(Rule *rule, const CPP_RULES____STRING &ruleName, Action action) : ruleName(ruleName), RuleHolder(rule, action) {}

std::optional<CPP::IteratorMatcher::MatchData> CPP::Rules::LogInput::match(Iterator &iterator, UndoRedo *undo, bool doAction) {
    auto match_ = rule->match(iterator, undo, doAction);
    if (!match_.has_value()) return std::nullopt;
    auto match = *match_;
    CPP_RULES____COUT << "input after rule '" << ruleName << "' : " << iterator.currentString() CPP_RULES____COUT_ENDL;
    if (doAction) action(Input(iterator, match, undo, match.matches));
    return match;
}

CPP::Rules::Optional::Optional(Rule *rule, Action action) : RuleHolder(rule, action) {}

std::optional<CPP::IteratorMatcher::MatchData> CPP::Rules::Optional::match(Iterator &iterator, UndoRedo *undo, bool doAction) {
    IteratorMatcher::MatchData match;
    match.begin = iterator.current();
    match.end = iterator.current();
    auto tmp_ = rule->match(iterator, undo, doAction);
    if (!tmp_.has_value()) return std::nullopt;
    auto tmp = *tmp_;
    if (tmp) {
        match.end = tmp.end;
        match.matches = tmp.matches;
    }
    match.matched = true;
    iterator.pushInfo();
    match.matches++;
    if (doAction) action(Input(iterator, match, undo, match.matches));
    return match;
}

CPP::Rules::OneOrMore::OneOrMore(Rule *rule, Action action) : RuleHolder(rule, action) {}

std::optional<CPP::IteratorMatcher::MatchData> CPP::Rules::OneOrMore::match(Iterator &iterator, UndoRedo *undo, bool doAction) {
    auto match_ = rule->match(iterator, undo, doAction);
    if (!match_.has_value()) return std::nullopt;
    auto match = *match_;
    if (match) {
        while (true) {
            auto tmp_ = rule->match(iterator, undo, doAction);
            if (!tmp_.has_value()) return std::nullopt;
            auto tmp = *tmp_;
            if (!tmp) break;
            match.end = tmp.end;
            match.matches += tmp.matches;
        }
        if (doAction) action(Input(iterator, match, undo, match.matches));
    }

    return match;
}

CPP::Rules::ZeroOrMore::ZeroOrMore(Rule *rule, Action action) : RuleHolder(new Optional(new OneOrMore(rule)), action) {}

std::optional<CPP::IteratorMatcher::MatchData> CPP::Rules::ZeroOrMore::match(Iterator &iterator, UndoRedo *undo, bool doAction) {
    auto match_ = rule->match(iterator, undo, doAction);
    if (!match_.has_value()) return std::nullopt;
    auto match = *match_;
    if (match && doAction) {
        action(Input(iterator, match, undo, match.matches));
    }
    return match;
}

CPP::Rules::MatchBUntilA::MatchBUntilA(Rule *A, Rule *B, Action action) : A(A), B(B), Rule(action) {}

std::optional<CPP::IteratorMatcher::MatchData> CPP::Rules::MatchBUntilA::match(Iterator &iterator, UndoRedo *undo, bool doAction) {
    // until A matches, match B
    IteratorMatcher::MatchData match;
    match.begin = iterator.current();
    match.end = iterator.current();
    match.matched = false;
    while (true) {
        auto tmp_ = A.match(iterator, undo, doAction);
        if (!tmp_.has_value()) return std::nullopt;
        auto tmp = *tmp_;
        if (!tmp) {
            tmp_ = B.match(iterator, undo, doAction);
            if (!tmp_.has_value()) return std::nullopt;
            tmp = *tmp_;
            if (!tmp) {
                return match;
            }
            match.end = tmp.end;
            match.matches += tmp.matches;
        } else {
            match.matched = true;
            match.end = tmp.end;
            match.matches += tmp.matches;
            if (doAction) action(Input(iterator, match, undo, match.matches));
            return match;
        }
    }
}

CPP::Rules::Or::Or(std::initializer_list<Rule *> rules, Action action) : Rule(action) {
    for (Rule * rule : rules) {
        this->rules.push_back(rule);
    }
}

std::optional<CPP::IteratorMatcher::MatchData> CPP::Rules::Or::match(Iterator &iterator, UndoRedo *undo, bool doAction) {
    IteratorMatcher::MatchData match;
    match.begin = iterator.current();
    match.end = iterator.current();
    match.matched = false;
    if (rules.size() == 0) {
        match.matched = true;
        iterator.pushInfo();
        match.matches++;
        if (doAction) action(Input(iterator, match, undo, match.matches));
        return match;
    }
    for (Rule & rule : rules) {
        auto match_ = rule.match(iterator, undo, doAction);
        if (!match_.has_value()) return std::nullopt;
        match = *match_;
        if (match) {
            if (doAction) action(Input(iterator, match, undo, match.matches));
            return match;
        }
    }
    return match;
}

CPP::Rules::Sequence::Sequence(std::initializer_list<Rule *> rules, Action action) : Rule(action) {
    for (Rule * rule : rules) {
        this->rules.push_back(rule);
    }
}

std::optional<CPP::IteratorMatcher::MatchData> CPP::Rules::Sequence::match(Iterator &iterator, UndoRedo *undo, bool doAction) {
    IteratorMatcher::MatchData match;
    match.begin = iterator.current();
    match.end = iterator.current();
    match.matched = false;
    if (rules.size() == 0) {
        match.matched = true;
        iterator.pushInfo();
        match.matches++;
        if (doAction) action(Input(iterator, match, undo, match.matches));
        return match;
    }
    for (Rule & rule : rules) {
        auto tmp_ = rule.match(iterator, undo, doAction);
        if (!tmp_.has_value()) return std::nullopt;
        auto tmp = *tmp_;
        if (!tmp) {
            iterator.popInfo(match.matches);
            match.matches = 0;
            return match;
        }
        match.end = tmp.end;
        match.matches += tmp.matches;
    }
    match.matched = true;
    iterator.pushInfo();
    match.matches++;
    if (doAction) action(Input(iterator, match, undo, match.matches));
    return match;
}

CPP::Rules::Until::Until(Rule *rule, Action action) : RuleHolder(rule, action) {}

std::optional<CPP::IteratorMatcher::MatchData> CPP::Rules::Until::match(Iterator &iterator, UndoRedo *undo, bool doAction) {
    IteratorMatcher::MatchData match;
    match.begin = iterator.current();
    match.end = iterator.current();
    match.matched = false;
    while(iterator.has_next()) {
        auto tmp_ = rule->match(iterator, undo, doAction);
        if (!tmp_.has_value()) return std::nullopt;
        auto tmp = *tmp_;
        if (tmp) {
            match.matched = true;
            match.end = tmp.end;
            match.matches = tmp.matches;
            if (doAction) action(Input(iterator, match, undo, match.matches));
            return match;
        } else {
            iterator.advance();
        }
    }
    iterator.setCurrent(match.begin);
    return match;
}

CPP::Rules::Range::Range(std::initializer_list<char> letters, Action action) : Rule(action) {
    for (char letter : letters) {
        this->letters += letter;
    }
}

std::optional<CPP::IteratorMatcher::MatchData> CPP::Rules::Range::match(Iterator &iterator, UndoRedo *undo, bool doAction) {
    if (!iterator.has_next()) {
        // unexpected EOF
        return IteratorMatcher::MatchData(false);
    }
    IteratorMatcher::MatchData match;
    match.begin = iterator.current();
    match.end = iterator.current();
    iterator.pushInfo();
    CPP_RULES____CHAR ch = iterator.next();
    Iterator l(letters);
    while (l.has_next()) {
        CPP_RULES____CHAR low = l.next();
        if (ch >= low) {
            if (l.has_next()) {
                CPP_RULES____CHAR high = l.next();
                if (low != high && ch > high) {
                    continue;
                }
            }
        } else {
            l.advance();
            continue;
        }
        match.end = iterator.current();
        match.matched = true;
        match.matches++;
        if (doAction) action(Input(iterator, match, undo, match.matches));
        return match;
    }
    // input does not match
    iterator.popInfo();
    match.matches = 0;
    return match;
}

CPP::Rules::At::At(Rule *rule, Action action) : RuleHolder(rule, action) {}

std::optional<CPP::IteratorMatcher::MatchData> CPP::Rules::At::match(Iterator &iterator, UndoRedo *undo, bool doAction) {
    IteratorMatcher::MatchData match;
    match.begin = iterator.current();
    match.end = iterator.current();



    // we do not want to return if we recieve an error

    IteratorMatcher::MatchData tmp = false;

    auto tmp_= rule->match(iterator, undo, false);

    if (tmp_.has_value()) tmp = *tmp_;



    iterator.popInfo(tmp.matches);
    tmp.matches = 0;
    if (tmp) {
        match.matched = true;
        iterator.pushInfo();
        match.matches++;
        if (doAction) action(Input(iterator, match, undo, match.matches));
        return match;
    }
    match.matched = false;
    return match;
}

CPP::Rules::NotAt::NotAt(Rule *rule, Action action) : RuleHolder(rule, action) {}

std::optional<CPP::IteratorMatcher::MatchData> CPP::Rules::NotAt::match(Iterator &iterator, UndoRedo *undo, bool doAction) {
    IteratorMatcher::MatchData match;
    match.begin = iterator.current();
    match.end = iterator.current();




    // we do not want to return if we recieve an error

    IteratorMatcher::MatchData tmp = false;

    auto tmp_= rule->match(iterator, undo, false);

    if (tmp_.has_value()) tmp = *tmp_;



    iterator.popInfo(tmp.matches);
    tmp.matches = 0;
    if (!tmp) {
        match.matched = true;
        iterator.pushInfo();
        match.matches++;
        if (doAction) action(Input(iterator, match, undo, match.matches));
        return match;
    }
    match.matched = false;
    return match;
}

CPP::Rules::UndoRedo::UndoRedo()
{
}

CPP::Rules::UndoRedo::~UndoRedo()
{
    command = nullptr;
    for (Command *cmd : commandStack)
    {
        delete cmd;
    }
    commandStack = {};
}

void CPP::Rules::UndoRedo::disable_push_command()
{
    disable++;
}

void CPP::Rules::UndoRedo::enable_push_command()
{
    if (disable > 0) {
        disable--;
    }
}

void CPP::Rules::UndoRedo::push_command(const CPP_RULES____STRING & description)
{
    if (disable == 0) {
        command = new Command(this, description);
        commandStack.push_back(command);
    }
}

void CPP::Rules::UndoRedo::print(Printer printer)
{
    if (commandStack.empty()) return;
    size_t i_ = commandStack.size()-1;
    while(true) {
        auto cmd = commandStack[i_];
        size_t i = cmd->commandStack.size()-1;
        while(true) {
            COMMAND command = cmd->commandStack[i];
            switch (command)
            {
                case COMMAND::COMMAND__BUFFER:
                    printer(cmd->description);
                    break;
                default:
                    break;
            }
            if (i == 0) break;
            i--;
        }
        if (i_ == 0) break;
        i_--;
    }
}

bool CPP::Rules::UndoRedo::undo()
{
    size_t count = commandStack.size();
    if (count == 0 || level >= count) return false;

    size_t index = level;
    level++;

    Command *cmd = commandStack[(count-1) - index];
    size_t i = cmd->commandStack.size()-1;
    while(true) {
        COMMAND command = cmd->commandStack[i];
        switch (command)
        {
            case COMMAND::SUB_COMMAND__UNDO_CUSTOM_DATA:
            {
                cmd->customStack.undo();
                break;
            }
            case COMMAND::COMMAND__BUFFER:
            default:
                break;
        }
        if (i == 0) break;
        i--;
    }
    return true;
}

bool CPP::Rules::UndoRedo::redo()
{
    size_t count = commandStack.size();
    if (count == 0 || level <= 0) return false;

    level--;
    size_t index = level;

    Command *cmd = commandStack[(count-1) - index];
    size_t i = cmd->commandStack.size()-1;
    while(true) {
        COMMAND command = cmd->commandStack[i];
        switch (command)
        {
            case COMMAND::SUB_COMMAND__REDO_CUSTOM_DATA:
            {
                cmd->customStack.redo();
                break;
            }
            case COMMAND::COMMAND__BUFFER:
            default:
                break;
        }
        if (i == 0) break;
        i--;
    }
    return true;
}

CPP::Rules::UndoRedo::Command::Command(UndoRedo *parent, const CPP_RULES____STRING & description) : parent(parent), description(description)
{
    this->commandStack.push_back(COMMAND::COMMAND__BUFFER);
}

CPP::Rules::UndoRedo::UNDO_KEY_PTR CPP::Rules::UndoRedo::Command::push_undo_iterator(Iterator &iter)
{
    return push_undo_custom_data<std::pair<Iterator::SaveState, Iterator*>>("ITERATOR",
        { iter.save(), &iter },
        [](auto data) {
            data.second[0].load(data.first);
        }
    );
}

void CPP::Rules::UndoRedo::Command::push_redo_iterator(CPP::Rules::UndoRedo::UNDO_KEY_PTR undo_key, Iterator &iter)
{
    push_redo_custom_data<std::pair<Iterator::SaveState, Iterator*>>("ITERATOR",
        undo_key,
        { iter.save(), &iter },
        [](auto data) {
            data.second[0].load(data.first);
        }
    );
}

CPP::Rules::UndoRedo::UNDO_KEY_PTR CPP::Rules::UndoRedo::Command::push_undo_string_replacement(CPP_RULES____STRING *input, const CPP_RULES____STRING::const_iterator &begin, const CPP_RULES____STRING::const_iterator &end, const CPP_RULES____STRING &replacement)
{
    return push_undo_custom_data<
            std::pair<
                std::pair<CPP_RULES____STRING*, CPP_RULES____STRING::difference_type>,
                std::pair<CPP_RULES____STRING::difference_type, const CPP_RULES____STRING>
            >
    >("STRING REPLACE " + replacement,
        {
            {input, begin - input->cbegin()}, {end - input->cbegin(), replacement}
        },
        [](auto data) {
            CPP_RULES____STRING * a = data.first.first;
            CPP_RULES____STRING::difference_type b = data.first.second;
            CPP_RULES____STRING::difference_type c = data.second.first;
            const CPP_RULES____STRING d = data.second.second;
            CPP_RULES____STRING_REPLACE____STRING_PTR__ITERATOR_BEGIN__ITERATOR_END__STRING(a, a->cbegin() + b, a->cbegin() + c, d);
        }
    );
}

void CPP::Rules::UndoRedo::Command::push_redo_string_replacement(CPP::Rules::UndoRedo::UNDO_KEY_PTR undo_key, CPP_RULES____STRING *input, const CPP_RULES____STRING::const_iterator &begin, const CPP_RULES____STRING::const_iterator &end, const CPP_RULES____STRING &replacement)
{
    push_redo_custom_data<
            std::pair<
                std::pair<CPP_RULES____STRING*, CPP_RULES____STRING::difference_type>,
                std::pair<CPP_RULES____STRING::difference_type, const CPP_RULES____STRING>
            >
    >("STRING REPLACE " + replacement,
        undo_key,
        {
            {input, begin - input->cbegin()}, {end - input->cbegin(), replacement}
        },
        [](auto data) {
            CPP_RULES____STRING * a = data.first.first;
            CPP_RULES____STRING::difference_type b = data.first.second;
            CPP_RULES____STRING::difference_type c = data.second.first;
            const CPP_RULES____STRING d = data.second.second;
            CPP_RULES____STRING_REPLACE____STRING_PTR__ITERATOR_BEGIN__ITERATOR_END__STRING(a, a->cbegin() + b, a->cbegin() + c, d);
        }
    );
}

CPP::Rules::UndoRedo::UNDO_KEY_PTR CPP::Rules::UndoRedo::Command::push_undo_string_insertion(CPP_RULES____STRING *input, const CPP_RULES____STRING::const_iterator &begin, const CPP_RULES____STRING::const_iterator &end)
{
    return push_undo_custom_data<
            std::pair<
                std::pair<CPP_RULES____STRING*, CPP_RULES____STRING::difference_type>,
                CPP_RULES____STRING::difference_type
            >
    >("STRING INSERT",
        {
            {input, begin - input->cbegin()},
            end - input->cbegin()
        },
        [](auto data) {
            CPP_RULES____STRING * a = data.first.first;
            CPP_RULES____STRING::difference_type b = data.first.second;
            CPP_RULES____STRING::difference_type c = data.second;
            CPP_RULES____STRING_REPLACE____STRING_PTR__ITERATOR_BEGIN__ITERATOR_END__STRING(a, a->cbegin() + b, a->cbegin() + c, "");
        }
    );
}

void CPP::Rules::UndoRedo::Command::push_redo_string_insertion(CPP::Rules::UndoRedo::UNDO_KEY_PTR undo_key, CPP_RULES____STRING *input, const CPP_RULES____STRING::const_iterator &end, const CPP_RULES____STRING &replacement)
{
    push_redo_custom_data<
            std::pair<
                std::pair<CPP_RULES____STRING*, CPP_RULES____STRING::difference_type>,
                const CPP_RULES____STRING
            >
    >("STRING INSERT " + replacement,
        undo_key,
        {
            {input, end - input->cbegin()},
            replacement
        },
        [](auto data) {
            CPP_RULES____STRING * a = data.first.first;
            CPP_RULES____STRING::difference_type b = data.first.second;
            const CPP_RULES____STRING c = data.second;
            CPP_RULES____STRING_INSERT____STRING_PTR__INTEGER__STRING(a, b, c);
        }
    );
}


CPP::Rules::UndoRedo::Command::CustomDataStack::~CustomDataStack() {
    for(auto p : stack) {
        if (p != nullptr) {
            p->deleter(p->data); // invoke deleter
            if (p->redo != nullptr) {
                p->redo->deleter(p->redo->data); // invoke deleter
                delete p->redo;
            }
            delete p;
        }
    }
}

bool CPP::Rules::UndoRedo::Command::CustomDataStack::undo() {
    size_t count = stack.size();
    if (count == 0 || level >= count) return false;

    size_t index = level;
    level++;

    LinkData * data = stack[index];
    data->invoker(data->data);
    return true;
}

bool CPP::Rules::UndoRedo::Command::CustomDataStack::redo() {
    size_t count = stack.size();
    if (count == 0 || level <= 0) return false;

    level--;
    size_t index = level;

    LinkData * data = stack[index]->redo;
    data->invoker(data->data);
    return true;
}
