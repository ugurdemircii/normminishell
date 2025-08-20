/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: udemirci <udemirci@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 20:19:06 by udemirci          #+#    #+#             */
/*   Updated: 2025/08/20 01:15:49 by udemirci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	go_next_quote(char *input, int *i)
{
	char	quote_type;

	quote_type = input[(*i)];
	(*i)++;
	while (input[(*i)] && input[(*i)] != quote_type)
		(*i)++;
	if (input[(*i)] == quote_type)
		(*i)++;
}
