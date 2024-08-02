# 42 Philosophers 


<p align="center">
  <img width="500" hight="500" align="center" alt="Dining Philosophers" src="https://upload.wikimedia.org/wikipedia/commons/thumb/7/7b/An_illustration_of_the_dining_philosophers_problem.png/1024px-        An_illustration_of_the_dining_philosophers_problem.png" > 
</p>
<p  align="center">

<em>
 The famous “Dining Philosophers Problem”
</em>
</p>

## [The dining philosophers problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem) is a famous problem in computer science used to illustrate common issues in [concurrent programming](https://en.wikipedia.org/wiki/Concurrency_(computer_science)).
---
# Introduction

## The Dining Philosophers problem involves philosophers sitting around a circular table. Each philosopher has a plate of spaghetti and needs [two forks]() to eat. The number of forks on the table equal to number of philosophers, one between each pair of plates. The challenge is to design a system where each philosopher can eat without causing deadlocks or starvation.This project implements a solution to the problem using C and [pthreads](https://www.man7.org/linux/man-pages/man7/pthreads.7.html). It demonstrates concurrent programming concepts, resource sharing, and deadlock prevention.

---
# Imagine a scenario

There are five philosophers sitting around a circular table.
Each philosopher has a plate of spaghetti in front of them.
Between each pair of plates is one fork **(so five forks total).**
To eat, a philosopher needs two forks - one from their left and one from their right.
How can we design a system where each philosopher can eat without causing a deadlock (where everyone is stuck waiting forever) or starvation (where someone never gets to eat)?


# Implementation
The solution uses the following key components:
### Philosophers as `threads`
Each philosopher can be represented as a separate thread in C. These threads run concurrently, just like the philosophers thinking and eating independently.
```c
void* simulate(void* data)
{
    // Philosopher's actions (thinking, picking up forks, eating, putting down forks and sleeping ...)
}
```
### Forks as shared resources 
The forks represent shared resources that multiple threads (philosophers) need to access. In C, we can represent these as **`mutex locks`**.
```c
pthread_mutex_t forks[5];
```
Picking up forks as locking:
When a philosopher picks up a fork, it's like a thread acquiring a lock on a shared resource.

```c
pthread_mutex_lock(&forks[left_fork]);
pthread_mutex_lock(&forks[right_fork]);
```
Putting down forks as unlocking:
When a philosopher puts down the forks, it's like a thread releasing the locks.
```c
pthread_mutex_unlock(&forks[left_fork]);
pthread_mutex_unlock(&forks[right_fork]);
```
# Deadlock scenario
If each philosopher picks up the left fork and waits for the right fork, we have a deadlock. This is similar to multiple threads each holding one resource and waiting for another, creating a circular dependency.
