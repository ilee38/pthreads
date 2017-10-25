# pthreads
Multithreaded programming using the Pthreads C library. (Operating Systems class homework project)

This project solves the classical Dining Philosophers problem.

Part2: uses mutex locks to provide syncrhonization among threads, however race conditions, leading to deadlocks are still possible.

Part3: uses condition variables, along with a mutex lock to solve the deadlock issue

Part4: takes a different approach, and uses the pthread_mutex_trylock() routine to solve deadlocks.
