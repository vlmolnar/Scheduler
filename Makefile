all:
	gcc -o printchars printchars.c -Wall -Wextra
	gcc -o starter starter.c -Wall -Wextra
	gcc -o shed my_sched.c -Wall -Wextra

sched:
	gcc -g -o printchars printchars.c -Wall -Wextra
	gcc -g -o sched my_sched.c pc_block.c sched.h -Wall -Wextra


clean:
	rm -f printchars
	rm -f starter
	rm -f sched
