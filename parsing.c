/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlorette <tlorette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 11:12:49 by frogus            #+#    #+#             */
/*   Updated: 2025/12/04 10:27:47 by tlorette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

static bool	is_space(char c)
{
	return (c >= 9 && c <= 13 || c == 32);
}

static char	*valid_input(char *str)
{
	int		len;
	char	*number;

	len = 0;
	while (is_space(*str))
		str++;
	if (*str == '+')
		++str;
	else if (*str == '-')
		print_error("not a positive number");
	if (!is_digit)
		print_error("not a valid digit");
	number = str;
	while (is_digit(*str++))
		len++;
	if (len > 10)
		print_error("INT MAX BUSTED");
	return (number);
}

static long	ft_atol(char *str)
{
	long	num;

	num = 0;
	str = valid_input(str);
	while (is_digit(*str))
		num = (num * 10) + (*str++ - 48);
	if (num > INT_MAX)
		print_error("INT MAX BUSTED");
	return (num);
}

void	parse_input(t_table *table, char **av)
{
	table->philo_nbr = ft_atol(av[1]);
	table->time_to_die = ft_atol(av[2]) * 1e3;
	table->time_to_eat = ft_atol(av[3]) * 1e3;
	table->time_to_sleep = ft_atol(av[4]) * 1e3;
	if (table->time_to_die < 6e4 || table->time_to_eat < 6e4
		|| table->time_to_sleep < 6e4)
		print_error("Wrong timestamps use less than 60ms timestamps");
	if (av[5])
		table->limit_of_meal = ft_atol(av[5]);
	else
		table->limit_of_meal = -1;
}
