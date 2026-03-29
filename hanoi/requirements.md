Programming and Data Structures
Laboratory instruction
March 6, 2026
General rules
When writing programs follow these rules:
 Clearly divide program into functions responsible for particular tasks.
 Use proper naming:
{ names of functions must be given reasonably to re
ect their responsibility (what they do),
{ names of variables must be given reasonably to re
ect their meaning (what they are),
{ there is nothing wrong in using longer names of functions and variables composed from a
few words,
{ choose naming convention you like and use it consequently in your programs,
{ do not mix dierent languages, e.g. Polish and English.
 Keep functions short.
 Do not duplicate code.
 Try to avoid comments. Instead use self-explaining names for functions and variables. Put a
comment only in case where the name itself is not sucient to explain well what the function
does or what the variable means. When describing variables and constants use substantival
form, e.g.: ,,angle of rotation in radians" instead of ,,variable used to represent angle of rotation
in radians".
 Using ,,magic numbers" is not allowed. All constants used in the program must be dened using
the#define preprocessor directive.
 Programs must be compiled using =g, =Wall and =pedant i c options e.g.:
gcc =g =Wall =pedant i c myprog . c =o myprog
1
Tasks
1 Rotating polygon 2
2 Tower of Hanoi 2
3 Tetris 3
4 Text processing 4
5 strtol function 5
6 Banking system 5
7 bsearch function 7
1 Rotating polygon
Time for writing program: 1 week.
Task description
Develop program that displays rotating polygon. Use functions from the SGL library (Simple Graphics
Library1). Initial values of the following variables must be dened using the#define directive:
 number of vertices,
 angle of rotation between consecutive frames.
Guidelines
 The polygon should rotate smoothly, without glitching and grow or shrink in each frame.
 Make sure that variable which represents angle of rotation never over
ows. To achieve this you
can keep the variable in the range of h0; 2i.
 Clear screen before drawing every new frame.
 The example of the program is available on the web-page for the PDSC lecture2.
 If your program runs into innite loop during development enter the following command and
click on the window of your program to close it:
x k i l l
2 Tower of Hanoi
Time for writing program: 2 weeks.
Task description
The aim of the exercise is to write a well-known Tower of Hanoi3 game. The array that stores images
of stacks must be equal to number of discs and pegs. Do not use bigger array than required.
1http://neo.dmcs.p.lodz.pl/pdsc/graphmanual.html
2http://neo.dmcs.p.lodz.pl/pdsc/rotpoly.html
3http://en.wikipedia.org/wiki/Tower_of_Hanoi
2
Guidelines
 Enable UndenedBehaviorSanitizer to check boundary errors. To turn on UndenedBehavior-
Sanitizer in projects that use the SGL library modify the makele as follows:
{ replace ,,gcc" with ,,gcc =f s a n i t i z e=unde f ined" (change required in two places).
 The moving discs must be animated.
 All parameters should be parametrized, e.g. when you change the number of discs or pegs all
dimensions must be recalculated to obtain nice view on the screen. Incorrect dimensions of discs
are not allowed.
 The game should allow displaying up to 10 pegs and large number of discs.
 Divide the program into intuitive functions.
 Use correct functions for reading key for pieces movement, the game must be smart, jumping
and freezing of discs are not allowed when you move up, left, right or down.
 Do not allocate memory, use static arrays.
 Use the following keys to control the game: 1, 2 ,..., 0, escape, enter.
 Display nice ,,Congratulation" message when the game is nished and wait for escape or enter
key to quit.
 The example of the game is available on the web-page for the PDSC lecture4.
3 Tetris
Time for writing program: 2 weeks.
Task description
The aim of the exercise is to write a well-known Tetris5 game. We supply you a denition of 4x4
pieces. Please use a rectangular matrix as a eld for the game, set its initial size to 10x20. Do not
use in your program bigger arrays than required.
Guidelines
 Enable UndenedBehaviorSanitizer to check boundary errors.
 All parameters should be parametrized, e.g. when you change the game eld array to 20x30.
 Divide the program into intuitive functions.
 The game must be responsive in the time between falling of the pieces. It should be possible to
perform a move at any time and the result should be visible immediately.
 Use correct functions for reading key for pieces movement, the game must be smart, jumping
and freezing of pieces are not allowed when you move left, right or rotate the piece.
 Do not allocate memory, use static arrays.
 Use the following keys to control the game: left arrow, right arrow, down arrow (falling down),
space (rotation), enter and escape.
4http://neo.dmcs.p.lodz.pl/pdsc/hanoi.html
5http://en.wikipedia.org/wiki/Tetris
3
 Display nice ,,Game over" message when the game is nished and wait for escape or enter key
to quit.
 The example of the game and pieces denition are available on the web-page for the PDSC
lecture6 7.
4 Text processing
Time for writing program: 2 weeks.
Task description
Write the program which processes text read from the standard input stream and writes the processed
text to the standard output stream. The input text contains several lines, some of them can be empty.
Each non empty line contains a positive octal number expressed as a sequence of characters from `0'
to `7', with possible leading and/or trailing whitespace characters. The processing computes sum of
all numbers and prints it. After that the numbers are printed without whitespace characters. Neither
maximum line length nor maximum number of lines is dened, so xed-size buers are not acceptable.
If the format of the input is incorrect, e.g. it contains any other characters than whitespace or
digits, print an error message.
For example, for the following input text:
1070104611
301201501012700
0000777777006
the expected output text is:
Sum:
301203571116517
Input numbers:
1070104611
301201501012700
0000777777006
Guidelines
 Maximum line length is not dened, so you will need to use dynamic memory allocation. Dene
the following function:
char* ge tLine ( )
which reads entire line from the standard input stream character by character using the ge t char
function and stores it in a dynamically allocated array. Expand the array as needed. Return
the pointer to the beginning of the line or NULL, if the end-of-le has been encountered before
any character has been read. Beware of the nal line not ending with the 'nn' character.
 Do not read the text from a le, use only the standard input.
6http://neo.dmcs.p.lodz.pl/pdsc/tetris.html
7http://neo.dmcs.p.lodz.pl/pdsc/pieces.inl
4
 You can still test your program using a text le. To redirect the standard input stream invoke
your program in the following way:
. / program name < input f i l ename
 The addition algorithm must operate on characters and strings and can not convert input num-
bers to integers.
 Add only two input numbers in a single step of the addition algorithm.
 Make sure your program works correctly when the input text is empty.
 Handle all possible memory allocation errors. Never exit your program without freeing all the
previously allocated memory.
 Use the library for random injection of memory allocation errors available on the web-page for
the PDSC lecture8. The library overrides the mal loc, c a l l o c and r e a l l o c functions from
the standard library. Test your program against dierent rates of random error injection.
 Use valgrind to verify there are no memory leaks.
5 strtol function
Time for writing program: 1 week.
Task description
Implement the s t r t o l function which is available in the standard library9. Declaration of the function
is as follows:
long int s t r t o l ( const char* nptr , char** endptr , int base )
The function converts the initial part of the string in npt r to a long integer value according to the
given base, which must be between 2 and 36 inclusive, or be the special value 0.
Guidelines
 Correctly check if the converted number is not over
owed.
 The function must process correctly negative numbers in each step of the conversion. It is not
allowed to process modulo number and modify its sign in the last step of the conversion.
 Carefully check if the program does not read chars outside the table pointed by npt r .
 Use the test program provided on the web-page for the PDSC lecture10.
6 Banking system
Time for writing program: 2 weeks.
8http://neo.dmcs.p.lodz.pl/pdsc/rand_malloc.tgz
9https://en.cppreference.com/w/c/string/byte/strtol
10http://neo.dmcs.p.lodz.pl/pdsc/strtol.tgz
5
Task description
The aim of the exercise is to write a program that simulates a customer bank account management
system. The system should allow to carry out basic operations on your virtual bank accounts. The
system should allow to:
 create a new account with the following elds describing the customer:
{ account number (generated by the program),
{ name,
{ surname,
{ address,
{ identication number (PESEL),
{ current balance,
{ car insurance (or insurances) for the account,
 list all accounts present in the database, including insurances,
 search for an account using one of the following elds: account number, name, surname, address,
identication number,
 make a deposit,
 make a withdrawal,
 make a money transfer between two accounts,
 take out insurance for a car:
{ car registration number and insurance price is entered when creating the insurance.
Grading
 If the program does not have the insurance functionality, the maximum grade is 60%.
 If the program has the functionality of taking one insurance per customer, the maximum grade
is 90%.
 Full grade can be obtained if the program has the functionality of taking unlimited number of
insurances per customer.
Guidelines
 All data should be read from and displayed on the terminal.
 The records must be read from the le and updated in the le, caching the records in memory
is not allowed. Update must take place after every modifying operation.
 The program must accept unlimited number of accounts.
 Always verify if a correct string of data was read from the terminal before you process the data.
 Limit numbers and strings to avoid over
ow and memory errors (boundary errors).
 Check if current operation is allowed.
 Ask for conrmation before every modifying operation on the bank account. Asking for conr-
mation of every operation is not accepted.
6
 Do not use account identiers longer than a few characters if typing them would be necessary
to perform operations.
 You can use another le or les to store the information about insurances.
7 bsearch function
Time for writing program: 1 week.
Task description
Implement the bs ear ch function - binary search of a sorted array - which is available in the standard
library11. Declaration of the function is as follows:
void* bs ear ch ( const void* key , const void* base ,
s i z e t num, s i z e t s i z e ,
int (* compar ) ( const void* , const void * ) ) ;
The function searches an array of num objects, the initial member of which is pointed to by base,
for a member that matches the object pointed to by key. The size of each member of the array is
specied by s i z e . The contents of the array should be in ascending sorted order according to the
comparison function referenced by compar. The compar routine is expected to have two arguments
which point to the key object and to an array member, in that order, and should return an integer
less than, equal to, or greater than zero if the key object is found, respectively, to be less than, to
match, or be greater than the array member. The bs ear ch function returns a pointer to a matching
member of the array, or NULL if no match is found. If there are multiple elements that match the
key, the element returned is unspecied.
11https://en.cppreference.com/w/c/algorithm/bsearch
7