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

int id = 0;

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

int init_server(char *port)
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

void broadcast(t_client *clients, int fd_broadcast, char *msg)
{
	while (clients != NULL)
	{
		if (clients->fd != fd_broadcast)
			send(clients->fd, msg, strlen(msg), 0);
		clients = clients->next;
	}
}

void accept_connection(t_client **clients, int server_fd)
{
	char msg[100];
	int client_fd;
	unsigned int addrlen;
	struct sockaddr_in cli;
	t_client *new_client;

	addrlen = sizeof(cli);
	client_fd = accept(server_fd, (struct sockaddr *)&cli, &addrlen);
	if (client_fd < 0)
	{
		free_clients(*clients);
		exit_error("Fatal error\n");
	}
	new_client = create_client(client_fd, id);
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
	sprintf(msg, "server: client %d just arrived\n", id++);
	broadcast(*clients, client_fd, msg);
}

void remove_client(t_client **clients, int fd_remove)
{
	char msg[100] = {0};
	t_client *prev = NULL;
	t_client *to_remove = *clients;

	while (to_remove != NULL && to_remove->fd != fd_remove)
	{
		prev = to_remove;
		to_remove = to_remove->next;
	}
	if (prev != NULL)
		prev->next = to_remove->next;
	else
		*clients = to_remove->next;
	sprintf(msg, "server: client %d just left\n", to_remove->id);
	close(to_remove->fd);
	free(to_remove);
	broadcast(*clients, -1, msg);
}

int get_line_size(char *str)
{
	int i = 0;

	while (str[i] != '\n' && str[i] != '\0')
		i++;
	return (i);
}

void send_data(t_client *clients, char *data, int isset_fd)
{
	int line_size = 0;
	char *src, *dest;
	char msg[50000] = {0};
	t_client *c;

	src = data;
	dest = msg;
	c = clients;
	while (c != NULL && c->fd != isset_fd)
		c = c->next;
	while (src[(line_size = get_line_size(src))] != '\0')
	{
		if (src[line_size] == '\n')
		{
			src[line_size] = '\0';
			line_size++;
		}
		dest += sprintf(dest, "client %d: %s\n", c->id, src);
		src += line_size;
	}
	broadcast(clients, isset_fd, msg);
}

int main(int argc, char **argv)
{
	int setsize;
	int server_fd;
	int bytes_recv = 0;
	char data[500000] = {0};
	fd_set read_fds;
	t_client *clients = NULL;

	if (argc != 2)
		exit_error("Wrong number of arguments\n");
	server_fd = init_server(argv[1]);
	while (1)
	{
		FD_ZERO(&read_fds);
		setsize = server_fd;
		FD_SET(server_fd, &read_fds);
		for (t_client *current = clients; current != NULL; current = current->next)
		{
			if (current->fd > setsize)
				setsize = current->fd;
			FD_SET(current->fd, &read_fds);
		}
		if (select(setsize + 1, &read_fds, NULL, NULL, NULL) < 0)
		{
			free_clients(clients);
			exit_error("Fatal error\n");
		}
		for (int isset_fd = 0; isset_fd <= setsize; isset_fd++)
		{
			if (FD_ISSET(isset_fd, &read_fds))
			{
				if (isset_fd == server_fd)
					accept_connection(&clients, server_fd);
				else if ((bytes_recv = recv(isset_fd, data, 500000, 0)) > 0)
					send_data(clients, data, isset_fd);
				else
					remove_client(&clients, isset_fd);
				bzero(data, bytes_recv);
			}
		}
	}
	free_clients(clients);
}