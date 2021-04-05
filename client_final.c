-http://zetcode.com/db/mysqlc/
-https://stackoverflow.com/questions/3453168/c-program-mysql-connection
-https://profs.info.uaic.ro/~computernetworks/files/NetEx/S12/ServerConcThread/servTcpConcTh2.c
-https://stackoverflow.com/questions/4172538/sending-multiple-messages-via-send-recv-socket-programming-c

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>

extern int errno;
int port=4000;
char username_global[25];
char user_name[25]="a", password[25]="a";

void SignIn(int number)
{
	char de_la_tastatura[1000], *pos, nume[256];
	int ok=0, ok2=0;
	FILE *file_desc;
	
	if(number==1)
	{
		printf ("introduceti parola de control: ");
  		fflush (stdout);
  		read (0, de_la_tastatura, sizeof(de_la_tastatura));
  		if ((pos=strchr(de_la_tastatura, '\n'))!=NULL) *pos = '\0';
  	
  		while (strcmp(de_la_tastatura, "a")!=0)
  		{
  			printf("PAROLA DE CONTROL GRESITA\n");
  			printf("introduceti parola de control: ");
  			fflush (stdout);
  			read (0, de_la_tastatura, sizeof(de_la_tastatura));
  			if ((pos=strchr(de_la_tastatura, '\n'))!=NULL)*pos = '\0';
  		}
  	
  		printf("Se poate crea contul!\n");
  	}
  	
  	while(1)
  	{
  		ok=0; ok2=0;
  		printf("Introduceti usernameul dorit: ");
  		fflush (stdout);
  		read (0, de_la_tastatura, sizeof(de_la_tastatura));
  		if ((pos=strchr(de_la_tastatura, '\n'))!=NULL) *pos = '\0';
  	
  		if(number==1) file_desc = fopen("/home/andreea/Desktop/administratori", "r");
  		else if(number==2) file_desc = fopen("/home/andreea/Desktop/utilizatori_obisnuiti", "r");
  			
  		while(fscanf(file_desc, "%[^\n]\n", nume)!=EOF)
			{if(strcmp(nume, de_la_tastatura)==0) ok=1;}
		
		if (strcmp("", de_la_tastatura)==0)
			{
				printf("INTRODUCETI UN USERNAME!\n");
  				ok2=1;
			}
		if (ok==1) printf("USERNAMEUL EXISTA!\n");
		else
		{
			if(number==1)file_desc = fopen("/home/andreea/Desktop/administratori", "a");
			else if(number==2)file_desc = fopen("/home/andreea/Desktop/utilizatori_obisnuiti", "a");
			fprintf(file_desc, "%s\n", de_la_tastatura);
   			fclose(file_desc);
    		}
    		
    		if(ok==0&&ok2==0) break;
    	
    	}
    	
    	while(1)
    	{
    		ok=0; ok2=0;
    		printf("Introduceti parola dorita: ");
  		fflush (stdout);
  		read (0, de_la_tastatura, sizeof(de_la_tastatura));
  		if ((pos=strchr(de_la_tastatura, '\n'))!=NULL) *pos = '\0';
  	
  		if (strcmp("", de_la_tastatura)==0)
		{
			printf("INTRODUCETI O PAROLA!\n");
  			ok2=1;
		}
  	
  		if(number==1) file_desc = fopen("/home/andreea/Desktop/parole_administratori", "a");
  		else if(number==2) file_desc = fopen("/home/andreea/Desktop/parole_useri", "a");
  		fprintf(file_desc, "%s\n", de_la_tastatura);
  	
  		if(ok==0&&ok2==0) break;
  	}
  	
   	fclose(file_desc);
   	printf("\n	CONT CREAT CU SUCCES!\n\n");
}

void LogIn(int number)
{
	char *pos, nume[256];
	int ok=0;
	FILE *file_desc, *file_desc1;
	
  while(1)
  {
  	char de_la_tastatura[1000];
  	ok=0;
  	printf("Introduceti usernameul: ");
  	fflush (stdout);
  	read (0, de_la_tastatura, sizeof(de_la_tastatura));
  	if ((pos=strchr(de_la_tastatura, '\n'))!=NULL) *pos = '\0';
  	
  	if(number==1) 
		{
			file_desc=fopen("/home/andreea/Desktop/administratori", "r");
			file_desc1=fopen("/home/andreea/Desktop/parole_administratori", "r");
		}
	else if(number==2) 
		{
			file_desc=fopen("/home/andreea/Desktop/utilizatori_obisnuiti", "r");
			file_desc1=fopen("/home/andreea/Desktop/parole_useri", "r");
		}
		
  	while(fscanf(file_desc, "%[^\n]\n", nume)!=EOF)
		{if(strcmp(nume, de_la_tastatura)==0) ok=1;}
		
	if (ok==0)
		printf("USERNAMEUL NU EXISTA IN BAZA DE DATE!\n");
	if (ok==1) 
		{	
			strcpy(username_global, de_la_tastatura);
			break;
		}
  }
    		
   while(1)
   {
   	char de_la_tastatura[1000];
   	ok=0;
    	printf("Introduceti parola: ");
  	fflush (stdout);
  	read (0, de_la_tastatura, sizeof(de_la_tastatura));
  	if ((pos=strchr(de_la_tastatura, '\n'))!=NULL) *pos = '\0';
  	
  	if(number==1) 
		{
			file_desc=fopen("/home/andreea/Desktop/administratori", "r");
			file_desc1=fopen("/home/andreea/Desktop/parole_administratori", "r");
		}
	else if(number==2) 
		{
			file_desc=fopen("/home/andreea/Desktop/utilizatori_obisnuiti", "r");
			file_desc1=fopen("/home/andreea/Desktop/parole_useri", "r");
		}
  	
  	
  	while(fscanf(file_desc1, "%[^\n]\n", nume)!=EOF)
		{if(strcmp(nume, de_la_tastatura)==0) ok=1;}
		
	
	if (ok==0)
		printf("PAROLA INCORECTA!\n");
	else 
		break;
   }
   fclose(file_desc);
   fclose(file_desc1);
   
   if(number==1)printf("\n	ADMINISTRATOR LOGAT CU SUCCES!\n\n");
   else if(number==2)printf("\n	USER LOGAT CU SUCCES!\n\n");
}


int AddSong(int sd, char command[256])
{
	if (send (sd, command, 256, 0) <= 0)
    	{
      		perror ("[client]Eroare la write() spre server.\n");
      		return errno;
    	}

  	char name[256];
  	printf("name: ");
  	fflush (stdout);
  	read (0, name, sizeof(name));fflush (stdout);
  	
  	if (send (sd, name, sizeof(name), 0) <= 0)
    	{
      		perror ("[client]Eroare la write() spre server.\n");
      		return errno;
    	}
    	
  	char description[256];
  	printf("some words about it: ");
  	fflush (stdout);
  	read (0, description, sizeof(description));
  	
  	if (send (sd, description, sizeof(description), 0) <= 0)
    	{
      		perror ("[client]Eroare la write() spre server.\n");
      		return errno;
    	}
    	
  	char genre[256];
  	printf("genuri: ");
  	fflush (stdout);
  	read (0, genre, sizeof(genre));
  	
  	if (send (sd, genre, sizeof(genre), 0) <= 0)
    	{
      		perror ("[client]Eroare la write() spre server.\n");
      		return errno;
    	}
    	
  	
  	char link[256];
  	printf("YouTube link: ");
  	fflush (stdout);
  	read (0, link, sizeof(link));
  	
  	if (send (sd, link, sizeof(link), 0) <= 0)
    	{
      		perror ("[client]Eroare la write() spre server.\n");
      		return errno;
    	}
    	
    	printf("\nSONG ADDED SUCCESFULLY\n");
}

int voteSong(int sd, char command[256])
{
	if (send (sd, command, 256, 0) <= 0)
    	{
      		perror ("[client]Eroare la write() spre server.\n");
      		return errno;
    	}
    	
    	char song_name[256];
    	
  	printf("song name: ");
  	fflush (stdout);
  	read (0, song_name, sizeof(song_name));
  	fflush (stdout);
  	
  	if (send (sd, song_name, sizeof(song_name), 0) <= 0)
    	{
      		perror ("[client]Eroare la write() spre server.\n");
      		return errno;
    	}
    	
    	printf("\nSONG VOTED SUCCESFULLY\n");
    	
}


int generalTop(int sd, char command[256])
{
	int in;
	if (send (sd, command, 256, 0) <= 0)
    	{
      		perror ("[client]Eroare la send() spre server.\n");
      		return errno;
    	}
    
    	/*while(read (sd, command, 256) > 0)
    	{
      		printf("\n%s\n", command);
    	}*/
    	
    	printf("\nGENERAL TOP ADDED IN FILE\n");
    	
}

int genreTop(int sd, char command[256])
{
	char *pos;
  	if (send (sd, command, 256, 0) <= 0)
    	{
      		perror ("[client]Eroare la write() spre server.\n");
      		return errno;
    	}
    	
    	char genre[256];
    	
  	printf("genre: ");
  	fflush (stdout);
  	read (0, genre, sizeof(genre));
  	if ((pos=strchr(genre, '\n'))!=NULL) *pos = '\0';
  	fflush (stdout);
  	
  	if (send (sd, genre, sizeof(genre), 0) <= 0)
    	{
      		perror ("[client]Eroare la write() spre server.\n");
      		return errno;
    	}
    	
    	printf("\nGENRE TOP -%s- ADDED IN FILE\n", genre);
}

int addComment(int sd, char command[256])
{
	int ok=0;
	char nume[25];
	FILE *file_desc;
	file_desc=fopen("/home/andreea/Desktop/restrictVote", "r");
	
	while(fscanf(file_desc, "%[^\n]\n", nume)!=EOF)
		{if(strcmp(nume, username_global)==0) ok=1;}
		
	if (ok==1) printf("COMMENT RESTRICTIONAT!\n");
	
	else
	{	
	  	if (send (sd, command, 256, 0) <= 0)
	    	{
	      		perror ("[client]Eroare la write() spre server.\n");
	      		return errno;
	    	}
	    	
	    	char comment[256], song_name[25];
	    	
	    	printf("song to add the comment to: ");
	  	fflush (stdout);
	  	read (0, song_name, sizeof(song_name));
	  	fflush (stdout);
	  	
	  	if (send (sd, song_name, sizeof(song_name), 0) <= 0)
	    	{
	      		perror ("[client]Eroare la write() spre server.\n");
	      		return errno;
	    	}
	  	
	  	printf("comment to add: ");
	  	fflush (stdout);
	  	read (0, comment, sizeof(comment));
	  	fflush (stdout);
	  	
	    	
	    	if (send (sd, comment, sizeof(comment), 0) <= 0)
	    	{
	      		perror ("[client]Eroare la write() spre server.\n");
	      		return errno;
	    	}
	    	printf("\nCOMMENT ADDED SUCCESFULLY\n");}
    	
}

int removeSong(int sd, char command[256])
{
	if (send (sd, command, 256, 0) <= 0)
    	{
      		perror ("[client]Eroare la write() spre server.\n");
      		return errno;
    	}
    	
    	char song_name[25];
    	
    	printf("song to remove: ");
  	fflush (stdout);
  	read (0, song_name, sizeof(song_name));
  	fflush (stdout);
  	
  	if (send (sd, song_name, sizeof(song_name), 0) <= 0)
    	{
      		perror ("[client]Eroare la write() spre server.\n");
      		return errno;
    	}
    	printf("\nSONG REMOVED SUCCESFULLY\n");
}

int restrictVote(int sd, char command[256])
{
    	char user_name[25];
    	FILE *file_desc;
    	
    	printf("user to restrict the vote: ");
  	fflush (stdout);
  	read (0, user_name, sizeof(user_name));
  	fflush (stdout);
  	
  	file_desc = fopen("/home/andreea/Desktop/restrictVote", "a");
  	fprintf(file_desc, "%s", user_name);
   	fclose(file_desc);
   	printf("\nVOTE RESTRICTED SUCCESFULLY\n");
  	
}
int main (int argc, char *argv[])
{
  int sd;			// descriptorul de socket
  struct sockaddr_in server;	// structura folosita pentru conectare 

  char command[256], username[25], password[25], buf[10], buffer[100], *pos;
  FILE *admin, *user;	

  /* cream socketul */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_port = htons (port);
  
  /* ne conectam la server */
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Eroare la connect().\n");
      return errno;
    }

  printf("Administratorul este rugat sa se logheze:\n");
  printf("	username: ");
  fflush (stdout);
  read (0, username, sizeof(username));
  if ((pos=strchr(username, '\n'))!=NULL) *pos = '\0';
    		
  while (strcmp(username, user_name)!=0)
  {
  	printf("USERNAME GRESIT\n");
  	printf("	username: ");
  	fflush (stdout);
  	read (0, username, sizeof(username));
  	if ((pos=strchr(username, '\n'))!=NULL)
    		*pos = '\0';
  }
  
  
  printf("	password: ");
  fflush (stdout);
  read (0, password, sizeof(password));
  if ((pos=strchr(password, '\n'))!=NULL)
    		*pos = '\0';
    		
  while (strcmp(password, password)!=0)
  {
  	printf("PAROLA GRESITA\n");
  	printf("	password: ");
  	fflush (stdout);
  	read (0, password, sizeof(password));
  	if ((pos=strchr(password, '\n'))!=NULL)
    		*pos = '\0';
  }
  
  //in acest punct, administratorul ar trebui sa fie conectat
  
  while(1)
  {
  printf ("Choose your action:\n	1.creeaza cont administrator\n	2.creeaza cont utilizator obisnuit\n	3.logheaza-te ca administrator\n	4.logheaza-te ca utilizator obisnuit\nAleg: ");
  
  fflush (stdout);
  read (0, command, sizeof(command));
  
  if ((pos=strchr(command, '\n'))!=NULL) *pos = '\0';
    		
  printf ("optiunea aleasa: %s\n", command);
  fflush (stdout);
  
  char de_la_tastatura[256], nume[25];
  
  //1.creeaza cont administrator
  if (strcmp(command, "1")==0)
  {
  	SignIn(1);
  	
  	printf ("Choose your action:\n	1.logheaza-te ca administrator\n	2.logheaza-te ca utilizator obisnuit\nAleg: ");
  fflush (stdout);
  read (0, command, sizeof(command));
  
  if ((pos=strchr(command, '\n'))!=NULL) *pos = '\0';
    		
  printf ("optiunea aleasa: %s\n", command);
  fflush (stdout);
  
  if (strcmp(command, "1")==0) 
  {LogIn(1);
  
  printf ("Choose your action:\n	6.remove song\n	7.remove vote option\nAleg: ");
  fflush (stdout);
  read (0, command, sizeof(command));
  
  if ((pos=strchr(command, '\n'))!=NULL)
    		*pos = '\0';
    		
  printf ("optiunea aleasa: %s\n", command);
  fflush (stdout);
  
  if (strcmp(command, "6")==0) removeSong(sd, command);

  if (strcmp(command, "7")==0) restrictVote(sd, command);
  }
  if (strcmp(command, "2")==0) 
  {LogIn(2);
  
  printf ("Choose your action:\n	1.add song\n	2.vote song\n	3.display overall top\n	4.display genre top\n	5.post a comment\nAleg: ");
  fflush (stdout);
  read (0, command, sizeof(command));
  
  if ((pos=strchr(command, '\n'))!=NULL)
    		*pos = '\0';
    		
  printf ("optiunea aleasa: %s\n", command);
  fflush (stdout);
  
  if (strcmp(command, "1")==0) AddSong(sd, command);
  
  if (strcmp(command, "2")==0) voteSong(sd, command);
  
  if (strcmp(command, "3")==0) generalTop(sd, command);
  
  if (strcmp(command, "4")==0) genreTop(sd, command);
  
  if (strcmp(command, "5")==0) addComment(sd, command);
  }
  
  }
  
  //2.creeaza cont utilizator obisnuit
  else if (strcmp(command, "2")==0)
  {
  	SignIn(2);
  	
   	printf ("Choose your action:\n	1.logheaza-te ca administrator\n	2.logheaza-te ca utilizator obisnuit\nAleg: ");
  fflush (stdout);
  read (0, command, sizeof(command));
  
  if ((pos=strchr(command, '\n'))!=NULL) *pos = '\0';
    		
  printf ("optiunea aleasa: %s\n", command);
  fflush (stdout);
  
  if (strcmp(command, "1")==0) 
  {LogIn(1);
  
  printf ("Choose your action:\n	6.remove song\n	7.remove vote option\nAleg: ");
  fflush (stdout);
  read (0, command, sizeof(command));
  
  if ((pos=strchr(command, '\n'))!=NULL)
    		*pos = '\0';
    		
  printf ("optiunea aleasa: %s\n", command);
  fflush (stdout);
  
  
  if (strcmp(command, "6")==0) removeSong(sd, command);
  
  if (strcmp(command, "7")==0) restrictVote(sd, command);

  }
  if (strcmp(command, "2")==0) 
  {LogIn(2);
  
  printf ("Choose your action:\n	1.add song\n	2.vote song\n	3.display overall top\n	4.display genre top\n	5.post a comment\nAleg: ");
  fflush (stdout);
  read (0, command, sizeof(command));
  
  if ((pos=strchr(command, '\n'))!=NULL)
    		*pos = '\0';
    		
  printf ("optiunea aleasa: %s\n", command);
  fflush (stdout);
  
  
  if (strcmp(command, "1")==0) AddSong(sd, command);
  
  if (strcmp(command, "2")==0) voteSong(sd, command);
  
  if (strcmp(command, "3")==0) generalTop(sd, command);
  
  if (strcmp(command, "4")==0) genreTop(sd, command);
  
  if (strcmp(command, "5")==0) addComment(sd, command);
  
  }
  
  }	
  
  //3.logheaza-te ca administrator
  else if (strcmp(command, "3")==0)
  {
 	LogIn(1);
 	
 	printf ("Choose your action:\n	6.remove song\n	7.remove vote option\nAleg: ");
  fflush (stdout);
  read (0, command, sizeof(command));
  
  if ((pos=strchr(command, '\n'))!=NULL)
    		*pos = '\0';
    		
  printf ("optiunea aleasa: %s\n", command);
  fflush (stdout);
  
  
  if (strcmp(command, "6")==0) removeSong(sd, command);
  
  if (strcmp(command, "7")==0) restrictVote(sd, command);
  
  }
  
  //3.logheaza-te ca utilizator obisnuit
  else if (strcmp(command, "4")==0)
  {
	LogIn(2);
	
	printf ("Choose your action:\n	1.add song\n	2.vote song\n	3.display overall top\n	4.display genre top\n	5.post a comment\nAleg: ");
  fflush (stdout);
  read (0, command, sizeof(command));
  
  if ((pos=strchr(command, '\n'))!=NULL)
    		*pos = '\0';
    		
  printf ("optiunea aleasa: %s\n", command);
  fflush (stdout);
  
  char aux[256];
  if (strcmp(command, "1")==0) AddSong(sd, command); 
  
  if (strcmp(command, "2")==0) voteSong(sd, command);
  
  if (strcmp(command, "3")==0) generalTop(sd, command);

  if (strcmp(command, "4")==0) genreTop(sd, command);
  
  if (strcmp(command, "5")==0) addComment(sd, command);
    
  }
  
  else {printf("CHOOSE AN ACTION!\n");}

  if (strcmp(command, "4")==0||strcmp(command, "3")==0||strcmp(command, "2")==0||strcmp(command, "1")==0||strcmp(command, "5")==0||strcmp(command, "6")==0||strcmp(command, "7")==0) break;
  
  }

  close (sd);
}