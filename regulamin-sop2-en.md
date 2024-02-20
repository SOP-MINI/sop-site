# Purpose of course

The first goal of the  *Operating Systems 2* course is to familiarize students with advanced problems of contemporary operating systems design and implementation. The second goal is to develop student knowledge and skills necessary to design and implement (in POSIX/Unix/Linux environment) multi-process/multi-threaded applications with the use of various inter-process communication means, synchronization techniques and network communication.

# Conducting classes

The subject consists of a lecture and a laboratory.

  - The course consists of 30 lecture hours in 2-hour blocks. Lectures take place according to the schedule provided on the course website.
    - The lecture materials are available on the course website.
    - Two lecture tests are scheduled per semester.
    - Each student has the right to retake each test once. The scope of material is the same as during primary term. Student must notify lecturer about own plans to retake the test(s) at least 24 hours before scheduled date. The grade from each retake test will replace the original one - if improved.
    - Total number of points from lecture tests is 100.
  - The laboratory consists of four classes denoted as L1, L2, L3, L4. The laboratory classes take place according to the schedule provided on the course website. The course website also includes preparation materials for classes.
  - Classes assess abilities in the topics given in the laboratory schedule. Students should prepare on their own for each class, using the materials on the course website and lecture materials. Each of the classes consists of three parts:
    - entry test (8 minutes): solving the LeOn test
    - programming task (120 minutes)
  - At the end of the semester, there is a retake where students can improve their grades in exactly one topic (L1, L2, L3, or L4). Student can retake either one part (test, program) or both. Registration for the retakes will start immediately after the last laboratory classes in the semester and will be open for **24 hours** only.
  - In the exam session, student may retake lecture tests as well. One may retake one of or both tests, one after another.


# Course materials

 - Materials provided during the lecture (e.g. slides, code snippets)
 - Literature given at the lecture and available on the subject's website
 - Descriptions of laboratory exercises on the subject page
 - [The GNU C library manual](http://www.gnu.org/software/libc/manual/)
 - [The Single UNIX specification Version 3](http://www.unix.org/version3/online.html) (includes IEEE Std 1003.1 and ISO / IEC 9945)
 - [POSIX FAQ](http://www.opengroup.org/austin/papers/posix_faq.html)


# Class attendance

  - Attendance at the lectures is not obligatory but recommended due to the close relation between the discussed material and the laboratory.
  - Attendance at the letures with tests is obligatory. In case of absence, the student receives 0 points for the given lecture test.
  - Attendance at the laboratory classes is obligatory. In case of absence, the student receives 0 points for activities carried during the classes in which he was absent.
  - Absence from two of the assessed laboratory classes L1/L2/L3/L4 results in failure to complete the course.

# Verification of achievement of learning outcomes

The learning outcomes are verified during the lecture and laboratory. 

## Lecture

There are two lecture tests during the semester.

  - Lecture tests have written, paper form and consist of several open and closed questions regarding topics discussed during the lectures up to given test.
  - Questions may also relate to basic topics discussed during preceding course - Operating Systems 1
  - For both tests one may obtain 100 pts in total. Scoring for each task is given in the task description.

## Laboratory

### Introductory test
  - Test verifying the lecture information and the preparation materials from the website. Each entry test includes material from the all preceding lectures and the all preceding laboratories as well as the current laboratory topic.
  - Multiple-choice questions (any number of answers greater than 0 can be correct), solved on the LeOn platform.
  - The entrance test consists of 4 questions, for each question student can get a score from the range [0, 1] points. In total, for the entire test, a maximum is 4 points
  - The entry test is scored as follows. In each multiple-choice question there are *n* correct answers and *m* incorrect answers (each answer is either correct or incorrect). There is 1/n points for each correct answer chosen, and for each incorrect answer chosen there are -1/m points. For each question, the sum of points resulting from the marked incorrect and correct answers is awarded, as long as the sum obtained is not negative. In this case, 0 points are awarded.
  - Time limit for the test is 8 minutes.

### Programming task

   - 0 to 21 points to get for each topic
   - Allotted time: 120 minutes
   - Laboratory tasks are solved using the Arch Linux system on the computers in the laboratory. It is not allowed to show solutions on student's own computer.
   - A programming task is divided into stages. Amount of points for each stage in provided within the task description. The stages must be performed in the prescribed order. It is not allowed to proceed to the next stage without completing the predecessors.
   - Each stage is checked and graded during the class. Use of proper tools and POSIX API, correct implementation and functional correctness are graded. Code style is not (except Lab3).
   - After finishing each stage students send solution to the server using git version control system. Submissions must obey following rules:
    - The task must be coded in *C* (not C++).
    - Submission may be sent only once one minute.
    - It is allowed only to modify solution files.
    - Solution files must be formatted according to clang-format configuration provided with task. Clang-format program is installed on computers in the laboratory and its usage will be explained during L0.
    - Solution program must compile using makefile from task repository.
    If solution does not follow one of the rules it will be automatically rejected and student get information about problem. Sending solution to the server is necessary for grading - solution not sent to server will be not graded. 

# Aids acceptable for use in verifying achievement of learning outcomes

  - During lecture tests one **cannot** use any aids besides one's own knowledge. Communication with other students is forbidden.
  - During introductory test it is **not allowed** to use any aids.
  - During graded coding tasks one **can** use man pages and any code from tutorials, own code repository, online and book examples and materials as long as the solution remains individual. In particular, it is not allowed to use AI-based solutions like chat GPT, Github Copilot and similar ones.

# The rules for passing the course and for calculating the final grade

 - To pass the course, it is necessary to obtain a total of at least 100 points (out of 200 possible), and to attend the number of classes required in the Attendance section. At least 50 points from the lecture part and 50 points from the laboratory part is required
 - The points from the retake always replace the result scored previously
 - **The final course grade** is based on total points acquired by student during the labs and lectures:
   - Grade 5.0 - total in range [180, 200]
   - Grade 4.5 - total in range [160, 180)
   - Grade 4.0 - total in range [140, 160)
   - Grade 3.5 - total in range [120, 140)
   - Grade 3.0 - total in range [100, 120)
   - Grade 2.0 - total in range [0, 100)

# Deadline and procedure for announcing grades

  - Partial grades
    - Number of points obtained for lecture tests is published in USOS at most 2 weeks after given test
    - The number of points obtained for the subsequent stages of the laboratory task is given at the time of checking by the teacher and written on a sheet with the content of the task
    - The number of points obtained for entry is displayed in moodle after the test is completed
    - Points from both parts of the laboratory are placed in USOS no later than 24 hours after the class. 
    - The number of points in USOS determines the final grade
  - Final grade
    - Issued in USOS within 24 hours from the publication of the results of the last L4 laboratory or second lecture test in USOS (depending on which happens last)
    - Corrected during the exam session within 24 hours from the publication of the results of the retakes in USOS


# Rules of repetition of classes due to failure of a subject

A person who did not pass the subject must repeat the entire subject. It is not possible to transfer partial points from previous semesters.

# Others

