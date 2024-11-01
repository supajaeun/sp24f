# 소스 파일 정의
SRCS = main.c  # 메인 소스 파일
LIBSRCS = open.c close.c put.c get.c  # 라이브러리 소스 파일

# 실행 파일 타겟 이름
TARG = kvs_static   # 정적 라이브러리를 사용한 실행 파일 이름
TARG_DYNAMIC = kvs_dynamic   # 동적 라이브러리를 사용한 실행 파일 이름

# 컴파일러 및 옵션 설정
CC = gcc    # 컴파일러로 gcc 사용
OPTS = -Wall -g  # 컴파일 옵션: 모든 경고 표시(-Wall), 디버그 정보 포함(-g)
LIBS = -L. -lkvs   # 링크 옵션: 현재 디렉토리에서 libkvs 라이브러리 사용

# 개별 파일 목록 정의
OBJS = $(SRCS:.c=.o) # 메인 소스 파일을 오브젝트 파일(.o)로 변환한 목록
LOBJS = $(LIBSRCS:.c=.o) # 라이브러리 소스 파일을 오브젝트 파일(.o)로 변환한 목록
LARS = libkvs.a     # 정적 라이브러리 파일 이름
LSO = libkvs.so     # 동적 라이브러리 파일 이름

# all 타겟: 모든 타겟(static과 dynamic)을 빌드
all: static dynamic

# 정적 라이브러리를 사용한 실행 파일 빌드
static: $(TARG)

# 정적 라이브러리를 사용해 kvs_static 실행 파일을 빌드
$(TARG): $(OBJS) $(LARS)
	$(CC) -o $(TARG) $(OBJS) $(LIBS) -static # 정적 링크 옵션(-static) 사용하여 kvs_static 빌드

# 정적 라이브러리 libkvs.a를 생성
$(LARS): $(LOBJS)
	ar rs $(LARS) $(LOBJS)                    # ar 명령으로 오브젝트 파일들을 묶어 libkvs.a 생성

# 동적 라이브러리를 사용한 실행 파일 빌드
dynamic: $(TARG_DYNAMIC)

# 동적 라이브러리를 사용해 kvs_dynamic 실행 파일을 빌드
$(TARG_DYNAMIC): $(OBJS) $(LSO)
	$(CC) -o $(TARG_DYNAMIC) $(OBJS) $(LIBS)  # 동적 라이브러리(libkvs.so)를 링크하여 kvs_dynamic 빌드

# 동적 라이브러리 libkvs.so를 생성
$(LSO): $(LOBJS)
	$(CC) -shared -o $(LSO) $(LOBJS)          # -shared 옵션으로 공유 라이브러리 libkvs.so 생성

# .c 파일을 .o 파일로 컴파일
%.o: %.c
	$(CC) $(OPTS) -fPIC -c $< -o $@           # 위치 독립 코드(-fPIC) 옵션으로 개별 .c 파일을 .o 파일로 컴파일

# clean 타겟: 빌드 결과 파일 삭제
clean:
	rm -f $(OBJS) $(LOBJS) $(LARS) $(LSO) $(TARG) $(TARG_DYNAMIC) # 빌드된 모든 파일 및 실행 파일 삭제
