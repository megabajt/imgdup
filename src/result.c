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

#include <glib.h>

#include "result.h"

#define ID_RESULT_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), ID_TYPE_RESULT, IdResultPrivate))

struct IdResultPrivate
{
    gchar      *first;
    gchar      *second;
    gfloat      score;
};

G_DEFINE_TYPE (IdResult, id_result, G_TYPE_OBJECT)

const gchar *
id_result_get_first (IdResult *result)
{
    g_return_val_if_fail (result != NULL, NULL);
    g_return_val_if_fail (ID_IS_RESULT (result), NULL);

    return result->priv->first;
}

const gchar *
id_result_get_second (IdResult *result)
{
    g_return_val_if_fail (result != NULL, NULL);
    g_return_val_if_fail (ID_IS_RESULT (result), NULL);

    return result->priv->second;
}

gfloat
id_result_get_score (IdResult *result)
{
    g_return_val_if_fail (result != NULL, -1);
    g_return_val_if_fail (ID_IS_RESULT (result), -1);

    return result->priv->score;
}

static void
id_result_init (IdResult *result)
{
    result->priv = ID_RESULT_GET_PRIVATE (result);
}

static void
id_result_finalize (GObject *object)
{
    g_return_if_fail (ID_IS_RESULT (object));

    IdResult *result = ID_RESULT (object);

    g_free (result->priv->first);
    g_free (result->priv->second);

    G_OBJECT_CLASS (id_result_parent_class)->finalize (object);
}

static void
id_result_class_init (IdResultClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);
    object_class->finalize = id_result_finalize;
    g_type_class_add_private (klass, sizeof (IdResultPrivate));
}

IdResult *
id_result_new (const gchar *first, const gchar *second, gfloat score)
{
    IdResult *result = g_object_new (ID_TYPE_RESULT, NULL);

    result->priv->first = g_strdup (first);
    result->priv->second = g_strdup (second);
    result->priv->score = score;

    return ID_RESULT (result);
}


