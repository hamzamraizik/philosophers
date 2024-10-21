/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_func.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmraizik <hmraizik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 00:44:04 by hmraizik          #+#    #+#             */
/*   Updated: 2024/09/02 00:44:05 by hmraizik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	ft_sleeping(t_data *data, int i)
{
	print_status(data, i, "is sleeping");
	ft_usleep(data->time_to_sleep);
}

void	ft_eating(t_data *data, int i)
{
	print_status(data, i, "is eating");
	ft_usleep(data->time_to_eat);
}

long	current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	print_status(t_data *data, int id, const char *status)
{
	sem_wait(data->print_lock);
	printf("%ld %d %s\n", current_time()
		- data->start_time, id, status);
	sem_post(data->print_lock);
}
