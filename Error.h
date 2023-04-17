#include "Iterator.h"

#ifndef QParse_RULES_ERROR2
#ifndef QParse_RULES_ERROR1_H
#define QParse_RULES_ERROR1_H
namespace QParse {
    namespace Rules {

        // this is used to implement the error reporting system
        // it's purpose is to "unwind" any modifications made to the input stream when an error occurs
        // providing a backtrace of changes
        // and provides the utilities to map the modified input stream to the original input stream

        class UndoRedo {
        private:
            enum COMMAND {
                COMMAND__BUFFER,

                SUB_COMMAND__UNDO_CUSTOM_DATA,
                SUB_COMMAND__REDO_CUSTOM_DATA
            };
        public:

            typedef void * UNDO_KEY_PTR;

            class Command {
                friend UndoRedo;
            private:

                struct CustomDataStack {

                    size_t level = 0;

                    struct LinkData {
                        bool is_undo = false;
                        QParse_RULES____STRING TAG;
                        std::function<void(void*)> invoker;
                        void * data = nullptr;
                        std::function<void(void*)> deleter;
                        LinkData * redo = nullptr;
                    };

                    QParse_RULES____VECTOR <LinkData*> stack;

                    ~CustomDataStack();

                    bool undo();
                    bool redo();
                };

                UndoRedo * parent;
                QParse_RULES____VECTOR <COMMAND> commandStack;
                CustomDataStack customStack;

            public:
                QParse_RULES____STRING description;
                Command(UndoRedo * parent, const QParse_RULES____STRING & description);

                UNDO_KEY_PTR push_undo_iterator(Iterator & iter);
                UNDO_KEY_PTR push_undo_string_replacement(QParse_RULES____STRING * input, const QParse_RULES____STRING::const_iterator & begin, const QParse_RULES____STRING::const_iterator & end, const QParse_RULES____STRING & replacement);
                UNDO_KEY_PTR push_undo_string_insertion(QParse_RULES____STRING * input, const QParse_RULES____STRING::const_iterator & begin, const QParse_RULES____STRING::const_iterator & end);

                void push_redo_iterator(UNDO_KEY_PTR undo_key, Iterator &iter);
                void push_redo_string_replacement(UNDO_KEY_PTR undo_key, QParse_RULES____STRING * input, const QParse_RULES____STRING::const_iterator & begin, const QParse_RULES____STRING::const_iterator & end, const QParse_RULES____STRING & replacement);
                void push_redo_string_insertion(UNDO_KEY_PTR undo_key, QParse_RULES____STRING * input, const QParse_RULES____STRING::const_iterator & end, const QParse_RULES____STRING & insertion);


                template <typename T>
                UNDO_KEY_PTR push_undo_custom_data(QParse_RULES____STRING tag, T args, std::function<void(T)> func) {
                    commandStack.push_back(COMMAND::SUB_COMMAND__UNDO_CUSTOM_DATA);

                    CustomDataStack::LinkData * undo = new CustomDataStack::LinkData();
                    undo->is_undo = true;
                    undo->TAG = QParse_RULES____STRING("UNDO ") + tag;
                    undo->invoker = [](void * data) {
                        typedef std::pair<T, std::function<void(T)>> FUNC;
                        FUNC * func = static_cast<FUNC*>(data);
                        func->second(func->first);
                    };
                    undo->data = new std::pair<T, std::function<void(T)>>(args, func);
                    undo->deleter = [](void* data) { delete static_cast<std::pair<T, std::function<void(T)>> *>(data); };
                    customStack.stack.push_back(undo);
                    return undo;
                }

                template <typename T>
                void push_redo_custom_data(QParse_RULES____STRING tag, UNDO_KEY_PTR undo_key, T args, std::function<void(T)> func) {
                    commandStack.push_back(COMMAND::SUB_COMMAND__REDO_CUSTOM_DATA);

                    CustomDataStack::LinkData * undo = static_cast<CustomDataStack::LinkData*>(undo_key);

                    undo->redo = new CustomDataStack::LinkData();
                    undo->redo->is_undo = false;
                    undo->redo->TAG = QParse_RULES____STRING("REDO ") + tag;
                    undo->redo->invoker = [](void * data) {
                        typedef std::pair<T, std::function<void(T)>> FUNC;
                        FUNC * func = static_cast<FUNC*>(data);
                        func->second(func->first);
                    };
                    undo->redo->data = new std::pair<T, std::function<void(T)>>(args, func);
                    undo->redo->deleter = [](void* data) { delete static_cast<std::pair<T, std::function<void(T)>> *>(data); };
                }
            };

        private:
            QParse_RULES____VECTOR <Command*> commandStack;
            int disable = 0;
            size_t level = 0;
        public:
            Command * command = nullptr;

            UndoRedo();
            ~UndoRedo();

            void disable_push_command();
            void enable_push_command();

            using Printer = std::function<void(const QParse_RULES____STRING & description)>;

            void push_command(const QParse_RULES____STRING & description);
            void print(Printer printer);
            bool undo();
            bool redo();
            void print_error(Iterator & iterator, const char * indent = "");
        };
    }
}

#endif // QParse_RULES_ERROR1_H
#endif // ifndef QParse_RULES_ERROR2

#ifdef QParse_RULES_ERROR2
#ifndef QParse_RULES_ERROR2_H
#define QParse_RULES_ERROR2_H
namespace QParse {
    namespace Rules {

        void printError(const QParse_RULES____STRING & message, Iterator & iterator, UndoRedo & undo);

        struct Error : Rule {
            QParse_RULES____STRING message;

            Error(const QParse_RULES____STRING &message, Action action = NO_ACTION);

            using Rule::match;

            virtual std::optional<IteratorMatcher::MatchData> match(Iterator &iterator, UndoRedo *undo, bool doAction = true) override;
        };

        class ErrorIfMatch : public RuleHolder {
#ifdef GTEST_API_
            public:
#endif
            QParse_RULES____STRING message;
        public:
            ErrorIfMatch(Rule *rule, const QParse_RULES____STRING & message, Action action = NO_ACTION);

            using Rule::match;

            virtual std::optional<IteratorMatcher::MatchData> match(Iterator &iterator, UndoRedo *undo, bool doAction = true) override;

        };

        class ErrorIfNotMatch : public RuleHolder {
#ifdef GTEST_API_
            public:
#endif
            QParse_RULES____STRING message;
        public:
            ErrorIfNotMatch(Rule *rule, const QParse_RULES____STRING & message, Action action = NO_ACTION);

            using Rule::match;

            virtual std::optional<IteratorMatcher::MatchData> match(Iterator &iterator, UndoRedo *undo, bool doAction = true) override;

        };

    }
}
#endif // QParse_RULES_ERROR2_H
#endif // QParse_RULES_ERROR2
