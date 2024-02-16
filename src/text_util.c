#include "text_util.h"
#include "game.h"
#include "renderer.h"
#include "render_states.h"
#include "render_objects.h"
#include "window.h"

void qTextUtil_DrawWrappedScrollingText( qGame_t* game, qDialogBoxRenderObjects_t* objects )
{
   qTextScrollRenderState_t* scrollState = game->renderer->renderStates->textScroll;
   char strLine[STRLEN_DEFAULT];
   uint32_t strLen = (uint32_t)strlen( objects->message );
   uint32_t substrEndIndex = scrollState->isRunning ? scrollState->currentCharIndex : strLen;
   uint32_t lineStartIndex, curIndex, writeIndex, cutoffIndex, lastSpaceIndex;
   sfVector2f pos = { objects->pos.x + objects->textOffset.x, objects->pos.y + objects->textOffset.y };

   sfText_setPosition( objects->text, pos );

   for ( lineStartIndex = 0, curIndex = 0, cutoffIndex = 0, lastSpaceIndex = 0; curIndex < strLen; curIndex++ )
   {
      writeIndex = curIndex - lineStartIndex;
      strLine[writeIndex] = objects->message[curIndex];

      if ( objects->message[curIndex] == ' ' || curIndex == strLen - 1 )
      {
         if ( objects->message[curIndex] == ' ' )
         {
            strLine[writeIndex] = '\0';
         }
         else
         {
            strLine[writeIndex + 1] = '\0';
         }

         sfText_setString( objects->text, strLine );

         if ( sfText_getGlobalBounds( objects->text ).width > objects->textWidth )
         {
            if ( lastSpaceIndex <= lineStartIndex )
            {
               // the entire line is one word
               curIndex = cutoffIndex + 1;
            }
            else
            {
               // the line has multiple words
               curIndex = lastSpaceIndex;
            }

            if ( substrEndIndex < curIndex )
            {
               strLine[substrEndIndex - lineStartIndex + 1] = '\0';
               sfText_setString( objects->text, strLine );
               qWindow_DrawText( game->window, objects->text );
               break;
            }
            // this check is only here to get around a compiler warning
            else if ( curIndex >= lineStartIndex )
            {
               strLine[curIndex - lineStartIndex] = '\0';
               sfText_setString( objects->text, strLine );
               qWindow_DrawText( game->window, objects->text );
            }

            if ( objects->message[curIndex] != ' ' && objects->message[curIndex] != '\0' )
            {
               curIndex--;
            }

            lineStartIndex = curIndex + 1;
            pos.y += objects->lineSize;
            sfText_setPosition( objects->text, pos );
         }
         else
         {
            if ( curIndex > substrEndIndex )
            {
               strLine[substrEndIndex - lineStartIndex + 1] = '\0';
               sfText_setString( objects->text, strLine );
               qWindow_DrawText( game->window, objects->text );
               break;
            }
            else if ( curIndex == strLen - 1 )
            {
               qWindow_DrawText( game->window, objects->text );
            }
            else
            {
               strLine[writeIndex] = ' ';
               lastSpaceIndex = writeIndex;
            }
         }
      }
      else if ( cutoffIndex <= lineStartIndex )
      {
         strLine[writeIndex + 1] = '\0';
         sfText_setString( objects->text, strLine );

         if ( sfText_getGlobalBounds( objects->text ).width > objects->textWidth )
         {
            cutoffIndex = curIndex - 1;
         }
      }
   }
}
