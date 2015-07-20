
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

# Network functions
sources += net/etComm.c
sources += net/etCommTCP.c

sourcePath=$(PWD)/../core
sourcesFull=$(addprefix $(sourcePath)/,$(sources))


