# atomic memory access

## 参考
https://gcc.gnu.org/onlinedocs/gcc-4.6.2/gcc/Atomic-Builtins.html

## __sync_lock_test_and_set
```
type __sync_lock_test_and_set (type *ptr, type value, ...)
```
```
This builtin, as described by Intel, is not a traditional test-and-set operation, but rather an atomic exchange operation. It writes value into *ptr, and returns the previous contents of *ptr.

Many targets have only minimal support for such locks, and do not support a full exchange operation. In this case, a target may support reduced functionality here by which the only valid value to store is the immediate constant 1. The exact value actually stored in *ptr is implementation defined.

This builtin is not a full barrier, but rather an acquire barrier. This means that references after the builtin cannot move to (or be speculated to) before the builtin, but previous memory stores may not be globally visible yet, and previous memory loads may not yet be satisfied.
```
交换value和ptr地址的值，返回ptr地址之前保存的值
1. oldptr_val = *ptr
2. *ptr = value
3. value = oldptr_val
4. return oldptr_val

## __sync_lock_release
```
void __sync_lock_release (type *ptr, ...)
```
```
This builtin releases the lock acquired by __sync_lock_test_and_set. Normally this means writing the constant 0 to *ptr.

This builtin is not a full barrier, but rather a release barrier. This means that all previous memory stores are globally visible, and all previous memory loads have been satisfied, but following memory reads are not prevented from being speculated to before the barrier.
```
释放 __sync_lock_test_and_set 中持有的锁，同时将 ptr 地址的值赋为0。

## __sync_synchronize
memory barrier，保证这条指令后面的指令一定在这条指令之前的指令后执行。