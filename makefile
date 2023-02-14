COMPILER_FLAGS = -mssse3 -ffunction-sections -fdata-sections -fno-strict-aliasing -fwrapv -fno-asynchronous-unwind-tables -fno-unwind-tables -fno-stack-protector -fno-rtti -fno-exceptions -std=gnu++20 -m64
COMPILER_WARNS = -Wall -Wno-format-truncation -Wno-format-zero-length -Wno-stringop-truncation -Wno-invalid-offsetof -Wno-format-truncation -Wno-format-zero-length -Wno-stringop-truncation -Wextra -Werror -Wno-missing-field-initializers
SOURCE_INPUT = source/main.cpp
OBJECT_INPUT =
#Modes 
RELEASE_FLAGS = -Os -flto -ftrivial-auto-var-init=zero
DEBUG_FLAGS = -g -Og -flto -ftrivial-auto-var-init=zero
#Links
STD = -lstdc++ -lgdi32 -luser32 -lkernel32 -lole32 -lzstd

debug:
	$(CXX) $(DEBUG_FLAGS) $(SOURCE_INPUT) $(COMPILER_WARNS) $(COMPILER_FLAGS) $(STD) -o debug.exe

release:
	$(CXX) $(COMPILER_FLAGS)  $(COMPILER_WARNS) $(RELEASE_FLAGS) $(SOURCE_INPUT) -DNDEBUG -o release.exe $(STD)

release-debug:
	$(CXX) $(RELEASE_FLAGS) $(SOURCE_INPUT) $(COMPILER_FLAGS) $(STD) -o release-debug.exe

release-O2:
	$(CXX) -O2 $(SOURCE_INPUT) $(COMPILER_FLAGS) $(COMPILER_WARNS) -DNDEBUG $(STD) -o release-o2.exe

release-O3:
	$(CXX) -O3 $(SOURCE_INPUT) $(COMPILER_FLAGS) $(COMPILER_WARNS) -DNDEBUG $(STD) -o release-o3.exe

release-Oz:
	$(CXX) -Oz $(SOURCE_INPUT) $(COMPILER_FLAGS) $(COMPILER_WARNS) -DNDEBUG $(STD) -o release-oz.exe

clean:
	rm edit release.exe debug.exe
