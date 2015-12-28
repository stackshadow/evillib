
# Main Parts
sources += core/etObject.c
sources += core/etDebug.c
sources += core/etInit.c
sources += core/etVersion.c

# Memory Functions
sources += memory/etMemoryBlock.c
sources += memory/etMemoryList.c
sources += memory/etMemory.c

# etString
sources += string/etString.c
sources += string/etStringChar.c

# System Functions
sources += system/etThread.c

# File functions
sources += system/etFile.c

# Network functions
sources += net/etComm.c
sources += net/etCommTCP.c

sources = $(shell find ./core -name "*.h" ! -wholename "*specialheaders/*" ! -wholename "*develop/*" -printf "%p " )

sourcePath=$(PWD)/core
sourcesFull=$(addprefix $(sourcePath)/,$(sources))
sourcesFull=$(sources)

