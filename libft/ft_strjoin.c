/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ridoming <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 17:42:38 by ridoming          #+#    #+#             */
/*   Updated: 2025/04/18 17:44:00 by ridoming         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	s1_len;
	size_t	s2_len;
	size_t	total_len;
	char	*new_str;

	if (!s1 || !s2)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	total_len = s1_len + s2_len;
	new_str = malloc((s1_len + s2_len + 1) * sizeof(char));
	if (new_str == NULL)
		return (NULL);
	ft_strlcpy(new_str, s1, total_len + 1);
	ft_strlcat(new_str, s2, total_len + 1);
	*(new_str + total_len) = '\0';
	return (new_str);
}
