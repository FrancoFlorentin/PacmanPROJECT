#include <SFML/Graphics.h>
#include <stdio.h>

sfVideoMode mode = {600, 800, 32};
sfRenderWindow* window;
sfTexture* menuBackground;
sfSprite* fondoMenu;
sfVector2u textureSize;
sfVector2u windowSize;
sfTexture* mapBackground;
sfSprite* fondoMapa;
sfVector2u sizeMapa;
sfFont* font;
sfRectangleShape* playButton;
sfText* playText;
sfRectangleShape* exitButton;
sfText* exitText;
sfVector2i mousePos;
sfFloatRect playBounds;
sfFloatRect exitBounds;


int inGame;
float scaleY, scaleX, scaleYMap, scaleXMap;

int windowConfig() {
  // Crear la ventana
  window = sfRenderWindow_create(mode, "Pacman", sfClose, NULL);
  if (!window)
    return 1; // Salir si falla la creación de la ventana
}

int loadBackgroundMenu() {
  menuBackground = sfTexture_createFromFile("wallpaper.jpg", NULL);
  if (!menuBackground) {
    sfRenderWindow_destroy(window);
    return 1; // Salir si falla la textura
  }

  // Crear un sprite para el fondo
  fondoMenu = sfSprite_create();
  sfSprite_setTexture(fondoMenu, menuBackground, sfTrue); // Establecer la textura al sprite

  // Obtener el tamaño de la imagen y la ventana
  textureSize = sfTexture_getSize(menuBackground);
  windowSize = sfRenderWindow_getSize(window);

  // Calcular el factor de escala para ajustar el fondo a la ventana
  scaleX = (float)windowSize.x / textureSize.x;
  scaleY = (float)windowSize.y / textureSize.y;
  sfSprite_setScale(fondoMenu, (sfVector2f){0.5, scaleY});
  sfSprite_setPosition(fondoMenu, (sfVector2f){-175, 0});
}

int loadBackgroundMap() {
  //! Carga imagen del mapa
  mapBackground = sfTexture_createFromFile("mapa.jpg", NULL);
  if (!mapBackground) {
    sfRenderWindow_destroy(window);
    return 1; // si falla
  }

  // Crear sprite del fondo del mapa
  fondoMapa = sfSprite_create();
  sfSprite_setTexture(fondoMapa, mapBackground, sfTrue);

  sizeMapa = sfTexture_getSize(mapBackground);

  scaleXMap = (float)windowSize.x / sizeMapa.x;
  scaleYMap = (float)windowSize.y / sizeMapa.y;
  sfSprite_setScale(fondoMapa, (sfVector2f){scaleXMap, scaleYMap});
}

int loadFonts() {
  // Configuración de fuentes y colores
  font = sfFont_createFromFile("pacfont.ttf");  // Cargar la fuente
  if (!font) {
      sfRenderWindow_destroy(window);
      return 1;
  }
}

void loadButtons() {
  // Configurar el fondo del botón "Jugar"
  playButton = sfRectangleShape_create();
  sfRectangleShape_setSize(playButton, (sfVector2f){250, 80});
  sfRectangleShape_setFillColor(playButton, (sfColor){0, 0, 0, 150});  // Color oscuro y semitransparente
  sfRectangleShape_setOutlineColor(playButton, sfWhite);               // Bordes blancos (opcional)
  sfRectangleShape_setOutlineThickness(playButton, 2);
  sfRectangleShape_setPosition(playButton, (sfVector2f){windowSize.x / 2 - 125, 195});

  // Configurar botón "Jugar"
  playText = sfText_create();
  sfText_setString(playText, "Jugar");
  sfText_setFont(playText, font);
  sfText_setCharacterSize(playText, 50);
  sfText_setPosition(playText, (sfVector2f){windowSize.x / 2 - 115, 200});  // Posición del botón "Jugar"

  // Configurar el fondo del botón "Salir"
  exitButton = sfRectangleShape_create();
  sfRectangleShape_setSize(exitButton, (sfVector2f){210, 80});
  sfRectangleShape_setFillColor(exitButton, (sfColor){0, 0, 0, 150});
  sfRectangleShape_setOutlineColor(exitButton, sfWhite);
  sfRectangleShape_setOutlineThickness(exitButton, 2);
  sfRectangleShape_setPosition(exitButton, (sfVector2f){windowSize.x / 2 - 105, 320});

  // Configurar botón "Salir"
  exitText = sfText_create();
  sfText_setString(exitText, "Salir");
  sfText_setFont(exitText, font);
  sfText_setCharacterSize(exitText, 50);
  sfText_setPosition(exitText, (sfVector2f){windowSize.x / 2 - 95, 323});  // Posición del botón "Salir
}

void eventHandler(sfVector2i mousePos) {
  sfEvent event;

  while (sfRenderWindow_pollEvent(window, &event)) {
    if (event.type == sfEvtClosed)
      sfRenderWindow_close(window); // Cerrar la ventana si el usuario lo solicita

    // Manejar clics del mouse
    if (event.type == sfEvtMouseButtonPressed && event.mouseButton.button == sfMouseLeft) {

      // Verificar si se hizo clic en el botón "Jugar"
      playBounds = sfRectangleShape_getGlobalBounds(playButton);
      if (sfFloatRect_contains(&playBounds, mousePos.x, mousePos.y)) {
        printf("Boton 'Jugar' presionado\n");
        inGame = 1;
      }

      // Verificar si se hizo clic en el botón "Salir"
      exitBounds = sfRectangleShape_getGlobalBounds(exitButton);
      if (sfFloatRect_contains(&exitBounds, mousePos.x, mousePos.y)) {
        sfRenderWindow_close(window);  // Cierra la ventana
      }
    }
  }
}

void buttonsHover(sfVector2i mousePos) {
  // Hover para el botón "Jugar"
  playBounds = sfRectangleShape_getGlobalBounds(playButton);
  if (sfFloatRect_contains(&playBounds, mousePos.x, mousePos.y)) {
      sfRectangleShape_setFillColor(playButton, sfBlack);  // Fondo negro en hover
  } else {
      sfRectangleShape_setFillColor(playButton, (sfColor){0, 0, 0, 150});  // Fondo original
  }

  // Hover para el botón "Salir"
  exitBounds = sfRectangleShape_getGlobalBounds(exitButton);
  if (sfFloatRect_contains(&exitBounds, mousePos.x, mousePos.y)) {
      sfRectangleShape_setFillColor(exitButton, sfBlack);
  } else {
      sfRectangleShape_setFillColor(exitButton, (sfColor){0, 0, 0, 150});
  }
}


int main() {

  inGame = 0;

  windowConfig();
  loadBackgroundMenu();
  loadBackgroundMap();
  loadFonts();
  loadButtons();



  while (sfRenderWindow_isOpen(window)) {
      // Detectar posicion del mouse
      mousePos = sfMouse_getPositionRenderWindow(window);
      eventHandler(mousePos);

      buttonsHover(mousePos);
      

      // Limpiar la ventana
      sfRenderWindow_clear(window, sfBlack);

      // Dibujar el fondo
      sfRenderWindow_drawSprite(window, fondoMenu, NULL);

      if (inGame == 0) {
        // Dibujar el menú con botones
        sfRenderWindow_drawRectangleShape(window, playButton, NULL);
        sfRenderWindow_drawText(window, playText, NULL);
        sfRenderWindow_drawRectangleShape(window, exitButton, NULL);
        sfRenderWindow_drawText(window, exitText, NULL);
      } else if (inGame == 1) {
      sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_display(window); // Actualizar ventana en negro
        sfSleep(sfSeconds(1.0)); // Esperar 1 segundo
        inGame = 2; // Cambiar al estado de mostrar la nueva imagen
      } else if (inGame == 2) {
        //Dibujar solo el fondo del juego          
        sfRenderWindow_drawSprite(window, fondoMapa, NULL);
      }

      // Mostrar la ventana
      sfRenderWindow_display(window);
    }

    // Liberar recursos
    sfText_destroy(playText);
    sfText_destroy(exitText);
    sfSprite_destroy(fondoMenu);
    sfRenderWindow_destroy(window);
}