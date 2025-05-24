# Operator Precedence Table Constructor

A simple C-based implementation to construct an Operator Precedence Table using grammar productions, helping visualize leading and trailing relationships between terminals and non-terminals.

---

## 🧪 Aim

To write a C program that constructs the **Operator Precedence Table** for a given grammar using the concepts of **LEADING** and **TRAILING** sets of non-terminals.

---

## ⚙️ Logic and Explanation

The program accepts a set of grammar productions in the form `A=α`, where `A` is a non-terminal and `α` is a combination of terminals and/or non-terminals.

1. **Identify Terminals and Non-Terminals**:  
   Parses the input grammar to separate out terminals (lowercase/characters/symbols) and non-terminals (uppercase letters).

2. **Compute LEADING and TRAILING Sets**:  
   - **LEADING**: First terminal symbols that can appear at the start of strings derived from non-terminals.
   - **TRAILING**: Last terminal symbols that can appear at the end of strings derived from non-terminals.

3. **Precedence Rules**:
   - `a = b` if `a` and `b` are adjacent terminals.
   - `a < b` if `a` is a terminal followed by a non-terminal that has `b` in its LEADING set.
   - `a > b` if `a` is a terminal preceded by a non-terminal that has `a` in its TRAILING set.
   - `$ < leading(start symbol)` and `trailing(start symbol) > $` rules are also applied.

4. **Output Tables**:
   - LEADING Table
   - TRAILING Table
   - Precedence Table

---

## 🧾 Sample Input and Output

### Input:
```yaml
Enter the number of productions: 3
Enter the productions:
E=E+T
T=TxV
V=a
```

### Output:
```yaml
LEADING Table:
LEADING(E) = a +
LEADING(T) = a
LEADING(V) = a

TRAILING Table:
TRAILING(E) = a
TRAILING(T) = a x
TRAILING(V) = a

Precedence Table:
+ x a $
. = - < -
x - = < -
a - > = >
$ < - < -
```

---

## 🌍 Real-World Applications

- **Compiler Design**: Used in syntax analysis and parsing to determine the correct order of operations.
- **Expression Evaluation**: Helps evaluate arithmetic/logical expressions where operator precedence affects the result.
- **Parser Construction**: Assists in building operator precedence parsers, a type of bottom-up parser.
- **Programming Language Design**: Ensures unambiguous grammar for operator handling in custom language design.

---
