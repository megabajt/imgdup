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


#ifndef __ID_ENGINE_H
#define __ID_ENGINE_H

#include "config.h"

#include <glib-object.h>

G_BEGIN_DECLS

#define ID_TYPE_ENGINE          (id_engine_get_type ())
#define ID_ENGINE(o)            (G_TYPE_CHECK_INSTANCE_CAST ((o), ID_TYPE_ENGINE, IdEngine))
#define ID_ENGINE_CLASS(k)      (G_TYPE_CHECK_CLASS_CAST((k), ID_TYPE_ENGINE, IdEngineClass))
#define ID_IS_ENGINE(o)         (G_TYPE_CHECK_INSTANCE_TYPE ((o), ID_TYPE_ENGINE))
#define ID_IS_ENGINE_CLASS(k)   (G_TYPE_CHECK_CLASS_TYPE ((k), ID_TYPE_ENGINE))
#define ID_ENGINE_GET_CLASS(o)  (G_TYPE_INSTANCE_GET_CLASS ((o), ID_TYPE_ENGINE, IdEngineClass))

typedef struct IdEnginePrivate IdEnginePrivate;

typedef struct
{
    GObject             parent;
    IdEnginePrivate    *priv;
} IdEngine;

typedef struct
{
    GObjectClass        parent_class;
} IdEngineClass;

#ifdef G_DEFINE_AUTOPTR_CLEANUP_FUNC
G_DEFINE_AUTOPTR_CLEANUP_FUNC(IdEngine, g_object_unref)
#endif

GType           id_engine_get_type      (void);
IdEngine       *id_engine_new           (void);
void            id_engine_set_threshold (IdEngine  *engine,
                                         gint threshold);
GList          *id_engine_compare_files (IdEngine  *engine,
                                         gchar    **files);

G_END_DECLS

#endif /* __ID_ENGINE_H */

