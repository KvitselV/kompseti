
#include <iostream>
#define _WINSOCK_DEPRECATED_NO_WARNINGS // подавление предупреждений библиотеки winsock2
#include <winsock2.h>// подключение заголовочного файла сокетов
#include <string> // строковый тип данных
#include <windows.h> // библиотека для сервера
#pragma comment (lib, "Ws2_32.lib")// подключение библиотеки для того, чтобы оставить комментарий в созданном объектном файле
#pragma warning(disable: 4996)// подавление предупреждения 4996 отключение предупреждения для определенной строки
using namespace std;
#define SRV_HOST "localhost" // установка адреса сервера (localhost) (ipconfig IPv4)
#define SRV_PORT 1234 // порт сервера
#define CLNT_PORT 1235 // порт клиента
#define BUF_SIZE 64 // буферизация с ограниченным объёмом

char TXT_ANSW[] = "I am your student\n";//вывод сообщения на экран
int main() {
char buff[1024]; //объём для записи слов в буфер
if (WSAStartup(0x0202, (WSADATA*)&buff[0])) {
// в if-е мы говорим что будем использовать библиотеку версии 2
// а WSADATA содержит указатель на ту структуру данных с которой мы будем работать через наши сокеты
// в нашем слуаче это наша структура
//buff[0] указ. на строковый буфер в который записыается результат начинаем сначала
cout << "Error WSAStartup \n" << WSAGetLastError(); // вызывает последнюю ошибку которая произошла для вызывающего потока
return -1;
}
SOCKET s; //сокет
int from_len;
char buf[BUF_SIZE] = { 0 };
hostent* hp; // структура для хранения информации о хосте ( его ip адрес, его имя и т.д. )
sockaddr_in clnt_sin, srv_sin; // структуры сокетов (клиент сервер соответственно)
s = socket(AF_INET, SOCK_STREAM, 0); // создаем сокет (domain, type, protocol)
clnt_sin.sin_family = AF_INET;// заполняем структуру клиентского сокета
clnt_sin.sin_addr.s_addr = 0; //Адрес в формате IPV4
clnt_sin.sin_port = htons(CLNT_PORT);//htons - приводит обычный беззначный тип short в сетевой порядок байтов(нужно чтобы сокеты смогли работать на своих уровнях)
bind(s, (sockaddr*)&clnt_sin, sizeof(clnt_sin));// связываем клиентский сокет со структурой
hp = gethostbyname(SRV_HOST); // вернет структуру hostent, если найдет хост с таким именем. Передает имя узла сети в его сетевой адрес (порт сервера)
srv_sin.sin_port = htons(SRV_PORT);// заполнение структуры сокета (преобразовываем значение порта)
srv_sin.sin_family = AF_INET; // указываем что сокет этой структуры принадлежит интернет протоколам версии 4 IPv4
((unsigned long*)&srv_sin.sin_addr)[0] = // устанавливаем адрес сервера через структуру hp которую получили сверху
((unsigned long**)hp->h_addr_list)[0][0]; // через функцию gethostbyname h_addr - адрес узла
connect(s, (sockaddr*)&srv_sin, sizeof(srv_sin));// устанавливаем соединение с сервером.(дескриптор сокета, информация из структуры,)
string mst; // переменная
do {
from_len = recv(s, (char*)&buf, BUF_SIZE, 0); //Функция служит для чтения данных из сокета.
// Первый аргумент - сокет-дескриптор, из которого читаются данные. Второй и третий аргументы - соответственно, адрес и длина буфера для записи читаемых данных. Четвертый параметр - это комбинация битовых флагов, управляющих режимами чтения. Если 4-ый агумент равен нулю, то считанные данные удаляются из сокета. Возвращает управление только после получения информации
buf[from_len] = 0;
cout << buf << endl;
//send(s, (char*)&TXT_ANSW, sizeof(TXT_ANSW), 0); // отправка данных на сервер аргуметы такие же как recv, только если 4-ый равен 0, то запись в сокет происходит в порядке поступления. Возвращает управление независимо от отправки данных
getline(cin, mst);//Функция принимает входной поток и строку в качестве параметров (cin - это консольный ввод) и читает строку текста из потока в строку.
int msg_size = mst.size();
send(s, (char*)&mst[0], msg_size, 0);
} while (mst != "Bye");
cout << "exit to infinity" << endl;
cin.get(); //используется для доступа к массиву символов.
closesocket(s); // отключаем После вызова сокет больше недоступен
return 0;
}