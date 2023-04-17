#ifndef QParse_RULES_INPUT_H
#define QParse_RULES_INPUT_H

#include "Error.h"
#include "IteratorMatcher.h"

namespace QParse {
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

            QParse_RULES____STRING string();

            static QParse_RULES____STRING string(Iterator & iterator, IteratorMatcher::MatchData &match);

            QParse_RULES____STRING quotedString(QParse_RULES____STRING quote = "'");

            static QParse_RULES____STRING quote(const QParse_RULES____STRING & string, QParse_RULES____STRING quote = "'");

            static QParse_RULES____STRING quote(const char & character, QParse_RULES____STRING quote = "'");

            QParse_RULES____STRING stringRemoveCharactersFromEnd(int tail);

            QParse_RULES____STRING quotedStringRemoveCharactersFromEnd(int tail, QParse_RULES____STRING quote = "'");

            QParse_RULES____STRING stringRemoveCharactersFromStart(int head);

            QParse_RULES____STRING quotedStringRemoveCharactersFromStart(int head, QParse_RULES____STRING quote = "'");

            QParse_RULES____STRING stringRemoveCharactersFromStartAndEnd(int head, int tail);

            QParse_RULES____STRING quotedStringRemoveCharactersFromStartAndEnd(int head, int tail, QParse_RULES____STRING quote = "'");

            void rescan();

            void eraseAndRescan();

            void replace(const char & character);

    #ifndef GTEST_API_
        private:
    #endif
            void replace_(const QParse_RULES____STRING & string);

    #ifndef GTEST_API_
        public:
    #endif
            void replace(const QParse_RULES____STRING & string);

            void replaceAndRescan(const char & character);

            void replaceAndRescan(const QParse_RULES____STRING & string);

            void insert(const char & character);

            void insert(const QParse_RULES____STRING & string);

            void insertAndRescan(const char & character);

            void insertAndRescan(const QParse_RULES____STRING & string);

            Iterator &getIterator() const;
        };
    }
}

#endif // QParse_RULES_INPUT_H
