/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 15:15:40 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/07/25 14:40:46 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

volatile sig_atomic_t	g_ack_received = 0;

void	handle_ack(int sig)
{
	(void)sig;
	g_ack_received = 1;
}

void	wait_for_ack(void)
{
	int	attempts;

	attempts = 0;
	while (!g_ack_received && attempts++ < 1000000)
		usleep(1);
	if (!g_ack_received)
		ft_printf("Didnt receive ackg in time.\n");
}

void	send_char_as_bits(unsigned	char c, int pid)
{
	int	current_bit;

	current_bit = 7;
	while (current_bit >= 0)
	{
		if ((c >> current_bit) & 1)
			kill(pid, SIGUSR1);
		else
			kill(pid, SIGUSR2);
		wait_for_ack();
		current_bit--;
		g_ack_received = 0;
	}
}

int	main(int argc, char *argv[])
{
	int					pid;
	char				*str;
	struct sigaction	sa;

	if (argc != 3)
	{
		ft_printf("Usage: %s <PID> <message>\n", argv[0]);
		return (1);
	}
	pid = ft_atoi(argv[1]);
	if (pid < 0)
	{
		ft_printf("dont try to log me out!! >)\n");
		return (0);
	}
	str = argv[2];
	sa.sa_handler = handle_ack;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGUSR1, &sa, NULL);
	while (*str)
		send_char_as_bits(*str++, pid);
	send_char_as_bits('\0', pid);
	return (0);
}
