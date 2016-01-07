


sources+= db/etDBTable.c
sources+= db/etDBColumn.c
sources+= db/etDBValue.c
sources+= db/etDBFilter.c
sources+= db/etDBRelations.c
sources+= db/etDB.c
sources+= dbdriver/etDBSQLite.c

sourcePath=$(PWD)/../extra
sourcesFull=$(addprefix $(sourcePath)/,$(sources))
