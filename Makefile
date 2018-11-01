all:
	gcc -o printchars printchars.c
	gcc -o starter starter.c
	gcc -o shed my_sched.c

sched:
	gcc -o printchars printchars.c
	gcc -o shed my_sched.c


clean:
	rm -f printchars
	rm -f starter
	rm -f sched
