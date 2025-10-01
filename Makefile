# Fichier makefile - Projet DACSC 2025-2026
.SILENT:

# Variables de repertoires
LIB=./lib
SERVEUR=./Serveur
CLIENT=./ClientConsultationBookerQt
TESTS=./tests

# Variable de compilation avec options
COMP=g++ -I $(LIB)
COMP_QT=g++ -I/usr/include/qt5 -I/usr/include/qt5/QtWidgets -I/usr/include/qt5/QtGui -I/usr/include/qt5/QtCore -fPIC
COMP_SERVEUR=g++ -I $(LIB) -I/usr/include/mysql -pthread

# Variables pour linkage
MYSQL_LINK=-L/usr/lib64/mysql -lmysqlclient -lpthread -lz -lm -lrt -lssl -lcrypto -ldl
QT_LINK=-lQt5Widgets -lQt5Gui -lQt5Core

# Fichiers objets
OBJS=$(LIB)/TCP.o $(SERVEUR)/CBP.o
OBJS_CLIENT=$(CLIENT)/main.o $(CLIENT)/mainwindowclientconsultationbooker.o $(CLIENT)/moc_mainwindowclientconsultationbooker.o $(LIB)/TCP.o
OBJS_TEST=$(TESTS)/ClientTest.o $(LIB)/TCP.o

# Executables
PROGRAMS=$(SERVEUR)/ServeurReservation $(CLIENT)/ClientQt $(TESTS)/ClientTest

all: $(PROGRAMS)

# ===== SERVEUR RESERVATION =====

$(SERVEUR)/ServeurReservation: $(SERVEUR)/ServeurReservation.cpp $(OBJS)
	echo Creation de ServeurReservation
	$(COMP_SERVEUR) $(SERVEUR)/ServeurReservation.cpp $(OBJS) $(MYSQL_LINK) -o $(SERVEUR)/ServeurReservation

$(SERVEUR)/CBP.o: $(SERVEUR)/CBP.cpp $(SERVEUR)/CBP.h
	echo Creation de CBP.o
	$(COMP_SERVEUR) $(SERVEUR)/CBP.cpp -c -o $(SERVEUR)/CBP.o

# ===== CLIENT QT =====

$(CLIENT)/ClientQt: $(OBJS_CLIENT)
	echo Creation de ClientQt
	$(COMP_QT) $(OBJS_CLIENT) $(QT_LINK) -o $(CLIENT)/ClientQt

$(CLIENT)/main.o: $(CLIENT)/main.cpp
	echo Creation de main.o
	$(COMP_QT) $(CLIENT)/main.cpp -c -o $(CLIENT)/main.o

$(CLIENT)/mainwindowclientconsultationbooker.o: $(CLIENT)/mainwindowclientconsultationbooker.cpp $(CLIENT)/mainwindowclientconsultationbooker.h
	echo Creation de mainwindowclientconsultationbooker.o
	$(COMP_QT) $(CLIENT)/mainwindowclientconsultationbooker.cpp -c -o $(CLIENT)/mainwindowclientconsultationbooker.o

$(CLIENT)/moc_mainwindowclientconsultationbooker.cpp: $(CLIENT)/mainwindowclientconsultationbooker.h
	echo Generation de moc_mainwindowclientconsultationbooker.cpp
	moc-qt5 $(CLIENT)/mainwindowclientconsultationbooker.h -o $(CLIENT)/moc_mainwindowclientconsultationbooker.cpp

$(CLIENT)/moc_mainwindowclientconsultationbooker.o: $(CLIENT)/moc_mainwindowclientconsultationbooker.cpp
	echo Creation de moc_mainwindowclientconsultationbooker.o
	$(COMP_QT) $(CLIENT)/moc_mainwindowclientconsultationbooker.cpp -c -o $(CLIENT)/moc_mainwindowclientconsultationbooker.o

# ===== TEST CLIENT =====

$(TESTS)/ClientTest: $(OBJS_TEST)
	echo Creation de ClientTest
	$(COMP) $(OBJS_TEST) -o $(TESTS)/ClientTest

$(TESTS)/ClientTest.o: $(TESTS)/ClientTest.cpp
	echo Creation de ClientTest.o
	$(COMP) $(TESTS)/ClientTest.cpp -c -o $(TESTS)/ClientTest.o

# ===== LIBRAIRIE TCP =====

$(LIB)/TCP.o: $(LIB)/TCP.cpp $(LIB)/TCP.h
	echo Creation de TCP.o
	$(COMP) $(LIB)/TCP.cpp -c -o $(LIB)/TCP.o

# ===== CIBLES UTILITAIRES =====

clean:
	rm -f $(OBJS) $(OBJS_CLIENT) $(OBJS_TEST) $(CLIENT)/moc_*.cpp core

clobber: clean
	rm -f $(PROGRAMS)