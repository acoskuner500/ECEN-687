TARGET ?= coskuner9366
INPUT = input.txt

build: $(TARGET).cpp
	g++.exe -o $(TARGET) $(TARGET).cpp

run:
	./$(TARGET).exe $(INPUT)

rebuild: build run

clean:
	rm -f *.exe

zip: clean
	zip -r coskuner9366 $(TARGET).*