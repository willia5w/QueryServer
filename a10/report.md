Dan Williams 8/9/2019

Difference between running DirectoryParser_MT with 1 thread VS no threads:
- There is no appreciable difference in performance between using no threads
VS 1 thread as there lack a presence of a race condition between threads. The small 
decrease in performance seen may be due to the addition of p_thread functions being used.

Testing with 1 core: (10 Files)
- Using no threads: 165.746015 seconds to execute.
- Using 1 thread: 178.197443 seconds to execute.
- Using 5 threads 241.547176 seconds to execute.
- Using 10 threads 227.640719 seconds to execute.

Testing with 2 cores: (10 Files)
- Using no threads: 169.282680 seconds to execute.
- Using 1 thread 175.426482 seconds to execute.
- Using 5 threads 212.177649 seconds to execute.
- Using 10 threads 201.214591 seconds to execute.

Explanation of observation:
- Regardless of number of cores used, the use of threads resulted in degraded performance.
- A race condition can explain this, however it is interesting that the use of 5 threads
 is less effective than 10 when this theoretically should result in less
 threads being locked.
