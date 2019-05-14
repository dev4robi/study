#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define COMMANDSIZ 64

int main()
{
	void *s_memory = NULL;
	int shmId;
	char *buffer;
	int isRun = 1;

	// 공유메모리 생성 or 얻기
	if ((shmId = shmget((key_t)9000, COMMANDSIZ, 0666 | IPC_CREAT)) == -1)
	{
		fprintf(stderr, "shmget() 실행 실패.\n");
		return 0;	
	}

	// 공유메모리 주소 얻기
	if ((s_memory = shmat(shmId, NULL, 0)) == (void *)-1)
	{
		fprintf(stderr, "shmat() 실행 실패.\n");
		return 0;
	}

	// 공유 메모리 주소와 내부 변수 포인터 연결
	buffer = (char *)s_memory;

	while (isRun)
	{
		// ON 이면 sndShm 프로세스가 넣어준 명령 접수
		if (!strncmp(buffer, "ON", 2))
		{
			// 명령 출력
			fprintf(stdout, "rcvShm: %s\n", buffer + 2);
			
			// 공유메모리 최상단을 NO로 채워서 접근 방지
			strncpy(buffer, "NO", 2);

			// 전달받은 메시지가 quit 이면 종료
			if (!strncmp(buffer + 2, "quit", 4))
			{
				isRun = 0;
			}
		}
	}

	// 프로세스와 공유 메모리 분리
	if (shmdt(s_memory) == -1)
	{
		fprintf(stderr, "shmdt() 실행 실패.\n");
		return 0;
	}

	// 공유 메모리 제거
	if (shmctl(shmId, IPC_RMID, 0) == -1)
	{
		fprintf(stderr, "shmctl 실행 실패.\n");
		return 0;
	}

	return 0;
}