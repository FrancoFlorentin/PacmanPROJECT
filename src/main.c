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

#define CELL_SIZE 2
#define ROWS 400
#define COLS 300

#define PACMAN_SPEED 0.015

// Definir direcciones
typedef enum {
    NONE,
    RIGHT,
    LEFT,
    UP,
    DOWN
} Direccion;

// Inicializa la dirección actual y la próxima dirección
Direccion direccion = NONE;
Direccion proxima_direccion = NONE;

// Mapa del juego
int mapa[ROWS][COLS];

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

      // Verificar si esta en el menu
      if (inGame == 0) {
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

      if (event.type == sfEvtMouseButtonPressed) {
          // Obtener la posición del mouse
          sfVector2i mouse_position = sfMouse_getPositionRenderWindow(window);
          printf("Posicion del mouse - x: %d, y: %d\n", mouse_position.x, mouse_position.y);
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

int cargar_mapa(const char *nombre_archivo) {
    FILE *archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
      perror("Error al abrir el archivo de mapa");
      return 1;
    }
    printf("mapa cargado!");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            int c = fgetc(archivo);
            if (c == EOF) {
                fprintf(stderr, "El archivo de mapa tiene menos celdas de las esperadas.\n");
                return 1;
            }
            if (c == '0' || c == '1' || c == '8') {
                mapa[i][j] = c - '0'; // Convierte el carácter a número
            } else if (c == '\n') {
                j--; // Ignora saltos de línea
            } else {
                fprintf(stderr, "Carácter inesperado en el archivo de mapa.\n");
                return 1;
            }
        }
    }

    fclose(archivo);
}

// Función para verificar si una celda es un camino libre
sfBool es_camino_libre(int x, int y) {
    int celda_x = x / CELL_SIZE;
    int celda_y = y / CELL_SIZE;
    return mapa[celda_y][celda_x] == 0;
}


int main() {
  inGame = 0;

  windowConfig();
  loadBackgroundMenu();
  loadBackgroundMap();
  loadFonts();
  loadButtons();

  // Cargar mapa
  cargar_mapa("mapa.txt");

  sfVector2f pacman_pos = {304, 256}; // Posición inicial de Pac-Man
  sfClock *clock = sfClock_create();

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
   
        inGame = 2; // Cambiar al estado de mostrar la nueva imagen
      } else if (inGame == 2) {
        //Dibujar solo el fondo del juego          
        sfRenderWindow_drawSprite(window, fondoMapa, NULL);
        
        // Dibuja a Pac-Man
        sfCircleShape *pacman = sfCircleShape_create();
        sfCircleShape_setRadius(pacman, 16);
        sfCircleShape_setOrigin(pacman, (sfVector2f){16, 16});
        sfCircleShape_setPosition(pacman, pacman_pos);
        sfCircleShape_setFillColor(pacman, sfYellow);
        sfRenderWindow_drawCircleShape(window, pacman, NULL);
        sfCircleShape_destroy(pacman);

        // Comprueba si ha pasado el tiempo necesario
        // Mueve a Pac-Man continuamente en la última dirección presionada
        if (sfTime_asSeconds(sfClock_getElapsedTime(clock)) >= PACMAN_SPEED) {
            // Detecta la última tecla presionada y actualiza la próxima dirección
            if (sfKeyboard_isKeyPressed(sfKeyRight))
                proxima_direccion = RIGHT;
            else if (sfKeyboard_isKeyPressed(sfKeyLeft))
                proxima_direccion = LEFT;
            else if (sfKeyboard_isKeyPressed(sfKeyUp))
                proxima_direccion = UP;
            else if (sfKeyboard_isKeyPressed(sfKeyDown))
                proxima_direccion = DOWN;

            // Verifica si puede cambiar a la próxima dirección en una intersección
            if (proxima_direccion == RIGHT && es_camino_libre(pacman_pos.x + CELL_SIZE, pacman_pos.y))
                direccion = proxima_direccion;
            else if (proxima_direccion == LEFT && es_camino_libre(pacman_pos.x - CELL_SIZE, pacman_pos.y))
                direccion = proxima_direccion;
            else if (proxima_direccion == UP && es_camino_libre(pacman_pos.x, pacman_pos.y - CELL_SIZE))
                direccion = proxima_direccion;
            else if (proxima_direccion == DOWN && es_camino_libre(pacman_pos.x, pacman_pos.y + CELL_SIZE))
                direccion = proxima_direccion;

            // Mueve a Pac-Man según la dirección actual
            if (direccion == RIGHT && es_camino_libre(pacman_pos.x + CELL_SIZE, pacman_pos.y)) {
                pacman_pos.x += CELL_SIZE;
                if (pacman_pos.x > 597)  // Teletransporte al borde izquierdo
                    pacman_pos.x = 0;
            }
            else if (direccion == LEFT && es_camino_libre(pacman_pos.x - CELL_SIZE, pacman_pos.y)) {
                pacman_pos.x -= CELL_SIZE;
                if (pacman_pos.x < 1)  // Teletransporte al borde derecho
                    pacman_pos.x = 597;
            }
            else if (direccion == UP && es_camino_libre(pacman_pos.x, pacman_pos.y - CELL_SIZE))
                pacman_pos.y -= CELL_SIZE;
            else if (direccion == DOWN && es_camino_libre(pacman_pos.x, pacman_pos.y + CELL_SIZE))
                pacman_pos.y += CELL_SIZE;

            // printf("Posición de Pac-Man: X = %.0f, Y = %.0f\n", pacman_pos.x, pacman_pos.y);  // Imprimir posición actual

            sfClock_restart(clock); // Reinicia el reloj
        }
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