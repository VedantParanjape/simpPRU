# Language Reference

* simpPRU is a procedural programming language.
* It is a statically typed language. Variables and functions must be assigned data types during compilation.
* It is typesafe, and data types of variables are decided during compilation.
* simPRU codes have a `.sim` extension.

## Datatypes

* `int` - Integer datatype
* `bool` - Boolean datatype
* `void` - Void datatype, can only be used a return type for functions

## Constants

* `<any_integer>` - Integer constant
* `true` - Boolean constant (True)
* `false` - Boolean constant (False)

## Operators

* `{`,`}` - Braces
* `(`,`)` - Parenthesis
* `/`,`*`,`+`,`-` - Arithmetic operators
* `>`,`<`,`==`,`!=`,`>=`,`<=` - Comparison operators
* `~`,`&`,`|` - Bitwise operators: not, and, or
* `not`,`and`,`or` - Logical operators: not, and, or
* `:=` - Assignment operator

***Bitwise operators*** and ***Logical operators*** cannot be interchangeably used.

```cpp
Correct: bool test := true & false | (false and true);
Wrong:   bool test := true & false | false and true;
```

* Result of Arithmetic operator is Integer constant.
* Result of Comparison, Bitwise and Logical operators is Boolean constant.
* Only Integer constants can be used with Arithmetic operators.
* Only Integer constants can be used with Comparison operators.
* Only Boolean constants can be used with Bitwise and Logical operators.

```cpp
Correct: bool out := 5 > 6;
Wrong:   int yy := 5 > 6;
```

## Variable declaration

* Datatype of variable needs to be specified during compile time.
* Variables can be assigned values after declarations.
* If variable is not assigned a value after declaration, it is set to `0` for `integer` and to `false` for `boolean` by default.  
* Variables can be assigned other variables of same datatype.
* Variables can be assigned expressions whose output is of same datatype.

### Declaration
  
```cpp
int var;
bool test_var;
```

### Assignment during Declaration

```cpp
int var := 99;
bool test_var := false;
```

### Assignment

```cpp
var := 45;
test_var := true;
```

* Variables to be assigned must be declared earlier.
* Datatype of the variables cannot change. Only appropriate expressions/constants of their respective datatypes can be assigned to the variables.
* Integer variable can be assigned only Integer expression/constant.
* Boolean variable can be assigned only Boolean expression/constant.

## Keyword and Identifiers

### Reserved keywords
 
| `true`  | `read_counter` | `stop_counter` |
| --- | --- | --- |
| **`false`** | **`start_counter`** | **`pwm`** |
| **`int`** | **`delay`** | **`digital_write`** |
| **`bool`** | **`digital_read`** | **`def`** |
| **`void`** | **`return`** | **`or`** |
| **`if`** | **`and`** | **`not`** |
| **`elif`** | **`continue`** | **`break`** |
| **`else`** | **`while`** | **`in`** |
| **`for`** | | |

### Valid identifier naming

* An identifier/variable name must be start with an alphabet or underscore (_) only, no other special characters, digits are allowed as first character of the identifier/variable name.

    `product_name, age, _gender`

* Any space cannot be used between two words of an identifier/variable; you can use underscore (_) instead of space.

    `product_name, my_age, gross_salary`


* An identifier/variable may contain only characters, digits and underscores only. No other special characters are allowed, and we cannot use digit as first character of an identifier/variable name (as written in the first point).

    `length1, length2, _City_1`

Detailed info: [https://www.includehelp.com/c/identifier-variable-naming-conventions.aspx](https://www.includehelp.com/c/identifier-variable-naming-conventions.aspx)

## Expressions

### Arithmetic expressions

```cpp
=> ((3+3)/2)
=> 3
```

### Boolean expressions

```cpp
=> (false & true) | ~false
=> true

=> (true and true) & (not false)
=> true

=> (3 > 4) and false
=> false

=> (4 != 3) & (false and true)
=> false

=> true
=> true

=> false
=> false
```

!!! note
    Expressions are evaluated following the [operator precedence](#operators)

## If-else statement

Statements in the if-block are executed only if the if-expression evaluates to `true`. If the value of expression is `true`, statement1 and any other statements in the block are executed and the else-block, if present, is skipped. If the value of expression is `false`, then the if-block is skipped and the else-block, if present, is executed. If elif-block are present, they are evaluated, if they become `true`, the statement is executed, otherwise, it goes on to eval next set of statements

### Syntax

```python
if : boolean_expression {
    statement 1
    ...
    ...
}
elif : boolean_expression {
    statement 2
    ...
    ...
    ...
}
else {
    statement 3
    ...
    ...
}
```

### Examples

```python
int a := 3;

if : a != 4 {
    a := 4;
}
elif : a > 4 {
    a := 10;
}
else {
    a := 0;
}
```

* This will evaluate as follows, since `a = 3`, if-block (`3!=4`) will evaluate to true, and value of a will be set to 4, and program execution will stop.

## For-loop statement

For loop is a range based for loop. Range variable is a local variable with scope only inside the for loop.

### Syntax

```python
for : var in range_1:range_2 {
    statement 1
    ....
    ....
}
```

* Here, for loop is a range based loop, value of integer variable `var` will vary from `range_1` to `range_2 - 1`. Value of `var` doesnot equal `range_2`. As of now range can't be reversed,i.e., `range_1 < range_2`
  
!!! Note
    **var** is a **integer**, and **range_1, range_2** can be **arithmetic expression, integer variable or integer constant**.

### Examples

```python
int sum := 0;

for : i in 1:4 {
    sum = sum + i;
}
```

```python
int mx := 32;
int nt;

for : j in 2:mx-10 {
    nt := nt + j;
}
```

## While-loop statement

While loop statement repeatedly executes a target statement as long as a given condition is true.

### Syntax

```python
while : boolean_expression {
    statement 1
    ...
    ...
}
```

### Examples

* Infinite loop

```python
while true {
    do_something..
    ...
}
```

* Normal loop, will repeat 30 times, before exiting

```python
int tag := 0;

while : tag < 30 {
    tag := tag + 1;
}
```

## Control statements

!!! warning
    `break` and `continue` should only be used inside looping statements

### break

`break;` is used to break execution in a loop statement, either `for loop` or `while loop`. It exits the loop upon calling.

#### Syntax

`break;`

#### Examples

```python
for : i in 0:9 {
    if : i == 3 {
        break;
    }
}
```

### continue

`continue` is used to continue execution in a loop statement, either `for loop` or `while loop`.

#### Syntax

`continue;`

#### Examples

```python
for : j in 9:19 {
    if : i == 12 {
        continue;
    }
    else {
        break;
    }
}
```