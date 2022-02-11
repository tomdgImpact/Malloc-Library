#!/bin/sh

LD_PRELOAD=../libmalloc.so factor 20 10 25 37
LD_PRELOAD=../libmalloc.so ls
