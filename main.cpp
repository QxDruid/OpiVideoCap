/* Подключение библиотек для демонизации */
#include <fcntl.h> 
#include <errno.h> 
#include <unistd.h> 
#include <syslog.h> 
#include <sys/types.h> 
#include <sys/stat.h> 

#include "AlarmClient.h"
#include <string>
#include <sstream>
#include <fstream>

#define CONFIG "/etc/alarm-client/config"
#define PIDFILE "/var/run/alarm-client.pid"
#define LOGFILE "/etc/alarm-client/log"

void savepid()
{
	std::ofstream pidf;
	pidf.open(PIDFILE,std::ios_base::out);
	pidf << getpid();
	pidf.close();
}


int log_it(std::string data)
{
	std::ofstream log;
	log.open(LOGFILE,std::ios_base::out);
	log << data << std::endl;
	log.close();
}

int WorkProc(void)
{
	std::ifstream config;
	
	config.open(CONFIG);
	if(!config.is_open())
	{
 		log_it("... Config can not opened");
 		exit(EXIT_FAILURE);
	}
 	std::string arg;
 	std::getline(config, arg);
 	config.close();

 	AlarmClient client;
 	client.start(arg);

 	log_it("... Some Error (Most real is: server offline). app Closed");
 	exit(EXIT_FAILURE);
}


int main(int argc,char ** argv)
{
	log_it("... Start");

	pid_t pid, sid;

/* Ответвляемся от родительского процесса */
pid = fork();
if(pid < 0)
{
	log_it("... fork failed");
	exit(EXIT_FAILURE);
}
if (pid > 0)
{
	log_it("... fork success");
	exit(EXIT_SUCCESS);
}


/* Изменяем файловую маску */
umask(0);
/* Создание нового SID для дочернего процесса */
sid = setsid();
if (sid < 0) {
  log_it("... sid failure");
  exit(EXIT_FAILURE);
}

// сохраняем pid нашего процесса чтоб потом управлять им
savepid(); //  <<---------------------------------------------------------------

/* Изменяем текущий рабочий каталог */
if ((chdir("/")) < 0) {
  log_it("... chdir failure");
  exit(EXIT_FAILURE);
}

/* Закрываем стандартные файловые дескрипторы 
close(STDIN_FILENO);
close(STDOUT_FILENO);
close(STDERR_FILENO);
*/

	/* запуск рабочепй программы */
	WorkProc();

    exit(EXIT_SUCCESS);
}

