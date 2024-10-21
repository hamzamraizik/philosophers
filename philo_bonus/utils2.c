/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmraizik <hmraizik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 00:43:46 by hmraizik          #+#    #+#             */
/*   Updated: 2024/09/15 21:36:16 by hmraizik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	update_infos_of_philo(t_data *data, t_philo *philo)
{
	sem_wait(data->sim_lock);
	philo->last_meal_time = current_time();
	philo->meals_counter += 1;
	sem_post(data->sim_lock);
}
