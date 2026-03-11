/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <ridoming@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 12:50:24 by ridoming          #+#    #+#             */
/*   Updated: 2025/04/18 13:17:04 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	substr_counter(char const *s, char c)
{
	int	i;
	int	counter;
	int	was_delimit;

	counter = 0;
	was_delimit = 1;
	if (!s || s[0] == '\0')
		return (0);
	i = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			if (was_delimit)
				counter++;
			was_delimit = 0;
		}
		else
		{
			was_delimit = 1;
		}
		i++;
	}
	return (counter);
}

static void	free_memory(char **matrix)
{
	int	i;

	if (matrix)
	{
		i = 0;
		while (matrix[i] != NULL)
		{
			free(matrix[i]);
			i++;
		}
		free(matrix);
	}
}

static char	*process_word(const char *s, char c, char **matrix, int i)
{
	const char	*word_end;
	size_t		word_len;

	word_end = s;
	while (*word_end != c && *word_end != '\0')
		word_end++;
	word_len = (size_t)(word_end - s);
	matrix[i] = (char *)malloc(word_len + 1);
	if (!matrix[i])
		return (NULL);
	ft_strlcpy(matrix[i], s, word_len + 1);
	return ((char *)word_end);
}

static char	**handle_split_error(char **matrix)
{
	if (matrix)
	{
		free_memory(matrix);
	}
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**matrix;
	int		i;
	int		word_count;
	char	*current_pos;

	word_count = substr_counter(s, c);
	matrix = (char **)malloc((word_count + 1) * sizeof(char *));
	if (!s || !matrix)
		return (handle_split_error(matrix));
	i = 0;
	current_pos = (char *)s;
	while (i < word_count)
	{
		while (*current_pos == c)
			current_pos++;
		current_pos = process_word(current_pos, c, matrix, i);
		if (!current_pos)
		{
			free_memory(matrix);
			return (NULL);
		}
		i++;
	}
	matrix[i] = NULL;
	return (matrix);
}
