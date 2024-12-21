// Mandlebrot.cpp : Defines the entry point for the application.
//

#include <SFML/Graphics.hpp>
#include <complex>
#include <chrono>
#include <iostream>

using namespace std;

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 800;
const unsigned int ITERATIONS = 90;
const unsigned int THRESHHOLD = 500;
const double X_COORD = -0.5;
const double Y_COORD = 0;
const double ZOOM = 4.0;
const double MOVE_SPEED = 2.0;  //coordinates per second without zoom
const double ZOOM_SPEED = 10.0; //magnification per second

const std::string VERT_SHADER = R"(
void main()
{
    // transform the vertex position
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    // transform the texture coordinates
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

    // forward the vertex color
    gl_FrontColor = gl_Color;
}
)";

const std::string FRAG_SHADER = R"(
uniform sampler2D texture;
uniform int threshhold;
uniform int iterations;
uniform float zoom;
uniform float x_coord;
uniform float y_coord;
uniform float aspectRatio; //width / height

//x is real component, y is imaginary component
vec2 squareComplex(vec2 num) {
	vec2 result;
	result.x = pow(num.x, 2.0) - pow(num.y, 2.0);
	result.y = 2*num.x*num.y;
	return result;
}

vec2 addComplex(vec2 num1, vec2 num2) {
	vec2 result;
	result.x = num1.x + num2.x;
	result.y = num1.y + num2.y;
	return result;
}

float magComplex(vec2 num) {
	return sqrt(pow(num.x, 2.0) + pow(num.y, 2.0));
}

vec4 mandlebrot(float x, float y) {

	vec4 color;
	color.rgba = vec4(0.0, 0.0, 0.0, 1.0);

	vec2 z;
	z.xy = vec2(0.0);
	vec2 c;
	c.xy = vec2(x, y);
	for (int i = 0; i < iterations; i++) {
		z = addComplex(squareComplex(z), c);
		if (magComplex(z) > threshhold) {
			color.rgba = vec4(float(i) / float(iterations), 0.1, 0.2, 1.0);
			return color;
		}
	}
	return color;
}

void main()
{
    float xScale = 1.0;
    float yScale = 1.0;
    if(aspectRatio > 1.0) {
        xScale = xScale*aspectRatio;
    } else {
        yScale = yScale/aspectRatio;
    }
    float x = (float(gl_TexCoord[0].x)*xScale - xScale/2) * 4.0 / zoom + x_coord;
    float y = (float(gl_TexCoord[0].y)*yScale - yScale/2) * 4.0 / zoom  + y_coord;
    gl_FragColor.rgba = mandlebrot(x, y).rgba;
}


)";


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
    if (!shader.loadFromMemory(VERT_SHADER, FRAG_SHADER))
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
    float aspectRatio = (float)WIDTH / (float)HEIGHT;
    // run the program as long as the window is open
    std::chrono::high_resolution_clock::time_point t0 = std::chrono::high_resolution_clock::now();
    while (window.isOpen())
    {
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::micro> time_span = t1 - t0;
        long tDiff = time_span.count();
        t0 = std::chrono::high_resolution_clock::now();

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
            if (event.type == sf::Event::Resized) {
                // update the view to the new size of the window
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
                
                sprite.setScale(event.size.width / float(WIDTH), event.size.height / float(HEIGHT));
                aspectRatio = float(event.size.width) / float(event.size.height);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            x_coord -= (double)tDiff / 1000000 * MOVE_SPEED / zoom;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            x_coord += (double)tDiff / 1000000 * MOVE_SPEED / zoom;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            y_coord -= (double)tDiff / 1000000 * MOVE_SPEED / zoom;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            y_coord += (double)tDiff / 1000000 * MOVE_SPEED / zoom;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Equal)) {
            zoom *= pow(ZOOM_SPEED, (double)tDiff / 1000000);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Dash)) {
            zoom *= pow(ZOOM_SPEED, -((double)tDiff / 1000000));;
        }

        


        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...

       

        int threshhold = THRESHHOLD;


        shader.setUniform("threshhold", threshhold);
        shader.setUniform("iterations", iterations);
        shader.setUniform("zoom", zoom);
        shader.setUniform("x_coord", x_coord);
        shader.setUniform("y_coord", y_coord);
        shader.setUniform("aspectRatio", aspectRatio);


        window.draw(sprite, &shader);

        // end the current frame
        window.display();
    }

    return 0;
}
