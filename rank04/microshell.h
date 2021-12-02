/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 19:29:08 by lucocozz          #+#    #+#             */
/*   Updated: 2021/12/01 22:25:58 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MICROSHELL_H
# define MICROSHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>

typedef struct s_command
{
	int					fd_in;
	int					fd_out;
	char				**argv;
	struct s_command	*next;
}						t_command;

int			ft_strlen(char *str);
void		ft_putstr_fd(int fd, char *str);
void		exit_error(int status, char *str);
t_command	*parse_input(int argc, char **argv, int *i);
void		free_pipeline(t_command *pipeline);
void		exec_pipeline(t_command	*pipeline, char **env);
void		cd(char **argv);

#endif