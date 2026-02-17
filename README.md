# Repo to practice Learning C++ parallelism

Reading the book C++ Concurrency in Action 2nd Edition and practicing the gained knowledge here.

## Chapter Notes

### 3 Sharing data between threads

| Wrapper | Standard | Member Functions (`lock`, `unlock`, etc.) | Key Features | Best Use Case |
| :--- | :---: | :---: | :--- | :--- |
| **`std::scoped_lock`** | C++17 | ❌ No | **Deadlock Avoidance**: Can lock multiple mutexes safely at once. Zero overhead for single mutex. | **Default choice** for C++17+. Use for all simple scopes and multi-lock scenarios. |
| **`std::unique_lock`** | C++11 | ✅ Yes | **Flexible**: Can be deferred, unlocked early, moved, and adopted. Required for `std::condition_variable`. | Complex logic, manual unlocking, transferring ownership, or waiting on condition variables. |
| **`std::shared_lock`** | C++14 | ✅ Yes | **Shared Ownership**: Allows multiple threads to hold the lock (Reader lock). | Use with `std::shared_mutex` for **Readers** in a Read-Write scenario. |
| **`std::lock_guard`** | C++11 | ❌ No | **Simple RAII**: Strictly scope-bound. Cannot be moved or unlocked early. | **Legacy Default**. Use only if stuck on C++11/14 (otherwise `scoped_lock` is superior). |

`g++ -std=c++23 -pthread <file>`