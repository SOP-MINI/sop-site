---
title: "Grading rules"
date: 2022-02-10T10:00:00+01:00
draft: false
layout: single
menu: sop2
---

## Grade

**Course grade** Acquired points sum up to 200 points. To get the minimal satisfactory grade (3.0) at least 50p from the lab part and at least 50p from the lecture part are needed. Additionally it is possible to pass with only 50-x points for the lab or lecture part, where 0 \< x \<= 5, but having at least 100+x points in total.

| Points in total | Grade |
| --------------- | ----- |
| 180 ≤ s ≤ 200   |       |
| 161 ≤ s \< 180  | 4.5   |
| 141 ≤ s \< 160  |       |
| 121 ≤ s \< 140  | 3.5   |
| 100 ≤ s \< 120  |       |
| s \< 100        |       |

### Lecture

  - Two lecture tests are scheduled per semester.
  - Each student has the right to retake each test once. The scope of material is the same as during primary term. Student must notify lecturer about own plans to retake the test(s) at least 24 hours before scheduled date. The grade from each retake test will replace the original one - if improved.
  - Total number of points from lecture tests is 100.

### Laboratories

- *ATTENTION*, the purpose of the labs is to test your self preparation level on the lecture and preparation materials topics.
- The lab consist of four graded classes (L1, L2, L3, L4) and L5 classes which are used for the retakes.
-   Graded lab is divided in the following parts: questions to the teacher about the lab tutorial (up to  35 minutes), an introductory test (8 minutes), a programming assignment (90 minutes). **For each lab up to 25 points can be acquired. 8 for the test and 17 for the programming assignment.**
-   During the the programming task the student demonstrates the program to the teacher on a lab workstation. The teacher checks the program source code then.
- An introductory test checks knowledge required during the class - lectures, man pages and other provided resources. The test is solved on moodle platform using the lab workstation.
- During an entry tests one **can not** use any written or electronic materials or any communication methods. During the test only a single browser window containing moodle can be opened. No other browser tabs, windows or other applications are allowed.
-   To acquire the points for the introductory test is is mandatory to return the signed paper with the programming assignment to the teacher.
-   Amount of material in scope of each class is different, thus the tutorial contents for each class are different. The tutorials for L1 and L3 are longer, the tutorial for L2 is short. L4 is a synthetic lab which requires knowledge from all earlier labs.
- After an entry test a programming task is handed out. The task is split into several stages.
- During graded coding tasks one **can** use man pages and any code from tutorials, own code repository, online and book examples and materials as long as the solution remains individual.
- During coding task one **can** consult the teacher.
- Stages have to be solved in order. One **can not** progress to a next stage before completing previous ones. Stages submitted without a predecessor won't be checked.
- The stages are graded as a whole. Either all points for the stage are given or nothing is given (in case of unfinished or malfunctioning stage). There is an exception: the last stage started by the student (only one) can be graded by the teacher with some partial points.
- The programming assignment must be presented on a lab workstation. Presentation on other computers is not accepted.
- Graded aspects include proper tools usage, correct implementation of required functionality and code style.
- To get the points for the programming assignment the student must: (1) return the paper with the assingment on which the points are written to the teacher and (2) copy the solution to the directory as explained below. Otherwise 0 points will be given.
- Laboratory L5 are available for students who want to retake one of the graded labs (L1/L2/L3/L4).
 	- The student can choose a single topic to retake. The retake will be a programming assignment and the introductory test in that topic.
	- Either both parts (programming+test) or a single part can be retaken.
	- It is not possible to retake more than one topic nor to retake different topic of the test and different topic of the programming assignment.
	- The retakes are done in two weeks (denoted as L5 in the schedule). During each weeks two topics will be retaken. The assignments of the topics to the dates will be given after retake sign-up is closed.
	- The result from the retake **replaces** the result from the lab even if it's lower then the earlier result.
	- Not attending any part of the retake results in leaving the old results for the given part.
	- The retake sign-up wil be on moodle platform. The sign-up will be open just after all groups finished L4 assignment and will by open for 26 hours **only**. To be allowed to retake one must sign-up during that period.

## Task solution formalities

  - The task must be coded in *C* (not C++)
  - The solution of each stage must be submitted as a separate archive named `$USER` in .tar.gz, .tar.xz or .tar.bz2 format. After typical extraction (flags -xzf, -xJf or -xjf) the archive must be unpacked to the directory named `$USER` where $USER is your login.
  - The archive must contain source files (.c, .h) and a **Makefile**
  - One should be able to compile the whole solution by issuing a single `make` command
  - The `-Wall` flag must be *obligatorily* turned on during gcc compilation as well as any other flags specified in the assignment
  - The archive must be copied to the network share location given in the task.
