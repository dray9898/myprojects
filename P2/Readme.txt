The program is implemented in three separate files. The dictionary,c file in charge of 
making a struct called dictionary that contains an array of strings and an int which 
has the number of words in the dictionary. The dictionary file opens the text file that will be 
made into a dictionary using the ioutil file. It starts the array of strings with a length of 8 char pointers
Then it will read one word at a time of the dictionary text using ioutil.c and add the  all caps version of the word
and a version which has the first letter only capitalized. This occurs repeatedly till dictionary text file is
completely read. 
Spellcheck is the main function that is called  the user to spell check the directory.
The spellcheck program takes the directory to be spell checked and the dictionary to be read. It uses the 
dictionary.c code that makes the dictionary from the dictionary files path. It recursively traverses through directory to be checked.
It tries to reaches the inner most file in the duirectory tree. It then calls the parse file function that uses the ioutil.c file 
to open and read one word at a time and checks if it is in the dictionary and if it is not it reports the column number and line number of the word.
After checking the lowest text file it then checks the file one level above. 


The tests that are being used to see if the program is correct is that it checks if hypenated words are spellechecked. 
The file path "P2/src/tests/aesops/northwind copy.txt". This file has all the words hyphenated and checks if each are mispelled. 
The file path "/common/users/shared/cs214s24pa1dr101ezy1/P2/src/tests/aesops" is used to test if the file and directory traversal is done correct. 
There is another directory in this directory but also text files and this is why this test is a good test for proper directory traversal. 
The dictio0nary for this test is "/common/users/shared/cs214s24pa1dr101ezy1/P2/src/tests/perfect/northwind.txt:"

 The second test is testing Trailing punctuation and capitalization by using various trailing puncutation around different words in the dictionary 
 to check if that word is marked as correctly spelled. The word "Half" and "HALF" are in the parenthesis.txt but in the dictionary file it is added as "half"
 this tests the capitalization. 