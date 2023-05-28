
1)

```
for(int i = 1; i < SIZE-1; ++i) {
    a[i] = a[i%argc];
}
```

This loop cannot be vectorized because the compiler recognizes that

2)

```c
for(int i = 0; i < SIZE; ++i) {
    b[i] = a[i];
}
```

This loop is not vectorized since multiple arrays (data refs) are used.
This probably is a problem with aliasing although the compiler could know that the arrays are not overlapping.

