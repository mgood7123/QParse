# QParse
A powerful Parser Combinator library with error reporting and input rewriting capabilities

## Building

define `CPP_RULES_USE_QT_FRAMEWORK` when building with QT6 for QT6 specialization support, uses `QString` and friends

otherwise leave `CPP_RULES_USE_QT_FRAMEWORK` undefined to build with `std` (non-QT) support, uses `std::string` and friends

to add support for another framework, see `framework_defines.h`

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

you may notice we do not delete `text` or `space`, this is because each `Rule` takes a `Rule*` object, and manages its lifetime automatically

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



we will cover some advance topics such as conditional expressions and stack expressions later

## Capabilities

TODO: document all capabilities

look in `Rules.h`, `Error.h`, `Conditionals.h`, and `Stack.h`
