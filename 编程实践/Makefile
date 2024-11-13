CXX = g++

CXXFLAGS = -Wall -g

LDFLAGS =

INCDIR =
SRCDIR =
OBJDIR = obj/


TARGET = aa
SOURCES = logger.cpp main.cpp
OBJECTS = $(SOURCES:%.cpp=$(OBJDIR)%.o)

# 默认目标
all: $(TARGET)

# 链接目标
$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $(TARGET)

# 编译源文件
$(OBJDIR)%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 防止 make 执行时显示这些规则
.PHONY: all clean
