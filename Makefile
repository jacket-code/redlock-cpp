# Redis Lock Makefile
CC = gcc
CCC = g++
CXX = g++
BASICOPTS = -g -O -Wall -fPIC -Wno-reorder 
CBASICOPTS = -g -O -Wall -fPIC
CFLAGS = $(CBASICOPTS)
CCFLAGS = $(BASICOPTS)
CXXFLAGS = $(BASICOPTS)
CCADMIN = 
INCLUDE = -I./ -I/usr/local/include/ -I./redlock-cpp/ -I./hiredis/
CINCLUDE = -I./ -I/usr/local/include/
LOCKLIB = -L./bin -lredlock -L./hiredis -lhiredis

# 动态库目录
TARGETDIR_BIN=bin
OUTPUT = libredlock.a
EXOUTPUT = LockExample
EXOUTPUTCLOCK = CLockExample

all: $(TARGETDIR_BIN)/$(OUTPUT) $(TARGETDIR_BIN)/$(EXOUTPUT) $(TARGETDIR_BIN)/$(EXOUTPUTCLOCK)

## 编译文件
OBJS_libcomm =  \
    $(TARGETDIR_BIN)/sds.o\
	$(TARGETDIR_BIN)/redlock.o

EXOBJS =  \
	$(TARGETDIR_BIN)/LockExample.o\
    $(TARGETDIR_BIN)/sds.o\
    $(TARGETDIR_BIN)/redlock.o

EXOBJSCLOCK =  \
	$(TARGETDIR_BIN)/CLockExample.o\
    $(TARGETDIR_BIN)/sds.o\
    $(TARGETDIR_BIN)/redlock.o

# 链接或归档
ARCPP = $(AR) $(ARFLAGS) $@ 
$(TARGETDIR_BIN)/$(OUTPUT): $(TARGETDIR_BIN) $(OBJS_libcomm)
	$(ARCPP) $(OBJS_libcomm)
$(TARGETDIR_BIN)/$(EXOUTPUT): $(TARGETDIR_BIN) $(EXOBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGETDIR_BIN)/$(EXOUTPUT) $(EXOBJS) $(LOCKLIB)
$(TARGETDIR_BIN)/$(EXOUTPUTCLOCK): $(TARGETDIR_BIN) $(EXOBJSCLOCK)
	$(CXX) $(CXXFLAGS) -o $(TARGETDIR_BIN)/$(EXOUTPUTCLOCK) $(EXOBJSCLOCK) $(LOCKLIB)

# 将原文件编译为 .o 文件
$(TARGETDIR_BIN)/%.o : ./redlock-cpp/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE)  -o $@  -c $(filter %.cpp, $^)
$(TARGETDIR_BIN)/%.o : ./redlock-cpp/%.c
	$(CC) $(CFLAGS) $(CINCLUDE)  -o $@  -c $<
$(TARGETDIR_BIN)/%.o : ./%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE)  -o $@  -c $(filter %.cpp, $^)

#### 清理目标，删除所生成的文件 ####
clean:
	rm -f \
		$(TARGETDIR_BIN)/$(OUTPUT) \
		$(TARGETDIR_BIN)/*.o 
	$(CCADMIN)
	rm -f -r $(TARGETDIR_BIN)

# 创建目标目录（如果需要）
$(TARGETDIR_BIN):
	mkdir -p $(TARGETDIR_BIN)
