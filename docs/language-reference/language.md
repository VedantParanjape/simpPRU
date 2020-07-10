# Language Syntax

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
* `Px_yz` - Pin mapping constants are Integer constant, where x is 1,2 or 8,9 and yz are the header pin numbers. For further details refer to [this](io.md)

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

## Comments

* TBD

## Keyword and Identifiers

### Reserved keywords
 
| | | |
| --- | --- | --- |
| **`true`**  | **`read_counter`** | **`stop_counter`** |
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

!!! Note
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

!!! Warning
    `break` and `continue` should only be used inside looping statements

### break

`break` is used to break execution in a loop statement, either `for loop` or `while loop`. It exits the loop upon calling.

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

## Functions

### Function definition

A function is a group of statements that together perform a task. You can divide up your code into separate functions. How you divide up your code among different functions is up to you, but logically the division usually is such that each function performs a specific task. A function declaration tells the compiler about a function's name, return type, and parameters. A function definition provides the actual body of the function. 

!!! warning
    Function must be defined before calling it.

#### Syntax

```python
def <function_name> : <data_type> : <data_type> <param_name>, <data_type> <param_name>, ... {
    statement 1;
    ...
    ...

    return <data_type>;
}
```

!!! Note
    If return data type is void, then return statement is not needed, and if still it is added, it must be return nothing, i.e., something like this `return ;`

!!! Warning
    `return` can only be present in the body of the function only once, that too at the end of the function, not inside any compound statements.
    
!!! fail "Wrong"
    * `return` inside a compound statement, this syntax is not allowed.
    ```python
    def test : int : int a {
        if : a < 4 {
            return a;
        }
    }
    ```

!!! done "Correct"
    * `return` is not inside compound statments, It should be placed only at the end of function definition
    ```python
    def test : int : int a {
        int gf := 8;
        if : a < 4 {
            gf := 4;
        }

        return gf;
    }
    ```


#### Examples

Examples according to return types

=== "Integer"
    ```python
    def test_func : int : int a, int b {
        int aa := a + 5;

        if : aa < 3 {
            aa : = 0;
        }

        return aa + b;
    }
    ```

=== "Boolean"
    ```python
    def compare : bool : int val {
        bool ret := false;

        if : val < 0 {
            ret := true;
        }

        return ret;
    }
    ```

=== "Void"
    ```python
    def example_func : void : bool qu {
        if : qu and true {
            ...
            do something
            ...
        }
    }

    def example_func_v : void : {
        int temp := 90;

        return;
    }
    ```

### Function call

Functions can be called only if, they have been defined earlier. They return data types according to their definition. Parameters are passed by value. Only pass by value is supported as of now.

#### Syntax

```python
function_name(var1, var2, ..);
```

#### Examples

We will consider functions defined in earlier [subsection](#examples_5)

=== "Integer"
    ```python
    int a := 55;
    int ret_val := test_func(4, a);
    ```

=== "Boolean"
    ```python
    bool val := compare(22);
    compare(-2);
    ```

=== "Void"
    ```python
    example_func(false);
    example_func_v();
    ```
