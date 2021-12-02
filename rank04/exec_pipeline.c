/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 22:08:04 by lucocozz          #+#    #+#             */
/*   Updated: 2021/12/02 15:16:37 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "microshell.h"

void	child(t_command *pipeline, char **env)
{
	dup2(pipeline->fd_in, STDIN_FILENO);
	dup2(pipeline->fd_out, STDOUT_FILENO);
	if (execve(pipeline->argv[0], pipeline->argv, env) == -1)
	{
		ft_putstr_fd(STDERR_FILENO, "error: cannot execute ");
		ft_putstr_fd(STDERR_FILENO, pipeline->argv[0]);
		ft_putstr_fd(STDERR_FILENO, "\n");
	}
}

void	parent(t_command *pipeline, pid_t pid)
{
	waitpid(pid, NULL, 0);
	if (pipeline->fd_in != STDIN_FILENO)
		close(pipeline->fd_in);
	if (pipeline->fd_out != STDOUT_FILENO)
		close(pipeline->fd_out);
}

void	exec_pipeline(t_command	*pipeline, char **env)
{
	pid_t	pid;
	int		fd[2];

	while (pipeline != NULL)
	{
		if (strcmp(pipeline->argv[0], "cd") == 0)
			cd(pipeline->argv);
		else
		{
			if (pipeline->next != NULL)
			{
				pipe(fd);
				pipeline->fd_out = fd[STDOUT_FILENO];
				pipeline->next->fd_in = fd[STDIN_FILENO];
			}
			pid = fork();
			if (pid == -1)
			{
				if (pipeline->fd_in != STDIN_FILENO)
					close(pipeline->fd_in);
				if (pipeline->fd_out != STDOUT_FILENO)
					close(pipeline->fd_out);
				free_pipeline(pipeline);
				exit_error(EXIT_FAILURE, "error: fatal\n");
			}
			else if (pid == 0)
				child(pipeline, env);
			else
				parent(pipeline, pid);
		}
		pipeline = pipeline->next;
	}
}
