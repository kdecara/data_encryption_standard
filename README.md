# data_encryption_standard
to use the program, compile with make and use the following program arguments:
./des [infile.txt] [outfile.txt] [64 bit key] ['e' for encryption 'd' for decryption]

# Intro to DES
The data encryption standard is a block cipher encryption algorithm developed by NIST and the NSA. while no longer used due to its small key size (56 bits)
variants of DES such as 3DES (using DES 3 times, with 3 seperate main keys) are still used, though computationally intensive. That said, it's implementation can still serve
to learn about cryptography, which is the goal of this project.

# Overview
DES utilizes a 56 bit key (while it takes a 64 bit key as input, the key is then mapped to 56 bits).
DES is a symmetric cipher, so the same key is used for encryption and decryption. Encryption of one block is handled in a feistel network, which consists of 
16 rounds, each round utilizing a different subkey k_i, derived from the originial key k. in each round the f() function takes the round key and right side of the block
as input. The result of f() is then used as an XOR mask for encrypting the left side of the block. The halves are then swapped, and the process continues with
L(i) = R(i-1)
R(i) = L(i-1)
Note that because the one side is used an XOR mask for encrypting the other side, no data is lost and decyrption becomes the same function as encryption.

# Internal Structure of DES
as mendtioned above, the feistel network consists of 16 rounds in which thr following occurs:
1. IP and inv_IP:
the input block B (64 bits) first undergoes an initial bitwise permuation IP (and inv_IP during decryption). Today these bitwise permutations do not increase the security of DES, but I've
included them as they are part of the standard. 
2. The block is split into left and right, each 32 bits.The block is then put through the feistel network, which consits of 16 rounds. In each round, the result of the f() function is used as an XOR mask for encrypting the
right half of the block. at the end of each round, the two halves are swapped so L(i) = R(i-1) and R(i) = L(i-1).
3. After the feistel network is done, one final swap occurs
4. the left and right halves are merged and either the encrypted or decrypted block is produced (in DES encryption and decryption are virtually the same function).

# Internal Structure of f() function
The f() function takes a 32 bit input (half of a block, either the left or right side) and 48 bit round key, k(i) as parameters, which then goes through the following steps:
1. the 32 bit input, either left or right, is expanded to 48 bits via the e() function
2. the result of the e() function is then XOR'd with the 48 bit round key k(i)
3. The result is split into 8 blocks of 6 bits each, which are fed into 8 different S-Boxes, which map a 6 bit input to a 4 bit output
4. the blocks are merged back into 32 bits and are placed into the permutation p() function, in which the block size does not change

# Internal Strucuture of Key Scheduling
The orginial 64 bit input key is mapped into 56 bits. This 56 bit key is the actual main key, k, so DES is actually a 56 bit key. The 56 bit key is split into halves
c (left) and d(right) each consisting of 28 bits. Depending on the round of the feistel network, these halves are rotated by 1 or 2 bits (left for encryption, right for
decryption). Then, the halves are joined again, and are put through the PC_2 permutation to create the round key k(i).

# Room for Futute Improvement 
Many design decisions were made early on which I believe hurt this implementation. First of all, there was no need to make a Block data type, when an unsigned long long could
have been used to store the data in either encryption or decryption. Second, the main function des.c is fairly cluttered in the feistel() function, declaring
a key struct in which much of the key initialization (56 bit mapping, half creation, etc.) is handled in one function call would help with this.
Many functions could be merged to save space and reduce clutter. For example, two different functions for both setting and getting the nth bit of a number are implemented
in key.h and byte_pack.h (and their equivalent .c files). Both of these could be merged.
