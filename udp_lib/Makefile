# Настройки компилятора и параметров
CC = gcc
CFLAGS = -Wall -Wextra -O2 -fPIC
TARGET = libudp

# Пути для установки (по стандарту GNU)
PREFIX ?= /usr/local
INCLUDEDIR ?= $(PREFIX)/include
LIBDIR ?= $(PREFIX)/lib

# Имена итоговых файлов библиотеки
STATIC_LIB = $(TARGET).a
SHARED_LIB = $(TARGET).so

# Главная цель — собрать оба варианта
all: $(STATIC_LIB) $(SHARED_LIB)

# Сборка статической библиотеки (.a)
$(STATIC_LIB): udp.o
	ar rcs $@ $^

# Сборка динамической библиотеки (.so)
$(SHARED_LIB): udp.o
	$(CC) -shared -o $@ $^

# Компиляция исходного кода в объектный файл
udp.o: udp.c udp.h
	$(CC) $(CFLAGS) -c $< -o $@

# Установка файлов в систему
install: all
	install -d $(DESTDIR)$(INCLUDEDIR)
	install -d $(DESTDIR)$(LIBDIR)
	install -m 644 udp.h $(DESTDIR)$(INCLUDEDIR)/
	install -m 644 $(STATIC_LIB) $(DESTDIR)$(LIBDIR)/
	install -m 755 $(SHARED_LIB) $(DESTDIR)$(LIBDIR)/
	ldconfig

# Удаление установленных файлов из системы
uninstall:
	rm -f $(DESTDIR)$(INCLUDEDIR)/udp.h
	rm -f $(DESTDIR)$(LIBDIR)/$(STATIC_LIB)
	rm -f $(DESTDIR)$(LIBDIR)/$(SHARED_LIB)
	ldconfig

# Очистка папки проекта от результатов сборки
clean:
	rm -f *.o *.a *.so

.PHONY: all install uninstall clean
