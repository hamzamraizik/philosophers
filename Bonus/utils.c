/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmraizik <hmraizik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 11:05:20 by hmraizik          #+#    #+#             */
/*   Updated: 2024/08/27 14:15:36 by hmraizik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(char *s)
{
	int		i;
	int		result;
	int		number;

	result = 0;
	number = 0;
	i = 0;
	while (s[i] == ' ' || s[i] == '\t' || s[i] == '+')
		i++;
	while (s[i] >= '0' && s[i] <= '9')
	{
		result = result * 10 + s[i] - '0';
		if (result < number)
			return (-1);
		number = result;
		i++;
	}
	if (s[i])
		return (-1);
	return (result);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		i;
	int		neg;

	neg = 0;
	i = 0;
	if (n < 0)
	{
		neg = 1;
		n = -n;
	}
	str = malloc(sizeof(char) * 12);
	if (!str)
		return (NULL);
	if (n == 0)
		str[i++] = '0';
	while (n > 0)
	{
		str[i++] = n % 10 + '0';
		n /= 10;
	}
	if (neg)
		str[i++] = '-';
	str[i] = '\0';
	return (str);
}

void	ft_usleep(long time, t_data *data)
{
	long	time_before;

	time_before = current_time();
	while (current_time() - time_before < time)
	{
		if (check(data) == 1)
			return ;
		usleep(100);
	}
}
