#include "Rules.h"
#include QParse_RULES____COUT_INCLUDE

QParse::Rules::Action QParse::Rules::NO_ACTION = [](QParse::Rules::Input) {};
QParse_RULES____VECTOR<QParse::Rules::RuleHolder::Reference*> QParse::Rules::RuleHolder::rules;

QParse::Rules::Rule::Rule(Action action) : action(action) {}

std::optional<QParse::IteratorMatcher::MatchData> QParse::Rules::Rule::match(const char *string, bool doAction, bool logErrors)
{
    if (undo != nullptr && allocated) {
        delete undo;
    }
    undo = new UndoRedo();
    allocated = true;
    return match(string, undo, doAction, logErrors);
}

std::optional<QParse::IteratorMatcher::MatchData> QParse::Rules::Rule::match(QParse_RULES____STRING &string, bool doAction, bool logErrors) {
    if (undo != nullptr && allocated) {
        delete undo;
    }
    undo = new UndoRedo();
    allocated = true;
    return match(string, undo, doAction, logErrors);
}

std::optional<QParse::IteratorMatcher::MatchData> QParse::Rules::Rule::match(Iterator &iterator, bool doAction, bool logErrors) {
    if (undo != nullptr && allocated) {
        delete undo;
    }
    undo = new UndoRedo();
    allocated = true;
    return match(iterator, undo, doAction, logErrors);
}

std::optional<QParse::IteratorMatcher::MatchData> QParse::Rules::Rule::match(const char *string, UndoRedo *undo, bool doAction, bool logErrors)
{
    Iterator iterator(string);
    return match(iterator, undo, doAction, logErrors);
}

std::optional<QParse::IteratorMatcher::MatchData> QParse::Rules::Rule::match(QParse_RULES____STRING &string, UndoRedo *undo, bool doAction, bool logErrors) {
    Iterator iterator(string);
    return match(iterator, undo, doAction, logErrors);
}

std::optional<QParse::IteratorMatcher::MatchData> QParse::Rules::Rule::match(Iterator &iterator, UndoRedo *undo, bool doAction, bool logErrors) {
    return IteratorMatcher::MatchData(iterator, false);
}

QParse::Rules::Rule::~Rule() {
    if (allocated) delete undo;
}

QParse::Rules::Success::Success(Action action) : Rule(action) {}

std::optional<QParse::IteratorMatcher::MatchData> QParse::Rules::Success::match(Iterator &iterator, UndoRedo *undo, bool doAction, bool logErrors) {
    IteratorMatcher::MatchData match(iterator, true);
    iterator.pushInfo();
    match.matches++;
    if (doAction) action(Input(iterator, match, undo, match.matches));
    return match;
}

QParse::Rules::AdvanceInputBy::AdvanceInputBy(Action action) : Rule(action), n(1) {}

QParse::Rules::AdvanceInputBy::AdvanceInputBy(int n, Action action) : Rule(action), n(n < 0 ? 0 : n) {}

std::optional<QParse::IteratorMatcher::MatchData> QParse::Rules::AdvanceInputBy::match(Iterator &iterator, UndoRedo *undo, bool doAction, bool logErrors) {
    IteratorMatcher::MatchData match(iterator, true);
    iterator.pushInfo();
    match.matches++;
    iterator.advance(n);
    match.end = iterator.current();
    if (doAction) action(Input(iterator, match, undo, match.matches));
    return match;
}

QParse::Rules::Fail::Fail(Action action) : Rule(action) {}

std::optional<QParse::IteratorMatcher::MatchData> QParse::Rules::Fail::match(Iterator &iterator, UndoRedo *undo, bool doAction, bool logErrors) {
    IteratorMatcher::MatchData match(iterator, false);
    if (doAction) action(Input(iterator, match, undo, match.matches));
    return match;
}

QParse::Rules::Any::Any(Action action) : Rule(action) {}

std::optional<QParse::IteratorMatcher::MatchData> QParse::Rules::Any::match(Iterator &iterator, UndoRedo *undo, bool doAction, bool logErrors) {
    IteratorMatcher::MatchData match = IteratorMatcher::match(iterator);
    if (match && doAction) {
        action(Input(iterator, match, undo, match.matches));
    }
    return match;
}

QParse::Rules::Char::Char(char character, Action action) : Rule(action), character(character) {}

std::optional<QParse::IteratorMatcher::MatchData> QParse::Rules::Char::match(Iterator &iterator, UndoRedo *undo, bool doAction, bool logErrors) {
    auto match = IteratorMatcher::match(iterator, character);
    if (match && doAction) {
        action(Input(iterator, match, undo, match.matches));
    }
    return match;
}

QParse::Rules::EndOfFile::EndOfFile(Action action) : Rule(action) {}

std::optional<QParse::IteratorMatcher::MatchData> QParse::Rules::EndOfFile::match(Iterator &iterator, UndoRedo *undo, bool doAction, bool logErrors) {
    IteratorMatcher::MatchData match(iterator, false);
    if (!iterator.has_next()) {
        match.matched = true;
        iterator.pushInfo();
        match.matches++;
        if (doAction) action(Input(iterator, match, undo, match.matches));
        return match;
    }

    return match;
}

QParse::Rules::NewlineOrEOF::NewlineOrEOF(Action action) : Rule(action) {}

std::optional<QParse::IteratorMatcher::MatchData> QParse::Rules::NewlineOrEOF::match(Iterator &iterator, UndoRedo *undo, bool doAction, bool logErrors) {
    auto match_ = Or({new String("\r\n"), new Char('\n')}).match(iterator, undo);
    if (!match_.has_value()) return std::nullopt;
    auto match = *match_;

    if (!match) {
      iterator.popInfo(match.matches);
      match.matches = 0;
    }
    
    if (match) {
        if (doAction) action(Input(iterator, match, undo, match.matches));
    } else if (!iterator.has_next()) {
        match.matched = true;
        iterator.pushInfo();
        match.matches++;
        if (doAction) action(Input(iterator, match, undo, match.matches));
    }

    return match;
}

QParse::Rules::Newline::Newline(Action action) : Rule(action) {}

std::optional<QParse::IteratorMatcher::MatchData> QParse::Rules::Newline::match(Iterator &iterator, UndoRedo *undo, bool doAction, bool logErrors) {
    auto match_ = Or({new String("\r\n"), new Char('\n')}).match(iterator, undo);
    if (!match_.has_value()) return std::nullopt;
    auto match = *match_;
    
    if (!match) {
      iterator.popInfo(match.matches);
      match.matches = 0;
    }

    if (match && doAction) {
        action(Input(iterator, match, undo, match.matches));
    }
    return match;
}

QParse::Rules::String::String(const QParse_RULES____STRING &string, Action action) : Rule(action), string(string) {}

std::optional<QParse::IteratorMatcher::MatchData> QParse::Rules::String::match(Iterator &iterator, UndoRedo *undo, bool doAction, bool logErrors) {
    auto match = IteratorMatcher::match(iterator, string);

    if (!match) {
      iterator.popInfo(match.matches);
      match.matches = 0;
    }
    if (match && doAction) {
        action(Input(iterator, match, undo, match.matches));
    }
    return match;
}

QParse::Rules::RuleHolder::RuleHolder(Rule *rule, Action action) : Rule(action) {
    if (rule == nullptr) return;
    setRule(rule);
}

QParse::Rules::RuleHolder::RuleHolder(const RuleHolder &other) {
    ref = other.ref;
    rule = other.rule;
    if (ref != nullptr) {
        ref->reference++;
    }
}

QParse::Rules::RuleHolder &QParse::Rules::RuleHolder::operator=(const RuleHolder &other) {
    ref = other.ref;
    rule = other.rule;
    if (ref != nullptr) {
        ref->reference++;
    }
    return *this;
}

void QParse::Rules::RuleHolder::setRule(Rule *rule)
{
    if (rule == nullptr) {
        if (ref != nullptr) {
            if (ref->reference > 0) {
                ref->reference--;
                if (ref->reference == 0) {
                    QParse_RULES____VECTOR<Reference*>::iterator r = rules.begin();
                    QParse_RULES____VECTOR<Reference*>::const_iterator e = rules.cend();
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

QParse::Rules::RuleHolder::RuleHolder(RuleHolder &&other) noexcept {
    ref = other.ref;
    rule = other.rule;
    if (ref != nullptr) {
        ref->reference++;
    }
}

QParse::Rules::RuleHolder &QParse::Rules::RuleHolder::operator=(RuleHolder &&other) noexcept {
    ref = other.ref;
    rule = other.rule;
    if (ref != nullptr) {
        ref->reference++;
    }
    return *this;
}

std::optional<QParse::IteratorMatcher::MatchData> QParse::Rules::RuleHolder::match(Iterator &iterator, UndoRedo *undo, bool doAction, bool logErrors) {
    if (ref == nullptr) { // we have no rule currently set
        IteratorMatcher::MatchData match(iterator, true);
        iterator.pushInfo();
        match.matches++;
        if (doAction) action(Input(iterator, match, undo, match.matches));
        return match;
    }
    auto match_ = rule->match(iterator, undo, doAction, logErrors);
    if (!match_.has_value()) return std::nullopt;
    auto match = *match_;
    if (!match) {
      iterator.popInfo(match.matches);
      match.matches = 0;
    }
    if (match && doAction) {
        action(Input(iterator, match, undo, match.matches));
    }
    return match;
}

QParse::Rules::RuleHolder::~RuleHolder() {
    setRule(nullptr);
}

std::optional<QParse::IteratorMatcher::MatchData> QParse::Rules::TemporaryAction::match(Iterator &iterator, UndoRedo *undo, bool doAction, bool logErrors) {
    auto match_ = rule->match(iterator, undo, false, logErrors);
    if (!match_.has_value()) return std::nullopt;
    auto match = *match_;
    if (!match) {
      iterator.popInfo(match.matches);
      match.matches = 0;
    }
    if (match && doAction) {
        action(Input(iterator, match, undo, match.matches));
    }
    return match;
}

QParse::Rules::LogCurrentCharacter::LogCurrentCharacter(Action action) : Rule(action) {}

std::optional<QParse::IteratorMatcher::MatchData> QParse::Rules::LogCurrentCharacter::match(Iterator &iterator, UndoRedo *undo, bool doAction, bool logErrors) {
    IteratorMatcher::MatchData match(iterator, true);
    iterator.pushInfo();
    match.matches++;
    if (iterator.enable_logging) QParse_RULES____COUT << "current character: " << Input::quote(iterator.peekNext()) QParse_RULES____COUT_ENDL;
    if (doAction) action(Input(iterator, match, undo, match.matches));
    return match;
}

QParse::Rules::LogMatchStatus::LogMatchStatus(Rule *rule, const QParse_RULES____STRING &ruleName, Action action) : RuleHolder(rule, action), ruleName(ruleName) {}

std::optional<QParse::IteratorMatcher::MatchData> QParse::Rules::LogMatchStatus::match(Iterator &iterator, UndoRedo *undo, bool doAction, bool logErrors) {
    auto match_ = rule->match(iterator, undo, doAction, logErrors);
    if (!match_.has_value()) return std::nullopt;
    auto match = *match_;
    if (!match) {
      iterator.popInfo(match.matches);
      match.matches = 0;
    }
    if (iterator.enable_logging) QParse_RULES____COUT << "rule '" << ruleName << "' was " << (match ? "matched" : "not matched") QParse_RULES____COUT_ENDL;
    if (doAction) action(Input(iterator, match, undo, match.matches));
    return match;
}

QParse::Rules::LogCapture::LogCapture(Rule *rule, const QParse_RULES____STRING &ruleName, Action action) : RuleHolder(rule, action), ruleName(ruleName) {}

std::optional<QParse::IteratorMatcher::MatchData> QParse::Rules::LogCapture::match(Iterator &iterator, UndoRedo *undo, bool doAction, bool logErrors) {
    auto match_ = rule->match(iterator, undo, doAction, logErrors);
    if (!match_.has_value()) return std::nullopt;
    auto match = *match_;
    if (!match) {
      iterator.popInfo(match.matches);
      match.matches = 0;
    }
    if (iterator.enable_logging) {
        if (match) {
            QParse_RULES____COUT << "rule '" << ruleName << "' captured " << Input(iterator, match, undo, 0).quotedString() <<"\n" QParse_RULES____COUT_ENDL;
        } else {
            QParse_RULES____COUT << "rule '" << ruleName << "' did not capture anything because it did not match" <<"\n" QParse_RULES____COUT_ENDL;
        }
    }
    if (doAction) action(Input(iterator, match, undo, match.matches));
    return match;
}

QParse::Rules::LogInput::LogInput(Rule *rule, const QParse_RULES____STRING &ruleName, Action action) : RuleHolder(rule, action), ruleName(ruleName) {}

std::optional<QParse::IteratorMatcher::MatchData> QParse::Rules::LogInput::match(Iterator &iterator, UndoRedo *undo, bool doAction, bool logErrors) {
    auto match_ = rule->match(iterator, undo, doAction, logErrors);
    if (!match_.has_value()) return std::nullopt;
    auto match = *match_;
    if (!match) {
      iterator.popInfo(match.matches);
      match.matches = 0;
    }
    if (iterator.enable_logging) QParse_RULES____COUT << "input after rule '" << ruleName << "' : " << iterator.currentString() QParse_RULES____COUT_ENDL;
    if (doAction) action(Input(iterator, match, undo, match.matches));
    return match;
}

QParse::Rules::LogTrace::LogTrace(Rule *rule, const QParse_RULES____STRING &ruleName, Action action) : RuleHolder(rule, action), ruleName(ruleName) {}

std::optional<QParse::IteratorMatcher::MatchData> QParse::Rules::LogTrace::match(Iterator &iterator, UndoRedo *undo, bool doAction, bool logErrors) {
    auto match_ = rule->match(iterator, undo, doAction, logErrors);
    if (!match_.has_value()) return std::nullopt;
    auto match = *match_;
    if (!match) {
      iterator.popInfo(match.matches);
      match.matches = 0;
    }
    if (iterator.enable_logging) {
        QParse_RULES____COUT << "rule: " << ruleName QParse_RULES____COUT_ENDL;
        QParse_RULES____COUT << "    match: " << (match ? "true" : "false") QParse_RULES____COUT_ENDL;
        QParse_RULES____COUT << "    doAction: " << (doAction ? "true" : "false") QParse_RULES____COUT_ENDL;
        QParse_RULES____COUT << "    logErrors: " << (logErrors ? "true" : "false") QParse_RULES____COUT_ENDL;
        if (match) {
            QParse_RULES____COUT << "    capture: " << Input(iterator, match, undo, 0).quotedString() QParse_RULES____COUT_ENDL;
        }
        QParse_RULES____COUT << "    input line: " QParse_RULES____COUT_ENDL;
        undo->print_error(iterator, "    ");
        undo->print([&](auto description) {
            QParse_RULES____COUT_NO_SPACE << description QParse_RULES____COUT_ENDL;
            undo->undo();
            undo->print_error(iterator, "    ");
        });
        while(undo->redo()) {};
    }
    if (doAction) action(Input(iterator, match, undo, match.matches));
    return match;
}

QParse::Rules::Optional::Optional(Rule *rule, Action action) : RuleHolder(rule, action) {}

std::optional<QParse::IteratorMatcher::MatchData> QParse::Rules::Optional::match(Iterator &iterator, UndoRedo *undo, bool doAction, bool logErrors) {
    auto tmp_ = rule->match(iterator, undo, doAction, logErrors);
    if (!tmp_.has_value()) return std::nullopt;
    auto match = *tmp_;
    if (!match) {
      iterator.popInfo(match.matches);
      match.matches = 0;
      match.matched = true;
    }
    if (doAction) action(Input(iterator, match, undo, match.matches));
    return match;
}

QParse::Rules::OneOrMore::OneOrMore(Rule *rule, Action action) : RuleHolder(rule, action) {}

std::optional<QParse::IteratorMatcher::MatchData> QParse::Rules::OneOrMore::match(Iterator &iterator, UndoRedo *undo, bool doAction, bool logErrors) {
    auto match_ = rule->match(iterator, undo, doAction, logErrors);
    if (!match_.has_value()) return std::nullopt;
    auto match = *match_;
    if (!match) {
      iterator.popInfo(match.matches);
      match.matches = 0;
    }
    if (match) {
        while (true) {
            auto tmp_ = rule->match(iterator, undo, doAction);
            if (!tmp_.has_value()) {
              iterator.popInfo(match.matches);
              return std::nullopt;
            }
            auto tmp = *tmp_;
            if (!tmp) {
              iterator.popInfo(tmp.matches);
              break;
            }
            match.matches += tmp.matches;
            match.end = tmp.end;
        }
        if (doAction) action(Input(iterator, match, undo, match.matches));
    }

    return match;
}

QParse::Rules::ZeroOrMore::ZeroOrMore(Rule *rule, Action action) : RuleHolder(new Optional(new OneOrMore(rule)), action) {}

std::optional<QParse::IteratorMatcher::MatchData> QParse::Rules::ZeroOrMore::match(Iterator &iterator, UndoRedo *undo, bool doAction, bool logErrors) {
    auto match_ = rule->match(iterator, undo, doAction, logErrors);
    if (!match_.has_value()) return std::nullopt;
    auto match = *match_;
    if (!match) {
      iterator.popInfo(match.matches);
      match.matches = 0;
    }
    if (match && doAction) {
        action(Input(iterator, match, undo, match.matches));
    }
    return match;
}

QParse::Rules::MatchBUntilA::MatchBUntilA(Rule *A, Rule *B, Action action) : Rule(action), A(A), B(B) {}

std::optional<QParse::IteratorMatcher::MatchData> QParse::Rules::MatchBUntilA::match(Iterator &iterator, UndoRedo *undo, bool doAction, bool logErrors) {
    // until A matches, match B
    IteratorMatcher::MatchData match(iterator, false);
    while (true) {
        auto tmp_ = A.match(iterator, undo, doAction, logErrors);
        if (!tmp_.has_value()) {
          iterator.popInfo(match.matches);
          return std::nullopt;
        }
        auto tmp = *tmp_;
        if (!tmp) {
            iterator.popInfo(tmp.matches);
            tmp_ = B.match(iterator, undo, doAction, logErrors);
            if (!tmp_.has_value()) {
              iterator.popInfo(match.matches);
              return std::nullopt;
            }
            tmp = *tmp_;
            match.matches += tmp.matches;
            if (!tmp) {
                iterator.popInfo(match.matches);
                match.matches = 0;
                return match;
            }
            match.end = tmp.end;
        } else {
            match.matched = true;
            match.end = tmp.end;
            match.matches += tmp.matches;
            if (doAction) action(Input(iterator, match, undo, match.matches));
            return match;
        }
    }
}

QParse::Rules::Or::Or(std::initializer_list<Rule *> rules, Action action) : Rule(action) {
    for (Rule * rule : rules) {
        this->rules.push_back(rule);
    }
}

std::optional<QParse::IteratorMatcher::MatchData> QParse::Rules::Or::match(Iterator &iterator, UndoRedo *undo, bool doAction, bool logErrors) {
    IteratorMatcher::MatchData match(iterator, false);
    if (rules.size() == 0) {
        match.matched = true;
        iterator.pushInfo();
        match.matches++;
        if (doAction) action(Input(iterator, match, undo, match.matches));
        return match;
    }
    for (Rule & rule : rules) {
        auto match_ = rule.match(iterator, undo, doAction, logErrors);
        if (!match_.has_value()) {
          iterator.popInfo(match.matches);
          return std::nullopt;
        }
        match = *match_;
        if (!match) {
          iterator.popInfo(match.matches);
          match.matches = 0;
        }
        if (match) {
            if (doAction) action(Input(iterator, match, undo, match.matches));
            return match;
        }
    }
    return match;
}

QParse::Rules::Sequence::Sequence(std::initializer_list<Rule *> rules, Action action) : Rule(action) {
    for (Rule * rule : rules) {
        this->rules.push_back(rule);
    }
}

std::optional<QParse::IteratorMatcher::MatchData> QParse::Rules::Sequence::match(Iterator &iterator, UndoRedo *undo, bool doAction, bool logErrors) {
    IteratorMatcher::MatchData match(iterator, false);
    if (rules.size() == 0) {
        match.matched = true;
        iterator.pushInfo();
        match.matches++;
        if (doAction) action(Input(iterator, match, undo, match.matches));
        return match;
    }
    for (Rule & rule : rules) {
        auto tmp_ = rule.match(iterator, undo, doAction, logErrors);
        if (!tmp_.has_value()) {
          iterator.popInfo(match.matches);
          return std::nullopt;
        }
        auto tmp = *tmp_;
        match.matches += tmp.matches;
        if (!tmp) {
            iterator.popInfo(match.matches);
            match.matches = 0;
            return match;
        }
        match.end = tmp.end;
    }
    match.matched = true;
    iterator.pushInfo();
    match.matches++;
    if (doAction) action(Input(iterator, match, undo, match.matches));
    return match;
}

QParse::Rules::Until::Until(Rule *rule, Action action) : RuleHolder(rule, action) {}

std::optional<QParse::IteratorMatcher::MatchData> QParse::Rules::Until::match(Iterator &iterator, UndoRedo *undo, bool doAction, bool logErrors) {
    return MatchBUntilA(new RuleHolder(rule), new AdvanceInputBy(1), action).match(iterator, undo, doAction, logErrors);
    /*
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
    */
}

QParse::Rules::Range::Range(std::initializer_list<char> letters, Action action) : Rule(action) {
    for (char letter : letters) {
        this->letters += letter;
    }
}

std::optional<QParse::IteratorMatcher::MatchData> QParse::Rules::Range::match(Iterator &iterator, UndoRedo *undo, bool doAction, bool logErrors) {
    if (!iterator.has_next()) {
        // unexpected EOF
        return IteratorMatcher::MatchData(iterator, false);
    }
    IteratorMatcher::MatchData match(iterator, false);
    iterator.pushInfo();
    QParse_RULES____CHAR ch = iterator.next();
    Iterator l(letters);
    while (l.has_next()) {
        QParse_RULES____CHAR low = l.next();
        if (ch >= low) {
            if (l.has_next()) {
                QParse_RULES____CHAR high = l.next();
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

QParse::Rules::At::At(Rule *rule, Action action) : RuleHolder(rule, action) {}

std::optional<QParse::IteratorMatcher::MatchData> QParse::Rules::At::match(Iterator &iterator, UndoRedo *undo, bool doAction, bool logErrors) {
    auto match_ = rule->match(iterator, undo, false, false);
    if (!match_.has_value()) return IteratorMatcher::MatchData(iterator, false);
    auto match = *match_;
    iterator.popInfo(match.matches);
    match.matches = 0;
    if (match && doAction) {
        action(Input(iterator, match, undo, match.matches));
    }
    return match;
}

QParse::Rules::NotAt::NotAt(Rule *rule, Action action) : RuleHolder(rule, action) {}

std::optional<QParse::IteratorMatcher::MatchData> QParse::Rules::NotAt::match(Iterator &iterator, UndoRedo *undo, bool doAction, bool logErrors) {
   auto match_ = rule->match(iterator, undo, false, false);
    auto match = match_.has_value() ? *match_ : IteratorMatcher::MatchData(iterator, false);
    iterator.popInfo(match.matches);
    match.matches = 0;
    match.matched = !match.matched;
    if (match && doAction) {
        action(Input(iterator, match, undo, match.matches));
    }
    return match;
}

QParse::Rules::UndoRedo::UndoRedo()
{
}

QParse::Rules::UndoRedo::~UndoRedo()
{
    command = nullptr;
    for (Command *cmd : commandStack)
    {
        delete cmd;
    }
    commandStack = {};
}

void QParse::Rules::UndoRedo::disable_push_command()
{
    disable++;
}

void QParse::Rules::UndoRedo::enable_push_command()
{
    if (disable > 0) {
        disable--;
    }
}

void QParse::Rules::UndoRedo::push_command(const QParse_RULES____STRING & description)
{
    if (disable == 0) {
        command = new Command(this, description);
        commandStack.push_back(command);
    }
}

void QParse::Rules::UndoRedo::print(Printer printer)
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

bool QParse::Rules::UndoRedo::undo()
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

bool QParse::Rules::UndoRedo::redo()
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

QParse::Rules::UndoRedo::Command::Command(UndoRedo *parent, const QParse_RULES____STRING & description) : parent(parent), description(description)
{
    this->commandStack.push_back(COMMAND::COMMAND__BUFFER);
}

QParse::Rules::UndoRedo::UNDO_KEY_PTR QParse::Rules::UndoRedo::Command::push_undo_iterator(Iterator &iter)
{
    return push_undo_custom_data<std::pair<Iterator::SaveState, Iterator*>>("ITERATOR",
        { iter.save(), &iter },
        [](auto data) {
            data.second[0].load(data.first);
        }
    );
}

void QParse::Rules::UndoRedo::Command::push_redo_iterator(QParse::Rules::UndoRedo::UNDO_KEY_PTR undo_key, Iterator &iter)
{
    push_redo_custom_data<std::pair<Iterator::SaveState, Iterator*>>("ITERATOR",
        undo_key,
        { iter.save(), &iter },
        [](auto data) {
            data.second[0].load(data.first);
        }
    );
}

QParse::Rules::UndoRedo::UNDO_KEY_PTR QParse::Rules::UndoRedo::Command::push_undo_string_replacement(QParse_RULES____STRING *input, const QParse_RULES____STRING::const_iterator &begin, const QParse_RULES____STRING::const_iterator &end, const QParse_RULES____STRING &replacement)
{
    return push_undo_custom_data<
            std::pair<
                std::pair<QParse_RULES____STRING*, QParse_RULES____STRING::difference_type>,
                std::pair<QParse_RULES____STRING::difference_type, const QParse_RULES____STRING>
            >
    >("STRING REPLACE " + replacement,
        {
            {input, begin - input->cbegin()}, {end - input->cbegin(), replacement}
        },
        [](auto data) {
            QParse_RULES____STRING * a = data.first.first;
            QParse_RULES____STRING::difference_type b = data.first.second;
            QParse_RULES____STRING::difference_type c = data.second.first;
            const QParse_RULES____STRING d = data.second.second;
            QParse_RULES____STRING_REPLACE____STRING_PTR__ITERATOR_BEGIN__ITERATOR_END__STRING(a, a->cbegin() + b, a->cbegin() + c, d);
        }
    );
}

void QParse::Rules::UndoRedo::Command::push_redo_string_replacement(QParse::Rules::UndoRedo::UNDO_KEY_PTR undo_key, QParse_RULES____STRING *input, const QParse_RULES____STRING::const_iterator &begin, const QParse_RULES____STRING::const_iterator &end, const QParse_RULES____STRING &replacement)
{
    push_redo_custom_data<
            std::pair<
                std::pair<QParse_RULES____STRING*, QParse_RULES____STRING::difference_type>,
                std::pair<QParse_RULES____STRING::difference_type, const QParse_RULES____STRING>
            >
    >("STRING REPLACE " + replacement,
        undo_key,
        {
            {input, begin - input->cbegin()}, {end - input->cbegin(), replacement}
        },
        [](auto data) {
            QParse_RULES____STRING * a = data.first.first;
            QParse_RULES____STRING::difference_type b = data.first.second;
            QParse_RULES____STRING::difference_type c = data.second.first;
            const QParse_RULES____STRING d = data.second.second;
            QParse_RULES____STRING_REPLACE____STRING_PTR__ITERATOR_BEGIN__ITERATOR_END__STRING(a, a->cbegin() + b, a->cbegin() + c, d);
        }
    );
}

QParse::Rules::UndoRedo::UNDO_KEY_PTR QParse::Rules::UndoRedo::Command::push_undo_string_insertion(QParse_RULES____STRING *input, const QParse_RULES____STRING::const_iterator &begin, const QParse_RULES____STRING::const_iterator &end)
{
    return push_undo_custom_data<
            std::pair<
                std::pair<QParse_RULES____STRING*, QParse_RULES____STRING::difference_type>,
                QParse_RULES____STRING::difference_type
            >
    >("STRING INSERT",
        {
            {input, begin - input->cbegin()},
            end - input->cbegin()
        },
        [](auto data) {
            QParse_RULES____STRING * a = data.first.first;
            QParse_RULES____STRING::difference_type b = data.first.second;
            QParse_RULES____STRING::difference_type c = data.second;
            QParse_RULES____STRING_REPLACE____STRING_PTR__ITERATOR_BEGIN__ITERATOR_END__STRING(a, a->cbegin() + b, a->cbegin() + c, "");
        }
    );
}

void QParse::Rules::UndoRedo::Command::push_redo_string_insertion(QParse::Rules::UndoRedo::UNDO_KEY_PTR undo_key, QParse_RULES____STRING *input, const QParse_RULES____STRING::const_iterator &end, const QParse_RULES____STRING &replacement)
{
    push_redo_custom_data<
            std::pair<
                std::pair<QParse_RULES____STRING*, QParse_RULES____STRING::difference_type>,
                const QParse_RULES____STRING
            >
    >("STRING INSERT " + replacement,
        undo_key,
        {
            {input, end - input->cbegin()},
            replacement
        },
        [](auto data) {
            QParse_RULES____STRING * a = data.first.first;
            QParse_RULES____STRING::difference_type b = data.first.second;
            const QParse_RULES____STRING c = data.second;
            QParse_RULES____STRING_INSERT____STRING_PTR__INTEGER__STRING(a, b, c);
        }
    );
}


QParse::Rules::UndoRedo::Command::CustomDataStack::~CustomDataStack() {
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

bool QParse::Rules::UndoRedo::Command::CustomDataStack::undo() {
    size_t count = stack.size();
    if (count == 0 || level >= count) return false;

    size_t index = level;
    level++;

    LinkData * data = stack[index];
    data->invoker(data->data);
    return true;
}

bool QParse::Rules::UndoRedo::Command::CustomDataStack::redo() {
    size_t count = stack.size();
    if (count == 0 || level <= 0) return false;

    level--;
    size_t index = level;

    LinkData * data = stack[index]->redo;
    data->invoker(data->data);
    return true;
}
