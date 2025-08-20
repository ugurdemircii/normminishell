/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: udemirci <udemirci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 20:20:20 by udemirci          #+#    #+#             */
/*   Updated: 2025/08/19 20:20:21 by udemirci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_redirect_node(t_redirect **list, t_redirect *new_redirect)
{
	t_redirect	*last;

	if (*list == NULL)
	{
		*list = new_redirect;
		return ;
	}
	last = *list;
	while (last->next)
		last = last->next;
	last->next = new_redirect;
}

t_redirect	*create_new_redirect(int type, char *file)
{
	t_redirect	*new_redirect;

	new_redirect = ft_calloc(1, sizeof(t_redirect));
	if (!new_redirect)
		return (NULL);
	new_redirect->type = type;
	new_redirect->file = ft_strdup(file);
	new_redirect->heredoc_fd = 0;
	new_redirect->next = NULL;
	return (new_redirect);
}

void	add_cmd_node(t_cmds **head, t_cmds *new_node)
{
	t_cmds	*tmp;

	if (!head || !new_node)
		return ;
	new_node->next = NULL;
	if (*head == NULL)
	{
		*head = new_node;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
}

void	add_env_node(t_env **head, t_env *new_node)
{
	t_env	*tmp;

	if (!new_node)
		return ;
	if (!*head)
	{
		*head = new_node;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
}

t_env	*create_env_node(char *key, char *value)
{
	t_env	*node;

	node = ft_calloc(1, sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	node->value = ft_strdup(value);
	node->next = NULL;
	return (node);
}
