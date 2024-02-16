#if !defined( TEXT_UTIL_H )
#define TEXT_UTIL_H

#include "common.h"

typedef struct qGame_t qGame_t;

void qTextUtil_DrawWrappedScrollingText( qGame_t* game,
                                         sfText* text,
                                         const char* str,
                                         sfVector2f pos,
                                         float width,
                                         float lineSpacing );

#endif // TEXT_UTIL_H
