#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;

//SE INICIA LA CLASE Y SE ASIGAN PUBLICOS EL DRAWABLE Y TRANSFORMABLE
class TileMap : public sf::Drawable, public sf::Transformable
{
private:
    //variables privados de LA CLASE SFML
    sf::VertexArray m_vertices;
    sf::Texture m_tileset;

//esta funcion es tomada de la documentacion, nos permite llamar el objeto adentro de window.draw()
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        //se aplica la transformacion
        states.transform *= getTransform();
        //la textura de states se le asigna la referencia de la textura privada
        states.texture = &m_tileset;
        // se llama la fucnion draw, y se pasa el variable privado y los estados de render states
        target.draw(m_vertices, states);
    }

//lo se es una mamada, pero no me eh adentrado mas en la documentacion por ahora esto fuciona
// aqui viene lo pesado


public:
    //esta fucnion asigna todo lo que llevara un objeto, actua como constructor
    bool load(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height)
    {
        // condicion para cargar la textura del argumento que se pasa en load
        if (!m_tileset.loadFromFile(tileset))
            return false;

        // se asigna la figura del vertex array que tendra la variable privada
        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.resize(width * height * 4);

        // estos loops funcionan como en una matriz 2D ya que se va a usar un array
        for (unsigned int i = 0; i < width; ++i)
            for (unsigned int j = 0; j < height; ++j)
            {
// a esta variable se le asigna el argumento tiles, el cual sera el array 2D, se le suma el i  y el j de los for loops y se multiplica por el argumento width
                int tileNumber = tiles[i + j * width];

//variables para encontrar la posicion deseada en la textura privada, solo produce numeros por ahora,
                int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
                int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);


//se asigna un puntero de  vertex,con lols valores de vertex array de la variable privada y se le aplica suma y multiplicacion
                sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

//esto asigna la posicion de cuadros invisibles, en donde la textura sera colocada, basicamente se asigna la posicion de los puntos del vertex array
                quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
                quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
                quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
                quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

//esto actua de similar manera, solo que aqui se asigna las coordenadas de la textura deseada
                quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
                quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
//esto segira pasando hasta que termine los for loops, es lo que hace que se dibuje una entidad enterea en mosaico,
//en vez de asignar muchas texturas, se crea una sola entidad como si fuera solo 1 sprite
// lo que se hizo en esta fucnion fue basicamente posicionar puntos en la ventana, y cortar la textura dependiendo del valor del array
            }

        return true;
    }

};

int main()
{
    //estos variables solo son para el jugador, no son importantes en el codigo
    float x_pos = 100;
    float y_pos = 90;
    int DATA;
    bool falling = false;


    // estos variables son para manipular el view de FSML
    float X_AX = 0.0f;
    float Y_AX = 567.0f;
    float X_SIZ = 320.0f;
    float Y_SIZ = 200.0f;
    float MOVE = Y_AX;
    float MOVE2 = X_AX;

    //SE CREA EL VIEW, (la camara, basicamente se hace un zoom y se puede manipular )
    sf::View VISTA(sf::FloatRect(X_AX, Y_AX, X_SIZ, Y_SIZ));
    // se crea la ventana
    sf::RenderWindow window(sf::VideoMode(1280, 734), "Tilemap");

    // aqui es donde se asigna la posicion deseada de la textura, si se fijan en la imagen le escribi los numeros en rojo, para probar los valores, y coordenadas de la textura
    const int level[] =
    {
        0, 0, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
        0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
        0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
        0, 0, 0, 0, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
        0, 0, 1, 0, 3, 3, 3, 0, 0, 0, 1, 1, 1, 2, 0, 0, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
        0, 0, 1, 0, 3, 0, 2, 2, 0, 0, 1, 1, 1, 1, 2, 0, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
        0, 0, 1, 0, 3, 0, 2, 2, 2, 0, 1, 1, 1, 1, 1, 1, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
        0, 0, 1, 0, 3, 2, 2, 2, 0, 0, 0, 0, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
        0, 0, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
        0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
        0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
        0, 0, 0, 0, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
        0, 0, 1, 0, 3, 3, 3, 0, 0, 0, 1, 1, 1, 2, 0, 0, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
        0, 0, 1, 0, 3, 0, 2, 2, 0, 0, 1, 1, 1, 1, 2, 0, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
        0, 0, 0, 0, 3, 0, 2, 2, 2, 0, 1, 1, 1, 1, 1, 1, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
        0, 0, 0, 0, 3, 2, 2, 2, 0, 0, 0, 0, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
        0, 0, 0, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
        0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
        0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
        0, 0, 0, 0, 2, 0, 3, 3, 3, 0, 1, 1, 1, 0, 0, 0, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
        0, 0, 0, 0, 3, 3, 3, 0, 0, 0, 1, 1, 1, 2, 0, 0, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
        0, 0, 0, 0, 3, 0, 2, 2, 0, 0, 1, 1, 1, 1, 2, 0, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
        0, 0, 1, 0, 0, 0, 2, 2, 2, 0, 1, 1, 1, 1, 1, 1, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
        3, 3, 1, 3, 3, 3, 3, 3, 1, 0, 0, 0, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3,
    };

// aqui se crea el objeto de la clase tile map, se carga la imagen, de que tamano queremos el mosaico singular, el valor del array.

    TileMap map;
//se asigna la imagen, se asigna un sf::vector tamano del cuadro de un mosaico alto y ancho , el array level,  lo ancho y lo alto del array lelvel,
    if (!map.load("MAP0.png", sf::Vector2u(32, 32), level, 40, 24))return -1;



    //esto solo dibuja al personaje, no es importante
    sf::Texture TEXTURAJ;
    if (!TEXTURAJ.loadFromFile("IMG1.png")) return 0;
    sf::Sprite P1(TEXTURAJ);
    P1.setScale(2.0f, 2.0f);
    //player gravity
    if(y_pos >= 80 && y_pos < 740)falling =true;
//////////////////////////////////////////////////////////////


    // run main loop
    while (window.isOpen())
    {
        // eventos
        sf::Event event;
        while (window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
                window.close();


        }

        //ESTO PREVIENE QUE EL VIEW NO SE PASE DE LA TEXTURA, SE PUEDE BORRAR SI QUIEREN VER MAS AYA DE LA TEXTURA DE MOSAICO
        if(MOVE >= 567.0){MOVE = 567.0;}
        if(MOVE <= 0.0){MOVE = 0.0;}
        if(MOVE2 <= 0.0){MOVE2 = 0.0;}
        if(MOVE2 >= 960){MOVE2 = 960;}

        //CONTROLES DEL view*******************
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
        MOVE = MOVE- 0.02;}

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
        MOVE = MOVE + 0.02;}

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
        MOVE2 = MOVE2 - 0.02;}

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
        MOVE2 = MOVE2 + 0.02;}
//*********************************************************************


        //CONTROLES DEL PERSONAJE
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
        y_pos -= 0.3; DATA += 1; if(DATA >=590){falling = true; DATA = 0;}}

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
        x_pos = x_pos - 0.05;}

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        x_pos = x_pos + 0.05;}
        //cout << "MOVE " << MOVE << " move 2 value : " << MOVE2 << endl;

        //gravedad
        if(falling){y_pos += 0.10;}
        if((y_pos == 742 || y_pos < 748) && y_pos > 741)falling = false;
        //cout << "falling? " << falling << " Y_POS "<< y_pos << " DATA " << DATA <<  endl;

        //previene que el jugador se pase de la pantalla
        //742 when view is active
        if(y_pos >= 742){y_pos = 742;}


        //movimiento del persoanje y del view, se actualiza
        P1.setPosition(x_pos, y_pos);
        VISTA.reset(sf::FloatRect(MOVE2, MOVE, X_SIZ, Y_SIZ));


//////////RENDER/////////////////////////////////////////////////////////////////////////////////

        window.clear();

        //aqui se pasa el obejto map con la fucnion de window.draw, se logro debido a la funcion privada de la clase
        window.draw(map);
        //activa el view
        window.setView(VISTA);
        //muestra personaje
        window.draw(P1);
        window.display();

    }
    return 0;
}



/*
            CONCLUSION
esta es una forma de ahorrarse rendimiento al dibujar muchas cosas a la vez,

ya que dibujar muchos sprites causa que se consuma mucha memoria, y alentaria el programa

es mas facil, componer un mapa de mosaico, y asignar entidades invisibles para dano o bloqueo,

por ejemplo, una piedra, o ya sea un hueco,

trate de darle mas vida a la entidad mosaico, pero esa fue la unica forma que se me ocurrio de como

se puede interactuar con el mosaico, lo importante es en que se preste atencion en el tamano de la textura y

el tamano del array,

por ejemplo, la imagen que use era de 65x65 pixeles,

como las lineas blancas que estan cruzadas toman un pixel,

la balancie al tratarla como imagen de 64 x 64,

por eso es que a los quad vertices se le asignaron 32 x 32,

ya en el array decides que lado de la textura quieres que se muestre, con la formula en la funcion publica de los quads,
el numero asignado en el arra se multiplica y se le pasa la formula de division con los numeros de los for loops

lo cual hace que se valla dibujando mosaico x mosaico 1 a la vez, y es lo que le da el efecto de una entidad sola.




*/


