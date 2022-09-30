# Purpose of course

The first goal of the *Operating Systems I* course is to familiarize students with basics of operating system operation and design. The second goal is to develop student skills in correct application of system functions to design and implementation of simple multi-process/multi-thread applications which communicate using standard input/output channels and files.

# Conducting classes

The subject consists of a lecture and a laboratory.

- The lectures introduce students to the course matter, in particular, to the topic covered by the laboratory.
   - The course consists of 15 lecture hours in 2-hour blocks. The last lecture is 1 hour long. Lectures take place according to the schedule provided on the course website.
   - The lecture materials are available on the course website.
   - There are no lecture tests. The lecture content will, however, appear in the introductory tests during the laboratory.

- The laboratory consists of five classes denoted as Lab0, Lab1, Lab2, Lab3, Lab4. The laboratory classes take place according to the schedule provided on the course website. The course website also includes preparation materials for classes.

  - L0 class is devoted to the preparation of the environment. During this class, students have to recall the skills of the Unix Fundamentals course and to configure the development environment. Any deficiencies in the configuration or skills (e.g., lack of intellisense, underlining errors in the editor or poor ability to navigate through directories) in all subsequent classes will not be an excuse for poor grades and cannot be used as an excuse for less strict grading.

  - Lab1, Lab2, Lab3 and Lab4 classes assess abilities in the topics given in the laboratory schedule. Students should prepare on their own for each class, using the materials on the course website and lecture materials. Each of the classes consists of three parts:

    - questions and answers (approx. 30 minutes): during this part, the teacher answers students' questions.
    - entry (8 minutes): solving the moodle test
    - programming task (90 minutes)
- In the exam session, there is a retake where you can improve your grades in exactly one topic (Lab1, Lab2, Lab3, or Lab4). You can retake either one part (test, program) or both. Registration for the retakes will start immediately after the last laboratory classes in the semester and will be open for **24 hours** only.


# Course materials

 - Materials provided during the lecture (e.g. slides, code snippets)
 - Literature given at the lecture and available on the subject's website
 - Descriptions of laboratory exercises on the subject page
 - [The GNU C library manual](http://www.gnu.org/software/libc/manual/)
 - [The Single UNIX specification Version 3](http://www.unix.org/version3/online.html) (includes IEEE Std 1003.1 and ISO / IEC 9945)
 - [POSIX FAQ](http://www.opengroup.org/austin/papers/posix_faq.html)


# Class attendance

- Attendance at the lectures is not obligatory but recommended due to the close connection between the discussed material and the laboratory.
- Attendance at the laboratory classes is obligatory. In case of absence, the student receives 0 points for activities carried during the classes in which he was absent.
- Absence from two of the assessed laboratory classes Lab1/Lab2/Lab3/Lab4 results in failure to complete the course.

# Verification of achievement of learning outcomes

The learning outcomes are verified during the laboratory. There are four laboratory classes during the semester, covering 4 topics, denoted as Lab1, Lab2, Lab3 and Lab4 on the course website.

Methods used for effects verification during the laboratory:

- Introductory test
  - Test verifying the lecture information and the preparation materials from the website. Each entry test includes material from the all preceding lectures and the all preceding laboratories as well as the current laboratory topic.
  - Multiple-choice questions (any number of answers greater than 0 can be correct), solved on the moodle platform.
  - The entrance test consists of 4 questions, for each question you can get a score from the range [0, 2] points. In total, for the entire test, a maximum is 8 points.
  - The entry test is scored as follows. In each multiple-choice question there are *n* correct answers and *m* incorrect answers (each answer is either correct or incorrect). There is 2/n points for each correct answer chosen, and for each incorrect answer chosen there are -2/m points. For each question, the sum of points resulting from the marked incorrect and correct answers is awarded, as long as the sum obtained is not negative. In this case, 0 points are awarded.
  - Time limit for the test is 8 minutes.

- Programming task

     - 0 to 17 points to get for each topic
     - Allotted time:  90 minutes
     - A programming task is divided into stages. Amount of points for each stage in provided within the task description. The stages must be performed in the prescribed order. It is not allowed to proceed to the next stage without completing the predecessors.
     - Each stage is checked and graded during the class. Use of proper tools and POSIX API, correct implementation and functional correctness are graded. Code style is not (except Lab3).
     - At the end of the class, the code version containing all the steps to be assessed must be copied to the network share (more information at the end of the page). Lack of implementation of any of the functionalities in the code sent means that the points for a given stage are reset to 0.
	 - The uploaded solution of **Lab3** will be additionally graded in terms of code quality and aesthetics. This will be checked after the lab. For the problems you will receive up to 10 (but no more than number of points from the lab) penalty points that will be subtracted from the base result of Lab3.
	 - Powtórka?? The solution of the Lab3 task will also be assessed in terms of code aesthetics. The assessment will be made after the class. Penalty points are awarded for problems in the code, which are subtracted from the points obtained during the classes. You can lose up to 10 points, but not more than the number of points obtained during the classes.
     - Laboratory tasks are solved using the Arch Linux system on the computers in the laboratory. It is not allowed to show solutions on your own computer.
	
- ## Lab task solution formalities

  - The task must be coded in *C* (not C++)
  - The solution of must be submitted as a archive in `.tar.gz`, `.tar.xz` or `.tar.bz2` format. After typical extraction (flags `-xzf`, `-xJf` or `-xjf`) the archive must be unpacked to the directory named `$USER` where `$USER` is your login.
  - The archive must contain source files (`.c`, `.h`) and a `Makefile`
  - One should be able to compile the whole solution by issuing a single `make` command
  - The `-Wall` flag and the other compiler flags required in the task must be **obligatorily** turned on during `gcc` compilation
  - The archive must be copied to the network share location given in the task.

# Aids acceptable for use in verifying achievement of learning outcomes

- The coding task is divided into several stages. Points for each stage are defined by the task (summing up to 17p.). Stages have to be solved in order. One **can not** progress to a next stage before completing previous ones.
- During graded coding tasks one **can** use man pages and any code from tutorials, own code repository, online and book examples and materials as long as the solution remains individual.

# The rules for passing the course and for calculating the final grade

 - To pass the course, it is necessary to obtain a total of at least 50 points (out of 100 possible), and to attend the number of classes required in the Attendance section.
 - The points from the retake always replace the result scored during the lab
 - **The final course grade** is based on total points acquired by student during the labs (100p tops):

| Points in total | Grade |
| --------------- | ----- |
| s ≥ 90          | 5     |
| 80 ≤ s \< 90    | 4.5   |
| 70 ≤ s \< 80    | 4     |
| 60 ≤ s \< 70    | 3.5   |
| 50 ≤ s \< 60    | 3     |
| s \< 50         | 2     |


# Deadline and procedure for announcing grades

- Partial grades

  - The number of points obtained for the subsequent stages of the laboratory task is given at the time of checking by the teacher and written on a sheet with the content of the task
  - The number of points obtained for entry is displayed in moodle after the test is completed
  - Points from both parts of the laboratory are placed in USOS no later than 24 hours after the class. The number of points in USOS determines the final grade
  
- Final grade

  - Issued in USOS within 24 hours from the publication of the results of the last L4 laboratory in USOS
  - Corrected during the exam session within 24 hours from the publication of the results of the retakes in USOS


# Rules of repetition of classes due to failure of a subject

A person who did not pass the subject must repeat the entire subject. It is not possible to transfer partial points from previous semesters.

# Others

