#include "Iterator.h"

QParse::Iterator::Iterator(QParse_RULES____STRING * allocated_input) {
  
    name = "unknown";
    
    input = allocated_input;
    
    allocated = true;

    info.iteratorCurrent = this->input->cbegin();
    info.current_char = info.iteratorCurrent[0];

    info.line_start = info.iteratorCurrent;
    info.line_end = info.iteratorCurrent;

    compute_line_end();
}

QParse::Iterator::Iterator(QParse_RULES____STRING &input) : input(&input) {
  
    name = "unknown";
    
    info.iteratorCurrent = this->input->cbegin();
    info.current_char = info.iteratorCurrent[0];

    info.line_start = info.iteratorCurrent;
    info.line_end = info.iteratorCurrent;

    compute_line_end();
}

QParse::Iterator::Iterator(const char * input) {
  
    name = "unknown";
    
    this->input = new QParse_RULES____STRING(input);
    allocated = true;
    info.iteratorCurrent = this->input->cbegin();
    info.current_char = info.iteratorCurrent[0];

    info.line_start = info.iteratorCurrent;
    info.line_end = info.iteratorCurrent;

    compute_line_end();
}

QParse::Iterator::~Iterator() {
    if (allocated) delete input;
}

void QParse::Iterator::set_next_prev_callback(std::function<void (const char *)> next_prev_callback) {
    this->next_prev_callback = next_prev_callback;
}

QParse::Iterator QParse::Iterator::copy() const {
    if (allocated) {
        // we need to copy our string since it will be deleted if this iterator dies
        Iterator iterator(QParse_RULES____COPY_STRING(input));
        iterator.name = name;
        auto save1 = save();
        iterator.load(save1);
        iterator.set_next_prev_callback(next_prev_callback);
        // we require a fresh stack
        return iterator;
    } else {
        Iterator iterator(*input);
        iterator.name = name;
        iterator.info = info;
        iterator.set_next_prev_callback(next_prev_callback);
        // we require a fresh stack
        return iterator;
    }
}

uint64_t QParse::Iterator::line() const {
    return info.current_line;
}

uint64_t QParse::Iterator::column() const {
    return info.current_column;
}

bool QParse::Iterator::has_next() const {
    return info.iteratorCurrent < input->cend();
}

QParse_RULES____CHAR QParse::Iterator::next() {
    next_prev_callback("[CALLED] next");

    info.current_char = info.iteratorCurrent[0];

    if (has_next()) {
        info.iteratorCurrent++;
        if (info.current_char == '\n') {
            info.current_column = 1;
            info.current_line++;

            info.line_start = info.iteratorCurrent;

            compute_line_end();
        } else {
            info.current_column++;
        }
    }

    return info.current_char;
}

bool QParse::Iterator::has_previous() const {
    return info.iteratorCurrent > input->cbegin();
}

QParse_RULES____CHAR QParse::Iterator::previous() {
    next_prev_callback("[CALLED] previous");

    if (has_previous()) {
        info.iteratorCurrent--;
    }

    info.current_char = info.iteratorCurrent[0];

    if (info.current_char == '\n') {
        info.current_line--;
        info.current_column = 1;
        compute_line_start();
    } else {
        info.current_column--;
    }

    return info.current_char;
}

QParse_RULES____CHAR QParse::Iterator::peekPrevious() const {
    if (info.iteratorCurrent > input->cbegin()) {
        return input[0][(info.iteratorCurrent - 1) - input->cbegin()];
    }
    return info.iteratorCurrent[0];
}

QParse_RULES____CHAR QParse::Iterator::peekNext() const {
    return info.iteratorCurrent[0];
}

QParse_RULES____STRING::const_iterator QParse::Iterator::cbegin() const {
    return input->cbegin();
}

QParse_RULES____STRING::const_iterator QParse::Iterator::current() const {
    return info.iteratorCurrent;
}

void QParse::Iterator::setCurrent(QParse_RULES____STRING::const_iterator current) {
    if (current < info.iteratorCurrent) {
        while(current < info.iteratorCurrent && has_previous()) {
            previous();
        }
        compute_line_start();
    }
    if (current > info.iteratorCurrent) {
        while(current > info.iteratorCurrent && has_next()) {
            next();
        }
        compute_line_end();
    }
}

QParse_RULES____STRING::difference_type QParse::Iterator::currentPosition() const {
    return info.iteratorCurrent - input->cbegin();
}

QParse_RULES____STRING::difference_type QParse::Iterator::currentPosition(QParse_RULES____STRING::const_iterator iterator) const {
    return iterator - input->cbegin();
}

QParse_RULES____STRING::const_iterator QParse::Iterator::peekPreviousCurrent() const {
    if (info.iteratorCurrent > input->cbegin()) {
        return info.iteratorCurrent - 1;
    }
    return info.iteratorCurrent;
}

QParse_RULES____STRING::const_iterator QParse::Iterator::peekNextCurrent() const {
    if (info.iteratorCurrent < input->cend()) {
        return info.iteratorCurrent + 1;
    }
    return info.iteratorCurrent;
}

QParse_RULES____STRING::const_iterator QParse::Iterator::cend() const {
    return input->cend();
}

void QParse::Iterator::reset() {
    while(!infoStack.empty()) {
        popInfo();
    }
    info.iteratorCurrent = input->cbegin();
    info.current_char = info.iteratorCurrent[0];
    info.current_column = 1;
    info.current_line = 1;
}

QParse_RULES____STRING QParse::Iterator::substr(QParse_RULES____STRING::const_iterator begin, QParse_RULES____STRING::const_iterator end) const {
    return QParse_RULES____STRING_SUBSTR____STRING_PTR__ITERATOR_BEGIN__ITERATOR_END(input, begin, end);
}

QParse_RULES____STRING QParse::Iterator::lineString() {
    return substr(info.line_start, info.line_end);
}

void QParse::Iterator::pushInfo() {
    infoStack.push_back(info);
}

void QParse::Iterator::pushInfoWithIterator(QParse_RULES____STRING::const_iterator iterator) {
    Info copy = info;
    info.iteratorCurrent = iterator;
    infoStack.push_back(copy);
}

bool QParse::Iterator::popInfo() {
    if (!infoStack.empty()) {
        info = infoStack.back();
        infoStack.pop_back();
        return true;
    }
    return false;
}

void QParse::Iterator::popInfo(size_t n) {
    for (size_t i = 0; i < n; ++i) {
        if (!popInfo()) break;
    }
}

void QParse::Iterator::compute_line_end() {
    auto s = info.iteratorCurrent;
    while(true) {
        if (info.iteratorCurrent[0] == '\n') {
            break;
        }
        if (!has_next()) break;
        info.iteratorCurrent++;
    }
    info.line_end = info.iteratorCurrent;
    info.iteratorCurrent = s;
}

void QParse::Iterator::compute_line_start() {
    auto s = info.iteratorCurrent;
    while(true) {
        if (info.iteratorCurrent[0] == '\n') {
            info.iteratorCurrent++;
            break;
        }
        if (!has_previous()) break;
        info.iteratorCurrent--;
    }
    info.line_start = info.iteratorCurrent;
    info.iteratorCurrent = s;
}


void QParse::Iterator::advance() {
    setCurrent(info.iteratorCurrent+1);
}

void QParse::Iterator::advance(size_t n) {
    setCurrent(info.iteratorCurrent+n);
}

QParse::Iterator::SaveState QParse::Iterator::save() const {
    SaveState saveState;
    saveState.info.iteratorCurrent = info.iteratorCurrent - input->cbegin();
    saveState.info.line_start = info.line_start - input->cbegin();
    saveState.info.line_end = info.line_end - input->cbegin();
    saveState.info.current_char = info.current_char;
    saveState.info.current_column = info.current_column;
    saveState.info.current_line = info.current_line;
    for (auto &&item : infoStack) {
        SaveInfo saveInfo;
        saveInfo.iteratorCurrent = item.iteratorCurrent - input->cbegin();
        saveInfo.line_start = item.line_start - input->cbegin();
        saveInfo.line_end = item.line_end - input->cbegin();
        saveInfo.current_char = item.current_char;
        saveInfo.current_column = item.current_column;
        saveInfo.current_line = item.current_line;
        saveState.infoStack.push_back(saveInfo);
    }
    return saveState;
}

QParse::Iterator::SaveState QParse::Iterator::save(QParse_RULES____STRING::const_iterator iterator) {
    SaveState saveState;
    saveState.info.iteratorCurrent = iterator - input->cbegin();
    return saveState;
}

void QParse::Iterator::load(SaveState &saveState) {
    info.iteratorCurrent = input->cbegin() + saveState.info.iteratorCurrent;
    info.line_start = input->cbegin() + saveState.info.line_start;
    info.line_end = input->cbegin() + saveState.info.line_end;
    info.current_char = saveState.info.current_char;
    info.current_column = saveState.info.current_column;
    info.current_line = saveState.info.current_line;
    infoStack.clear();
    for (auto &item : saveState.infoStack) {
        Info i;
        i.iteratorCurrent = input->cbegin() + item.iteratorCurrent;
        i.line_start = input->cbegin() + item.line_start;
        i.line_end = input->cbegin() + item.line_end;
        i.current_char = item.current_char;
        i.current_column = item.current_column;
        i.current_line = item.current_line;
        infoStack.push_back(i);
    }
}

void QParse::Iterator::load(SaveState &saveState, QParse_RULES____STRING::const_iterator &iterator) {
    iterator = input->cbegin() + saveState.info.iteratorCurrent;
}

QParse_RULES____STRING QParse::Iterator::currentString() const {
    return QParse_RULES____STRING_SUBSTR____STRING_PTR__POS(input, currentPosition());
}

QParse_RULES____STRING QParse::Iterator::currentString(QParse_RULES____STRING::const_iterator iterator) const {
    return QParse_RULES____STRING_SUBSTR____STRING_PTR__POS(input, currentPosition(iterator));
}






























#if (false)

bool QParse::Iterator2::current_has_next() const {
    return iteratorCurrent < input->cend();
}

bool QParse::Iterator2::parent_has_next() const {
    return parent == nullptr ? false : parent->has_next();
}

bool QParse::Iterator2::current_has_previous() const {
    return iteratorCurrent > input->cbegin();
}

bool QParse::Iterator2::parent_has_previous() const {
    return parent == nullptr ? false : parent->has_previous();
}

int QParse::Iterator2::parentCount(int count) const {
    if (parent == nullptr) {
        return count;
    }
    return parent->parentCount(count+1);
}

QParse::Iterator2::Iterator2(std::string &input) : iteratorCurrent(input.cbegin()), parent(nullptr), input(&input) {}

QParse::Iterator2::Iterator2(Iterator2 &parent, std::string &input) : iteratorCurrent(input.cbegin()), parent(&parent), input(&input) {}

int QParse::Iterator2::parentCount() const {
    if (parent == nullptr) {
        return 0;
    } else {
        return parent->parentCount(1);
    }
}

bool QParse::Iterator2::has_previous() const {
    bool c = current_has_previous();
    return c ? c : parent_has_previous();
}

std::optional<char> QParse::Iterator2::previous() {
    if (current_has_previous()) {
        iteratorCurrent--;
        return iteratorCurrent[0];
    } else if (parent_has_previous()) {
        return parent->previous();
    } else return std::nullopt;
}

std::optional<char> QParse::Iterator2::peekPrevious() const {
    if (current_has_previous()) {
        return input[0][(iteratorCurrent - 1) - input->cbegin()];
    } else if (parent_has_previous()) {
        return parent->peekPrevious();
    } else return std::nullopt;
}

bool QParse::Iterator2::has_next() const {
    bool c = current_has_next();
    return c ? c : parent_has_next();
}

std::optional<char> QParse::Iterator2::next() {
    if (current_has_next()) {
        const char c = iteratorCurrent[0];
        iteratorCurrent++;
        return c;
    } else if (parent_has_next()) {
        return parent->next();
    } else return std::nullopt;
}

std::optional<char> QParse::Iterator2::peekNext() const {
    if (current_has_next()) {
        return iteratorCurrent[0];
    } else if (parent_has_next()) {
        return parent->peekNext();
    } else return std::nullopt;
}

std::string::const_iterator QParse::Iterator2::cbegin() const {
    return input->cbegin();
}

std::string::const_iterator QParse::Iterator2::current() const {
    return iteratorCurrent;
}

void QParse::Iterator2::setCurrent(std::string::const_iterator current) {
    iteratorCurrent = current;
}

std::string::difference_type QParse::Iterator2::currentPosition() const {
    return iteratorCurrent - input->cbegin();
}

std::string::difference_type QParse::Iterator2::currentPosition(std::string::const_iterator iterator) const {
    return iterator - input->cbegin();
}

std::optional<std::string::const_iterator> QParse::Iterator2::peekPreviousCurrent() const {
    if (current_has_previous()) {
        return iteratorCurrent - 1;
    } else if (parent_has_previous()) {
        return parent->peekPreviousCurrent();
    } else return std::nullopt;
}

std::optional<std::string::const_iterator> QParse::Iterator2::peekNextCurrent() const {
    if (current_has_next()) {
        return iteratorCurrent + 1;
    } else if (parent_has_next()) {
        return parent->peekNextCurrent();
    } else return std::nullopt;
}

std::string::const_iterator QParse::Iterator2::cend() const {
    return input->cend();
}

void QParse::Iterator2::reset() {
    iteratorCurrent = input->cbegin();
    iteratorStack.clear();
}

std::string QParse::Iterator2::substr(std::string::const_iterator begin, std::string::const_iterator end) const {
    return input->substr(begin - input->cbegin(), end - begin);
}

void QParse::Iterator2::pushIterator() {
    iteratorStack.push_back(iteratorCurrent);
}

void QParse::Iterator2::pushIterator(std::string::const_iterator iterator) {
    iteratorStack.push_back(iterator);
}

void QParse::Iterator2::popIterator() {
    if (!iteratorStack.empty()) {
        iteratorCurrent = iteratorStack.back();
        iteratorStack.pop_back();
    }
}

void QParse::Iterator2::popIterator(size_t n) {
    for (size_t i = 0; i < n; ++i) {
        if (iteratorStack.empty()) break;
        iteratorCurrent = iteratorStack.back();
        iteratorStack.pop_back();
    }
}

void QParse::Iterator2::setCurrentToIteratorPops(size_t n) {
    if (iteratorStack.size() > n) {
        iteratorCurrent = *(iteratorStack.cend() - n);
    } else {
        iteratorCurrent = cbegin();
    }
}

void QParse::Iterator2::advance() {
    iteratorCurrent++;
}

void QParse::Iterator2::advance(size_t n) {
    iteratorCurrent += n;
}

QParse::Iterator2::SaveState QParse::Iterator2::save() {
    SaveState saveState;
    saveState.iteratorCurrent = iteratorCurrent - input->cbegin();
    for (auto &&item : iteratorStack) {
        saveState.iteratorStack.push_back(item - input->cbegin());
    }
    return saveState;
}

QParse::Iterator2::SaveState QParse::Iterator2::save(std::string::const_iterator iterator) {
    SaveState saveState;
    saveState.iteratorCurrent = iterator - input->cbegin();
    return saveState;
}

void QParse::Iterator2::load(SaveState &saveState) {
    iteratorCurrent = input->cbegin() + saveState.iteratorCurrent;
    iteratorStack.clear();
    for (auto &&item : saveState.iteratorStack) {
        iteratorStack.push_back(input->cbegin() + item);
    }
}

void QParse::Iterator2::load(SaveState &saveState, std::string::const_iterator &iterator) {
    iterator = input->cbegin() + saveState.iteratorCurrent;
}

std::string QParse::Iterator2::currentString() const {
    return input->substr(currentPosition());
}

std::string QParse::Iterator2::currentString(std::string::const_iterator &iterator) const {
    return input->substr(currentPosition(iterator));
}

#endif
