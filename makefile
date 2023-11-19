IDIR = ./include
CXX = clang++
CFLAGS = -I$(IDIR) \
		 -I./third_party/boost_1_73_0 \
		 -I./third_party/CLI11/include \
		 -Wno-logical-op-parentheses

ODIR = obj
LDIR = ./lib
SDIR = ./src

LIBS =

_DEPS = ast.hpp \
		ast_adapted.hpp \
		config.hpp \
		parser.hpp \
		parser_def.hpp
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o parser.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

.PHONY: all clean
.SECONDARY: main-build

all: pre-build main-build

main-build: plurals-parser

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

plurals-parser: $(OBJ)
	$(CXX) -o $@ $^ $(CFLAGS) $(LIBS)

pre-build:
	@if [ ! -d "./third_party/boost_1_73_0" ] ; then                                                    \
		echo "INFO: Downloading boost libraries";                                                   \
		wget https://boostorg.jfrog.io/artifactory/main/release/1.73.0/source/boost_1_73_0.tar.bz2; \
		tar --bzip2 -xf boost_1_73_0.tar.bz2;                                                       \
		mv boost_1_73_0 third_party/;                                                               \
		rm boost_1_73_0.tar.bz2;                                                                    \
	else                                                                                                \
		echo "INFO: No need to download boost libraries";                                           \
	fi
	@if git submodule status | egrep -q '^[-]|^[+]' ; then 		                                    \
		echo "INFO: Need to reinitialize git submodules"; 		                            \
		git submodule update --init; 							            \
	else 														                         \
		echo "INFO: No need to reinitialize git submodules"; 	                                    \
	fi

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
