COMPILER = g++
COMPILER_FLAGS = -mssse3 -ffunction-sections -fdata-sections -fno-strict-aliasing -fwrapv -fno-asynchronous-unwind-tables -fno-unwind-tables -fno-stack-protector -fno-rtti -fno-exceptions -std=gnu++20 -m64
COMPILER_WARNS = -Wall -Wno-format-truncation -Wno-format-zero-length -Wno-stringop-truncation -Wno-invalid-offsetof -Wno-format-truncation -Wno-format-zero-length -Wno-stringop-truncation -Wextra -Werror -Wno-missing-field-initializers
SOURCE_INPUT = source/main.cpp
OBJECT_INPUT =
#Modes 
RELEASE_FLAGS = -O3 -flto
DEBUG_FLAGS = -g -Og -flto
#Links
STD = -IE:\RandomTools\mingw64-2\include -LE:\RandomTools\mingw64-2\lib -lstdc++ -lgdi32 -luser32 -lkernel32 -lole32 -lzstd

#Order matters sometimes (top to bottom)
debug:
	$(CXX) $(DEBUG_FLAGS) $(SOURCE_INPUT) $(COMPILER_WARNS) $(COMPILER_FLAGS) $(STD) -o debug.exe

release:
	$(COMPILER) $(COMPILER_FLAGS)  $(COMPILER_WARNS) $(RELEASE_FLAGS) $(SOURCE_INPUT) -DNDEBUG -o release.exe $(STD)

release-debug:
	$(COMPILER) $(RELEASE_FLAGS) $(SOURCE_INPUT) $(COMPILER_FLAGS) $(STD) -o release-debug.exe

release-Os:
	$(COMPILER) -Os $(SOURCE_INPUT) $(COMPILER_FLAGS) -DNDEBUG $(STD) -o release-o2.exe

clean:
	rm edit release.exe debug.exe