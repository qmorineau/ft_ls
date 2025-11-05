#include "ft_ls.h"

// static void map_addfront(t_map **head, t_map *new)
// {
// 	if (!head || !*head)
// 		(*head) = new;
// 	else
// 	{
// 		new->next = (*head);
// 		(*head) = new;
// 	}
// }

// static void map_uid_addfront(t_map_uid **head, t_map_uid *new)
// {
// 	if (!head || !*head)
// 		(*head) = new;
// 	else
// 	{
// 		new->next = (*head);
// 		(*head) = new;
// 	}
// }

// int map_set(t_map **map, char **key, char **value)
// {
// 	t_map *node = map_get(*map, *key);
// 	if (!node)
// 	{
// 		node = malloc(sizeof(t_map));
// 		if (!node)
// 			return (0);
// 		node->key = *key;
// 		node->value = *value;
// 		node->next = NULL;
// 		map_addfront(map, node);
// 	}
// 	else
// 	{
// 		free(node->value);
// 		free(node->key);
// 		node->key = *key;
// 		node->value = *value;
// 	}
// 	return (1);
// }

// int map_set_uid(t_map_uid **map, uid_t id, char **value)
// {
// 	t_map_uid *node = map_get_id(*map, id);
// 	if (!node)
// 	{
// 		node = malloc(sizeof(t_map_uid));
// 		if (!node)
// 			return (0);
// 		node->key = id;
// 		node->value = *value;
// 		node->next = NULL;
// 		node->len = ft_strlen(node->value);
// 		map_uid_addfront(map, node);
// 	}
// 	else
// 	{
// 		free(node->value);
// 		node->key = id;
// 		node->value = *value;
// 	}
// 	return (1);
// }

// t_map *map_get(t_map *map, char *key)
// {
// 	t_map *tmp;

// 	tmp = map;
// 	while (tmp)
// 	{
// 		if (strncmp(key, tmp->key, ft_strlen(key) + 1) == 0)
// 			return tmp;
// 		tmp = tmp->next;
// 	}
// 	return NULL;
// }

// t_map_uid *map_get_id(t_map_uid *map, uid_t id)
// {
// 	t_map_uid *tmp;

// 	tmp = map;
// 	while (tmp)
// 	{
// 		// printf("%d == %d\n", id, tmp->key);
// 		if (id == tmp->key)
// 			return tmp;
// 		tmp = tmp->next;
// 	}
// 	return NULL;
// }


t_map *map_get(t_map_pool *map, void *key)
{
	if (!map || !key)
		return (NULL);
	if (map->type == STR)
	{
		char *key_str = (char *) key;
		size_t key_len = ft_strlen(key);
		while (map)
		{
			for (int i = 0; i < map->it; i++)
			{
				if (!ft_strncmp(key,  map->pool[map->it].key.str, key_len))
					return (&map->pool[map->it]);
			}
			map = map->next;
		}
	}
	else
	{
		uid_t key_uid = (uid_t) key;
		while (map)
		{
			for (int i = 0; i < map->it; i++)
			{
				if (key_uid == map->pool[map->it].key.uid)
					return (&map->pool[map->it]);
			}
			map = map->next;
		}
	}
	return (NULL);
}

t_map *get_new_map(t_map_pool **head, t_map_type type)
{
	if (!*head)
	{
		t_map_pool	*new_pool = malloc(sizeof(t_map_pool));
		if (!new_pool)
			exit(2); // manage error
		new_pool->it = 0;
		new_pool->next = NULL;
		*head = new_pool;
		new_pool->pool[new_pool->it].type = type;
		return (&new_pool->pool[new_pool->it++]);
	}

	t_map_pool *tmp = *head;
	while (tmp->next)
		tmp = tmp->next;

	if (tmp->it == POOL_ITEMS_NUMBER)
	{
		t_map_pool	*new_pool = malloc(sizeof(t_map_pool));
		if (!new_pool)
			exit(2); // manage error
		new_pool->it = 0;
		new_pool->next = NULL;
		tmp->next = new_pool;
		tmp = tmp->next;
	}
	tmp->pool[tmp->it].type = type;
	return (&tmp->pool[tmp->it++]);
}

int map_set(t_map_pool **map, void *key, char *value)
{
	t_map *new_map = get_new_map(map, (*map)->type);
	if (!new_map)
		return (0);
	if ((*map)->type == STR)
	{
		new_map->key.str = (char *) key;
	}
	else
	{

	}
	new_map->value = value;
	new_map->len = ft_strlen(value);
	return (1);
}

void	map_pool_clear(t_map_pool **pool_head)
{
	t_map_pool *to_free = *pool_head;
	t_map_pool *tmp;

	while (to_free)
	{
		for (int i = 0; i < to_free->it; i++)
			ast_clear(&to_free->pool[i]);
		tmp = to_free->next;
		free(to_free);
		to_free = tmp;
	}
}

// from pool
t_map *find_extension(t_map_pool *map, char *name)
{
	if (map->type != STR)
		return (NULL);

	while (map)
	{
		for (int i = 0; i < map->it; i++)
		{
			int k = 
			for (int j = ft_strlen(map->pool[i].key.str); j >= 0; j--)
			{
				if (map->pool[i].key.str[j] != name[i])
					break;
				if (j == 1)
					return (&map->pool[i]);
			}
		}
		map = map->next;
	}
	return NULL;
}