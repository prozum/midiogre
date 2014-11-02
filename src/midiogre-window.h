#ifndef __MIDIOGREWIN_H
#define __MIDIOGREWIN_H

#include "midiogre-app.h"

#include <gtk/gtk.h>

#define MIDIOGRE_APP_WINDOW_TYPE (midiogre_app_window_get_type ())
#define MIDIOGRE_APP_WINDOW(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), MIDIOGRE_APP_WINDOW_TYPE, MidiogreAppWindow))

typedef struct _MidiogreAppWindow         MidiogreAppWindow;
typedef struct _MidiogreAppWindowClass    MidiogreAppWindowClass;

GType                   midiogre_app_window_get_type     (void);
MidiogreAppWindow      *midiogre_app_window_new          (MidiogreApp *app);
void                    midiogre_app_window_open         (MidiogreAppWindow *win,
                                                         GFile            *file);


#endif
