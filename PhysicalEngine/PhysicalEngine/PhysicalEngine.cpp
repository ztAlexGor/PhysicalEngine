#include "MathUtils/Vector.h"
#include "Engine/Body.h"
#include "Shapes/Circle.h"
#include "Shapes/Polygon.h"
#include <SFML/Graphics.hpp>
#include "Engine/World.h"

sf::Vector2f VecToVec2f(const Vector vec);
void drawCircle(sf::RenderWindow& window, const Circle* c, sf::Vector2f pos);
void drawCircle(sf::RenderWindow& window, const Circle* c, sf::Vector2f pos, bool);
void drawPolygon(sf::RenderWindow& window, const Polygon* p, sf::Vector2f pos, float angle);
void drawBody(sf::RenderWindow& window, const Body& body);
void draw(sf::RenderWindow& window/*, Body& b*/);
void draw(sf::RenderWindow& window, std::string msg, sf::Vector2f pos);


int main()
{
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(1500, 1000), L"Physics", sf::Style::Default);
    World world;
    //world.SetGravity(Vector(0.f, -10.f));
    const float timeStep = 0.1f;
    sf::Clock clock;

    //Body polygonBody{ BodyInit {.position = Vector(500.f, 300.f)} };
    //polygonBody.SetShape(new Polygon(std::vector<Vector>{ Vector(-100, 50), Vector(-100, -50), Vector(100, -50), Vector(100, 50), Vector(0, 150) }), MaterialInfo(), 1.f);

    //std::vector<Body> bodies;


    //bodies.emplace_back(Body { BodyInit{.position = Vector(400.f, 300.f)} });
    //bodies[bodies.size() - 1].SetShape(new Circle(50.f), MaterialInfo(), 1.f);

    //bodies.emplace_back(Body{ BodyInit{.position = Vector(500.f, 200.f)} });
    //bodies[bodies.size() - 1].SetShape(new Circle(100.f), MaterialInfo(), 1.f);

    for (int i = 0; i < 200; ++i) {
        Body b{ BodyInit{.position = Vector(rand()%1500, rand() % 1000)}};
        b.SetShape(new Circle(rand() % 30 + 1), MaterialInfo(), 1.f);
        b.SetStatic(rand() % 2);
        world.AddBody(b);

    }

    Body b{ BodyInit{.position = Vector(500.f, 200.f), .adittionalData = "key"}};
    //b.SetShape(new Polygon(std::vector<Vector>{ Vector(300, 0), Vector(500, 50), Vector(400, 200), Vector(200, 0) }), MaterialInfo(), 1.f);
    b.SetShape(new Circle(50.3332f), MaterialInfo(), 100.f);
    //b.SetStatic(true);
    world.AddBody(b);
    
    while (window.isOpen())
    {
        int start = clock.getElapsedTime().asMicroseconds();


        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
            else if (event.type == sf::Event::MouseWheelScrolled) {
                for (Body& b : world.GetBodies()) {
                    if (b.GetAdittionalData() == "key" || b.GetAdittionalData() == "keyCollision")
                    {
                        int delta = event.mouseWheelScroll.delta;
                        b.SetAngle(b.GetAngle() + delta * 5);
                        break;
                    }
                }
            }
        }


        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            for (Body& b : world.GetBodies()) {
                if (b.GetAdittionalData() == "key" || b.GetAdittionalData() == "keyCollision")
                {
                    const sf::Vector2i mousePosition{ sf::Mouse::getPosition(window) };
                    const sf::Vector2f mouseCoord{ window.mapPixelToCoords(mousePosition) };
                    Vector direction(mouseCoord.x - b.GetPosition().x, mouseCoord.y - b.GetPosition().y);
                    b.ApplyImpulse(direction * 5000, timeStep);
                    //b.AddForce(direction * 10);
                    break;
                }
            }
        }


        world.Step(timeStep,5);
        //polygonBody.SetAngle(polygonBody.GetAngle() + 0.1);

        window.clear();
        //draw(window/*, polygonBody*/);
        //drawBody(window, polygonBody);
        for (auto b : world.GetBodies()) {
            drawBody(window, b);
        }

        draw(window, std::to_string(1'000'000/(clock.getElapsedTime().asMicroseconds() - start)), sf::Vector2f(1400, 20));
        window.display();
    }
    return 0;
}


void drawCircle(sf::RenderWindow& window, const Circle* c, sf::Vector2f pos, bool isCollision) {
    float radius = c->GetRadius();
    sf::CircleShape cs(radius);
    cs.setPosition(pos.x - radius, pos.y - radius);
    if (isCollision)cs.setFillColor(sf::Color::Red);
    window.draw(cs);
}

void drawCircle(sf::RenderWindow& window, const Circle* c, sf::Vector2f pos) {
    float radius = c->GetRadius();
    sf::CircleShape cs(radius);
    cs.setPosition(pos.x - radius, pos.y - radius);
    window.draw(cs);
}

void drawPolygon(sf::RenderWindow& window, const Polygon* p, sf::Vector2f pos, float angle) {
    std::vector<Vector>vert = p->GetVertices();

    sf::ConvexShape polygon;
    polygon.setPointCount(vert.size());

    for (int i = 0; i < vert.size(); ++i) {
        polygon.setPoint(i, VecToVec2f(vert[i]));
    }

    polygon.setPosition(pos);
    polygon.setRotation(angle);

    sf::Vector2f centroidPos = VecToVec2f(p->GetCentroid());
    centroidPos += pos;

    const float radius = 4;
    sf::CircleShape centroid(radius);
    centroid.setOutlineColor(sf::Color(0, 0, 0));
    centroid.setPosition(sf::Vector2f(centroidPos.x - radius, centroidPos.y - radius));
    centroid.setFillColor(sf::Color::Red);

    window.draw(polygon);
    window.draw(centroid);
}

void drawBody(sf::RenderWindow& window, const Body& body)
{
    sf::Vector2f position = VecToVec2f(body.GetPosition());

    if (body.GetShapeType() == Shape::EType::circle) {
        drawCircle(window, dynamic_cast<const Circle*>(body.GetShape()), position, (body.GetAdittionalData() == "Collision" || body.GetAdittionalData() == "keyCollision"));
    }
    else {
        drawPolygon(window, dynamic_cast<const Polygon*>(body.GetShape()), position, body.GetAngle());
    }
}

void draw(sf::RenderWindow& window/*, Body& b*/)
{
    //drawCircle(window, Circle(10.f));
    //drawPolygon(window, Polygon({ Vector(-100, 50), Vector(-100, -50), Vector(100, -50), Vector(100, 50), Vector(0, 150) }));
    //Polygon p({ Vector(300, 0), Vector(500, 50), Vector(400, 200), Vector(200, 0) });
    //new Polygon(std::vector<Vector>{ Vector(-500, 5), Vector(-500, -5), Vector(500, -5), Vector(500, 5) })

    
    Body circleBody{ BodyInit {.position = Vector(1000.f, 700.f)} };
    circleBody.SetShape(new Circle(10.f), MaterialInfo(), 1.f);
    drawBody(window, circleBody);



    //drawBody(window, b);


}

void draw(sf::RenderWindow& window, std::string msg, sf::Vector2f pos)
{
    sf::Font myFont;

    if (!myFont.loadFromFile("ARIAL.TTF")) {
        throw "no font";
    }

    sf::Text text;
    text.setString(msg);
    text.setFont(myFont);
    text.setCharacterSize(50);
    
    text.setFillColor(sf::Color(128, 128, 0));
    text.move(pos);

    window.draw(text);
}


sf::Vector2f VecToVec2f(const Vector vec) {
    return sf::Vector2f(vec.x, vec.y);
}