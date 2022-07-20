# Get Next Line

> 파일 디스크립터로부터 읽혀진, 개행으로 끝나는 한 줄을 반환하는 함수를 구현하는 프로젝트 입니다.

`open()` 함수로 파일에 접근하여 `fd`을 얻어 `read()` 함수를 통해 파일을 읽을 수 있습니다. 해당 프로젝트의 `GNL()` 함수는 파일을 개행까지 한줄 읽어오는 함수 입니다. 개행없이 파일이 끝나는 경우엔 개행없이 파일 끝까지 읽어옵니다. `GNL()` 함수의 일반적 사용은 다음과 같습니다.

```.c
char *line = 0;

if (!(fd = open("file_name", O_RDONLY)))
	{
		printf("\nError in open\n");
		return (0);
	}
	while ((i = get_next_line(fd, &line)) > 0)
	{
		printf("%s\n", line);
		free(line);
		j++;
	}
	printf("%s\n", line);
	free(line);
	close(fd);
```

-----

## Contents

- [Get Next Line](#get-next-line)
	- [Contents](#contents)
	- [Key Word](#key-word)
		- [📌 read()](#-read)
		- [📌 FD](#-fd)
		- [📌 Buffer](#-buffer)
		- [📌 Static 변수](#-static-변수)
		- [📌 Dangling Pointer](#-dangling-pointer)
		- [📌 gcc -D](#-gcc--d)
		- [📌 Redirection](#-redirection)
		- [ft_strlen](#ft_strlen)

-----

## Key Word

### 📌 read()
> #include <unistd.h>


**PROTOTYPE**

```c
ssize_t read (int fd, void *buf, size_t len)
```

- 	ssize_t형

	크기를 나타낼 때 size_t형을 쓴 바가 있습니다. 이는 unsigned int형으로, <sys/types.h> 헤더에 typedef 선언이 되어있습니다. 마찬가지로 `ssize_t형`은 signed int형으로, read()가 정상적으로 읽어오지 못했을 때 `-1을 반환`하므로써 오류를 보이기 위함입니다. int 자료형이 있음에도 불구하고 `size_t형과 ssize_t형을 따로 구분하여 사용하는 이유`는 다음과 같습니다. 32bit OS를 보편적으로 사용하는 현재에는 int형이 32bit로 쓰이지만, 16bit OS에서는 int형이 16 bit 였습니다. 즉 시스템에 따라 자료형의 표현 방식이 틀려지기 때문에 내가 구현한 프로그램을 다른 시스템에서 실행 시키기 위해서는 코드 수정 자체가 불가피해집니다. 이러한 문제가 있으므로 4byte 자료형이 필요한 곳에 헤더파일에 unsigned int를 size_t형으로 정의해 쓴다면 이후 시스템이 변경될 경우 그 시스템 상에서 4byte를 나타내는 자료형을 가지고 헤더파일에 선언되어 있는 size_t형을 재정의 해주기만 하면 됩니다. 이것이 int, double, short 등과 같은 특정 자료형에 종속되지 않도록, 새로운 이름의 자료형을 정의해 주는 이유가 됩니다.


	[ssize_t형 참조 사이트](https://blog.naver.com/kor31/80014818884)

-	int fd

	OPEN() 시스템 콜로 열린 파일을 가리키는 파일 지정 번호

-	void *buf

	파일에서 읽은 데이터를 저장할 메모리 공간

-	size_t len

	읽을 데이터의 크기

**RETURN VALUE**

-	파일 읽기 성공 : 0보다 큰 수 (읽어들인 byte 수)
-	읽을 데이터 없을 시 : 0 (EOF : End Of File 파일의 끝을 만났을 때)
-	파일 읽기 실패 : -1

	[errno]<br>
	`EAGAIN` : file을 open할 때에 O_NONBLOCK flag이 설정되었으며, 현재 읽을 데이터가 없어서 read()함수를 바로 return함.<br>
 	`EAGAIN` or `EWOULDBLOCK` : fd가 socket이고 socket에 O_NONBLOCK으로 설정된 경우 읽을 데이터를 수신하지 못하여 바로 return함.<br>
	`EBADF` : fd가 유효하지 않은 file descriptor임.<br>
 	`EFAULT` : buf가 access할 수 없는 영역의 데이터 buffer임. 주로 변수를 pointer로 설정한 후에 malloc(3)하지 않은 경우<br>
 	`EINTR` : signal이 발생하여 interrupt됨.<br>
 	`EINVAL` : fd, buf, count 등의 변수가 잘못 설정된 경우. (예, count가 -값이 설정된 경우 등)<br>
 	`EIO` : I/O 오류. background process에서 terminal에 대한 I/O를 시도했다든 지...<br>
 	`EISDIR` : open된 fd가 directory인 경우<br>

	[read errno 참조 사이트](https://www.it-note.kr/201)

<div align = "right">
	<b><a href = "#Contents">↥ top</a></b>
</div>

------

### 📌 FD
> file descriptor

**DESCRIPTION**

-	파일 디스크립터에 대하여

	`파일 디스크립터`란 리눅스 혹은 유닉스 계열의 시스템에서 `프로세스`가 파일을 다룰 때 사용하는 개념으로, 프로세스에서 특정 파일에 접근할 때 사용하는 추상적인 값입니다. 유닉스 시스템에서 일반적인 정규파일부터 디렉토리, 소켓, 파이프, 블록 디바이스 등 모든 객체들을 파일로 관리합니다. 유닉스 시스템에서 프로세스가 이 파일들에 접근할 때 파일 디스크립터를 이용합니다. 프로세스가 실행 중에 파일을 open()하면 커널을 해당 프로세스의 파일 디스크립터 숫자 중 사용하지 않는 가장 작은 값을 할당 해줍니다. 그 다음 프로세스가 열려있는 파일에 시스템 콜을 이용해서 접근할 떄, 파일 디스크립터 값을 이용해서 파일을 지정할 수 있습니다.

-	3가지 Default input/ output streams
	-	stdin = 0 : 표준 입력(Standard Input)
	-	stdout = 1 : 표준 출력(Standard Output)
	-	stderr = 2 : 표준 에러(Standard Error)


	파일 오픈 or 소켓 생성 시 부여되는 파일 디스크립터는 3부터 시작합니다. 프로세스가 열려있는 파일에 시스템 콜을 이용해서 접근할 때, 파일 디스크립터 값을 이용해 파일을 지칭할 수 있습니다.

-	fd table

	각각의 `프로세스`는 `파일 디스크립터 테이블`을 가지고 있습니다. 파일 디스크립터 테이블에는 `파일 디스크립터`가 저장되어 있습니다. 프로세스가 생성될 때 기본적으로 0, 1, 2에 해당하는 fd가 매핑됩니다. `파일 디스크립터 테이블`의 각 항목은 `FD 플래그`와 `파일 테이블로의 포인터`를 가지고 있습니다. 이 포인터를 이용하여 FD를 통해 파일을 참조할 수 있는 겁니다. 프로세스는 FD 테이블과 FD 테이블의 정보를 직접 고칠 수 없으며 반드시 `커널`을 통해 수정할 수 있습니다. 

-	redirection

	`리다이렉션`이란 `출력의 방향을 바꾸는 것`을 말합니다. 예를들어 일반 컴퓨터의 `표준 출력` 장치는 `모니터`이고 `표준 입력` 장치는 `키보드` 입니다. 출력의 방향을 바꾼다는 것은 모니터로 출력되어야 할 내용을 파일로 저장 시킨다든지 또는 통신 포트나 프린터로 출력의 방향을 바꿀 수 있음을 말합니다. 표준 입력을 받거나 표준 출력을 할 뿐만 아니라, 파일로 표준 입력을 받고, 파일로 표준 출력을 받을 수 있습니다.

	```.vim
	ls > tmp.txt
	```
	> ls의 출력물을 tmp.txt로 보냅니다.

	```.vim
	
	```

-	OPEN_MAX

	FD의 최대값을 말하며, 단일 프로그램에 허용되는 최대 열린 파일 수를 정의하는 상수 입니다. 즉, 하나의 프로세스 당 최대 OPEN_MAX개의 파일을 열 수 있습니다. OPEN_MAX 값은 플랫폼에 따라 다릅니다. 유닉스 시스템에서 C언어의 OPEN_MAX는 limits.h에 정의되어 있습니다. `getconf OPEN_MAX` 명령어로 값을 확인할 수 있습니다.

	[fd 참조 사이트1](https://twofootdog.tistory.com/51)<br>
	[fd 참조 사이트2](https://code4human.tistory.com/123)<br>

**ISSUES**

프로세스

스트림(stream)

소켓

<div align = "right">
	<b><a href = "#Contents">↥ top</a></b>
</div>

------

### 📌 Buffer
>


<div align = "right">
	<b><a href = "#Contents">↥ top</a></b>
</div>

------

### 📌 Static 변수
> 정적 변수

<div align = "right">
	<b><a href = "#Contents">↥ top</a></b>
</div>

------

### 📌 Dangling Pointer
> 댕글링 포인터

<div align = "right">
	<b><a href = "#Contents">↥ top</a></b>
</div>

------

### 📌 gcc -D
>

------
### 📌 Redirection
>

<div align = "right">
	<b><a href = "#Contents">↥ top</a></b>
</div>

------

### ft_strlen
> string length

<div align = "right">
	<b><a href = "#Contents">↥ top</a></b>
</div>

------

[sample]

**PROTOTYPE**

```c
ssize_t read (int fd, void *buf, size_t len)
```

**DESCRIPTION**

문자열 `s`의 길이를 구한다.

**RETURN VALUE**

문자열 `s`의 길이

**ISSUES**

`size_t`형을 반환하므로 count value도 동일한 자료형으로 선언한다.
-   [size_t형에 대하여](#size_t형에-대하여)

<div align = "right">
	<b><a href = "#Contents">↥ top</a></b>
</div>

--------------------------------------------------