/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 15:15:40 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/07/21 17:36:53 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"
#include "libft/libft.h"
#include "libft/printf/ft_printf.h"

volatile sig_atomic_t	g_ack_received = 0;

void	handle_ack(int sig)
{
	(void)sig;
	g_ack_received = 1;
	write(1, "[ACK received]\n", 15);
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
		usleep(1000);
		current_bit--;
	}
	ft_printf("Waiting for ACK...\n");
	while (!g_ack_received)
		pause();
	ft_printf("ACK received. Sending next character.\n");
	g_ack_received = 0;
}

int	main(int argc, char *argv[])
{
	int					pid;
	char				*str;
	struct sigaction	sa;

	struct timeval start, end; // THIS CAN BE REMOVED JUST FOR TESTING 
	long elapsed_microseconds; // TESTING

	gettimeofday(&start, NULL); // ⏱️ start timing TESTING

	if (argc != 3)
	{
		ft_printf("Usage: %s <PID> <message>\n", argv[0]);
		return (1);
	}
	pid = ft_atoi(argv[1]);
	str = argv[2];
	sa.sa_handler = handle_ack;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGUSR1, &sa, NULL);
	while (*str)
		send_char_as_bits(*str++, pid);

	send_char_as_bits('\0', pid); // end-of-message

	gettimeofday(&end, NULL); // ⏱️ end timing

	elapsed_microseconds = (end.tv_sec - start.tv_sec) * 1000000L 
					 + (end.tv_usec - start.tv_usec);

	printf("Sent message in %ld microseconds (%.2f ms)\n",
		elapsed_microseconds,
		elapsed_microseconds / 1000.0);


	return (0);
}
