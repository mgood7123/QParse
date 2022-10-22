#include "Iterator.h"

CPP::Iterator::Iterator(CPP_RULES____STRING * allocated_input) {
    this->input = input;
    allocated = true;

    info.iteratorCurrent = this->input->cbegin();
    info.current_char = info.iteratorCurrent[0];

    info.line_start = info.iteratorCurrent;

    compute_line_end();
}

CPP::Iterator::Iterator(CPP_RULES____STRING &input) : input(&input) {
    info.iteratorCurrent = this->input->cbegin();
    info.current_char = info.iteratorCurrent[0];

    info.line_start = info.iteratorCurrent;

    compute_line_end();
}

CPP::Iterator::Iterator(const char * input) {
    this->input = new CPP_RULES____STRING(input);
    allocated = true;
    info.iteratorCurrent = this->input->cbegin();
    info.current_char = info.iteratorCurrent[0];

    info.line_start = info.iteratorCurrent;

    compute_line_end();
}

CPP::Iterator::~Iterator() {
    if (allocated) delete input;
}

void CPP::Iterator::set_next_prev_callback(std::function<void (const char *)> next_prev_callback) {
    this->next_prev_callback = next_prev_callback;
}

CPP::Iterator CPP::Iterator::copy() {
    if (allocated) {
        // we need to copy our string since it will be deleted if this iterator dies
        Iterator iterator(CPP_RULES____COPY_STRING(input));
        auto save1 = save();
        iterator.load(save1);
        iterator.set_next_prev_callback(next_prev_callback);
        // we require a fresh stack
        return iterator;
    } else {
        Iterator iterator(*input);
        iterator.info = info;
        iterator.set_next_prev_callback(next_prev_callback);
        // we require a fresh stack
        return iterator;
    }
}

uint64_t CPP::Iterator::line() const {
    return info.current_line;
}

uint64_t CPP::Iterator::column() const {
    return info.current_column;
}

bool CPP::Iterator::has_next() const {
    return info.iteratorCurrent < input->cend();
}

CPP_RULES____CHAR CPP::Iterator::next() {
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

bool CPP::Iterator::has_previous() const {
    return info.iteratorCurrent > input->cbegin();
}

CPP_RULES____CHAR CPP::Iterator::previous() {
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

CPP_RULES____CHAR CPP::Iterator::peekPrevious() const {
    if (info.iteratorCurrent > input->cbegin()) {
        return input[0][(info.iteratorCurrent - 1) - input->cbegin()];
    }
    return info.iteratorCurrent[0];
}

CPP_RULES____CHAR CPP::Iterator::peekNext() const {
    return info.iteratorCurrent[0];
}

CPP_RULES____STRING::const_iterator CPP::Iterator::cbegin() const {
    return input->cbegin();
}

CPP_RULES____STRING::const_iterator CPP::Iterator::current() const {
    return info.iteratorCurrent;
}

void CPP::Iterator::setCurrent(CPP_RULES____STRING::const_iterator current) {
    if (current < info.iteratorCurrent) {
        while(current < info.iteratorCurrent) {
            previous();
        }
        compute_line_start();
    }
    if (current > info.iteratorCurrent) {
        while(current > info.iteratorCurrent) {
            next();
        }
        compute_line_end();
    }
}

CPP_RULES____STRING::difference_type CPP::Iterator::currentPosition() const {
    return info.iteratorCurrent - input->cbegin();
}

CPP_RULES____STRING::difference_type CPP::Iterator::currentPosition(CPP_RULES____STRING::const_iterator iterator) const {
    return iterator - input->cbegin();
}

CPP_RULES____STRING::const_iterator CPP::Iterator::peekPreviousCurrent() const {
    if (info.iteratorCurrent > input->cbegin()) {
        return info.iteratorCurrent - 1;
    }
    return info.iteratorCurrent;
}

CPP_RULES____STRING::const_iterator CPP::Iterator::peekNextCurrent() const {
    if (info.iteratorCurrent < input->cend()) {
        return info.iteratorCurrent + 1;
    }
    return info.iteratorCurrent;
}

CPP_RULES____STRING::const_iterator CPP::Iterator::cend() const {
    return input->cend();
}

void CPP::Iterator::reset() {
    while(!infoStack.empty()) {
        popInfo();
    }
    info.iteratorCurrent = input->cbegin();
    info.current_char = info.iteratorCurrent[0];
    info.current_column = 1;
    info.current_line = 1;
}

CPP_RULES____STRING CPP::Iterator::substr(CPP_RULES____STRING::const_iterator begin, CPP_RULES____STRING::const_iterator end) const {
    return CPP_RULES____STRING_SUBSTR____STRING_PTR__ITERATOR_BEGIN__ITERATOR_END(input, begin, end);
}

CPP_RULES____STRING CPP::Iterator::lineString() {
    return substr(info.line_start, info.line_end);
}

void CPP::Iterator::pushInfo() {
    infoStack.push_back(info);
}

void CPP::Iterator::pushInfoWithIterator(CPP_RULES____STRING::const_iterator iterator) {
    Info copy = info;
    info.iteratorCurrent = iterator;
    infoStack.push_back(copy);
}

bool CPP::Iterator::popInfo() {
    if (!infoStack.empty()) {
        info = infoStack.back();
        compute_line_start();
        compute_line_end();
        infoStack.pop_back();
        return true;
    }
    return false;
}

void CPP::Iterator::popInfo(size_t n) {
    for (size_t i = 0; i < n; ++i) {
        if (!popInfo()) break;
    }
}

void CPP::Iterator::compute_line_end() {
    auto s = info.iteratorCurrent;
    if (!has_next()) {
        info.line_end = info.iteratorCurrent;
        return;
    }
    while(has_next()) {
        info.iteratorCurrent++;
        auto h = has_next();
        if (info.iteratorCurrent[0] == '\n' || !h) {
            if (h) info.iteratorCurrent--;
            info.line_end = info.iteratorCurrent;
            break;
        }
    }
    info.iteratorCurrent = s;
}

void CPP::Iterator::compute_line_start() {
    auto s = info.iteratorCurrent;
    if (!has_previous()) {
        info.line_start = info.iteratorCurrent;
        return;
    }
    while(has_previous()) {
        info.iteratorCurrent--;
        bool h = has_previous();
        if (info.iteratorCurrent[0] == '\n' || !h) {
            if (h) info.iteratorCurrent++;
            info.line_start = info.iteratorCurrent;
            break;
        }
    }
    info.iteratorCurrent = s;
}


void CPP::Iterator::advance() {
    setCurrent(info.iteratorCurrent+1);
}

void CPP::Iterator::advance(size_t n) {
    setCurrent(info.iteratorCurrent+n);
}

CPP::Iterator::SaveState CPP::Iterator::save() {
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

CPP::Iterator::SaveState CPP::Iterator::save(CPP_RULES____STRING::const_iterator iterator) {
    SaveState saveState;
    saveState.info.iteratorCurrent = iterator - input->cbegin();
    return saveState;
}

void CPP::Iterator::load(SaveState &saveState) {
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

void CPP::Iterator::load(SaveState &saveState, CPP_RULES____STRING::const_iterator &iterator) {
    iterator = input->cbegin() + saveState.info.iteratorCurrent;
}

CPP_RULES____STRING CPP::Iterator::currentString() const {
    return CPP_RULES____STRING_SUBSTR____STRING_PTR__POS(input, currentPosition());
}

CPP_RULES____STRING CPP::Iterator::currentString(CPP_RULES____STRING::const_iterator iterator) const {
    return CPP_RULES____STRING_SUBSTR____STRING_PTR__POS(input, currentPosition(iterator));
}






























#if (false)

bool CPP::Iterator2::current_has_next() const {
    return iteratorCurrent < input->cend();
}

bool CPP::Iterator2::parent_has_next() const {
    return parent == nullptr ? false : parent->has_next();
}

bool CPP::Iterator2::current_has_previous() const {
    return iteratorCurrent > input->cbegin();
}

bool CPP::Iterator2::parent_has_previous() const {
    return parent == nullptr ? false : parent->has_previous();
}

int CPP::Iterator2::parentCount(int count) const {
    if (parent == nullptr) {
        return count;
    }
    return parent->parentCount(count+1);
}

CPP::Iterator2::Iterator2(std::string &input) : iteratorCurrent(input.cbegin()), parent(nullptr), input(&input) {}

CPP::Iterator2::Iterator2(Iterator2 &parent, std::string &input) : iteratorCurrent(input.cbegin()), parent(&parent), input(&input) {}

int CPP::Iterator2::parentCount() const {
    if (parent == nullptr) {
        return 0;
    } else {
        return parent->parentCount(1);
    }
}

bool CPP::Iterator2::has_previous() const {
    bool c = current_has_previous();
    return c ? c : parent_has_previous();
}

std::optional<char> CPP::Iterator2::previous() {
    if (current_has_previous()) {
        iteratorCurrent--;
        return iteratorCurrent[0];
    } else if (parent_has_previous()) {
        return parent->previous();
    } else return std::nullopt;
}

std::optional<char> CPP::Iterator2::peekPrevious() const {
    if (current_has_previous()) {
        return input[0][(iteratorCurrent - 1) - input->cbegin()];
    } else if (parent_has_previous()) {
        return parent->peekPrevious();
    } else return std::nullopt;
}

bool CPP::Iterator2::has_next() const {
    bool c = current_has_next();
    return c ? c : parent_has_next();
}

std::optional<char> CPP::Iterator2::next() {
    if (current_has_next()) {
        const char c = iteratorCurrent[0];
        iteratorCurrent++;
        return c;
    } else if (parent_has_next()) {
        return parent->next();
    } else return std::nullopt;
}

std::optional<char> CPP::Iterator2::peekNext() const {
    if (current_has_next()) {
        return iteratorCurrent[0];
    } else if (parent_has_next()) {
        return parent->peekNext();
    } else return std::nullopt;
}

std::string::const_iterator CPP::Iterator2::cbegin() const {
    return input->cbegin();
}

std::string::const_iterator CPP::Iterator2::current() const {
    return iteratorCurrent;
}

void CPP::Iterator2::setCurrent(std::string::const_iterator current) {
    iteratorCurrent = current;
}

std::string::difference_type CPP::Iterator2::currentPosition() const {
    return iteratorCurrent - input->cbegin();
}

std::string::difference_type CPP::Iterator2::currentPosition(std::string::const_iterator iterator) const {
    return iterator - input->cbegin();
}

std::optional<std::string::const_iterator> CPP::Iterator2::peekPreviousCurrent() const {
    if (current_has_previous()) {
        return iteratorCurrent - 1;
    } else if (parent_has_previous()) {
        return parent->peekPreviousCurrent();
    } else return std::nullopt;
}

std::optional<std::string::const_iterator> CPP::Iterator2::peekNextCurrent() const {
    if (current_has_next()) {
        return iteratorCurrent + 1;
    } else if (parent_has_next()) {
        return parent->peekNextCurrent();
    } else return std::nullopt;
}

std::string::const_iterator CPP::Iterator2::cend() const {
    return input->cend();
}

void CPP::Iterator2::reset() {
    iteratorCurrent = input->cbegin();
    iteratorStack.clear();
}

std::string CPP::Iterator2::substr(std::string::const_iterator begin, std::string::const_iterator end) const {
    return input->substr(begin - input->cbegin(), end - begin);
}

void CPP::Iterator2::pushIterator() {
    iteratorStack.push_back(iteratorCurrent);
}

void CPP::Iterator2::pushIterator(std::string::const_iterator iterator) {
    iteratorStack.push_back(iterator);
}

void CPP::Iterator2::popIterator() {
    if (!iteratorStack.empty()) {
        iteratorCurrent = iteratorStack.back();
        iteratorStack.pop_back();
    }
}

void CPP::Iterator2::popIterator(size_t n) {
    for (size_t i = 0; i < n; ++i) {
        if (iteratorStack.empty()) break;
        iteratorCurrent = iteratorStack.back();
        iteratorStack.pop_back();
    }
}

void CPP::Iterator2::setCurrentToIteratorPops(size_t n) {
    if (iteratorStack.size() > n) {
        iteratorCurrent = *(iteratorStack.cend() - n);
    } else {
        iteratorCurrent = cbegin();
    }
}

void CPP::Iterator2::advance() {
    iteratorCurrent++;
}

void CPP::Iterator2::advance(size_t n) {
    iteratorCurrent += n;
}

CPP::Iterator2::SaveState CPP::Iterator2::save() {
    SaveState saveState;
    saveState.iteratorCurrent = iteratorCurrent - input->cbegin();
    for (auto &&item : iteratorStack) {
        saveState.iteratorStack.push_back(item - input->cbegin());
    }
    return saveState;
}

CPP::Iterator2::SaveState CPP::Iterator2::save(std::string::const_iterator iterator) {
    SaveState saveState;
    saveState.iteratorCurrent = iterator - input->cbegin();
    return saveState;
}

void CPP::Iterator2::load(SaveState &saveState) {
    iteratorCurrent = input->cbegin() + saveState.iteratorCurrent;
    iteratorStack.clear();
    for (auto &&item : saveState.iteratorStack) {
        iteratorStack.push_back(input->cbegin() + item);
    }
}

void CPP::Iterator2::load(SaveState &saveState, std::string::const_iterator &iterator) {
    iterator = input->cbegin() + saveState.iteratorCurrent;
}

std::string CPP::Iterator2::currentString() const {
    return input->substr(currentPosition());
}

std::string CPP::Iterator2::currentString(std::string::const_iterator &iterator) const {
    return input->substr(currentPosition(iterator));
}

#endif
