// Mandlebrot.cpp : Defines the entry point for the application.
//

#include <SFML/Graphics.hpp>
#include <complex>

using namespace std;

const unsigned int WIDTH = 1200;
const unsigned int HEIGHT = 1200;
const unsigned int ITERATIONS = 80;
const unsigned int THRESHHOLD = 500;
const double X_COORD = -0.5;
const double Y_COORD = 0;
const double ZOOM = 4.0;

int main()
{

    // create the window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "My window");

    //load shaders
    sf::Shader shader;

    if (!sf::Shader::isAvailable())
    {
        return 1;
    }
    if (!shader.loadFromFile("C:/Users/kinga/Documents/Programming/Mandlebrot_viewer/src/shaders/mandlebrot.vert", "C:/Users/kinga/Documents/Programming/Mandlebrot_viewer/src/shaders/mandlebrot.frag"))
    {
        return 1;
    }

    sf::Texture texture;
    texture.create(WIDTH, HEIGHT);

    sf::Sprite sprite(texture); // needed to draw the texture on screen

    //vars
    float x_coord = 0.f;
    float y_coord = 0.f;
    float zoom = 1.f;
    int iterations = 40;
    int threshhold = 500;
    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.scancode == sf::Keyboard::Scan::RBracket) {
                    iterations += 5;
                }
                if (event.key.scancode == sf::Keyboard::Scan::LBracket) {
                    iterations -= 5;
                }
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            x_coord -= 0.005 / zoom;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            x_coord += 0.005 / zoom;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            y_coord -= 0.005 / zoom;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            y_coord += 0.005 / zoom;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Equal)) {
            zoom *= 1.008;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Dash)) {
            zoom /= 1.008;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Apostrophe)) {
            threshhold += 1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::SemiColon)) {
            threshhold -= 1;
        }

        


        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...

       

		
        shader.setUniform("threshhold", threshhold);
        shader.setUniform("iterations", iterations);
        shader.setUniform("zoom", zoom);
        shader.setUniform("x_coord", x_coord);
        shader.setUniform("y_coord", y_coord);


        window.draw(sprite, &shader);

        // end the current frame
        window.display();
    }

    return 0;
}
