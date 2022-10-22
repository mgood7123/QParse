#ifndef CPP_RULES_INPUT_H
#define CPP_RULES_INPUT_H

#include "Error.h"
#include "IteratorMatcher.h"

namespace CPP {
    namespace Rules {
        class Input {
#ifdef GTEST_API_
        public:
#endif
            Iterator &iterator;
            IteratorMatcher::MatchData & match;
            UndoRedo *undo = nullptr;
            int pops;
            bool executed = false;

        public:
            Input(Iterator &iterator, IteratorMatcher::MatchData &match, UndoRedo *undo, int pops);

            Input copy(Iterator & copy);

            CPP_RULES____STRING string();

            static CPP_RULES____STRING string(Iterator & iterator, IteratorMatcher::MatchData &match);

            CPP_RULES____STRING quotedString(CPP_RULES____STRING quote = "'");

            static CPP_RULES____STRING quote(const CPP_RULES____STRING & string, CPP_RULES____STRING quote = "'");

            static CPP_RULES____STRING quote(const char & character, CPP_RULES____STRING quote = "'");

            CPP_RULES____STRING stringRemoveCharactersFromEnd(int tail);

            CPP_RULES____STRING quotedStringRemoveCharactersFromEnd(int tail, CPP_RULES____STRING quote = "'");

            CPP_RULES____STRING stringRemoveCharactersFromStart(int head);

            CPP_RULES____STRING quotedStringRemoveCharactersFromStart(int head, CPP_RULES____STRING quote = "'");

            CPP_RULES____STRING stringRemoveCharactersFromStartAndEnd(int head, int tail);

            CPP_RULES____STRING quotedStringRemoveCharactersFromStartAndEnd(int head, int tail, CPP_RULES____STRING quote = "'");

            void rescan();

            void eraseAndRescan();

            void replace(const char & character);

    #ifndef GTEST_API_
        private:
    #endif
            void replace_(const CPP_RULES____STRING & string);

        public:
            void replace(const CPP_RULES____STRING & string);

            void replaceAndRescan(const char & character);

            void replaceAndRescan(const CPP_RULES____STRING & string);

            void insert(const char & character);

            void insert(const CPP_RULES____STRING & string);

            void insertAndRescan(const char & character);

            void insertAndRescan(const CPP_RULES____STRING & string);

            Iterator &getIterator() const;
        };
    }
}

#endif // CPP_RULES_INPUT_H
