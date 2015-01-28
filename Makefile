# Makefile

# プログラム名とオブジェクトファイル名
program = enum_induced_subtree
srcdir = ./src
objs = \
	$(srcdir)/Vertex.o \
	$(srcdir)/Graph.o \
	$(srcdir)/CandidateList.o \
	$(srcdir)/AdjacentList.o \
	$(srcdir)/EnumInducedSubtrees.o \
	$(srcdir)/main.o \

# 定義済マクロの再定義
CXX      = g++
CXXFLAGS = -Wextra -O3 -std=c++11 -I./include
LIBS     = -L./src -lboost_program_options

# サフィックスルール適用対象の拡張子の定義
.SUFFIXES: .cpp .o

# プライマリターゲット
.PHONY: all
all: depend $(program)

# プログラムの生成ルール
$(program): $(objs)
	$(CXX) $(CXXFLAGS) -o $(program) $^ $(LIBS)

# サフィックスルール
.c.o:
	$(CXX) $(CXXFLAGS) -c $<

# ファイル削除用ターゲット
.PHONY: clean
clean:
	$(RM) $(program) $(srcdir)/*.o depend.inc

# ヘッダーファイルの依存関係
.PHONY: depend
depend: $(objs:.o=.cpp)
	-@ $(RM) depend.inc
	-@ for i in $^; do\
		$(CXX) $(CXXFLAGS) -MM $$i | sed "s/\ [_a-zA-Z0-9][_a-zA-Z0-9]*\.cpp//g" >> depend.inc;\
	done

-include depend.inc
