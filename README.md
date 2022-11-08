# QParse
A powerful Parser Combinator library with error reporting and input rewriting capabilities


## Usage

#include <CPP/Rules_Extra.h>

## Building

define `CPP_RULES_USE_QT_FRAMEWORK` when building with QT6 for QT6 specialization support, uses `QString` and friends

otherwise leave `CPP_RULES_USE_QT_FRAMEWORK` undefined to build with `std` (non-QT) support, uses `std::string` and friends

to add support for another framework, see `framework_defines.h`

## capabilities

TODO: document all capabilities

look in `Rules.h`, `Conditionals.h` and `Stack.h`
