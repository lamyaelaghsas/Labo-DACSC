.SILENT:

COMP = g++ -I lib/ -I serveur/
LIB_TCP = lib/TCP.o
SERVEUR_OBJS = serveur/CBP.o serveur/ACBP.o
MYSQL_FLAGS = -I/usr/include/mysql -L/usr/lib64/mysql -lmysqlclient -lpthread

all: ServeurReservation ClientReservation

# ============= SERVEUR RESERVATION =============
ServeurReservation: serveur/ServeurReservation.cpp $(LIB_TCP) $(SERVEUR_OBJS)
	echo Création de ServeurReservation
	$(COMP) serveur/ServeurReservation.cpp $(LIB_TCP) $(SERVEUR_OBJS) -o ServeurReservation $(MYSQL_FLAGS)

serveur/CBP.o: serveur/CBP.cpp serveur/CBP.h
	echo Création de CBP.o
	$(COMP) -c serveur/CBP.cpp -o serveur/CBP.o $(MYSQL_FLAGS)

serveur/ACBP.o: serveur/ACBP.cpp serveur/ACBP.h
	echo Création de ACBP.o
	$(COMP) -c serveur/ACBP.cpp -o serveur/ACBP.o

# ============= LIBRAIRIE TCP =============
lib/TCP.o: lib/TCP.cpp lib/TCP.h
	echo Création de TCP.o
	$(COMP) -c lib/TCP.cpp -o lib/TCP.o

# ============= CLIENT RESERVATION Qt =============
ClientReservation: ClientConsultationBookerQt/main.o ClientConsultationBookerQt/mainwindowclientconsultationbooker.o ClientConsultationBookerQt/moc_mainwindowclientconsultationbooker.o $(LIB_TCP)
	echo Création de ClientReservation
	g++ -o ClientReservation ClientConsultationBookerQt/main.o ClientConsultationBookerQt/mainwindowclientconsultationbooker.o ClientConsultationBookerQt/moc_mainwindowclientconsultationbooker.o $(LIB_TCP) /usr/lib64/libQt5Widgets.so /usr/lib64/libQt5Gui.so /usr/lib64/libQt5Core.so /usr/lib64/libGL.so -lpthread

ClientConsultationBookerQt/main.o: ClientConsultationBookerQt/main.cpp
	echo Création de main.o
	g++ -c -pipe -g -std=gnu++11 -Wall -W -D_REENTRANT -fPIC -I./ClientConsultationBookerQt -I./lib -isystem /usr/include/qt5 -isystem /usr/include/qt5/QtWidgets -isystem /usr/include/qt5/QtGui -isystem /usr/include/qt5/QtCore -o ClientConsultationBookerQt/main.o ClientConsultationBookerQt/main.cpp

ClientConsultationBookerQt/mainwindowclientconsultationbooker.o: ClientConsultationBookerQt/mainwindowclientconsultationbooker.cpp ClientConsultationBookerQt/mainwindowclientconsultationbooker.h
	echo Création de mainwindowclientconsultationbooker.o
	g++ -c -pipe -g -std=gnu++11 -Wall -W -D_REENTRANT -fPIC -I./ClientConsultationBookerQt -I./lib -isystem /usr/include/qt5 -isystem /usr/include/qt5/QtWidgets -isystem /usr/include/qt5/QtGui -isystem /usr/include/qt5/QtCore -o ClientConsultationBookerQt/mainwindowclientconsultationbooker.o ClientConsultationBookerQt/mainwindowclientconsultationbooker.cpp

ClientConsultationBookerQt/moc_mainwindowclientconsultationbooker.o: ClientConsultationBookerQt/moc_mainwindowclientconsultationbooker.cpp
	echo Création de moc_mainwindowclientconsultationbooker.o
	g++ -c -pipe -g -std=gnu++11 -Wall -W -D_REENTRANT -fPIC -I./ClientConsultationBookerQt -I./lib -isystem /usr/include/qt5 -isystem /usr/include/qt5/QtWidgets -isystem /usr/include/qt5/QtGui -isystem /usr/include/qt5/QtCore -o ClientConsultationBookerQt/moc_mainwindowclientconsultationbooker.o ClientConsultationBookerQt/moc_mainwindowclientconsultationbooker.cpp

# ============= NETTOYAGE =============
clean:
	rm -f lib/*.o serveur/*.o ClientConsultationBookerQt/*.o core
	echo Suppression des fichiers objets

clobber: clean
	rm -f ServeurReservation ClientReservation
	echo Suppression des exécutables