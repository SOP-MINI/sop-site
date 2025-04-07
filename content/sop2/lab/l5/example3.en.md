---
title: "Task on pipe/FIFO"
date: 2022-02-01T19:32:59+01:00
bookHidden: true
---

## Task
In this task, we will simulate the OPS Laboratory. The laboratory consists of `n` (`3 <= n <= 20`, the only program parameter) students and one teacher. Students solve a task divided into four stages, worth 3, 6, 7, and 5 points respectively. Each student has a skill level `k` (a random integer, `3 <= k <= 9`), which determines their ability to solve the task.

## Stages
1. **Process Creation:**
   - The main process (teacher) creates `n` child processes (students).
   - Each student prints their process ID and exits.

2. **Attendance Check:**
   - Students communicate with the teacher through pipes.
   - A shared pipe allows students to write to the teacher, while the teacher communicates with each student through individual pipes.
   - The teacher takes attendance by sending `Teacher: Is [PID] here?` to each student and the standard output.
   - Students respond with `Student [PID]: HERE` through both the shared pipe and standard output.
   - After the attendance check, all processes exit.

3. **Task Stages:**
   - After the attendance check, students begin the task by following these steps for each stage:
     1. Generate a random integer `t` in the range `[100, 500]`.
     2. Wait `t` milliseconds.
     3. Generate a random integer `q` from `[1, 20]`, representing their attempt score.
     4. Compute their total attempt score as `k + q`.
     5. Send their process ID and attempt score to the teacher.
     6. The teacher compares the attempt score against the stage difficulty (`d`), which is calculated as the stage's base points plus a random integer from `[1, 20]`.
     7. If `k + q >= d`, the student succeeds; otherwise, they fail. The information is sent back to the student.
     8. The teacher prints one of the following messages:
        - `Teacher: Student [PID] finished stage [X]` (on success).
        - `Teacher: Student [PID] needs to fix stage [X]` (on failure).
     9. If a student completes all four stages, they print `Student [PID]: I NAILED IT!` and exit.

   - The teacher waits for all students to finish. After all students exit, the teacher prints `Teacher: IT'S FINALLY OVER!` and exits.

4. **Time Limit:**
   - The laboratory session may end if time runs out. This is handled using the `alarm(2)` function after attendance is checked.
   - When the `SIGALRM` signal is received, the teacher prints `Teacher: END OF TIME!`, stops grading students, and prints a summary table before releasing resources and exiting.
   - Each student detects that the teacher has left and prints `Student [PID]: Oh no, I haven't finished stage [X]. I need more time.` before exiting.

## Example output

```
./sop-lab 5
Student: 47807
Student: 47808
Student: 47809
Student: 47810
Teacher: 47806
Teacher: Is 47807 here?
Student 47807: HERE!
Teacher: Is 47808 here?
Student: 47811
Student 47808: HERE!
Teacher: Is 47809 here?
Student 47809: HERE!
Teacher: Is 47810 here?
Student 47810: HERE!
Teacher: Is 47811 here?
Student 47811: HERE!
Teacher: Student 47808 finished stage 1
Teacher: Student 47809 finished stage 1
Teacher: Student 47811 finished stage 1
Teacher: Student 47807 needs to fix stage 1
Teacher: Student 47810 finished stage 1
Teacher: Student 47809 finished stage 2
Teacher: Student 47807 finished stage 1
Teacher: Student 47808 needs to fix stage 2
Teacher: Student 47810 finished stage 2
Teacher: Student 47811 needs to fix stage 2
Teacher: Student 47807 needs to fix stage 2
Teacher: Student 47809 finished stage 3
Teacher: Student 47810 needs to fix stage 3
Teacher: Student 47808 needs to fix stage 2
Teacher: Student 47807 needs to fix stage 2
Teacher: Student 47809 needs to fix stage 4
Teacher: Student 47811 needs to fix stage 2
Teacher: Student 47807 needs to fix stage 2
Teacher: Student 47809 needs to fix stage 4
Teacher: Student 47807 finished stage 2
Teacher: Student 47811 needs to fix stage 2
Teacher: Student 47810 needs to fix stage 3
Teacher: Student 47808 needs to fix stage 2
Teacher: Student 47809 needs to fix stage 4
Teacher: Student 47807 finished stage 3
Teacher: Student 47811 finished stage 2
Teacher: Student 47810 needs to fix stage 3
Teacher: Student 47808 needs to fix stage 2
Student 47807: I NAILED IT!
Teacher: Student 47807 finished stage 4
Teacher: END OF TIME!
Teacher: 47807 - 21
Teacher: 47808 - 3
Teacher: 47809 - 16
Teacher: 47810 - 9
Teacher: 47811 - 9
Teacher: IT'S FINALLY OVER!
Student 47809: Oh no, I haven't finished stage 4. I need more time.
Student 47810: Oh no, I haven't finished stage 3. I need more time.
Student 47811: Oh no, I haven't finished stage 3. I need more time.
Student 47808: Oh no, I haven't finished stage 2. I need more time.
```
