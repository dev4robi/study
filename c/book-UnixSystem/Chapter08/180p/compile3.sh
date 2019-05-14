# build program [useMsg] using [useMsg.c] with [msgLib.a] file.

gcc -o useMsg useMsg.c msgLib.a

# this is a same shell instruction over this line

# gcc -o useMsg useMsg.c -L/home/robi/desktop/Study/C/Book_UnixSystem/180p -lmsgLib