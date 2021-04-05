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
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <mysql.h>
#include <time.h>

/* portul folosit */
#define PORT 4000

/* codul de eroare returnat de anumite apeluri */
extern int errno;

typedef struct song_attributes{
	int id; 
	int client;
	char name[25];
	char descrip[256]; 
	char genres[256];
	char link[256];
	char command[25];
	char song_to_vote[25];
	char genre[25];
	char comment[256];
}song_attributes;

static void *treat(void *);

char *server = "localhost";
char *user = "andreea";
char *password = "proiectlaretele"; /* set me first */
char *database = "top_music";
	
struct MYSQL * connection()
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	
	conn = mysql_init(NULL);
	char interrogation[500];
	
	if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
	return conn;
}
	
//addSong adauga o melodie in baza de date
void addSong(int id, char name[25], char descrip[256], char genres[256], char link[256], int client)
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	conn = connection();
	
	char interrogation[1000], *token;
	
	if (strchr(genres, ',')!=NULL)
		{
   			token = strtok(genres, ",");
   		 	while( token != NULL ) 
   		 	{
      			sprintf (interrogation, "insert into songs values(%d, '%s', '%s', '%s', '%s', %d, '%s')", id, name, descrip, token, link, 0, "");
			if (mysql_query(conn, interrogation)) 
				{
					fprintf(stderr, "%s\n", mysql_error(conn));
					exit(1);
				}
      			token = strtok(NULL, ",");
   			}
   		}
   	else
   		{
   			sprintf (interrogation, "insert into songs values(%d, '%s', '%s', '%s', '%s', %d, '%s')", id, name, descrip, genres, link, 0, "");
			if (mysql_query(conn, interrogation)) 
				{
					fprintf(stderr, "%s\n", mysql_error(conn));
					exit(1);
				}
   		}
	
	//write(client, "andreea", sizeof("andreea"));
	printf("song added\n");
	
	mysql_free_result(res);
	mysql_close(conn);
 
}

//voteSong creste cu 1 numarul de voturi al melodiei care se vrea a fi votata
void voteSong(char song_to_vote[25])
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	conn = connection();

	char interrogation1[256], interrogation2[256];
	int nr_voturi;

	sprintf (interrogation1, "select nr_votes from songs where name='%s'", song_to_vote);
	if (mysql_query(conn, interrogation1)) 
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
	
	res = mysql_use_result(conn);
   
	while ((row = mysql_fetch_row(res)) != NULL)
		nr_voturi=atoi(row[0]);
		
	sprintf (interrogation2, "update songs set nr_votes=%d where name='%s'", nr_voturi+1, song_to_vote);
	if (mysql_query(conn, interrogation2)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
	
	printf("song voted\n");
	
	mysql_free_result(res);
	mysql_close(conn);
 
}

//generalTop adauga intr-un file topul tuturor melodiilor din baza de date stabilit pe baza numarul de voturi, la momentul rularii comenzii
int generalTop(int client)
{	
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	FILE *file_desc;
	time_t rawtime;
	conn=connection();
  
  	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	if (mysql_query(conn, "select * from songs order by nr_votes desc")) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
	
	res = mysql_use_result(conn);
	int position=1;
	file_desc=fopen("/home/andreea/Desktop/top", "a");
	fprintf(file_desc, "\n\n General top updated on %s --------------------------------------------\n", asctime (timeinfo));
	
	while ((row = mysql_fetch_row(res)) != NULL)
		{fprintf(file_desc, "%d - %s\n", position, row[1]);
		position++;}
	fclose(file_desc);
	/*if (write (client, "andreea", sizeof("andreea")) <= 0)
    		{perror ("[Thread]Eroare la send() catre client.\n");}*/
	
	printf("top added\n");
	
	mysql_free_result(res);
	mysql_close(conn);
 
}

//genreTop adauga intr-un file topul tuturor melodiilor din baza de date, apartinand unui anumit gen muzical, stabilit pe baza numarul de voturi, la momentul rularii comenzii
void genreTop(char genre[25])
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	FILE *file_desc;
	time_t rawtime;
	conn=connection();
  
  	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	
	char interrogation[256];
	
	sprintf (interrogation, "select * from songs where genre='%s' order by nr_votes desc", genre);
	if (mysql_query(conn, interrogation)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
	
	res = mysql_use_result(conn);
	int position=1;
	file_desc=fopen("/home/andreea/Desktop/top", "a");
	fprintf(file_desc, "\n\n Genre top updated on %s -%s-\n --------------------------------------------\n", asctime (timeinfo), genre);
	while ((row = mysql_fetch_row(res)) != NULL)
		{fprintf(file_desc, "%d - %s\n", position, row[1]);
		position++;}
    	fclose(file_desc);
	
	printf("top added\n");
	
	mysql_free_result(res);
	mysql_close(conn);
 
}

//addComment adauga un comentariu in coloana "comments" din tabelul cu melodii
void addComment(char comment[256], char name[25])
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	conn = connection();
	
	char interrogation[500];
	
	sprintf (interrogation, "update songs set comments='%s' where name='%s';", comment, name);

	if (mysql_query(conn, interrogation)) 
	{
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}

	printf("comment added\n");
	
	mysql_free_result(res);
	mysql_close(conn);
 
}

//removeSong sterge o melodie din baza de date
void removeSong(char name[25])
{
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	conn = connection();
	
	char interrogation[500];
	
	sprintf (interrogation, "delete from songs where name='%s'", name);

	if (mysql_query(conn, interrogation)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}

	printf("song removed\n");
	
	mysql_free_result(res);
	mysql_close(conn);
 
}

int main ()
{
  struct sockaddr_in server;
  struct sockaddr_in from;	
  int sd, pid, i=0, on=1;

  pthread_t th[100];

  /* crearea unui socket */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[server]Eroare la socket().\n");
      return errno;
    }
    
  /* utilizarea optiunii SO_REUSEADDR */
  setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
  
  /* pregatirea structurilor de date */
  bzero (&server, sizeof (server));
  bzero (&from, sizeof (from));

    server.sin_family = AF_INET;	
    server.sin_addr.s_addr = htonl (INADDR_ANY);
    server.sin_port = htons (PORT);
  
  /* atasam socketul */
  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("[server]Eroare la bind().\n");
      return errno;
    }

  /* punem serverul sa asculte daca vin clienti sa se conecteze */
  if (listen (sd, 2) == -1)
    {
      perror ("[server]Eroare la listen().\n");
      return errno;
    }
    
  /* servim in mod concurent clientii...folosind thread-uri */
  while (1)
    {
      int client, nr_crt, length = sizeof (from);
      
      song_attributes * args;
      args=(struct song_attributes*)malloc(sizeof(struct song_attributes));
      
      MYSQL *conn;
      MYSQL_RES *res;
      MYSQL_ROW row;
      
      char *server = "localhost";
      
      printf ("[server]Asteptam la portul %d...\n",PORT);
      fflush (stdout);

	conn = mysql_init(NULL);
	
	/* Connect to database */
	if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
	if (mysql_query(conn, "select count(*) from songs")) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}
   
	res = mysql_use_result(conn);
   	nr_crt = atoi(mysql_fetch_row(res)[0])+1;
   	args->id=nr_crt;

      /* acceptam un client (stare blocanta pina la realizarea conexiunii) */
      if ( (client = accept (sd, (struct sockaddr *) &from, &length)) < 0)
	{
	  perror ("[server]Eroare la accept().\n");
	  continue;
	}

	args->client=client;
	
	pthread_create(&th[i], NULL, &treat, args);	      
				
	}//while    
};			

		
static void *treat(void * arguments)
{		
      	struct song_attributes args;
	args = *((struct song_attributes *)arguments);
		
	int in, index = 0, messageLength = 160;
	char *pos, message[20][256];
	
	while((in=recv (args.client, message[index], sizeof(message[index]), 0))>0)
	{
		if ((pos=strchr(message[index], '\n'))!=NULL)
    		*pos = '\0';
		//printf("%s\n", message[index]);
		index++;
	}
	
	strcpy(args.command, message[0]);
	if (strcmp(args.command, "1")==0)
	{	
		strcpy(args.name, message[1]);
		strcpy(args.descrip, message[2]);
		strcpy(args.genres, message[3]);
		strcpy(args.link, message[4]);
		
	}
	
	if (strcmp(args.command, "2")==0)
	{	
		strcpy(args.song_to_vote, message[1]);
			
	}
	
	if (strcmp(args.command, "4")==0)
	{	
		strcpy(args.genre, message[1]);
			
	}
	
	if (strcmp(args.command, "5")==0)
	{	
		strcpy(args.name, message[1]);
		strcpy(args.comment, message[2]);		
	}
	
	if (strcmp(args.command, "6")==0)
	{	
		strcpy(args.name, message[1]);		
	}

		
	if (strcmp(args.command, "1")==0)	
		{addSong(args.id, args.name, args.descrip, args.genres, args.link, args.client);}
		
	if (strcmp(args.command, "2")==0)	
		{voteSong(args.song_to_vote);}

	if (strcmp(args.command, "3")==0)	
		{generalTop(args.client);}
		
	if (strcmp(args.command, "4")==0)	
		{genreTop(args.genre);}
		
	if (strcmp(args.command, "5")==0)	
		{addComment(args.comment, args.name);}
		
	if (strcmp(args.command, "6")==0)	
		{removeSong(args.name);}
	
	/* am terminat cu acest client, inchidem conexiunea */
	close ((intptr_t)arguments);
	return(NULL);	
	
	pthread_detach(pthread_self());
  		
};



