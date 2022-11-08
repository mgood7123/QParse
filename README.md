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

```

auto text = new Rules::OneOrMore(
  new Rules::Range('a', 'z')
);

auto space = new Rules::Char(' ');

Rules::Sequence({ text, space, text }).match(iter);
```

here, we match against `one or more` range of `a to z`, followed by a space, followed by `one or more` range of `a to z`

## Capabilities

TODO: document all capabilities

look in `Rules.h`, `Error.h`, `Conditionals.h`, and `Stack.h`
