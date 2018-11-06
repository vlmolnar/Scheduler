all:
	gcc -o printchars printchars.c -Wall -Wextra
	gcc -o starter starter.c -Wall -Wextra
	gcc -o shed my_sched.c -Wall -Wextra

sched:
	gcc -o printchars printchars.c -Wall -Wextra
	gcc -o shed my_sched.c pc_block.c -Wall -Wextra


clean:
	rm -f printchars
	rm -f starter
	rm -f sched
