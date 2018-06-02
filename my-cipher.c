#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void to_binary(int decimal_key, char binary_key[]);
void to_binary_msg(int decimal_key, char binary_key[]);
void bitXor(char x[], char y[]);
void split(char str[], char left[], char right[]);
void join(char left[], char right[], char * result);
void expand(char str[7], char diffused[9]);
void encrypt(char buffer[13], char binary_key[9]);
void f(char r_side[7], char binary_key[9]);
void s1(char s[], char cipher[]);
void s2(char s[], char cipher[]);
void flip_halves(char string[13]);
void overwrite(char original[], char next[]);
void overwritekey(char original[], char next[]);
void overwritemsg(char original[], char next[]);
int feed(char message[], char section[], int count);
int stream(char message[], char section[], int count);
void buffer(char message[], char bmsg[], int count);
void keyforward(char okey[], char nkey[]);
void keybackward(char okey[], char nkey[]);
void ecb(char key[], char * message, int rounds, int direction);
void eecb(char key[], char message[], char result[], int rounds);
void decb(char key[], char message[], char result[], int rounds);
void cbc(char key[], char * message, int rounds, int direction);
void ctr(char key[], char * message, int rounds, int direction);
void cstealing(char section[],char *message, int size, char last[]);
void cstealing2(char section[],char *message, int size, char last[]);

int main(int argc, char * argv[]) {
    int state = 0; //encryption 0, decryption 1
    FILE * file;
    //printf("GOAL ./my−cipher [−−ECB/−−CTR/−−CBC] [−−enc/−−dec] [int rounds] [decimal key]\n");
    int rounds = atoi(argv[3]);
    //======================//
    //Retrieve Key
    //======================//
    int decimal_key = atoi(argv[4]);

    //check for bad key
    if (!decimal_key || decimal_key > 512) {
      printf("BAD KEY! Syntax: ./my−cipher.o key [key <511]\n");
      exit(0);
    }
    //======================//
    //Get PLain Text
    //======================//
if (strcmp(argv[2], "--enc") == 0) {state = 0;file = fopen("plaintext.txt", "r");}
    else if (strcmp(argv[2], "--dec") == 0) {state = 1; file = fopen("decryptedtext.txt", "r");}
    else {
      printf("invalid choice\n");
      exit(0);
    }


    fseek(file, 0, SEEK_END); /* Go to end of file */
    int size = ftell(file);
    rewind(file);
    int i;

    char buffer[size];
    for (i = 0; i < size; i++) {
      fscanf(file, "%c", & buffer[i]);
    }
    buffer[size - 1] = '\0';
    fclose(file);
    //==========END==========//

    //binary key, initially 0
    //to convert to 9 bit change 8 to 9 >>>ALSO CHANGE THE OG FUNCTION<<<
    char binary_key[10];
    to_binary(decimal_key, binary_key);

    binary_key[9] = '\0';

    //pass decimal key and retrieve key in binary

    if (strcmp(argv[1], "--ECB") == 0) {
      ecb(binary_key, buffer, rounds, state);
    } else if (strcmp(argv[1], "--CBC") == 0) {
      cbc(binary_key, buffer, rounds, state);
    } else if (strcmp(argv[1], "--CTR") == 0) {
      ctr(binary_key, buffer, rounds, state);
    } else {
      printf("not a valid encryption\n");
      exit(0);
    }

    //==========END==========//

    // TAKE PLAIN TEXT
    // TAKE A key
    // Run SPN on it

    //Li = Ri and Ri = Li-1 XOR f(Ri-1, Ki)

    //f(Ri-1, Ki)
    //Expand Ri-1
    //    {A,B,C,D,E,F}
    //-> {A,B,D,C,D,C,E,F}

    //S-box
    //S-box takes 4 bit input and 3 bit output
    //first bit {0,1} will determinethe row
    //last 3 bits determine the column

    printf("\n");

  } //END MAIN

/*
 * Function:  to_binary
 * --------------------
 *  converts a passed decimal number below 511 to a binary number
 *
 *  returns: void
 */

void to_binary(int decimal_key, char binary_key[]) {
    int size = strlen(binary_key);
    //to convert to 9 bit change PARAMETER *binary_key declaration 9 to 10
    //>>>ALSO CHANGE THE OG DECLARATION<<<
    //parallel arrays with corresponding decimal to binary values
    int decimal_value[9] = {
      256,
      128,
      64,
      32,
      16,
      8,
      4,
      2,
      1
    };

    for (int i = 0; i < 9; i++) {
      //finds the first decimal which fits into the given number,
      //adds the corresponding binary value to *binary_key
      if (decimal_key / decimal_value[i] == 1) {
        binary_key[i] = '1';
        decimal_key -= decimal_value[i]; //refresh the original decimal value
      } else {
        binary_key[i] = '0';
      }
    }

  } //end
  /*
   * Function:  to_binary_msg
   * --------------------
   *  converts a passed decimal number below 12 numbers to a binary number
   *
   *  returns: void
   */

void to_binary_msg(int decimal_key, char binary_key[]) {
    int size = strlen(binary_key);
    //to convert to 9 bit change PARAMETER *binary_key declaration 9 to 10
    //>>>ALSO CHANGE THE OG DECLARATION<<<
    //parallel arrays with corresponding decimal to binary values
    int decimal_value[12] = {
      2048,
      1024,
      512,
      256,
      128,
      64,
      32,
      16,
      8,
      4,
      2,
      1
    };

    for (int i = 0; i < 12; i++) {
      //finds the first decimal which fits into the given number,
      //adds the corresponding binary value to *binary_key
      if (decimal_key / decimal_value[i] == 1) {
        binary_key[i] = '1';
        decimal_key -= decimal_value[i]; //refresh the original decimal value
      } else {
        binary_key[i] = '0';
      }
    }

  } //end
  /*
   * Function:  bitXor
   * --------------------
   *  reads in two strings and overwrites the second with the xored value
   *
   *  returns: void
   */
void bitXor(char x[], char y[]) {
    int i;
    for (i = 0; i < strlen(y); i++) {
      if(y[i]=='\0') break;
      if (x[i] == y[i]) y[i] = '0';
      else y[i] = '1';
    }
  }
  /*
   * Function:  split
   * --------------------
   *   splits one string into left and right. note to allocate right amount of memory before handing to this.
   *
   *  returns: void
   */
void split(char str[], char left[], char right[]) {
    int j = strlen(str) / 2;
    memcpy(left, str, j);
    memcpy(right, str + j, j);
  }
  /*
   * Function: join
   * --------------------
   *   joins two strings into one, it is expected both strings be the same size
   *
   *  returns: void
   */
void join(char left[], char right[], char * result) {
    size_t j = strlen(left);
    memcpy(result, left, j);
    memcpy(result + j, right, j);
  }
  /*
   * Function: permutation
   * --------------------
   *   gives initial and final permutation based off of int positions in array
   *   (this is a function needed for in DES but may not be used here)
   *  returns: void
   */
void permutation(char cipher[], char c[]) {
  //permutation algorithm e.g 10 bit of message becomes first bit, 6 the second

  int IP[4][3] = {
    {
      10,
      6,
      2
    },
    {
      12,
      8,
      4
    },
    {
      9,
      5,
      1
    },
    {
      11,
      7,
      3
    }
  };

  int i, j, x, counter = 0;
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 3; j++) {

      x = IP[i][j];
      c[counter] = cipher[x - 1];
      counter++;
    }
  }
}

/*
 * Function: f
 * --------------------
 *   hand 8 bit key and empty 8 bit array to cipher
 *   this is main part of encryption for DES, this function takes the right side
 *   side of the msg and splits it encrypts(diffuses) it then xors it with the key
 *   this value is then put through two code books to get final value
 *  returns: void
 */
void f(char r_side[7], char binary_key[9]) {
  binary_key[8] = '\0';
  //printf("\nEXEC f\n");

  //Expand Ri-1
  //    {A,B,C,D,E,F}
  //-> {A,B,D,C,D,C,E,F}
  char diffused[9];
  diffused[8] = '\0';
  expand(r_side, diffused);
 // printf(" right %s        diffused   |result: %s\n", r_side, diffused);

  //XOR Ri with Key

  bitXor(binary_key, diffused);
  //printf(" XOR key %s with diffused |result: %s\n",binary_key,diffused);

  //split Ri into two 4 bit strings

  char four_bit_left[5];
  char four_bit_right[5];
  four_bit_left[4] = '\0';
  four_bit_right[4] = '\0';
  split(diffused, four_bit_left, four_bit_right);

 // printf(" left %s right %s\n",four_bit_left,four_bit_right);

  //S-box
  //S-box takes 4 bit input and 3 bit output
  //first bit {0,1} will determinethe row
  //last 3 bits determine the column

  char left_sbox[4];
  left_sbox[3] = '\0';

  char right_sbox[4];
  right_sbox[3] = '\0';

  s1(four_bit_left, left_sbox);
  s2(four_bit_right, right_sbox);

  //printf(" left_sbox %s right_sbox %s\n",left_sbox,right_sbox);

  //joins the resulting s boxes
  join(left_sbox, right_sbox, r_side);

  //printf(" joined: r_side result: %s\n",r_side);
  //printf(" left_sbox %s right_sbox %s\n",left_sbox,right_sbox);
  //printf("END f\n");

}

/*
 * Function: s1
 * --------------------
 *   shrink 4 bits to a 3 bit based on s1 box(code book)
 *
 *  returns: void
 */
void s1(char s[], char cipher[]) {

  if (strcmp(s, "0000") == 0) strcpy(cipher, "101");
  if (strcmp(s, "0001") == 0) strcpy(cipher, "010");
  if (strcmp(s, "0010") == 0) strcpy(cipher, "001");
  if (strcmp(s, "0011") == 0) strcpy(cipher, "110");
  if (strcmp(s, "0100") == 0) strcpy(cipher, "011");
  if (strcmp(s, "0101") == 0) strcpy(cipher, "100");
  if (strcmp(s, "0110") == 0) strcpy(cipher, "111");
  if (strcmp(s, "0111") == 0) strcpy(cipher, "000");

  if (strcmp(s, "1000") == 0) strcpy(cipher, "001");
  if (strcmp(s, "1001") == 0) strcpy(cipher, "100");
  if (strcmp(s, "1010") == 0) strcpy(cipher, "110");
  if (strcmp(s, "1011") == 0) strcpy(cipher, "010");
  if (strcmp(s, "1100") == 0) strcpy(cipher, "000");
  if (strcmp(s, "1101") == 0) strcpy(cipher, "111");
  if (strcmp(s, "1110") == 0) strcpy(cipher, "101");
  if (strcmp(s, "1111") == 0) strcpy(cipher, "011");

}

/*
 * Function: s2
 * --------------------
 *   shrink 4 bits to a 3 bit based on s2 box(code book)
 *
 *  returns: void
 */
void s2(char s[], char cipher[]) {
    if (strcmp(s, "0000") == 0) strcpy(cipher, "100");
    if (strcmp(s, "0001") == 0) strcpy(cipher, "000");
    if (strcmp(s, "0010") == 0) strcpy(cipher, "110");
    if (strcmp(s, "0011") == 0) strcpy(cipher, "101");
    if (strcmp(s, "0100") == 0) strcpy(cipher, "111");
    if (strcmp(s, "0101") == 0) strcpy(cipher, "001");
    if (strcmp(s, "0110") == 0) strcpy(cipher, "011");
    if (strcmp(s, "0111") == 0) strcpy(cipher, "010");

    if (strcmp(s, "1000") == 0) strcpy(cipher, "101");
    if (strcmp(s, "1001") == 0) strcpy(cipher, "011");
    if (strcmp(s, "1010") == 0) strcpy(cipher, "000");
    if (strcmp(s, "1011") == 0) strcpy(cipher, "111");
    if (strcmp(s, "1100") == 0) strcpy(cipher, "110");
    if (strcmp(s, "1101") == 0) strcpy(cipher, "010");
    if (strcmp(s, "1110") == 0) strcpy(cipher, "001");
    if (strcmp(s, "1111") == 0) strcpy(cipher, "100");

  }
  /*
   * Function: expand
   * --------------------
   *  increase the size of a 6 bit string to 8 bit
   *  encrypted right is put into cipher array
   *  returns: void
   */

void expand(char str[7], char diffused[9]) {

    int diffusion[] = {
      0,
      1,
      3,
      2,
      3,
      2,
      4,
      5
    };
    int x, j;
    for (j = 0; j < 8; j++) {
      x = diffusion[j];
      diffused[j] = str[x];
    }
  }
  /*
   * Function: flip_halves
   * --------------------
   *  made specifically for buffer. takes buffer and flips the
   *  two equal sides 6 and 6
   *  returns: void
   */
void flip_halves(char string[13]) {

    char l_side[7];
    char r_side[7];
    l_side[6] = '\0';
    r_side[6] = '\0';
    //     result  left    right
    split(string, l_side, r_side);

    //   left   rigjt  result
    join(r_side, l_side, string);

  }
  /*
   * Function: encrypt
   * --------------------
   *  increase the size of a 6 bit string to 8 bit
   *  encrypted right is put into cipher array
   *  returns: void
   */
void encrypt(char buffer[13], char binary_key[9]) {

  //printf("\nbuffer: %s\n",buffer);

  //Li = Ri and Ri = Li-1 XOR f(Ri-1, Ki)
  buffer[12] = '\0';
  char l_side[7];
  char r_side[7];
  l_side[6] = '\0';
  r_side[6] = '\0';
  split(buffer, l_side, r_side);

  // make temp for XORing later
  char l_side_temp[7];
  l_side_temp[6] = '\0';
  overwrite(l_side, l_side_temp);

  //Li = Ri-1
  overwrite(r_side, l_side);

  //printf("temp(real) l_side: %s\n",l_side_temp);

  //printf("left(r) %s right %s buffer %s\n",l_side ,r_side, buffer);
  //f(Ri-1, Ki)
  f(r_side, binary_key);

  //printf("temp(real) l_side: %s\n",l_side_temp);
  //printf("left(r) %s right %s buffer %s\n",l_side ,r_side, buffer);
  //XORED l with r sides
  bitXor(l_side_temp, r_side);

  //printf("\nXORed w    left_temp %s right %s buffer %s\n",l_side_temp ,r_side, buffer);

  join(l_side, r_side, buffer);

  //printf("\nleft(r) %s right %s |       new: buffer %s\n",l_side ,r_side, buffer);

}

/*
 * Function: cbc
 * --------------------
 *  feed 12 bits of the message at the time
 *  encryption and decryption for cbc type
 *  returns: void
 */
void cbc(char key1[], char * message, int rounds, int direction) {
  int count = 0;
  char section[13];
  section[12] = '\0';
  //iv
  char result[13] = "111100001111";
  result[12] = '\0';
  int stop = (strlen(message)/12);
  char key[10];
  key[9] = '\0';
  overwritekey(key1, key);
  //rules for encryption
  if (direction == 0) {
    //take 12 bits from message and add too count to keep track of position
    while (feed(message, section, count) == 0 && count!=stop-1) {
      bitXor(result, section);
      eecb(key, section, result, rounds);
      overwritemsg(section, result);
      printf("%s", section);
      count++;
      overwritekey(key1, key);
    }
    //ciphertext-stealing
    bitXor(result, section);
      eecb(key, section, result, rounds);
      overwritekey(key1, key);
    int size = strlen(message)-((stop)*12);
    char last[12+size];
    last[12+size]='\0';
    cstealing(section,message, size, last);
    count=0;
    while(stream(last,section,count)==0){
    bitXor(result, section);
    eecb(key, section, result, rounds);
      overwritemsg(section, result);
      printf("%s", section);
      count++;
      overwritekey(key1, key);}

      printf("%s", section);


  }
  //needed for passing next section for decryption
  char r[13] = "111100001111";
  r[12] = '\0';
  //rules for decryption
  if (direction == 1) {
    while (feed(message, section, count) == 0&& count!=stop-1) {
      overwritemsg(section, result);
      decb(key, section, result, rounds);
      bitXor(r, section);

      printf("%s", section);
      overwritemsg(result, r);
      count++;
      overwritekey(key1, key);
    }
overwritemsg(section, result);
      decb(key, section, result, rounds);
      bitXor(r, section);
      overwritekey(key1, key);
    int size = strlen(message)-((stop)*12);
    char last[12+size];
    last[12+size]='\0';
    cstealing(section,message, size, last);
    count=0;
    while(stream(last,section,count)==0){
    overwritemsg(section, result);
      decb(key, section, result, rounds);
      bitXor(r, section);
      printf("%s", section);
      overwritemsg(result, r);
      count++;
      overwritekey(key1, key);}

      printf("%s", section);
  }
}

void ctr(char key1[], char * message, int rounds, int direction) {
  int count = 0;
  int counter=strlen(message)/12;
  char section[13];
  section[12] = '\0';
  char result[13];
  result[12] = '\0';

  char key[10];
  int ctrinput; //int value of nonce
  key[9] = '\0';
  overwritekey(key1, key);
  //rules for encryption
  if (direction == 0) {
    while (stream(message, section, count) == 0) {
      ctrinput = 3855;
      ctrinput += counter;
      to_binary_msg(ctrinput, result);
      eecb(key, result, result, rounds);
      bitXor(result, section);
      printf("%s", section);
      overwritekey(key1, key);
      count++;
      counter++;
    }
    ctrinput = 3855;
    ctrinput += counter;
    to_binary_msg(ctrinput, result);
    eecb(key, result, result, rounds);
    bitXor(result, section);
    printf("%s", section);
  }

  //rules for decryption
  if (direction == 1) {
    while (stream(message, section, count) == 0) {
      ctrinput = 3855;
      ctrinput += counter;
      to_binary_msg(ctrinput, result);
      eecb(key, result, result, rounds);
      bitXor(result, section);
      printf("%s", section);
      overwritekey(key1, key);
      count++;
      counter++;
    }
ctrinput = 3855;
    ctrinput += counter;
    to_binary_msg(ctrinput, result);
    eecb(key, result, result, rounds);
    bitXor(result, section);
    printf("%s", section);
  }
}

/*
 * Function: ecb
 * --------------------
 *  feed 12 bits of the message at the time
 *  encryption and decryption for ecb type
 *  returns: void
 */

void ecb(char key1[], char * message, int rounds, int direction) {
  int count = 0;
  char section[13];
  char result[13];
  section[12] = '\0';
  result[12] = '\0';
  char key[10];
  key[9] = '\0';
  overwritekey(key1, key);
  //rules for encryption
  if (direction == 0) {
    while (feed(message, section, count) == 0) {

      eecb(key, section, result, rounds);
      printf("%s", section);
      count++;
      overwritekey(key1, key);
    }
    eecb(key, section, result, rounds);
    printf("%s", section);

  }
  //rules for decryption
  if (direction == 1) {
    while (feed(message, section, count) == 0) {
      decb(key, section, result, rounds);
      printf("%s", section);
      count++;
      overwritekey(key1, key);
    }

  }
}
void eecb(char key[], char message[], char result[], int rounds) {
    char nkey[9];
    nkey[8] = '\0';
    int i;
    for (i = 0; i < rounds; i++) {
      keyforward(key, nkey);
      //run encryption algorithm
      encrypt(message, nkey);
    }

  }
  //buffer is causing problems with decryption
void decb(char key[], char message[], char result[], int rounds) {
    char nkey[9];
    int i;
    int j;
    for (i = 0; i < rounds + 1; i++) {
      keyforward(key, nkey);
    }

    for (j = 0; j < rounds; j++) {
      keybackward(key, nkey);
      flip_halves(message);

      //run encryption algorithm
      encrypt(message, nkey);
      flip_halves(message);

    }

  }
  /*
   * Function: overwrite
   * --------------------
   *  this is for replacing left and right at the end of ethe rounds
   * this was done because of overlapped caused by strcpy
   *  returns: void
   */
void overwrite(char original[], char next[]) {
  int i;

  for (i = 0; i < 6; i++) {
    next[i] = original[i];
  }
}
void overwritekey(char original[], char next[]) {
  int i;

  for (i = 0; i < 9; i++) {
    next[i] = original[i];
  }
}

void overwritemsg(char original[], char next[]) {
    int i;

    for (i = 0; i < 12; i++) {
      next[i] = original[i];
    }
  }
  /*
   * Function: keyforward
   * --------------------
   *  feed 9 bit key and gives 8 bit key
   *
   *  returns: void
   */
void keyforward(char okey[], char nkey[]) {
    char key[10];
    key[0] = okey[1];
    key[1] = okey[2];
    key[2] = okey[3];
    key[3] = okey[4];
    key[4] = okey[5];
    key[5] = okey[6];
    key[6] = okey[7];
    key[7] = okey[8];
    key[8] = okey[0];

    int i;
    for (i = 0; i < 8; i++) {
      nkey[i] = key[i];
      okey[i] = key[i];
    }
    okey[8] = key[8];
  }
  /*
   * Function: keybackward
   * --------------------
   *  creates 8 bit key from 9 bit key then rewrites original key
   *  with new order of key
   *
   *  returns: void
   */
void keybackward(char okey[], char nkey[]) {
  char key[10];
  key[0] = okey[8];
  key[1] = okey[0];
  key[2] = okey[1];
  key[3] = okey[2];
  key[4] = okey[3];
  key[5] = okey[4];
  key[6] = okey[5];
  key[7] = okey[6];
  key[8] = okey[7];

  int i;
  for (i = 0; i < 8; i++) {
    nkey[i] = key[i];
    okey[i] = key[i];
  }
  okey[8] = key[8];
}
/*
 * Function: stream
 * --------------------
 *  cut message into 12 bits  and remove 12 bits from message
 * if message less then 12 bits adds no buffer just returns what's left
 *  returns: int (0 means full 12 bits, 1 means empty or last section)
 */
int stream(char message[], char section[], int count){
  // read 12 bits until there is less then 12 bits
  int size = strlen(message) - (12 * count);
   int stop = count * 12;
  int i;
  if (size < 12) {

    for (i = 0; i < size; i++) {
      section[i] = message[i+stop];
    }
    section[size]='\0';
    return size;
  } else {
    //copy first twelve
    for (i = 0; i < 12; i++) {
      section[i] = message[i + (count * 12)];
    }
  }

  //if successful else return 1 for end point
  return 0;
}
/*
 * Function: feed
 * --------------------
 *  cut message into 12 bits  and remove 12 bits from message
 *
 *  returns: int (0 means full 12 bits, 1 means empty or last section)
 */
int feed(char message[], char section[], int count) {
    // read 12 bits until there is less then 12 bits
    int size = strlen(message) - (12 * count);
    int i;
    if (size < 12) {
      char bmsg[13];
      buffer(message, bmsg, count);
      for (i = 0; i < 12; i++) {
        section[i] = bmsg[i];
      }

      return 1;
    } else {
      //copy first twelve
      for (i = 0; i < 12; i++) {
        section[i] = message[i + (count * 12)];
      }
    }

    //if successful else return 1 for end point
    return 0;
  }
  /*
   * Function: buffer
   * --------------------
   *  adds chars until 12 bits long
   *
   *  returns: void
   */
void buffer(char message[], char bmsg[], int count) {

  int size = count * 12;
  int a = strlen(message) - size;
  char buff[13] = "101011110011";
  int i, c = 0;

  for (i = 0; i < 12; i++) {
    if (i >= a) {
      bmsg[i] = buff[c];
      c++;
    } else {
      bmsg[i] = message[i + size];
    }
  }
}
/*
*cipher-text-stealing switchs locations of text and returns in new order
*/
void cstealing(char section[],char *message, int size, char last[]){
int i;
int msglen=strlen(message);
int stack=(msglen/12)-1;
int start= (stack*12);
char cm[size];
cm[size]='\0';
char cp[12-size];
cm[12-size]='\0';
int count=0;
int counter=0;
//cm
for(i=0; i<size;i++){
cm[i]=section[i];
}
//cp
for(i=0; i<(12-size);i++){
cp[i]=section[size+i];
}
//join pm and cp
for(i=0; i<(12+size); i++){
if(i<size)last[i]=message[msglen-size+i];
else if(i>=size && i<12){ last[i]=cp[count];count++;}
else {last[i]=cm[counter]; counter++;}
}

}
