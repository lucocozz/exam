/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/30 19:29:08 by lucocozz          #+#    #+#             */
/*   Updated: 2021/12/01 10:29:08 by lucocozz         ###   ########.fr       */
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

typedef enum e_token
{
	Pipe,
	Semicolon,
	Command
}	t_token;


typedef struct s_node
{
	t_token			type;
	struct s_node	*left;
	struct s_node	*right;
	char			**command;
}					t_node;

int		ft_strlen(char *str);
void	ft_putstr_fd(int fd, char *str);

void	exit_error(int status, char *str);
t_node	*build_ast(int size, char **arg);


#endif