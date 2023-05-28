#include "MathUtils/Vector.h"
#include "Engine/Body.h"
#include "Shapes/Circle.h"
#include "Shapes/Polygon.h"
#include <SFML/Graphics.hpp>
#include "Engine/World.h"
#include "RedBall.h"






sf::Vector2f VecToVec2f(const Vector vec);
void drawCircle(sf::RenderWindow& window, const Circle* c, sf::Vector2f pos);
void drawCircle(sf::RenderWindow& window, const Circle* c, sf::Vector2f pos, bool);
void drawPolygon(sf::RenderWindow& window, const Polygon* p, sf::Vector2f pos, float angle);
void drawPolygon(sf::RenderWindow& window, const Polygon* p, sf::Vector2f pos, float angle, bool);
void drawBody(sf::RenderWindow& window, const Body& body);
void draw(sf::RenderWindow& window);
void draw(sf::RenderWindow& window, const std::string& msg, sf::Vector2f pos);
void DrawWindow(sf::RenderWindow& window, World& world, float time);

void SetTestScene(World& world);
void SetScene1(World& world);


int main()
{
    start();
    return 0;
}















    //srand(time(NULL));
    //sf::RenderWindow window(sf::VideoMode(1500, 1000), L"Physics", sf::Style::Default);
    //World world;

    //const float FPS = 80.f;
    //const float InvFPS = 1.f / FPS;
    //sf::Clock clock;

    ////SetTestScene(world);
    //SetScene1(world);


    ////window.setFramerateLimit(0.6);
    ////int a = 0;
    ////int lastFrame = clock.getElapsedTime().asMicroseconds();
    //while (window.isOpen())
    //{
    //    //int start = clock.getElapsedTime().asMicroseconds();


    //    sf::Event event;
    //    while (window.pollEvent(event))
    //    {
    //        if (event.type == sf::Event::Closed) window.close();
    //        //else if (event.type == sf::Event::MouseWheelScrolled) {
    //        //    for (Body& b : world.GetBodies()) {
    //        //        if (b.GetAdittionalData() == "key" || b.GetAdittionalData() == "keyCollision")
    //        //        {
    //        //            int delta = event.mouseWheelScroll.delta;
    //        //            b.SetAngleD(b.GetAngleD() + delta * 5);
    //        //            break;
    //        //        }
    //        //    }
    //        //}
    //    }

    //    float dt = clock.getElapsedTime().asSeconds();

    //    unsigned int nr = std::floor(dt * FPS + 0.5f);
    //    for (int i = 0; i < nr; ++i)
    //    {
    //        world.Step(InvFPS, 5);
    //        DrawWindow(window, world, dt * 1000/*clock.getElapsedTime().asMicroseconds()*/);

    //        clock.restart();
    //        //a++;
    //        //if (a > 660) {
    //        //    int t = clock.getElapsedTime().asMicroseconds() - lastFrame;
    //        //    int fps = a * 1000000 / t;
    //        //    int c = 9;
    //        //}
    //        if (nr > 1) {
    //            int c = 9;
    //        }
    //    }


    //}













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

void drawPolygon(sf::RenderWindow& window, const Polygon* p, sf::Vector2f pos, float angle, bool isCollision) {
    std::vector<Vector>vert = p->GetVertices();

    sf::ConvexShape polygon;
    polygon.setPointCount(vert.size());

    for (int i = 0; i < vert.size(); ++i) {
        polygon.setPoint(i, VecToVec2f(vert[i]));
    }

    polygon.setPosition(pos);
    polygon.setRotation(angle);
    if (isCollision)polygon.setFillColor(sf::Color::Red);

    sf::Vector2f centroidPos = VecToVec2f(p->GetCentroid());
    centroidPos += pos;

    const float radius = 4;
    sf::CircleShape centroid(radius);
    centroid.setOutlineColor(sf::Color(0, 0, 0));
    centroid.setPosition(sf::Vector2f(centroidPos.x - radius, centroidPos.y - radius));
    centroid.setFillColor(sf::Color::Blue);


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
        drawPolygon(window, dynamic_cast<const Polygon*>(body.GetShape()), position, body.GetAngleD(), (body.GetAdittionalData() == "Collision" || body.GetAdittionalData() == "keyCollision"));
    }
}

void draw(sf::RenderWindow& window/*, Body& b*/)
{
    //drawCircle(window, Circle(10.f));
    //drawPolygon(window, Polygon({ Vector(-100, 50), Vector(-100, -50), Vector(100, -50), Vector(100, 50), Vector(0, 150) }));
    //Polygon p({ Vector(300, 0), Vector(500, 50), Vector(400, 200), Vector(200, 0) });
    //new Polygon(std::vector<Vector>{ Vector(-500, 5), Vector(-500, -5), Vector(500, -5), Vector(500, 5) })

    
    Body circleBody{ BodyInit {.position = Vector(1000.f, 700.f)} };
    circleBody.SetShape(new Circle(10.f), Material(), 1.f);
    drawBody(window, circleBody);



    //drawBody(window, b);


}

void draw(sf::RenderWindow& window, const std::string& msg, sf::Vector2f pos)
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

void DrawWindow(sf::RenderWindow& window, World& world, float time)
{
    window.clear();

    for (auto b : world.GetBodies()) {
        drawBody(window, b);
    }

    draw(window, std::to_string(1'000'000 / time), sf::Vector2f(1350, 20));


    draw(window, std::to_string(time), sf::Vector2f(1350, 60));

    window.display();
}

void SetTestScene(World& world)
{
    //world.SetGravity(Vector(0.f, 10.f));

    //Body polygonBody{ BodyInit {.position = Vector(500.f, 300.f)} };
    //polygonBody.SetShape(new Polygon(std::vector<Vector>{ Vector(-100, 50), Vector(-100, -50), Vector(100, -50), Vector(100, 50), Vector(0, 150) }), MaterialInfo(), 1.f);

    //std::vector<Body> bodies;


    //bodies.emplace_back(Body { BodyInit{.position = Vector(400.f, 300.f)} });
    //bodies[bodies.size() - 1].SetShape(new Circle(50.f), MaterialInfo(), 1.f);

    //bodies.emplace_back(Body{ BodyInit{.position = Vector(500.f, 200.f)} });
    //bodies[bodies.size() - 1].SetShape(new Circle(100.f), MaterialInfo(), 1.f);

    for (int i = 0; i < 1; ++i) {
        //Body b{ BodyInit{.position = Vector(rand() % 1500, rand() % 1000)} };
        //b.SetShape(new Circle(rand() % 30 + 1), MaterialInfo(), 1.f);
        //b.SetStatic(rand() % 2);

        Body b{ BodyInit{.position = Vector(750, 500)} };
        //b.SetShape(new Polygon(std::vector<Vector>{ Vector(400, 0), Vector(500, 50), Vector(400, 200), Vector(300, 0) }), MaterialInfo(), 1.f);
        b.SetShape(new Circle(100), Material(), 1.f);
        //b.SetStatic(true);
        world.AddBody(b);

    }

    Body b{ BodyInit{.position = Vector(840, 420.f), .adittionalData = "key"} };
    //b.SetShape(new Polygon(std::vector<Vector>{ Vector(50, 30), Vector(-50, 30), Vector(-50, -30), Vector(50, -30) }), MaterialInfo(), 1.f);
    b.SetShape(new Polygon(std::vector<Vector>{ Vector(400, 0), Vector(500, 50), Vector(400, 200), Vector(300, 0) }), Material(), 1.f);
    //b.SetShape(new Polygon(std::vector<Vector>{ Vector(60, 0), Vector(100, 10), Vector(80, 40), Vector(40, 0) }), MaterialInfo(), 1.f);
    //b.SetShape(new Circle(50.3332f), MaterialInfo(), 1.f);
    //b.SetGravityScale(0);
    //b.SetStatic(true);
    world.AddBody(b);

    //if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
    //    for (Body& b : world.GetBodies()) {
    //        if (b.GetAdittionalData() == "key" || b.GetAdittionalData() == "keyCollision")
    //        {
    //            const sf::Vector2i mousePosition{ sf::Mouse::getPosition(window) };
    //            const sf::Vector2f mouseCoord{ window.mapPixelToCoords(mousePosition) };
    //            Vector direction(mouseCoord.x - b.GetPosition().x, mouseCoord.y - b.GetPosition().y);
    //            Vector vel = b.GetVelocity() / 2;
    //            if (Vector::DotProduct(vel, direction) < 0)b.SetVelocity(vel * 0.98);
    //            //b.ApplyImpulse(direction * 500, timeStep);
    //            b.SetPosition(Vector(mouseCoord.x, mouseCoord.y));
    //            break;
    //        }
    //    }
    //}
}

void SetScene1(World& world)
{
    world.SetGravity(Vector(0.f, 10.f));
    //world.SetFrictionEnable(true);

    Material wood = Material::wood();
    Material plastic = Material::plastic();


    Body box{ BodyInit {.position = Vector(380.f, 50.f), .angle = -45} }; // .position = Vector(380.f, 50.f)
    box.SetShape(new Polygon(120, 50), wood, 40);

    // box sliders
    Body slider1{ BodyInit {.position = Vector(350.f, 120.f), .angle = 30 } };
    slider1.SetShape(new Polygon(150, 10), wood, 4);
    slider1.SetStatic(true);

    Body slider2{ BodyInit {.position = Vector(450.f, 170.f), .angle = 30 } };
    slider2.SetShape(new Polygon(100, 10), wood, 4);
    slider2.SetStatic(true);


    Body ball{ BodyInit {.position = Vector(680.f, 310.f)} };
    ball.SetShape(new Circle(30), plastic, 2);//plastic

    // ball sliders
    Body ballSlider1{ BodyInit {.position = Vector(700.f, 355.f) } };
    ballSlider1.SetShape(new Polygon(100, 10), wood, 4);
    ballSlider1.SetStatic(true);

    Body ballSlider2{ BodyInit {.position = Vector(880.f, 425.f), .angle = -35 } };
    ballSlider2.SetShape(new Polygon(250, 10), wood, 4);
    ballSlider2.SetStatic(true);

    Body ballSlider3{ BodyInit {.position = Vector(700.f, 550.f), .angle = 35 } };
    ballSlider3.SetShape(new Polygon(200, 10), wood, 4);
    ballSlider3.SetStatic(true);

    Body ballSlider4{ BodyInit {.position = Vector(880.f, 675.f), .angle = -35 } };
    ballSlider4.SetShape(new Polygon(200, 10), wood, 4);
    ballSlider4.SetStatic(true);


    // add all bodies to the world
    world.AddBody(ball);
    //world.AddBody(box);
    //world.AddBody(slider1);
    //world.AddBody(slider2);
    //world.AddBody(ballSlider1);
    //world.AddBody(ballSlider2);
    //world.AddBody(ballSlider3);
    //world.AddBody(ballSlider4);
}


sf::Vector2f VecToVec2f(const Vector vec) {
    return sf::Vector2f(vec.x, vec.y);
}


