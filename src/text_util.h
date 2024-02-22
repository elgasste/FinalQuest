#if !defined( TEXT_UTIL_H )
#define TEXT_UTIL_H

#include "common.h"
#include "enums.h"

typedef struct qGame_t qGame_t;
typedef struct qDialogBoxRenderObjects_t qDialogBoxRenderObjects_t;

const char* qTextUtil_IndefiniteArticleFromEnum( qIndefiniteArticle_t article, sfBool lower );
void qTextUtil_DrawWrappedScrollingText( qGame_t* game, qDialogBoxRenderObjects_t* objects );

#endif // TEXT_UTIL_H
