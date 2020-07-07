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
* `>`,`<`,`=`,`!=`,`>=`,`<=` - Comparison operators
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