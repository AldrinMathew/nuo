# **`nuo`** library for C++20

`nuo` provides the following:

- [**`Maybe<T>`**](#maybe) type to handle cases where null-like values are required. This type is accompanied by `Null` class which can be instantiated and passed as a `null` value. This pattern makes the user to handle null cases explictly.
- [**`Vague<T>`**](#vague) type to handle cases where there should either be a value or an exception/problem. But instead of throwing and catching exceptions, you will have to handle it when you want to unwrap the value. There is a `Problem` type accompanying this that can be used to pass a problem encountered during logic.
- [**`Vec<T>`**](#vec) type that helps in storing dynamically allocated sequence of elements of a specified type. The allocated size is doubled and the existing elements are moved if there is no more pre-allocated buffer space left

Standard libraries used by this library: `exception`, `string`, `initializer_list`

This library might be compatible with previous standards of C++, but that is untested.

## Maybe

```cpp
/* No value */
auto var = Maybe<int>(); // Null or
auto var = Maybe<int>(Null()); // Null
var.has(); // Returns false since the value is Null
var.get(); // Throws Exception
var.getOr(50); // Returns 50 since there is no value

/* Value */
auto var = Maybe<int>(10); // 10
var.has(); // Returns true since the value is 10
var.get(); // Returns 10
var.getOr(50); // Returns 10 since there is a value already
```

## Vague

## Vec

