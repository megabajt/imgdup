/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2017 Marcin Banasiak <marcin.banasiak@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include <limits.h>
#include <stdlib.h>
#include <glib.h>

#include "engine.h"
#include "hash.h"
#include "result.h"

#define ID_ENGINE_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), ID_TYPE_ENGINE, IdEnginePrivate))

struct IdEnginePrivate
{
    gint        threshold;

    GHashTable *phash_table;
    GMutex      phash_table_lock;
};

G_DEFINE_TYPE (IdEngine, id_engine, G_TYPE_OBJECT)

void
id_engine_set_threshold (IdEngine *engine, gint threshold)
{
    g_return_if_fail (engine != NULL);
    g_return_if_fail (ID_IS_ENGINE (engine));

    engine->priv->threshold = threshold;
}

static GList *
id_engine_get_absolute_file_paths (gchar **files)
{
    g_return_val_if_fail (files != NULL, NULL);

    GList *result = NULL;
    gchar *cwd = g_get_current_dir ();

    for (int i = 0; i < g_strv_length (files); i++) {
        if (g_path_is_absolute (files[i])) {
            result = g_list_append (result, g_strdup (files[i]));
        } else {
            gchar *path = g_build_filename (cwd, files[i], NULL);
            gchar *canonical_path = realpath (path, NULL);

            if (canonical_path == NULL) {
                g_printerr ("Failed to get absolute path for: %s\n", files[i]);
            } else {
                result = g_list_append (result, canonical_path);
            }

            g_free (path);
        }
    }

    g_free (cwd);

    return result;
}

static void
id_engine_compute_hash (gpointer data, gpointer user_data)
{
    g_return_if_fail (ID_IS_ENGINE (user_data));

    IdEngine *engine = ID_ENGINE (user_data);
    GError *error = NULL;
    guint64 *hash = g_new (guint64, 1);

    *hash = id_phash_for_file (data, &error);
    
    if (error != NULL) {
        g_printerr ("%s\n", error->message);
        g_error_free (error);
        return;
    }

    g_mutex_lock (&engine->priv->phash_table_lock);

    g_hash_table_insert (engine->priv->phash_table, g_strdup (data), hash);

    g_mutex_unlock (&engine->priv->phash_table_lock);
}

GList *
id_engine_compare_files (IdEngine *engine, gchar **files)
{
    g_return_val_if_fail (engine != NULL, NULL);

    GList *paths = id_engine_get_absolute_file_paths (files);
    GList *compare_results = NULL;

    if (paths != NULL) {
        GThreadPool *pool = g_thread_pool_new (id_engine_compute_hash,
                                               engine,
                                               g_get_num_processors (),
                                               FALSE,
                                               NULL);

        for (GList *l = paths; l != NULL; l = l->next) {
            if (!g_thread_pool_push (pool, l->data, NULL)) {
                g_printerr ("Failed to compute hash in thread for: %s\n", l->data);
            }
        } 

        g_thread_pool_free (pool, FALSE, TRUE);

        for (GList *first = paths; first != NULL; first = first->next) {
            gpointer first_value = g_hash_table_lookup (engine->priv->phash_table, first->data);
            
            if (first_value == NULL) {
                g_printerr ("There is no hash for: %s\n", first->data);
                continue;
            }

            guint64 first_hash = *(guint64 *)(first_value);
            
            for (GList *second = first->next; second != NULL; second = second->next) {
                gpointer second_value = g_hash_table_lookup (engine->priv->phash_table, second->data);
                
                if (second_value == NULL) {
                    g_printerr ("There is no hash for: %s\n", second->data);
                    continue;
                }
                
                guint64 second_hash = *(guint64 *)(second_value);

                gint distance = id_hash_hamming_distance (first_hash, second_hash);
                gfloat score = (1.0f - (gfloat) distance / 64.0f) * 100.0f;

                if (score > engine->priv->threshold) {
                    compare_results = g_list_append (compare_results, id_result_new (first->data, second->data, score));
                }
            }
        }
    }

    return compare_results;
}

static void
id_engine_init (IdEngine *engine)
{
    engine->priv = ID_ENGINE_GET_PRIVATE (engine);
    
    engine->priv->threshold = 0;

    engine->priv->phash_table = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, g_free);
    g_mutex_init (&engine->priv->phash_table_lock);
}

static void
id_engine_finalize (GObject *object)
{
    g_return_if_fail (ID_IS_ENGINE (object));

    IdEngine *engine = ID_ENGINE (object);

    g_hash_table_unref (engine->priv->phash_table);
    g_mutex_clear (&engine->priv->phash_table_lock);

    G_OBJECT_CLASS (id_engine_parent_class)->finalize (object);
}

static void
id_engine_class_init (IdEngineClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    object_class->finalize = id_engine_finalize;
    g_type_class_add_private (klass, sizeof (IdEnginePrivate));
}

IdEngine *
id_engine_new (void)
{
    IdEngine *engine = g_object_new (ID_TYPE_ENGINE, NULL);

    return ID_ENGINE (engine);
}

