# Coding Guidelines
## Basics
- Stick to the principles of Object Oriented Design
- Leave the code cleaner than you have found it
- Use comments to describe the why, not the how

## Planning
- One branch per feature/bugfix
- Split up any task that will take longer than a week
- Merge branches back as soon as possible

## Conventions

### File encoding and line endings
- The source files may be restricted to ASCII encoding, or use UTF-8 *without* *BOM* 
(this warning specifically targets developers using Windows).
- Line endings may be LF or CRLF, although the former is preferred. CR line endings are not valid.
- Encoding conversion macros (```L"", _T(""), T(""), …```) are considered a plague and will be rejected 
(unless they are in platform-dependent source files which only gets compiled on those platforms).

### Preprocessor
#### Include guards
**Include guards** have an uppercase name reflecting the current header file name, and are terminated by `_HPP`.

*They are not to be prefixed or suffixed by any underscore.*

In fact, all global names (that includes defines) starting with one underscore and a capital letter or two underscores 
are reserved by the C and C++ standard. 
As of the 2012-01-16 C++11 Working Draft, this is specified in 17.6.4.3.2/1 `[global.names]`.

*(Yet, many developers keep using this bad practice)*

As a side note for MSVC developers: ```#pragma once``` isn't an include guard and can't replace nor even supplement it. 
Except for Windows-specific source files, any file containing it will be rejected.
### Indentation, whitespace and line length
- An indentation level = two spaces. Tabs aren't considered a consistent identation method.
- Empty lines are not to be idented, i.e. /really/ empty.
- There may not be more than 2 consecutive empty lines.
- Namespace blocks does not indent, nor are switch case labels.
- When additional identation would make the code more legible (e.g. chained variable declarations), 
more spaces can be added if their count is low; else, just increase the identation level by one.
- Trailing whitespace isn't allowed.
- Maximum line length is 100 UTF-8 codepoints. In layman terms, 100 characters.
    ```cpp
namespace glPortal {

void func() {
  int i = 0;
  switch (i) {
  case 0:
    break;
  default:
    break;
  }

  // The following is correct, additional whitespace is low
  int a = 1,
      b = 2,
      c = 3;

  // The following isn't correct, too much whitespace
  std::unique_ptr<int> ptrA(new int),
                       ptrB(new int);

  // Use the following instead:
  std::unique_ptr<int> ptrC(new int), ptrD(new int), ptrE(new int), ptrF(new int), ptrG(new int),
    ptrH(new int);
  // Here, the line wrapped because we hit the 100-character limit
}

}
     ```
### Pointer and References
Pointer/reference mark sticks to the *variable* name (*not* function name), 
or when there is no variable name, to the type itself.
     ```cpp
Type var1, *var2, &var3 = thing;
Type* getPtr(Type *namedParm);
Type& getRef(UnnamedParm*);

// The following is inherently stupid:
Type *idiotGetPtr(Type *namedParm);
// because it doesn't return Type, it returns Type* !
// Moreover the function name has nothing to do with that pointer mark on it.
    ```
### Blocks of code
- *Always* open a new block of code after a control structure
- Open them on the line of the control statement
- Put code on the next line
     ```cpp
if (cond) {
  code();
}

// The following is bad:
if (cond)
{
  code();
}

if (cond)
  code();

if (cond) code();
 ```
