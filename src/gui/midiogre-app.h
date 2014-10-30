#ifndef __MIDIOGREAPP_H
#define __MIDIOGREAPP_H

#include <gtk/gtk.h>

#define MIDIOGRE_APP_TYPE (midiogre_app_get_type ())
#define MIDIOGRE_APP(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), MIDIOGRE_APP_TYPE, MidiogreApp))

typedef struct _MidiogreApp MidiogreApp;
typedef struct _MidiogreAppClass MidiogreAppClass;

GType        midiogre_app_get_type (void);
MidiogreApp *midiogre_app_new (void);

#endif
