#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>
#include <strings.h>

typedef struct s_client
{
	int fd;
	int id;
	struct s_client *next;
} t_client;

void exit_error(const char *msg)
{
	write(STDERR_FILENO, msg, strlen(msg));
	exit(EXIT_FAILURE);
}

int create_server(char *port)
{
	int server_fd;
	struct sockaddr_in servaddr;

	// socket create and verification
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1)
		exit_error("Fatal error\n");
	bzero(&servaddr, sizeof(servaddr));
	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(2130706433); // 127.0.0.1
	servaddr.sin_port = htons(atoi(port));
	// Binding newly created socket to given IP and verification
	if ((bind(server_fd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
		exit_error("Fatal error\n");
	if (listen(server_fd, 10) != 0)
		exit_error("Fatal error\n");
	return (server_fd);
}

void free_clients(t_client *clients)
{
	t_client *to_free;

	while (clients != NULL)
	{
		to_free = clients;
		clients = clients->next;
		close(to_free->fd);
		free(to_free);
	}
}

t_client *create_client(int fd, int id)
{
	t_client *client;

	client = malloc(sizeof(t_client));
	if (client == NULL)
		return (NULL);
	client->fd = fd;
	client->id = id;
	client->next = NULL;
	return (client);
}

void broadcast(t_client *clients, int id_broadcast, char *msg)
{
	while (clients != NULL)
	{
		if (clients->id != id_broadcast)
			send(clients->fd, msg, strlen(msg), 0);
		clients = clients->next;
	}
}

void accept_connection(t_client **clients, int server_fd, int id)
{
	char msg[100];
	int connfd;
	unsigned int addrlen;
	struct sockaddr_in cli;
	t_client *new_client;

	addrlen = sizeof(cli);
	connfd = accept(server_fd, (struct sockaddr *)&cli, &addrlen);
	if (connfd < 0)
	{
		free_clients(*clients);
		exit_error("Fatal error\n");
	}
	sprintf(msg, "server: client %d just arrived\n", id);
	broadcast(*clients, -1, msg);
	new_client = create_client(connfd, id);
	if (new_client == NULL)
	{
		free_clients(*clients);
		exit_error("Fatal error\n");
	}
	if (*clients == NULL)
		*clients = new_client;
	else
	{
		new_client->next = *clients;
		*clients = new_client;
	}
}

void remove_client(t_client **clients, int id_remove)
{
	t_client *prev;
	t_client *to_remove;

	to_remove = *clients;
	if (to_remove != NULL && to_remove->id == id_remove)
	{
		*clients = to_remove->next;
		close(to_remove->fd);
		free(to_remove);
	}
	else
	{
		while (to_remove != NULL && to_remove->id != id_remove)
		{
			prev = to_remove;
			to_remove = to_remove->next;
		}
		if (to_remove != NULL)
		{
			prev->next = to_remove->next;
			close(to_remove->fd);
			free(to_remove);
		}
	}
}

int main(int argc, char **argv)
{
	char data[4096];
	char msg[4196];
	int server_fd;
	int last_id = 0;
	int bytes_recv;
	fd_set read_fds;
	t_client *clients = NULL;
	t_client *current = NULL;

	if (argc != 2)
		exit_error("Wrong number of arguments\n");
	server_fd = create_server(argv[1]);
	while (1)
	{
		FD_ZERO(&read_fds);
		FD_SET(server_fd, &read_fds);
		for (current = clients; current != NULL; current = current->next)
			FD_SET(current->fd, &read_fds);
		if (select(FD_SETSIZE, &read_fds, NULL, NULL, NULL) < 0)
		{
			free_clients(clients);
			exit_error("Fatal error\n");
		}
		if (FD_ISSET(server_fd, &read_fds))
		{
			accept_connection(&clients, server_fd, last_id);
			last_id++;
		}
		current = clients;
		while (current != NULL)
		{
			if (FD_ISSET(current->fd, &read_fds))
			{
				bytes_recv = recv(current->fd, data, 4096, 0);
				if (bytes_recv < 1)
				{
					t_client *tmp;

					tmp = current;
					current = current->next;
					sprintf(msg, "server: client %d just left\n", tmp->id);
					remove_client(&clients, tmp->id);
					broadcast(clients, -1, msg);
				}
				else
				{
					sprintf(msg, "client %d: %s", current->id, data);
					broadcast(clients, current->id, msg);
					current = current->next;
				}
			}
			else
				current = current->next;
		}
		bzero(msg, 4196);
		bzero(data, 4096);
	}
	free_clients(clients);
}