#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H
int startTCPClient();
int verify_username(int socket_fd,char username[]);
char * generateH1(int socket_fd, char R[]);
int verify_hash_H1(int socket_fd, char H1[]);
char * prompt_and_read(char * prompt);
int sizecheck(char array1[], char array2[]);
#endif