.SILENT:

# Variables communes
MYSQL_FLAGS = -I/usr/include/mysql -m64 -L/usr/lib64/mysql -lmysqlclient -lpthread -lz -lm -lrt -lssl -lcrypto -ldl
THREAD_FLAGS = -pthread
QT_FLAGS = -I/usr/include/qt5 -I/usr/include/qt5/QtWidgets -I/usr/include/qt5/QtGui -I/usr/include/qt5/QtCore -fPIC
QT_LIBS = /usr/lib64/libQt5Widgets.so /usr/lib64/libQt5Gui.so /usr/lib64/libQt5Core.so

all: TestSocket ServeurReservation TestClient CreationBD ClientQt

TestSocket: SocketLib/Socket.cpp SocketLib/TestSocket.cpp
	echo "Creation du TestSocket"
	g++ -o SocketLib/TestSocket SocketLib/TestSocket.cpp SocketLib/Socket.cpp

ServeurReservation: Serveur/ServeurReservation.cpp Serveur/CBP.cpp SocketLib/Socket.cpp
	echo "Creation du ServeurReservation"
	g++ -o Serveur/ServeurReservation Serveur/ServeurReservation.cpp Serveur/CBP.cpp SocketLib/Socket.cpp $(THREAD_FLAGS)

TestClient: Serveur/TestClient.cpp SocketLib/Socket.cpp
	echo "Creation du TestClient"
	g++ -o Serveur/TestClient Serveur/TestClient.cpp SocketLib/Socket.cpp

CreationBD: BD_Hospital/CreationBD.cpp
	echo "Creation du CreationBD"
	g++ -o BD_Hospital/CreationBD BD_Hospital/CreationBD.cpp $(MYSQL_FLAGS)

# Client Qt - Style cours (tout intégré)
ClientQt: ClientConsultationBookerQt/main.o ClientConsultationBookerQt/mainwindowclientconsultationbooker.o ClientConsultationBookerQt/moc_mainwindowclientconsultationbooker.o SocketLib/Socket.o
	echo "Creation du Client Qt"
	g++ -o ClientConsultationBookerQt/ClientQt \
		ClientConsultationBookerQt/main.o \
		ClientConsultationBookerQt/mainwindowclientconsultationbooker.o \
		ClientConsultationBookerQt/moc_mainwindowclientconsultationbooker.o \
		SocketLib/Socket.o \
		$(QT_LIBS)

ClientConsultationBookerQt/main.o: ClientConsultationBookerQt/main.cpp
	echo "Compilation main.o"
	g++ -c ClientConsultationBookerQt/main.cpp -o ClientConsultationBookerQt/main.o $(QT_FLAGS)

ClientConsultationBookerQt/mainwindowclientconsultationbooker.o: ClientConsultationBookerQt/mainwindowclientconsultationbooker.cpp
	echo "Compilation mainwindowclientconsultationbooker.o"
	g++ -c ClientConsultationBookerQt/mainwindowclientconsultationbooker.cpp -o ClientConsultationBookerQt/mainwindowclientconsultationbooker.o $(QT_FLAGS)

ClientConsultationBookerQt/moc_mainwindowclientconsultationbooker.o: ClientConsultationBookerQt/moc_mainwindowclientconsultationbooker.cpp
	echo "Compilation moc_mainwindowclientconsultationbooker.o"
	g++ -c ClientConsultationBookerQt/moc_mainwindowclientconsultationbooker.cpp -o ClientConsultationBookerQt/moc_mainwindowclientconsultationbooker.o $(QT_FLAGS)

SocketLib/Socket.o: SocketLib/Socket.cpp
	echo "Compilation Socket.o"
	g++ -c SocketLib/Socket.cpp -o SocketLib/Socket.o

clean:
	echo "Nettoyage des executables"
	rm -f SocketLib/TestSocket SocketLib/*.o
	rm -f Serveur/ServeurReservation Serveur/TestClient
	rm -f BD_Hospital/CreationBD
	rm -f ClientConsultationBookerQt/ClientQt ClientConsultationBookerQt/*.o