# Senior-Project-2021

This program was made for my senior project to complete my bachelor’s degree, and was created in the time span of about 3 weeks. It’s a cryptographic hash function based on the stream cipher RC4, and although I didn’t have much time in the project for testing, it seems to work very well. It functions using three main steps:

1. Initializing a 32x32 array I call the RNG Matrix
2. Swapping different values of the RNG Matrix
3. Condensing the information in the RNG Matrix into a 128 bit digest

Here is a short explanation for how each of the steps work. For a much more in depth explanation, please see the paper I wrote about this project. (Senior Project Report – Monica Ingham.docx)

## Part 1: RNG Matrix Initialization

This part loops through each character of the input, and fills the matrix with pseudorandom numbers based on the character and its position in the input. At every location, there is a bitwise XOR operation calculated with the existing value and the value created from the current character to create the value inputted into the matrix. Once finished, it creates a 32x32 matrix of pseudorandom numbers.

## Part 2: RNG Matrix Value Swapping

The second part is very similar to the KSA from RC4, and is just as simple. All it does is loop through every location, calculate another location, and swap the two values. This helps to iron out any patterns in the RNG Matrix left over from the first step.

## Part 3: Digest Creation

This last step condenses all of the information in the RNG Matrix into a 128 bit message digest (32 digit hexadecimal number), which is used as the output of the algorithm. It first splits up the 32x32 matrix into 16 8x8 matrices, and calculates these individually to create two digits of the digest each. With each 8x8 chunk, it does a bitwise XOR operation on each row, and adds up each of these values from all the rows to create a pseudorandom number, which is then converted to hexadecimal and appended to the digest.


Even though this was created in three weeks, and is by far my biggest and most ambitious project, I still felt cramped for time and there is much more I wanted to do with different tests in the given time frame.
