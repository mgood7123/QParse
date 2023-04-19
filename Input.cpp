#include "Rules.h"

QParse::Rules::Input::Input(Iterator &iterator, IteratorMatcher::MatchData &match, UndoRedo *undo, int pops) : iterator(iterator), match(match), undo(undo), pops(pops) {}

QParse::Rules::Input QParse::Rules::Input::copy(Iterator &copy) {
    return Input(copy, match, undo, 0);
}

QParse_RULES____STRING QParse::Rules::Input::string(Iterator & iterator, IteratorMatcher::MatchData &match) {
    return iterator.substr(match.begin, match.end);
}

QParse_RULES____STRING QParse::Rules::Input::string() {
    return iterator.substr(match.begin, match.end);
}

QParse_RULES____STRING QParse::Rules::Input::quotedString(QParse_RULES____STRING quote) {
    QParse_RULES____STRING out = quote;
    out += string();
    out += quote;
    return out;
}

QParse_RULES____STRING QParse::Rules::Input::quote(const QParse_RULES____STRING &string, QParse_RULES____STRING quote) {
    QParse_RULES____STRING out = quote;
    out += string;
    out += quote;
    return out;
}

QParse_RULES____STRING QParse::Rules::Input::quote(const char &character, QParse_RULES____STRING quote) {
    QParse_RULES____STRING out = quote;
    out += character;
    out += quote;
    return out;
}

QParse_RULES____STRING QParse::Rules::Input::stringRemoveCharactersFromEnd(int tail) {
    return iterator.substr(match.begin, match.end - tail);
}

QParse_RULES____STRING QParse::Rules::Input::quotedStringRemoveCharactersFromEnd(int tail, QParse_RULES____STRING quote) {
    QParse_RULES____STRING out = quote;
    out += stringRemoveCharactersFromEnd(tail);
    out += quote;
    return out;
}

QParse_RULES____STRING QParse::Rules::Input::stringRemoveCharactersFromStart(int head) {
    return iterator.substr(match.begin + head, match.end);
}

QParse_RULES____STRING QParse::Rules::Input::quotedStringRemoveCharactersFromStart(int head, QParse_RULES____STRING quote) {
    QParse_RULES____STRING out = quote;
    out += stringRemoveCharactersFromEnd(head);
    out += quote;
    return out;
}

QParse_RULES____STRING QParse::Rules::Input::stringRemoveCharactersFromStartAndEnd(int head, int tail) {
    return iterator.substr(match.begin + head, match.end - tail);
}

QParse_RULES____STRING QParse::Rules::Input::quotedStringRemoveCharactersFromStartAndEnd(int head, int tail, QParse_RULES____STRING quote) {
    QParse_RULES____STRING out = quote;
    out += stringRemoveCharactersFromStartAndEnd(head, tail);
    out += quote;
    return out;
}

void QParse::Rules::Input::rescan() {
    QParse::Rules::UndoRedo::UNDO_KEY_PTR undo_iterator;

    if (undo != nullptr) {
        undo->push_command("After rescan");
        undo->disable_push_command();
        undo_iterator = undo->command->push_undo_iterator(iterator);
    }
    
    auto st = iterator.save(match.end);
    auto sti = iterator.save(iterator.info.line_end);
    iterator.popInfo(pops);
    match.end = match.begin;
    iterator.load(sti, iterator.info.line_end);
    
    if (undo != nullptr) {
        undo->command->push_redo_iterator(undo_iterator, iterator);
        undo->enable_push_command();
    }
}

void QParse::Rules::Input::eraseAndRescan() {
    if (undo != nullptr) {
        undo->push_command(QParse_RULES____STRING("After erasing ") + Input::quote(this->string()) + " and rescan");
        undo->disable_push_command();
    }
    replace("");
    rescan();

    if (undo != nullptr) {
        undo->enable_push_command();
    }
}

void QParse::Rules::Input::replace(const char &character) {
    if (undo != nullptr) {
        undo->push_command(QParse_RULES____STRING("After replacing ") + Input::quote(this->string()) + " with " + Input::quote(character));
        undo->disable_push_command();
    }

    char string[2] = {character, '\0'};
    replace_(string);

    if (undo != nullptr) {
        undo->enable_push_command();
    }
}

void QParse::Rules::Input::replace_(const QParse_RULES____STRING &string) {

    // modifying a string invalidates the iterators

    QParse_RULES____STRING old_string = Input::string(iterator, match);

    QParse::Rules::UndoRedo::UNDO_KEY_PTR undo_iterator;

    if (undo != nullptr) {
        undo->push_command(QParse_RULES____STRING("After replacing ") + Input::quote(this->string()) + " with " + Input::quote(string));
        undo->disable_push_command();
        undo_iterator = undo->command->push_undo_iterator(iterator);
    }

    auto savePoint1 = iterator.save();
    auto savePoint2 = iterator.save(match.begin);

    auto old_end_savePoint = iterator.save(match.end);

    QParse_RULES____STRING_REPLACE____STRING_PTR__ITERATOR_BEGIN__ITERATOR_END__STRING(iterator.input, match.begin, match.end, string);
    iterator.load(savePoint1);
    iterator.load(savePoint2, match.begin);

    QParse_RULES____STRING::const_iterator old_end;

    iterator.load(old_end_savePoint, old_end);
    
    match.end = match.begin + string.size();


    if (undo != nullptr) {
        auto k = undo->command->push_undo_string_replacement(iterator.input, match.begin, match.end, old_string);
        undo->command->push_redo_string_replacement(k, iterator.input, match.begin, old_end, string);
        undo->command->push_redo_iterator(undo_iterator, iterator);
        undo->enable_push_command();
    }
}

void QParse::Rules::Input::replace(const QParse_RULES____STRING &string) {
    QParse::Rules::UndoRedo::UNDO_KEY_PTR undo_iterator;
    if (undo != nullptr) {
        undo->push_command(QParse_RULES____STRING("After replacing ") + Input::quote(this->string()) + " with " + Input::quote(string));
        undo->disable_push_command();
        undo_iterator = undo->command->push_undo_iterator(iterator);
    }
    replace_(string);
    iterator.setCurrent(match.end);
    if (undo != nullptr) {
        undo->command->push_redo_iterator(undo_iterator, iterator);
        undo->enable_push_command();
    }
}

void QParse::Rules::Input::replaceAndRescan(const char &character) {
    if (undo != nullptr) {
        undo->push_command(QParse_RULES____STRING("After replacing ") + Input::quote(this->string()) + " with " + Input::quote(character) + " and rescan");
        undo->disable_push_command();
    }
    replace(character);
    rescan();
    if (undo != nullptr) {
        undo->enable_push_command();
    }
}

void QParse::Rules::Input::replaceAndRescan(const QParse_RULES____STRING &string) {
    if (undo != nullptr) {
        undo->push_command(QParse_RULES____STRING("After replacing ") + Input::quote(this->string()) + " with " + Input::quote(string) + " and rescan");
        undo->disable_push_command();
    }
    replace_(string);
    rescan();
    if (undo != nullptr) {
        undo->enable_push_command();
    }
}

void QParse::Rules::Input::insert(const char &character) {
    if (undo != nullptr) {
        undo->push_command(QParse_RULES____STRING("After inserting ") + Input::quote(character));
        undo->disable_push_command();
    }
    char string[2] = {character, '\0'};
    insert(string);
    if (undo != nullptr) {
        undo->enable_push_command();
    }
}

void QParse::Rules::Input::insert(const QParse_RULES____STRING &string) {

    QParse::Rules::UndoRedo::UNDO_KEY_PTR undo_iterator;

    if (undo != nullptr) {
        undo->push_command(QParse_RULES____STRING("After inserting ") + Input::quote(string));
        undo->disable_push_command();
        undo_iterator = undo->command->push_undo_iterator(iterator);
    }

    // modifying a string invalidates the iterators

    auto savePoint1 = iterator.save();
    auto savePoint2 = iterator.save(match.begin);
    auto savePoint3 = iterator.save(match.end);
    QParse_RULES____STRING_INSERT____STRING_PTR__INTEGER__STRING(iterator.input, iterator.currentPosition(match.end), string);
    iterator.load(savePoint1);
    iterator.load(savePoint2, match.begin);
    iterator.load(savePoint3, match.end);

    QParse_RULES____STRING::const_iterator old_end = match.end;

    match.end += string.size();

    iterator.compute_line_end();

    if (undo != nullptr) {
        auto k = undo->command->push_undo_string_insertion(iterator.input, old_end, iterator.input->cbegin() + string.size());
        undo->command->push_redo_string_insertion(k, iterator.input, old_end, string);
        undo->command->push_redo_iterator(undo_iterator, iterator);
        undo->enable_push_command();
    }

}

void QParse::Rules::Input::insertAndRescan(const char &character) {
    if (undo != nullptr) {
        undo->push_command(QParse_RULES____STRING("After inserting ") + Input::quote(character) + " and rescan");
        undo->disable_push_command();
    }
    insert(character);
    rescan();
    if (undo != nullptr) {
        undo->enable_push_command();
    }
}

void QParse::Rules::Input::insertAndRescan(const QParse_RULES____STRING &string) {
    if (undo != nullptr) {
        undo->push_command(QParse_RULES____STRING("After inserting ") + Input::quote(string) + " and rescan");
        undo->disable_push_command();
    }
    insert(string);
    rescan();
    if (undo != nullptr) {
        undo->enable_push_command();
    }
}

QParse::Iterator &QParse::Rules::Input::getIterator() const {
    return iterator;
}
