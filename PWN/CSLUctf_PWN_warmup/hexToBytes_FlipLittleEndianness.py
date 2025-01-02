#!/usr/bin/python3
import struct
flag_part1 = struct.pack("<Q", 0x726f667b554c5343).decode()
flag_part2 = struct.pack("<Q", 0x7d72747374346d).decode()  # Replace with the next leak
flag = flag_part1 + flag_part2
print(flag)