#include "../minishell.h"

static int update_var(t_env *env, const char *key, const char *value)
{
    t_env *curr = env;
    
    while (curr)
    {
        if (ft_strcmp(curr->key, key) == 0)
        {
            free(curr->value);
            if (value)
                curr->value = ft_strdup(value);
            else
                curr->value = NULL;
            return 1;
        }
        curr = curr->next;
    }
    return 0;
}
static int create_and_add_node(t_env **env, const char *key, const char *value)
{
    t_env *new_node;
	new_node = malloc(sizeof(t_env));
    if (!new_node)
        return -1;
    new_node->key = ft_strdup(key);
    if (value)
        new_node->value = ft_strdup(value);
    else
        new_node->value = NULL;
    new_node->next = NULL;
    add_env_node(env, new_node);
    return 0;
}

int set_env_var(t_env **env, const char *key, const char *value)
{
    if (!env || !key)
        return -1;

    if (update_var(*env, key, value))
        return 0;

    return create_and_add_node(env, key, value);
}