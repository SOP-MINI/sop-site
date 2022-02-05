---
title: "Grading rules"
date: 2022-02-10T10:00:00+01:00
draft: false
layout: single
menu: sop1
---

## Grade

**Course grade** is based on total points acquired by student during the labs (100p tops):

| Points in total | Grade |
| --------------- | ----- |
| s≥90            | 5     |
| 80≤ s \<90      | 4.5   |
| 70≤ s \<80      | 4     |
| 60≤ s \<70      | 3.5   |
| 50≤ s \<60      | 3     |
| s\<50           | 2     |

### Lecture

  - Lectures introduce the course subjects to the students, also practical aspects covered by laboratory classes. Thus lectures are scheduled for the first half of the semester as 2 hour modules.
  - No lecture tests are scheduled in this semester, but the lectured knowledge will be tested during laboratory entry tests.

### Laboratories

  - *ATTENTION*, the purpose of the labs is to test your self preparation level on the lecture and preparation materials topics.
  - All graded works must be completed **individually**.
  - The lab consists of introductory labs (L0), **three** graded labs (L1, L2, L3, L4), and an additional lab (L5).
  - Laboratory tasks are solved using Arch Linux installed on lab workstations. No solutions presented on other computers will be accepted.
  - Graded labs (L1/L2/L3/L4) begin with an entry test in the Moodle platform measuring technical knowledge required for the labs - lecture, man pages and tutorials. Later, a coding task is handed out. Students have 90 to solve and submit it.
  - **Each graded lab is worth 25 points including 8 points for entry test and 17 points for coding task.**
  - During an entry tests one **can not** use any written or electronic materials or any communication methods.
  - The coding task is divided into several stages. Points for each stage are defined by the task (summing up to 17p.). Stages have to be solved in order. One **can not** progress to a next stage before completing previous ones.
  - During graded coding tasks one **can** use man pages and any code from tutorials, own code repository, online and book examples and materials as long as the solution remains individual.
  - Each stage is checked and graded during the class. Use of proper tools and POSIX API, correct implementation and functional correctness are graded. Code style is not (except L3, see below).
  - At the end of the lab the source code of the program has to be uploaded (see Task solution formalities below). The code must contain all stages graded during the lab. Any points for stages that are not present in the source will be cancelled after the lab. Failure to upload the code results in 0 points for the lab.
  - The uploaded solution of **L3** will be additionally graded in terms of code quality and aesthetics. This will be checked after the lab. For the problems you will receive up to 10 (but no more than number of points from the lab) penalty points that will be subtracted from the base result from L3.
  - Absence during two of graded labs L1/L2/L3/L4 results in subject failure.
  - If the teacher decides that the solution to a laboratory task or project is not individual, the subject fails.
  - Laboratory L5 is a possibility for retaking one of the graded labs (L1/L2/L3/L4). Only single topic can be retaken. Either one part (introductory test or programming task) or both parts of the same topic can be retaken. Grades from the retake replace the grades from original task. **During the retakes no odd/even week group division is used. Each of retake dates will have assigned two topics instead.**
  - All classes will be conducted (without breaks) according to the [schedule]({{< ref "harmonogram" >}}).

## Lab task solution formalities

  - The task must be coded in *C* (not C++)
  - The solution of must be submitted as a archive in .tar.gz, .tar.xz or .tar.bz2 format. After typical extraction (flags -xzf, -xJf or -xjf) the archive must be unpacked to the directory named $USER where $USER is your login.
  - The archive must contain source files (.c, .h) and a Makefile
  - One should be able to compile the whole solution by issuing a single *make* command
  - The *-Wall* flag and the other compiler flags required in the task must be *obligatorily* turned on during gcc compilation
  - The archive must be copied to the network share location given in the task.
