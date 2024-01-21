# Purpose of course

The first goal of the *Operating Systems 1* course is to familiarize students with basics of operating system operation and design. The second goal is to develop student skills in correct application of system functions to design and implementation of simple multiprocess/multi-thread applications which communicate using standard input/output channels and files.

# Conducting classes

The subject consists of a lecture and a laboratory.

  - The lectures introduce students to the course matter, in particular, to the topic covered by the laboratory.
    - The course consists of 15 lecture hours in 2-hour blocks. The last lecture is 1 hour long. Lectures take place according to the schedule provided on the course website.
    - The lecture materials are available on the course website.
    - There are no lecture tests. The lecture content will, however, appear in the introductory tests during the laboratory.
  - The laboratory consists of five classes denoted as L0, L1, L2, L3, L4. The laboratory classes take place according to the schedule provided on the course website. The course website also includes preparation materials for classes.
  - L0 class is devoted to the preparation of the environment. During this class, students have to recall the skills of the Unix Fundamentals course and to configure the development environment. Any deficiencies in the configuration or skills (e.g., lack of intellisense, underlining errors in the editor or poor ability to navigate through directories) in all subsequent classes will not be an excuse for poor grades and cannot be used as an excuse for less strict grading. Moreover, during L0 students will be familiarized with solution submission system used during following classes.
  - L1, L2, L3 and L4 classes assess abilities in the topics given in the laboratory schedule. Students should prepare on their own for each class, using the materials on the course website and lecture materials. Each of the classes consists of three parts:
    - entry test (8 minutes): solving the LeOn test
    - programming task (120 minutes)
  - At the end of the semester, there is a retake where students can improve their grades in exactly one topic (L1, L2, L3, or L4). Student can retake either one part (test, program) or both. Registration for the retakes will start immediately after the last laboratory classes in the semester and will be open for **24 hours** only.


# Course materials

  - Materials provided during the lecture (e.g. slides, code snippets)
  - Literature given at the lecture and available on the subject's website
  - Descriptions of laboratory exercises on the subject page
  - [The GNU C library manual](http://www.gnu.org/software/libc/manual/)
  - [The Single UNIX specification Version 3](http://www.unix.org/version3/online.html) (includes IEEE Std 1003.1 and ISO / IEC 9945)
  - [POSIX FAQ](http://www.opengroup.org/austin/papers/posix_faq.html)


# Class attendance

  - Attendance at the lectures is not obligatory but recommended due to the close relation between the discussed material and the laboratory.
  - Attendance at the laboratory classes is obligatory. In case of absence, the student receives 0 points for activities carried during the classes in which he was absent.
  - Absence from two of the assessed laboratory classes L1/L2/L3/L4 results in failure to complete the course.

# Verification of achievement of learning outcomes

The learning outcomes are verified during the laboratory. There are five laboratory classes during the semester, denoted as L0, L1, L2, L3 and L4 on the course website.

## L0

During L0 class students solve simple programming task for which they can score up to 4 points.

## L1-L4

### Introductory test
  - Test verifying the lecture information and the preparation materials from the website. Each entry test includes material from the all preceding lectures and the all preceding laboratories as well as the current laboratory topic.
  - Multiple-choice questions (any number of answers greater than 0 can be correct), solved on the LeOn platform.
  - The entrance test consists of 4 questions, for each question student can get a score from the range [0, 1] points. In total, for the entire test, a maximum is 4 points
  - The entry test is scored as follows. In each multiple-choice question there are *n* correct answers and *m* incorrect answers (each answer is either correct or incorrect). There is 1/n points for each correct answer chosen, and for each incorrect answer chosen there are -1/m points. For each question, the sum of points resulting from the marked incorrect and correct answers is awarded, as long as the sum obtained is not negative. In this case, 0 points are awarded.
  - Time limit for the test is 8 minutes.

### Programming task

   - 0 to 20 points to get for each topic
   - Allotted time: 120 minutes
   - Laboratory tasks are solved using the Arch Linux system on the computers in the laboratory. It is not allowed to show solutions on student's own computer.
   - A programming task is divided into stages. Amount of points for each stage in provided within the task description. The stages must be performed in the prescribed order. It is not allowed to proceed to the next stage without completing the predecessors.
   - Each stage is checked and graded during the class. Use of proper tools and POSIX API, correct implementation and functional correctness are graded. Code style is not (except Lab3).
   - After finishing each stage students send solution to the server using git version control system as described during L0. Submissions must obey following rules:
    - The task must be coded in *C* (not C++).
    - Submission may be sent only once one minute.
    - It is allowed only to modify solution files.
    - Solution files must be formatted according to clang-format configuration provided with task. Clang-format program is installed on computers in the laboratory and its usage will be explained during L0.
    - Solution program must compile using makefile from task repository.
    If solution does not follow one of the rules it will be automatically rejected and student get information about problem. Sending solution to the server is necessary for grading - solution not sent to server will be not graded. 

# Aids acceptable for use in verifying achievement of learning outcomes

  - During introductory test it is **not allowed** to use any aids.
  - During graded coding tasks one **can** use man pages and any code from tutorials, own code repository, online and book examples and materials as long as the solution remains individual. In particular, it is not allowed to use AI-based solutions like chat GPT, Github Copilot and similar ones.

# The rules for passing the course and for calculating the final grade

 - To pass the course, it is necessary to obtain a total of at least 50 points (out of 100 possible), and to attend the number of classes required in the Attendance section.
 - The points from the retake always replace the result scored during the lab
 - **The final course grade** is based on total points acquired by student during the labs (100p tops):
   - Grade 5.0 - total in range [90, 100]
   - Grade 4.5 - total in range [80, 90)
   - Grade 4.0 - total in range [70, 80)
   - Grade 3.5 - total in range [60, 70)
   - Grade 3.0 - total in range [50, 60)
   - Grade 2.0 - total in range [0, 50)


# Deadline and procedure for announcing grades

  - Partial grades
    - The number of points obtained for the subsequent stages of the laboratory task is given at the time of checking by the teacher and written on a sheet with the content of the task
    - The number of points obtained for entry is displayed in LeOn after the test is completed
    - Points from both parts of the laboratory are placed in USOS no later than 24 hours after the class. The number of points in USOS determines the final grade
  - Final grade
    - Issued in USOS within 24 hours from the publication of the results of the last L4 laboratory in USOS
    - Corrected during the exam session within 24 hours from the publication of the results of the retakes in USOS


# Rules of repetition of classes due to failure of a subject

A person who did not pass the subject must repeat the entire subject. It is not possible to transfer partial points from previous semesters.

# Others

