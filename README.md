# Repo to practice Learning C++ parallelism

Reading the book C++ Concurrency in Action 2nd Edition and practicing the gained knowledge here.

`g++ -std=c++23 -pthread <file>`


### 3 Sharing data between threads

| Wrapper | Standard | Member Functions (`lock`, `unlock`, etc.) | Key Features | Best Use Case |
| :--- | :---: | :---: | :--- | :--- |
| **`std::scoped_lock`** | C++17 | ❌ No | **Deadlock Avoidance**: Can lock multiple mutexes safely at once. Zero overhead for single mutex. | **Default choice** for C++17+. Use for all simple scopes and multi-lock scenarios. |
| **`std::unique_lock`** | C++11 | ✅ Yes | **Flexible**: Can be deferred, unlocked early, moved, and adopted. Required for `std::condition_variable`. | Complex logic, manual unlocking, transferring ownership, or waiting on condition variables. |
| **`std::shared_lock`** | C++14 | ✅ Yes | **Shared Ownership**: Allows multiple threads to hold the lock (Reader lock). | Use with `std::shared_mutex` for **Readers** in a Read-Write scenario. |
| **`std::lock_guard`** | C++11 | ❌ No | **Simple RAII**: Strictly scope-bound. Cannot be moved or unlocked early. | **Legacy Default**. Use only if stuck on C++11/14 (otherwise `scoped_lock` is superior). |

### Chapter 4: Synchronizing Concurrent Operations

When threads need to wait for specific events or conditions, repeatedly checking a flag in shared memory is wasteful. The C++ Standard Library provides robust facilities to handle synchronization, primarily through condition variables and futures.


#### 1. Condition Variables
Condition variables allow a thread to block (sleep) until notified that a specific condition is true.
* **Types**: C++ provides `std::condition_variable` (works exclusively with `std::unique_lock<std::mutex>`) and `std::condition_variable_any` (works with any lockable type).
* **Waiting (`wait`)**: A thread calls `wait()` on a condition variable, passing the locked mutex. This atomically unlocks the mutex and suspends the thread. When woken, the thread reacquires the lock before returning from `wait()`.
* **Predicates & Spurious Wakeups**: Waiting threads can occasionally wake up without being explicitly notified (spurious wakeups). To handle this safely, `wait()` accepts a predicate (like a lambda expression) to verify the condition is actually met.
* **Notifying (`notify_one` / `notify_all`)**: When an event occurs (like adding an item to a queue), the working thread calls `notify_one()` to wake a single waiting thread, or `notify_all()` to wake all waiting threads.


#### 2. Waiting for One-Off Events with Futures
While condition variables are useful for repeating events, futures are designed for one-off events (like the result of a background calculation). 
* **`std::future` vs `std::shared_future`**: `std::future` represents unique ownership of an asynchronous result; `get()` can only be called once. `std::shared_future` is copyable, allowing multiple threads to safely wait for and access the same result.
* **Retrieving Data (`get`)**: Calling `get()` on a future blocks the thread until the future becomes "ready," at which point it returns the value.

#### 3. Starting Background Tasks with `std::async`
`std::async` is a high-level function template used to start an asynchronous task and return a `std::future` to retrieve the result.
* **Launch Policies**: You can control execution using the `std::launch` parameter. `std::launch::async` forces the task to run on a new thread, while `std::launch::deferred` delays execution until `wait()` or `get()` is called on the future (running it synchronously on the calling thread). By default, the implementation may choose either.

#### 4. `std::packaged_task`
* **Purpose**: `std::packaged_task` ties a `std::future` to a specific function or callable object. When the packaged task is invoked, it runs the wrapped function and stores the return value in the associated future.
* **Usage**: It acts as a callable object itself, making it a great building block for task schedulers or thread pools where tasks are placed on a queue to be executed later.

#### 5. Explicitly Setting Values with `std::promise`
* **Purpose**: `std::promise` provides a lower-level mechanism to explicitly set a value (or an exception) that can later be read through an associated `std::future`.
* **Usage**: A waiting thread blocks on the future, while the provider thread uses the promise's `set_value()` method to fulfill the promise and make the future ready. This is especially useful in scenarios like handling asynchronous network connections.

#### 6. Handling Exceptions
If an asynchronous task (via `std::async`, `std::packaged_task`, or `std::promise`) throws an exception, the exception is caught, stored in the future instead of the result, and re-thrown when the waiting thread calls `get()`. For promises, you can explicitly store an exception using `set_exception()`.
