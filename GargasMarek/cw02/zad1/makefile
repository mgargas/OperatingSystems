GCC = gcc -Wall -std=c99 -ggdb
M_PATH = main
OUTPUT = wyniki.txt


all: trivial tests

trivial:
	$(GCC) main.c lib.c sys.c -o $(M_PATH)
	make clean

clean:
	rm -f *.o

test:
	./main generate dane 100 512 lib

tests:
	#copy [source_file_name] [dest_file_name] [record_amount] [record_size] [sys or lib]
	#sort [file_name] [record_amount] [record_size] [sys or lib]
	#generate [file_name] [records] [length] [sys or lib]

	echo Testy dla sortowania: > $(OUTPUT)

	echo Ilosc rekordow: 1500, rozmiar: 4 >> $(OUTPUT)
	./$(M_PATH) generate sys_data 1500 4 sys 
	./$(M_PATH) copy sys_data lib_data 1500 4 sys
	./$(M_PATH) sort sys_data 1500 4 sys >> $(OUTPUT)
	./$(M_PATH) sort lib_data 1500 4 lib >> $(OUTPUT)
	echo >> $(OUTPUT)
	
	echo Ilosc rekordow: 1500, rozmiar: 512 >> $(OUTPUT)
	./$(M_PATH) generate sys_data 1500 512 sys 
	./$(M_PATH) copy sys_data lib_data 1500 512 sys
	./$(M_PATH) sort sys_data 1500 512 sys >> $(OUTPUT)
	./$(M_PATH) sort lib_data 1500 512 lib >> $(OUTPUT)
	echo >> $(OUTPUT)

	echo Ilosc rekordow: 1500, rozmiar: 4096 >> $(OUTPUT)
	./$(M_PATH) generate sys_data 1500 4096 sys 
	./$(M_PATH) copy sys_data lib_data 1500 4096 sys
	./$(M_PATH) sort sys_data 1500 4096 sys >> $(OUTPUT)
	./$(M_PATH) sort lib_data 1500 4096 lib >> $(OUTPUT)
	echo >> $(OUTPUT)

	echo Ilosc rekordow: 1500, rozmiar: 8192 >> $(OUTPUT)
	./$(M_PATH) generate sys_data 1500 8192 sys 
	./$(M_PATH) copy sys_data lib_data 1500 8192 sys
	./$(M_PATH) sort sys_data 1500 8192 sys >> $(OUTPUT)
	./$(M_PATH) sort lib_data 1500 8192 lib >> $(OUTPUT)
	echo >> $(OUTPUT)
	echo >> $(OUTPUT)

	echo Ilosc rekordow: 6000, rozmiar: 4 >> $(OUTPUT)
	./$(M_PATH) generate sys_data 6000 4 sys 
	./$(M_PATH) copy sys_data lib_data 6000 4 sys
	./$(M_PATH) sort sys_data 6000 4 sys >> $(OUTPUT)
	./$(M_PATH) sort lib_data 6000 4 lib >> $(OUTPUT)
	echo >> $(OUTPUT)
	
	echo Ilosc rekordow: 6000, rozmiar: 512 >> $(OUTPUT)
	./$(M_PATH) generate sys_data 6000 512 sys 
	./$(M_PATH) copy sys_data lib_data 6000 512 sys
	./$(M_PATH) sort sys_data 6000 512 sys >> $(OUTPUT)
	./$(M_PATH) sort lib_data 6000 512 lib >> $(OUTPUT)
	echo >> $(OUTPUT)

	echo Ilosc rekordow: 6000, rozmiar: 4096 >> $(OUTPUT)
	./$(M_PATH) generate sys_data 6000 4096 sys 
	./$(M_PATH) copy sys_data lib_data 6000 4096 sys
	./$(M_PATH) sort sys_data 6000 4096 sys >> $(OUTPUT)
	./$(M_PATH) sort lib_data 6000 4096 lib >> $(OUTPUT)
	echo >> $(OUTPUT)

	echo Ilosc rekordow: 6000, rozmiar: 8192 >> $(OUTPUT)
	./$(M_PATH) generate sys_data 6000 8192 sys 
	./$(M_PATH) copy sys_data lib_data 6000 8192 sys
	./$(M_PATH) sort sys_data 6000 8192 sys >> $(OUTPUT)
	./$(M_PATH) sort lib_data 6000 8192 lib >> $(OUTPUT)
	echo >> $(OUTPUT)
	echo >> $(OUTPUT)

	echo Testy dla kopiowania: >> $(OUTPUT)

	echo Ilosc rekordow: 1500, rozmiar: 4 >> $(OUTPUT)
	./$(M_PATH) generate data 1500 4 sys 
	./$(M_PATH) copy data sys_data 1500 4 sys >> $(OUTPUT)
	./$(M_PATH) copy data lib_data 1500 4 lib >> $(OUTPUT)
	echo >> $(OUTPUT)

	echo Ilosc rekordow: 1500, rozmiar: 512 >> $(OUTPUT)
	./$(M_PATH) generate data 1500 512 sys 
	./$(M_PATH) copy data sys_data 1500 512 sys >> $(OUTPUT)
	./$(M_PATH) copy data lib_data 1500 512 lib >> $(OUTPUT)
	echo >> $(OUTPUT)

	echo Ilosc rekordow: 1500, rozmiar: 4096 >> $(OUTPUT)
	./$(M_PATH) generate data 1500 4096 sys 
	./$(M_PATH) copy data sys_data 1500 4096 sys >> $(OUTPUT)
	./$(M_PATH) copy data lib_data 1500 4096 lib >> $(OUTPUT)
	echo >> $(OUTPUT)

	echo Ilosc rekordow: 1500, rozmiar: 8192 >> $(OUTPUT)
	./$(M_PATH) generate data 1500 8192 sys 
	./$(M_PATH) copy data sys_data 1500 8192 sys >> $(OUTPUT)
	./$(M_PATH) copy data lib_data 1500 8192 lib >> $(OUTPUT)
	echo >> $(OUTPUT)
	echo >> $(OUTPUT)

	echo Ilosc rekordow: 6000, rozmiar: 4 >> $(OUTPUT)
	./$(M_PATH) generate data 6000 4 sys 
	./$(M_PATH) copy data sys_data 6000 4 sys >> $(OUTPUT)
	./$(M_PATH) copy data lib_data 6000 4 lib >> $(OUTPUT)
	echo >> $(OUTPUT)

	echo Ilosc rekordow: 6000, rozmiar: 512 >> $(OUTPUT)
	./$(M_PATH) generate data 6000 512 sys 
	./$(M_PATH) copy data sys_data 6000 512 sys >> $(OUTPUT)
	./$(M_PATH) copy data lib_data 6000 512 lib >> $(OUTPUT)
	echo >> $(OUTPUT)

	echo Ilosc rekordow: 6000, rozmiar: 4096 >> $(OUTPUT)
	./$(M_PATH) generate data 6000 4096 sys 
	./$(M_PATH) copy data sys_data 6000 4096 sys >> $(OUTPUT)
	./$(M_PATH) copy data lib_data 6000 4096 lib >> $(OUTPUT)
	echo >> $(OUTPUT)

	echo Ilosc rekordow: 6000, rozmiar: 8192 >> $(OUTPUT)
	./$(M_PATH) generate data 6000 8192 sys 
	./$(M_PATH) copy data sys_data 6000 8192 sys >> $(OUTPUT)
	./$(M_PATH) copy data lib_data 6000 8192 lib >> $(OUTPUT)
	echo >> $(OUTPUT)
	echo >> $(OUTPUT)
	
