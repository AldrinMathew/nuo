# Option

A header-only library for Option type in C++

You need `exception` standard library for this to work

You can use the library like the following

```cpp
/* No value */
auto var = Option<int>(); // Null or
auto var = Option<int>(Null); // Null or
auto var = Option<int>(NullTy()); // Null
var.has(); // Returns false since the value is Null
var.get(); // Throws OptionNullException
var.getOr(50); // Returns 50 since there is no value

/* Value */
auto var = Option<int>(10); // 10
var.has(); // Returns true since the value is 10
var.get(); // Returns 10
var.getOr(50); // Returns 10 since there is a value
```

`Null` is an instance of `NullTy` provided by the file. It is not different from `NullTy()`
