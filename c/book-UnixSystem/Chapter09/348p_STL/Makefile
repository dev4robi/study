# 컴파일러 옵션 지정
CXX = g++
# CXX = C++
CCFLG = -g -I. -c

# 실행 파일 지정
TARGET = CheckTrace

# 소스파일과 오브젝트 파일 지정
SRCS = Config.cxx Trace.cxx
OBJS = Config.o Trace.o

.SUFFIXES: .cxx;

.cxx.o:
	$(CXX) $(CCFLG) $<

# Target
all :
	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS)

clean :
	rm $(OBJS)
	rm $(TARGET)