# QParse
A powerful Parser Combinator library with error reporting and input rewriting capabilities

## Building

define `CPP_RULES_USE_QT_FRAMEWORK` when building with QT6 for QT6 specialization support, uses `QString` and friends

otherwise leave `CPP_RULES_USE_QT_FRAMEWORK` undefined to build with `std` (non-QT) support, uses `std::string` and friends

to add support for another framework, see `framework_defines.h`

### Example

#### moderate:

```c
    Iterator it = content;

    auto single_comment = new Rules::Sequence({
        new Rules::String("//"),
        new Rules::Optional(new Rules::Char(' ')),
        new Rules::Until(new Rules::At(new Rules::NewlineOrEOF), [](CPP::Rules::Input i) {
            std::cout << "comment: " << i.quotedString() << std::endl;
        }),
        new Rules::Optional(new Rules::Newline)
    });

    auto block_comment = new Rules::Sequence({
        new Rules::String("#COMMENT_BEGIN"),
        new Rules::Until(new Rules::Sequence({
            new Rules::Newline(),
            new Rules::String("#COMMENT_END")}
        ))
    }, [](CPP::Rules::Input i) {
        //std::cout << "block comment: " << i.quotedString() << std::endl;
    });

    auto line = new Rules::Sequence({
        new Rules::Until(new Rules::At(new Rules::NewlineOrEOF), [](CPP::Rules::Input i) {
            std::cout << "line: " << i.quotedString() << std::endl;
        }),
        new Rules::Optional(new Rules::Newline)
    });

    auto empty_line = new Rules::Sequence({
        new Rules::MatchBUntilA(new Rules::At(new Rules::Newline), new Rules::Or({new Rules::Char(' '), new Rules::Char('\t')})),
        new Rules::Newline
    });

    if (!Rules::MatchBUntilA(new Rules::EndOfFile,
            new Rules::Or({
                single_comment,
                block_comment,
                empty_line,
                line,
                new Rules::Error("unexpected token")
            }
    )).match(it)) {
        return -1;
    }
```

#### complex

```c
    std::string a = "../../wl_syscalls.decl";
    auto content = readFile(a);

    typedef struct Info {
        bool is_typedef;
        std::vector<std::string> comment;
        std::string current_typedef, current_syscall, current_arguments, current_argument_count, current_arguments_usages;
    } Info;

    Info info;
    info.is_typedef = false;

    std::vector<Info> syscalls;

    using namespace CPP;

    Iterator it = content;

    auto space = new Rules::Or({new Rules::Char(' '), new Rules::Char('\t')});

    auto spaces = new Rules::ZeroOrMore(space);

    auto single_comment = new Rules::Sequence({
        new Rules::String("//"),
        spaces,
        new Rules::Sequence({
            new Rules::Until(new Rules::At(new Rules::NewlineOrEOF), [&](CPP::Rules::Input i) {
                info.comment.push_back(i.string());
            }),
            new Rules::Optional(new Rules::Newline)
        })
    });

    auto block_comment = new Rules::Sequence({
        new Rules::String("#COMMENT_BEGIN"), new Rules::Newline(),
        new Rules::MatchBUntilA(
            new Rules::Sequence({
                new Rules::String("#COMMENT_END"), new Rules::Newline()
            }),
            new Rules::Sequence({
                new Rules::Until(new Rules::At(new Rules::NewlineOrEOF), [&](CPP::Rules::Input i) {
                    info.comment.push_back(i.string());
                }),
                new Rules::Optional(new Rules::Newline)
            })
        )
    });
    
    auto c_ident = Rules_NS_LogTrace1(new Rules::Sequence({
        new Rules::Range({'a', 'z', 'A', 'Z', '_'}),
        new Rules::Optional(new Rules::OneOrMore(new Rules::Range({'a', 'z', 'A', 'Z', '0', '9', '_'})))
    }), "c_ident");

    auto number = Rules_NS_LogTrace1(new Rules::OneOrMore(new Rules::Range({'0', '9'})), "number");

    auto c_value = Rules_NS_LogTrace1(new Rules::Or({c_ident, number}), "c_value");

    auto syscall = new Rules::TemporaryAction(c_ident, [&](CPP::Rules::Input i) {
        std::string sl = i.string();
        std::transform(sl.begin(), sl.end(), sl.begin(), std::tolower);
        if (!info.is_typedef) {
            info.current_syscall = sl;
        } else {
            info.current_typedef = sl;
            info.is_typedef = false;
        }
    });

    auto arguments = new Rules::Sequence({
        new Rules::Char('<'),
        spaces,
        new Rules::Or({
            Rules_NS_LogTrace1(new Rules::Sequence({
                Rules_NS_LogTrace(new Rules::NotAt(
                    new Rules::Or({
                        new Rules::Sequence({
                            new Rules::Char('>'),
                            spaces,
                            new Rules::Or({
                                single_comment,
                                new Rules::Newline()
                            })
                        }),
                        new Rules::String("...")
                    })
                )),
                new Rules::ErrorIfNotMatch(new Rules::TemporaryAction(number, [&](Rules::Input i) { info.current_argument_count = i.string(); }), "expected an integer"),
                new Rules::ErrorIfNotMatch(new Rules::Char(','), "expected comma after number of arguments"),
                spaces,
                Rules_NS_LogTrace1(new Rules::ErrorIfNotMatch(new Rules::Sequence({
                    c_ident,
                    new Rules::Until(new Rules::At(new Rules::Sequence({spaces, new Rules::Char('>')})))
                }, [&](CPP::Rules::Input i) {
                    info.current_arguments = i.string();
                }), "expected argument declarations, followed by closing '>'"), "argument declaration"),
                spaces,
                new Rules::ErrorIfNotMatch(new Rules::Char('>'), "expected closing '>'"),
                spaces,
                new Rules::ErrorIfNotMatch(new Rules::Char('<'), "expected opening '<'"),
                spaces,
                new Rules::ErrorIfNotMatch(new Rules::Sequence({
                    new Rules::Optional(new Rules::Char('&')),
                    c_value,
                    // we cannot use an Until rule here since we need to skip '->' if it occurs
                    Rules_NS_LogTrace1(new Rules::MatchBUntilA(
                        Rules_NS_LogTrace1(new Rules::At(new Rules::Char('>')), "at >"),
                        new Rules::Or({
                            new Rules::Sequence({
                                new Rules::At(new Rules::String("->")),
                                Rules_NS_LogTrace1(new Rules::String("->"), "ignore ->")
                            }),
                            new Rules::Any
                        })
                    ), "argument usages"),
                }, [&](CPP::Rules::Input i) { info.current_arguments_usages = i.string(); }), "expected argument usages, followed by closing '>'"),
            }), "numbered arguments"),
            Rules_NS_LogTrace1(new Rules::If(
                [&]() { return info.current_typedef.length() != 0; },
                new Rules::ErrorIfMatch(
                    new Rules::At(new Rules::String("..."))
                    , "typedef declaration does not support varadic arguments (...)"
                ),
                new Rules::Optional(
                    new Rules::String("...", [&](CPP::Rules::Input i) { info.current_arguments = i.string(); })
                )
            ), "any number of arguments")
        }),
        spaces,
        new Rules::ErrorIfNotMatch(new Rules::Char('>'), "expected closing '>'")
    });

    auto syscall_line_end = new Rules::Or({
        single_comment,
        new Rules::Newline,
        new Rules::At(new Rules::EndOfFile)
    });

    auto syscall_line = new Rules::Sequence({
        syscall,
        spaces,
        new Rules::Optional(Rules_NS_LogTrace(arguments)),
        spaces,
        syscall_line_end,
    });

    auto syscall_line__or__typedef_syscall_line = new Rules::Or({
        new Rules::Sequence({
            new Rules::At(new Rules::Sequence({
                Rules_NS_LogTrace1(syscall, "at syscall"), spaces,
                new Rules::Or({
                    Rules_NS_LogTrace1(new Rules::Char('<'), "at <"),
                    Rules_NS_LogTrace1(syscall_line_end, "at syscall line end")
                })
            })),
            syscall_line
        }),
        new Rules::Sequence({
            new Rules::At(new Rules::Sequence({
                Rules_NS_LogTrace1(new Rules::String("typedef"), "at typedef"),
                spaces,
                Rules_NS_LogTrace1(syscall, "at syscall"), spaces,
                Rules_NS_LogTrace1(syscall, "at syscall"), spaces,
                new Rules::Or({
                    Rules_NS_LogTrace1(new Rules::Char('<'), "at <"),
                    Rules_NS_LogTrace1(syscall_line_end, "at syscall line end")
                })
            })),
            new Rules::String("typedef", [&](CPP::Rules::Input i) { info.is_typedef = true; }),
            spaces,
            new Rules::ErrorIfNotMatch(syscall, "expected syscall"),
            spaces,
            syscall_line
        }),
    }, [&](Rules::Input i) {
        syscalls.push_back(info);
        info.is_typedef = false;
        info.current_typedef.clear();
        info.comment.clear();
        info.comment.shrink_to_fit();
        info.current_syscall.clear();
        info.current_argument_count.clear();
        info.current_arguments.clear();
        info.current_arguments_usages.clear();
    });

    auto empty_line = new Rules::Sequence({
        new Rules::MatchBUntilA(new Rules::At(new Rules::NewlineOrEOF), space),
        new Rules::Optional(new Rules::Newline)
    }, [&](CPP::Rules::Input i) {
        info.comment.clear();
        info.comment.shrink_to_fit();
    });

    if (!Rules::MatchBUntilA(
        new Rules::EndOfFile,
        new Rules::ErrorIfNotMatch(
            new Rules::Or({
                single_comment,
                block_comment,
                empty_line,
                Rules_NS_LogTrace(syscall_line__or__typedef_syscall_line),
            }),
            "expected a comment, empty line, typedef syscall, or a syscall declaration"
        )
    ).match(it)) {
        printf("failed to parse syscalls.decl\n");
        return -1;
    }

    if (syscalls.empty()) {
        printf("no syscalls specified in syscalls.decl\n");
        return 0;
    }
```
parses the following
```
// syscall.decl
//
//     file format:
//         // comment, this can < contain anything % at @ all !
//
//         #COMMENT_BEGIN
//         this is a block comment
//             this can < contain anything % at @ all !
//         #COMMENT_END
//
//         // syscall documentation goes directly above syscall
//         // this can be
//         // multiple comments
//         #COMMENT_BEGIN
//               or multiple
//         #COMMENT_END
//         #COMMENT_BEGIN
//               block comments
//         #COMMENT_END
//         // or both
//         syscall                                                              // you can even put documentation here!  zero arguments example: foo  
//         syscall <>                                                           // zero arguments                                       example: foo  <>
//         syscall <argc, arg declaration> <argument usage>                     // argc arguments,                                      example: foo  <2, int foo, float bar> <foo, bar>
//         syscall <...>                                                        // up to 125 arguments of any type                      example: foo  <...>
//
//         // syscalls can also be typedef'd
//         //
//         // a typedef is like a mapping that maps the input of syscall_to to the input of syscall_from
//
//         // can map to any syscall that can accept specified argument types
//         //   eg, struct IO arg -> int, int*
//         //   eg, struct IO arg -> arg.input, &arg.outputInt   // passing arg.input would produce a compile error since it expects int* but we passed int
//         
//         //   eg, struct IO arg -> int, float*
//         //   eg, struct IO arg -> arg.input, &arg.outputFloat // passing arg.outputInt or arg.input would produce a compile error since it expects float* but we passed int or int*
//         //
//         typedef syscall_from syscall_to <argc, arg declaration> <argument usage>   // typedef argc arguments,                              example: typedef foo foo1  <2, foobar f> <f.foo, f.bar>
//         typedef syscall_from syscall_to <...>                                      // NOT SUPPORTED !!!    IT IS IMPOSSIBLE TO RELIABLE TYPEDEF A VARADIC ARGUMENT !
```

## Usage

### Basic Usage

```cpp
#include <CPP/Rules_Extra.h>
```

we first start with an `Iterator`

```cpp
using namespace CPP;

Iterator iter = "some string";
```

we then create a `Grammar`

a `Grammar` is a set of `Rule` objects that define the `Grammar Definition`

these are sometimes called `expressions` as they `express` how you want to parse something

for example, lets parse `"some string"`

```cpp
using namespace CPP;

Iterator iter = "some string";

Rules::String("some string").match(iter);
```

here, we match "some string" exactly, character for character

this is too easy, lets match it via an expression

we know it consists of alphabetical characters and a white space

for this we will use `Rules::Sequence` and `Rules::Range`

```cpp

auto text = new Rules::OneOrMore(
  new Rules::Range('a', 'z')
);

auto space = new Rules::Char(' ');

Rules::Sequence({ text, space, text }).match(iter);
```

here, we match against `one or more` range of `a to z`, followed by a space, followed by `one or more` range of `a to z`

you may notice we do not delete `text` or `space`, this is because each `Rules::` takes a `Rule*` object, and manages its lifetime automatically

```cpp
{
    Rules::At(
        new Rules::Char('a') // managed by At
    );
}
// At is destructed
// Char gets deleted by the destructor

```

this is done via a hidden `RuleHolder*` object

a `RuleHolder` object extends `Rule`, and manages the lifetime of a `Rule` object via `reference counting`

this makes it possible to use a `Rule` in multiple places without worrying about `dangling references` or `use-after-free`


all `Rules::` that accept `Rule*` objects must store each `Rule*` object in a `RuleHolder*` object

this is done by simply extending `RuleHolder` or by extending `Rule` and then storing each `Rule*` inside a `RuleHolder`

a `RuleHolder` can only contain one `Rule` at a time

we will cover some advance topics such as `conditional expressions`, `stack expressions`, `error reporting`, and `input modification` later

## Capabilities

TODO: document all capabilities

look in `Rules.h`, `Error.h`, `Conditionals.h`, and `Stack.h`
